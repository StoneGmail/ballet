#include "ctpif.h"
#include <cmath>
#include <cfloat>
#include <ctime>
#include "utility.h"

#include "rapidjson/document.h"
using namespace rapidjson;


static int read_instruments_json(string &filepath, CtpInstruments &instrument) {
  ifstream in(filepath);
  if (in.eof())
    return MDERR_CTP_CONFIG_FILE_EMPTY;

  char buffer[MAX_JSON_FILE_SIZE] = "";
  in.read(buffer, MAX_JSON_FILE_SIZE);
  rapidjson::StringStream ss(buffer);
  rapidjson::Document jsondoc;
  jsondoc.ParseStream(ss);
  if (jsondoc.HasParseError()) {
    cout << "Paser instrument " << filepath << " Error at " << jsondoc.GetErrorOffset() << endl;
    cout << buffer + jsondoc.GetErrorOffset() << endl;
    return MDERR_CTP_CONFIG_PARSE_ERROR;
  }

  if (jsondoc.HasMember("instrument")) {
    rapidjson::Value &list = jsondoc["instrument"];
    for (rapidjson::SizeType i = 0; i < list.Size(); ++i)
      instrument.push_back(list[i].GetString());
  } else
    return MDERR_CTP_CONFIG_NO_INSTRUMENT;
  return (0);
}

static int parse_ctp_param(CtpParam* ctp_param, Value &dif) {
  if (dif.HasMember("param")) {
    Value &param = dif["param"];
    if (param.HasMember("ctpmd-logpath")) {
      ctp_param->ctpmd_logpath = param["ctpmd-logpath"].GetString();
      // cout << ctp_param->ctpmd_logpath << endl;
    } else
      return MDERR_CTP_CONFIG_NO_LOGPATH;

    if (param.HasMember("brokerid")) {
      ctp_param->brokerid = param["brokerid"].GetString();
      // cout << ctp_param->brokerid << endl;
    } else
      return MDERR_CTP_CONFIG_NO_BROKERID;

    if (param.HasMember("clientid")) {
      ctp_param->clientid = param["clientid"].GetString();
      // cout << ctp_param->clientid << endl;
    } else
      return MDERR_CTP_CONFIG_NO_CLIENTID;

    if (param.HasMember("password")) {
      ctp_param->password = param["password"].GetString();
      // cout << ctp_param->password << endl;
    } else
      return MDERR_CTP_CONFIG_NO_PASSWORD;
    if (param.HasMember("udp"))
      ctp_param->udp = param["udp"].GetBool();
    else
      return MDERR_CTP_CONFIG_SERVER_NO_UDP;

    if (param.HasMember("mduri")) {
      rapidjson::Value &servers = param["mduri"];
      for (SizeType i = 0; i < servers.Size(); ++i) {
        ctp_param->mduri.push_back(servers[i].GetString());
      }
    } else
      return MDERR_CTP_CONFIG_NO_SERVERS;

    if (param.HasMember("tduri")) {
      rapidjson::Value &servers = param["tduri"];
      for (SizeType i = 0; i < servers.Size(); ++i) {
        ctp_param->tduri.push_back(servers[i].GetString());
      }
    } else
      return MDERR_CTP_CONFIG_NO_SERVERS;

    if (param.HasMember("shared_mem_name")) {
      ctp_param->shred_mem_name = param["shared_mem_name"].GetString();
    } else
      return MDERR_CTP_CONFIG_NO_SHARED_MEM_NAME;
    return 0;
  } else {
    return MDERR_CTP_CONFIG_NO_PARAM;
  }
}

static int parse_data_interface(CtpParam* ctp_param, rapidjson::Document &doc) {
  rapidjson::Value &dif = doc["data-interface"];
  for (rapidjson::SizeType i = 0; i < dif.Size(); ++i) {
    if (dif[i].HasMember("name")) {
      if (!strcmp("ctp", dif[i]["name"].GetString()))
        parse_ctp_param(ctp_param, dif[i]);
    } else
      return MDERR_JSON_DATAINF_PARAM_NO_NAME;
  }
  // for(Value::ConstMemberIterator itr = doc.MemberBegin(); itr !=
  // doc.MemberEnd(); ++itr)
  //   printf("Type of member %s is %s\n", itr->name.GetString(),
  //   kTypeNames[itr->value.GetType()]);
  return SUCCESSED;
}

