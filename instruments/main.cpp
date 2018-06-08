#include <iostream>
#include <thread>
#include <signal.h>

#include "ctpif_data.h"
#include "ctpif.h"
#include "utility.h"
#include "instgeter.h"

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

static InstramentGeter* insgeter = 0;

static
void thread_get_instruments() {
  if (!insgeter)
    return;

  insgeter->run();
}

//static void signal_fun(int sig) {
//  printf("got signal %d\n", sig);
//}

int main(int argc, char *argv[])
{
#ifdef _WINDOWS
#ifdef _DEBUG_
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
  _CrtDumpMemoryLeaks();
#endif //_DEBUG_
#endif //_WINDOWS

  if (argc != 4) {
      cout << "Usage: instruments <ctp config path> <logpath> <instruments files path>" << endl;
      return (0);
  }

  GmdParam* param = initConfig(argv[1]);
  if (!param) return -1;

  //SIGUSR1   30,10,16    Term    User-defined signal 1
  //SIGUSR2   31,12,17    Term    User-defined signal 2

  //signal(SIGINT, signal_fun);
  //signal(SIGKILL, signal_fun);
  //signal(30, signal_fun);
  //signal(10, signal_fun);
  //signal(16, signal_fun);
  //signal(31, signal_fun);
  //signal(12, signal_fun);
  //signal(17, signal_fun);

  insgeter = new InstramentGeter(param, argv[2], argv[3]);
  insgeter->init();

  std::thread t(thread_get_instruments);

  while(true) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    if (insgeter->is_stoped())
      break;
  }

  SafeDeletePtr(insgeter);

  releaseConfig(param);

  return 0;
}

