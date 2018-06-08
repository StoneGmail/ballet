#include <chrono>
#include <iostream>
#include <stdio.h>
#include <thread>
#include <signal.h>

#include "ctpif_data.h"
#include "ctpif.h"
#include "ctpmdspi.h"
#include "ctptdspi.h"
#include "mem.h"
#include "rapidjson/document.h"
#include "str.h"
#include "utility.h"
#include "ctpif_data_fio.h"


#ifdef _WINDOWS
// check memory leak

#ifdef _DEBUG_
#ifndef DBG_NEW
#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DBG_NEW
#endif
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#endif // _DEBUG_

#endif //_WINDOWS

using namespace std;

static GmdParam* gmd_param = nullptr;
static CtpMdSpi* gmdspi = nullptr;
static CtpTradeSpi* gtdspi = nullptr;
static std::thread thCtpmd;
static std::thread thCtptd;
static bool run = false;

static CtpCandleWriter* writer = nullptr;

static int write_ctpdata_to_memory(CtpMarketData *data)
{
    writer->addNewTick(data);
    return 0;
}


static void connect_to_tdif() {
  // connect to trade insterface and get something then break
  gtdspi = new CtpTradeSpi(gmd_param, gmd_param->gtdlog);
  thCtptd = std::thread{connect_to_ctp_tdif, gtdspi};
  std::this_thread::sleep_for(std::chrono::seconds(10));
  gtdspi->login();
  std::this_thread::sleep_for(std::chrono::seconds(10));
  gtdspi->prepare_environment();

  for (;run;)
    std::this_thread::sleep_for(std::chrono::seconds(10));

  gtdspi->logout();
  std::this_thread::sleep_for(std::chrono::seconds(10));

  //SafeDeletePtr(gtdspi);
  thCtptd.detach();
}

static int subscribe_data(CtpCandleWriter* writer) {
  gmdspi = new CtpMdSpi(gmd_param);
  gmdspi->setCtpCandleWriter(writer);
  gmdspi->set_market_data_cb(write_ctpdata_to_memory);
  thCtpmd = std::thread{connect_to_ctp_mdif, gmdspi};
  std::this_thread::sleep_for(std::chrono::seconds(10));
  gmdspi->user_login();
  std::this_thread::sleep_for(std::chrono::seconds(10));

  if (gmdspi->get_status() == CTPMD_STATUS_USER_LOGIN)
      gmdspi->sub_instruments();

//  std::this_thread::sleep_for(std::chrono::seconds(10));
//  mylog(gmd_param->gmdlog, L_INFO, "about disconnect...");
//  gmdspi->OnFrontDisconnected(0);

  return 0;
}


static void close_mdif() {
  gmdspi->user_logout();
  std::this_thread::sleep_for(std::chrono::seconds(10));
  if (gmdspi->get_status() == CTPMD_STATUS_USER_LOGOUT)
      gmdspi->close();

  thCtpmd.detach();
}

void got_signal(int s) {
  if (s == SIGINT || s == SIGTERM) {
    printf("got signal %d\n", s);
    run = false;
  }
}

/**
 * @brief Usage: gmd <config file> <instruments list file> [nommd]
                 config file: it's path of config file
                 instruments list file: it's a list in json format
                 nommd: optional, indecate store the CTPMMD file or not
 */
int main(int argc, char *argv[]) {
#ifdef _WINDOWS
#ifdef _DEBUG_
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
  _CrtDumpMemoryLeaks();
#endif //_DEBUG_
#endif //_WINDOWS

  //408, 72
  //printf("%lu, %lu\n", sizeof(CtpMarketData), sizeof(CTPMD));

  if (argc != 2) {
    cout << "Usage: md <config file>" << endl;
    cout << "config file: it's path of config file" << endl;
    return (0);
  }

  gmd_param = initConfig(argv[1]);
  if (!gmd_param) {
    cout << "can not init config file\n";
    return -1;
  }

  signal(SIGINT, got_signal);
  signal(SIGTERM, got_signal);
  writer = new CtpCandleWriter(gmd_param);

  run = true;
  std::thread thread_tdif(connect_to_tdif);

  if (subscribe_data(writer)) {
    cout << "error for subscrie data\n";
    return -2;
  }

  uint8_t j = 0;
  time_t today_exit_1 = 0, today_exit_2 = 0, time_now;
  const char * exit_time_1 = "16:00:00";
  const char * exit_time_2 = "02:40:00";

  while (run) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    if (!(j % 240)) {
        today_exit_1 = get_exit_time(exit_time_1);
        today_exit_2 = get_exit_time(exit_time_2);
    }

    time_now = time(0);
    if ((time_now >= today_exit_1 && time_now <= today_exit_1 + 3600)
            || (time_now >= today_exit_2 && time_now <= today_exit_2 + 3600)) {
        printf("it is time to exit(%ld)", time_now);
        break;
    }

    ++j;
  }


  // std::this_thread::sleep_for(std::chrono::seconds(12000));
  // std::this_thread::sleep_for(std::chrono::seconds(120));

  close_mdif();

  writer->close_interface();
  std::this_thread::sleep_for(std::chrono::seconds(30));
  SafeDeletePtr(writer);

  // delete gtdspi;

  run = false;
  thread_tdif.join();

  SafeDeletePtr(gtdspi);
  SafeDeletePtr(gmdspi);
  releaseConfig(gmd_param);

  return 0;
}