static int parse_instrument(CtpParam* ctp_param, Document &doc) {
  rapidjson::Value &ins = doc["instrument"];
  if (ins.HasMember("file")) {
    const char *ptr = ins["file"].GetString();
    if (*ptr)
      ctp_param->inst_file = string(ptr);
    else
      ctp_param->instrument.clear();
  }

  if (ctp_param->inst_file.empty()) {
    // read from this json config
    if (ins.HasMember("list")) {
      rapidjson::Value &list = ins["list"];
      for (rapidjson::SizeType i = 0; i < list.Size(); ++i) {
        // cout << ins[i].GetString() << endl;
        ctp_param->instrument.push_back(list[i].GetString());
      }
    } else
      return MDERR_CTP_CONFIG_NO_INSTRUMENT;
  } else {
    // read instruments from other file
    return read_instruments_json(ctp_param->inst_file, ctp_param->instrument);
  }

  return SUCCESSED;
}

/*
    "max-time-long": 9.5,
    "log-path": "/work/sean/projects/build-md/log/gmd.log",
    "data-store-path": "/work/sean/projects/build-md/data/store",
    "tick-data-path": "/work/sean/projects/build-md/data/tick",
    "data-realtime-path": "/work/sean/projects/build-md/data/rt",
    "cal_store_path": "/work/sean/projects/build-md/data/cal"
*/
static int parse_gmd_param(GmdParam* gmd_param, Document &doc) {
  rapidjson::Value &gmd = doc["gmd-param"];
  if (gmd.HasMember("log-path")) {
    strcpy_s(gmd_param->gmdlog, MAX_PATH, gmd["log-path"].GetString());
    // cout << gmd_param->gmdlomax-tick-per-secg << endl;
  } else
    return MDERR_GMD_CONFIG_ERROR;

  if (gmd.HasMember("tdlog-path")) {
    strcpy_s(gmd_param->gtdlog, MAX_PATH, gmd["tdlog-path"].GetString());
    // cout << gmd_param->gmdlomax-tick-per-secg << endl;
  } else
    return MDERR_GMD_CONFIG_ERROR;

  if (gmd.HasMember("data-store-path"))
    strcpy_s(gmd_param->data_store_path, MAX_PATH,
             gmd["data-store-path"].GetString());
  else
    return MDERR_GMD_CONFIG_ERROR;

  if (gmd.HasMember("tick-data-path"))
    strcpy_s(gmd_param->tick_data_path, MAX_PATH,
             gmd["tick-data-path"].GetString());
  else
    return MDERR_GMD_CONFIG_ERROR;

  if (gmd.HasMember("tick-bin-path"))
    strcpy_s(gmd_param->tick_bin_path, MAX_PATH,
             gmd["tick-bin-path"].GetString());
  else
    return MDERR_GMD_CONFIG_ERROR;

  if (gmd.HasMember("data-realtime-path"))
    strcpy_s(gmd_param->data_rtpath, MAX_PATH,
             gmd["data-realtime-path"].GetString());
  else
    return MDERR_GMD_CONFIG_ERROR;

  if (gmd.HasMember("cal-store-path"))
    strcpy_s(gmd_param->cal_store_path, MAX_PATH,
             gmd["cal-store-path"].GetString());
  else
    return MDERR_GMD_CONFIG_ERROR;

  if (gmd.HasMember("max-time-long"))
    gmd_param->max_time_long =
        (uint32_t)(gmd["max-time-long"].GetDouble() * 3600);
  else
    return MDERR_GMD_CONFIG_ERROR;

  if (gmd.HasMember("max-tick-per-sec"))
    gmd_param->max_tick_per_sec = (uint32_t)(gmd["max-tick-per-sec"].GetUint());
  else
    return MDERR_GMD_CONFIG_ERROR;

  if (gmd.HasMember("tick-buffer-num")) /// amout number of tick buffer
    gmd_param->tick_buffer_num = (uint32_t)(gmd["tick-buffer-num"].GetUint());
  else
    return MDERR_GMD_CONFIG_ERROR;

  if (gmd.HasMember("save-tickbuf-time")) /// interval time for save tick buffer
                                          /// to disk(persistent), in minutes
    gmd_param->save_tickbuf_time =
        (uint32_t)(gmd["save-tickbuf-time"].GetUint());
  else
    return MDERR_GMD_CONFIG_ERROR;

  if (gmd.HasMember("save-loop-interval"))
    gmd_param->save_loop_interval =
        (uint32_t)(gmd["save-loop-interval"].GetUint());
  else
    return MDERR_GMD_CONFIG_ERROR;

  if (gmd.HasMember("exit-time"))
    gmd_param->exit_time = gmd["exit-time"].GetString();
  else
    return MDERR_GMD_CONFIG_ERROR;

  return SUCCESSED;
}

int parse_gmd_config(GmdParam* gmd_param, Document &doc) {
  int result = parse_data_interface(gmd_param->ctp_param, doc);
  if (result)
    return result;
  result = parse_instrument(gmd_param->ctp_param, doc);
  if (result)
    return result;
  result = parse_gmd_param(gmd_param, doc);
  return result;
}



bool invalid_data(CtpMarketData& data) {
  return (data.AskPrice1 > 10e10
          || data.BidPrice1 > 10e10
          || data.LastPrice > 10e10);
}


GmdParam* initConfig(const char *filepath) {
    if (!filepath)
      return nullptr;   //MDERR_CTP_CONFIG_FILE_ISNULL;

    ifstream in(filepath);
    if (in.eof())
      return nullptr;   //MDERR_CTP_CONFIG_FILE_EMPTY;

  char buffer[MAX_JSON_FILE_SIZE] = "";
  in.read(buffer, MAX_JSON_FILE_SIZE);
  rapidjson::StringStream ss(buffer);
  rapidjson::Document jsondoc;
  jsondoc.ParseStream(ss);

  if (jsondoc.HasParseError()) {
    cout << "config path is " << filepath << endl;
    cout << "JSON Error at " << jsondoc.GetErrorOffset() << endl;
    cout << buffer + jsondoc.GetErrorOffset() << endl;
    return nullptr;   //MDERR_CTP_CONFIG_PARSE_ERROR;
  }

  GmdParam* param = new GmdParam;
  param->ctp_param = new CtpParam;

  int result = SUCCESSED;
  if (jsondoc["data-interface"].IsArray())
    result = parse_gmd_config(param, jsondoc);
  else
    result = MDERR_CTP_CONFIG_FORMAT_ERROR;


  if (result != SUCCESSED) {
    SafeDeletePtr(param->ctp_param);
    SafeDeletePtr(param);
    return nullptr;
  }
  return param;
}

void releaseConfig(GmdParam *param) {
  SafeDeletePtr(param->ctp_param);
  SafeDeletePtr(param);
}

void convert_ctpmd_2_ctpmmd(CTPMMD *mmd, CtpMarketData *data) {
  memset(mmd, 0, sizeof(CTPMMD));
  mmd->AskPrice1 = data->AskPrice1, mmd->AskVolume1 = data->AskVolume1,
  mmd->BidPrice1 = data->BidPrice1, mmd->BidVolume1 = data->BidVolume1,
  mmd->LastPrice = data->LastPrice, mmd->OpenInterest = data->OpenInterest,
  mmd->UpdateMillisec = data->UpdateMillisec, mmd->Volume = data->Volume;
  // mmd->Turnover = data->Turnover;
  mmd->UpdateTime = make_the_time(data->TradingDay, data->UpdateTime);
  // memcpy(&mmd->AskPrice1, &data->AskPrice1, sizeof(mmd->AskPrice1));
  // memcpy(&mmd->AskVolume1, &data->AskVolume1, sizeof(mmd->AskVolume1));
  // memcpy(&mmd->BidPrice1, &data->BidPrice1, sizeof(mmd->BidPrice1));
  // memcpy(&mmd->BidVolume1, &data->BidVolume1, sizeof(mmd->BidVolume1));
  // memcpy(&mmd->LastPrice, &data->LastPrice, sizeof(mmd->LastPrice));
  // memcpy(&mmd->OpenInterest, &data->OpenInterest, sizeof(mmd->OpenInterest));
  // memcpy(&mmd->Turnover, &data->Turnover, sizeof(mmd->Turnover));
  // memcpy(&mmd->UpdateMillisec, &data->UpdateMillisec,
  //        sizeof(mmd->UpdateMillisec));
  // memcpy(&mmd->Volume, &data->Volume, sizeof(mmd->Volume));
}

void convert_ctpmd_2_ctpmd(DATAPTR d1, DATAPTR d2) {
  CTPMD *md = (CTPMD*)d1;
  CtpMarketData *data = (CtpMarketData*)d2;
  memset(md, 0, sizeof(CTPMD));
  md->AskPrice1 = data->AskPrice1, md->AskVolume1 = data->AskVolume1,
  md->BidPrice1 = data->BidPrice1, md->BidVolume1 = data->BidVolume1,
  md->LastPrice = data->LastPrice, md->OpenInterest = data->OpenInterest,
  md->Volume = data->Volume;
  md->key = (double)make_the_time(data->TradingDay, data->UpdateTime)
          + (float)(data->UpdateMillisec) / 1000;
  md->Turnover = data->Turnover;

  // memcpy(&mmd->AskPrice1, &data->AskPrice1, sizeof(mmd->AskPrice1));
  // memcpy(&mmd->AskVolume1, &data->AskVolume1, sizeof(mmd->AskVolume1));
  // memcpy(&mmd->BidPrice1, &data->BidPrice1, sizeof(mmd->BidPrice1));
  // memcpy(&mmd->BidVolume1, &data->BidVolume1, sizeof(mmd->BidVolume1));
  // memcpy(&mmd->LastPrice, &data->LastPrice, sizeof(mmd->LastPrice));
  // memcpy(&mmd->OpenInterest, &data->OpenInterest, sizeof(mmd->OpenInterest));
  // memcpy(&mmd->Turnover, &data->Turnover, sizeof(mmd->Turnover));
  // memcpy(&mmd->UpdateMillisec, &data->UpdateMillisec,
  //        sizeof(mmd->UpdateMillisec));
  // memcpy(&mmd->Volume, &data->Volume, sizeof(mmd->Volume));
}


int ctp_2_candle(DATAPTR bar_ptr, DATAPTR ctpdata_ptr) {
  auto bar = (CandleBar*)bar_ptr;
  auto ctpdata = (CtpMarketData*) ctpdata_ptr;

  // time_t tt = make_the_time(ctpdata->ActionDay, ctpdata->UpdateTime);
  time_t t = time(0);
  struct tm lt;
  localtime_s(&lt, &t);

  //21:51:18 500
  lt.tm_hour = atoi(ctpdata->UpdateTime);
  lt.tm_min = atoi(ctpdata->UpdateTime + 3);
  lt.tm_sec = atoi(ctpdata->UpdateTime + 6);

  time_t tt = mktime(&lt);

  if (abs(tt - t) > 10000) {
    uint32_t date = atoi(ctpdata->TradingDay);
    lt.tm_year = (int)(date / 10000) - 1900;
    lt.tm_mon = (int)(date - (lt.tm_year + 1900) * 10000) / 100 - 1;
    lt.tm_mday = (int)(date - (lt.tm_year + 1900) * 10000 - (lt.tm_mon + 1) * 100);
  }


  // must adjust the clock of server
  if (abs(tt - t) > 600) {
    printf("Is not trading time %s, %s-%s.%d > %ld > %ld > %ld\n",
           ctpdata->InstrumentID, ctpdata->TradingDay, ctpdata->UpdateTime, ctpdata->UpdateMillisec,
           tt, t, abs(tt - t));
    fflush(stdout);
    return -1;
  }

  bar->High = bar->Low = bar->Close = bar->Open = ctpdata->LastPrice;
  bar->Volume = ctpdata->AskVolume1 + ctpdata->BidVolume1;
  bar->key = (double)tt + (double)(ctpdata->UpdateMillisec) / 1000;
  bar->openInterest = ctpdata->OpenInterest;
  // printf("ActionDay %s, UpdateTime %s, time is %d, %.02lf\n", ctpdata->ActionDay, ctpdata->UpdateTime, bar->key);
  // {
  //   time_t t = (time_t)(bar->key);
  //   localtime_s(&lt, &t);
  //   uint32_t date = (lt.tm_year + 1900) * 10000 + (lt.tm_mon + 1) * 100 + lt.tm_mday;
  //   uint32_t dtime = lt.tm_hour * 10000 + lt.tm_min * 100 + lt.tm_sec;
  //   printf("candle %u %06u\n", date, dtime);
  // }
  return 0;
}

bool its_new_candle_data(int64_t pos, DATAPTR old, DATAPTR bar_data) {
  if (pos < 0) return false;

  auto bar = (CandleBar*)bar_data;
  auto bo = (CandleBar*)old;
#ifdef _DEBUG_
  if (bo->key - bar->key > 3600)
      bar->key += 24 * 60 * 60;
#endif //_DEBUG_

  int64_t itt = ((int64_t)(bar->key) / 10) * 10;
  // printf("%.02f - %.02f = %.02f, itt %ld, its new %d\n",
  //       bar->key, bo->key, bar->key - bo->key, itt,  (itt - (int)(bo->key) >= 10));
  if (bar->key - bo->key < 0) {
    struct tm lt;
    time_t t = (time_t)bo->key;
    localtime_s(&lt, &t);
    printf("old time: %04d/%02d/%02d %02d:%02d:%02d -> ",
           lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday,
           lt.tm_hour, lt.tm_min, lt.tm_sec);
    t = (time_t)bar->key;
    localtime_s(&lt, &t);
    printf("New time: %04d/%02d/%02d %02d:%02d:%02d\n",
           lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday,
           lt.tm_hour, lt.tm_min, lt.tm_sec);
  }
  return (itt - (int)(bo->key) >= 10);
  // time_t tt = (time_t)((int)(bar->key));
  // struct tm lt;
  // localtime_s(&lt, &tt);
  // return (0 == lt.tm_sec % 10);
}

void merge_candle_data(DATAPTR bar_old, DATAPTR bar_new) {
  auto bn = (CandleBar*)bar_new;
  auto bo = (CandleBar*)bar_old;
  if (bo->key > 0) {
    bo->Close = bn->Close;
    bo->High = std::max(bn->High, bo->High);
    bo->Low = std::min(bn->Low, bo->Low);
    bo->Volume += bn->Volume;
    if ((int64_t)(bo->key) % 10)
        bo->key = (int)((int64_t)(bo->key) / 10) * 10;
  } else {
    bo->High = bo->Low = bo->Close = bo->Open = bn->Open;
    bo->Volume = bn->Volume;
    bo->key = (int64_t)(((int64_t)bn->key) / 10) * 10;
  }
  bo->openInterest = bn->openInterest;
}


static double get_interger(const char* t) {
  if (8 < strlen(t)) {
    fprintf(stdout, "WTF %s\n", t);
    return 0;
  }
  double p = atoi(t) * std::pow(10, 8 - strlen(t));
  return p / 100;
}

int sample_tick_data(const char *tick_csv, const char *mdfile, const char* csvfile, const int interval) {
  FILE *fpIn = 0, *fpOut = 0;
  if (fopen_s(&fpIn, tick_csv, "r"))
    return -1;

  char buf[1024];
  char date[32];
  char time[32];
  int ignore1, ignore2;
  int amount = 0;
  CTPMD* md = 0;

  while (fgets(buf, 1024, fpIn)) {
    if ((*buf == 0) || (*buf == '#') || (*buf == '\r') || (*buf == '\n'))
      continue;

    ++amount;
  }

  fprintf(stdout, "total %d lines\n", amount);
  fseek(fpIn, 0, SEEK_SET);

  md = new CTPMD[amount];
  CTPMD* mdp = md;
  int app = 0;

  double prev_time = 0;

  while (fgets(buf, 1024, fpIn)) {
    if ((*buf == 0) || (*buf == '#') || (*buf == '\r') || (*buf == '\n'))
      continue;
    sscanf(buf, "%[^,],0.%[^,],%d,%lf,%d,%lf,%lf,%lf,%lf,%lf,%lf", date, time,
           &ignore1, &mdp->Volume, &ignore2, &mdp->AskVolume1, &mdp->BidVolume1,
           &mdp->LastPrice, &mdp->AskPrice1, &mdp->BidPrice1, &mdp->OpenInterest);

    double t = get_interger(time);
    time_t all_t = make_the_time(date, (uint32_t)t);
    mdp->key = (double)all_t + t - (double)((uint32_t)t);

    ++mdp;
    ++app;
  }

  fclose(fpIn);

  fprintf(stdout, "all data is %d\n", app);

  if (fopen_s(&fpOut, mdfile, "wb")) {
    delete[] md;
    return -2;
  }

  FILE* fpCsv = 0;
  if (fopen_s(&fpCsv, csvfile, "w")) {
    fclose(fpOut);
    delete[] md;
    return -2;
  }


  prev_time = 0;
  app = 0;
  for (int i = 0; i < amount; ++i) {
    if (md[i].key - prev_time >= interval) {
      if (1 != fwrite(md + i, sizeof(CTPMD), 1, fpOut)) {
        printf("write ctpmd error\n");
        amount = -3;
        break;
      }

      struct tm ttm;
      time_t ttt = (time_t)md[i].key;
      localtime_s(&ttm, &ttt);
      fprintf(fpCsv, "%04d/%02d/%02d %02d:%02d:%02d, %0.2f, %0.2f, %0.2f, %0.2f, %0.2f, %0.2f, %0.2f, %0.2f, %0.2f\n",
              ttm.tm_year + 1900, ttm.tm_mon + 1, ttm.tm_mday,
              ttm.tm_hour, ttm.tm_min, ttm.tm_sec,
              md[i].key,
              md[i].LastPrice,
              md[i].BidPrice1,
              md[i].AskPrice1,
              md[i].Volume,
              md[i].BidVolume1,
              md[i].AskVolume1,
              md[i].OpenInterest,
              md[i].Turnover);

      prev_time = md[i].key;
      ++app;
    }
  }
  fclose(fpCsv);
  fclose(fpOut);

  delete[] md;

  printf("write app %d\n", app);
  return amount;


  return 0;
}

