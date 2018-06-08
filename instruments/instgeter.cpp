#include "ctpif_data.h"
#include "instgeter.h"
#include "mylog.h"
#include "str.h"
#include "utility.h"

#include <thread>

InstramentGeter::InstramentGeter(GmdParam* param, const char *logpath, const char* inst_path)
    : CtpTradeSpi(param, logpath), stop_(false), fpBinary(nullptr) {
  strcpy_s(inst_path_, MAX_PATH, inst_path);
}

InstramentGeter::~InstramentGeter() {}

bool InstramentGeter::is_stoped() {
  return stop_;
}

int InstramentGeter::init() {
  for (auto p : gmd_param_->ctp_param->instrument)
    vecRegex.push_back(std::regex(p));

  //remove old instrument and all-instrument json
  clear_instrument_files();

  //remove old and create empty bin file
  char filepath[MAX_PATH];
  snprintf(filepath, MAX_PATH, "%s/instruments.bin", inst_path_);
  remove(filepath);
  fpBinary = 0;
  if (fopen_s(&fpBinary, filepath, "a+b"))
    return (-1);

  return (0);
}

bool InstramentGeter::try_stop() {
  if (fpBinary) fclose(fpBinary);

  save_instruments_to_json();

  stop_ = true;
  CtpTradeSpi::stop();
  std::this_thread::sleep_for(std::chrono::seconds(60));
  return stop_;
}

void InstramentGeter::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
                                     CThostFtdcRspInfoField *pRspInfo,
                                     int nRequestID, bool bIsLast) {
  UNUSED(nRequestID);
  UNUSED(bIsLast);

  if (pRspInfo->ErrorID) {
    mylog(logpath_, L_ERR, "%s, %s, login error %s", pRspUserLogin->UserID,
          pRspUserLogin->BrokerID, pRspInfo->ErrorMsg);
    return;
  }

  set_session(pRspUserLogin);

  mylog(logpath_, L_INFO, "User %s has Logined", pRspUserLogin->UserID);
  CThostFtdcQryInstrumentField req;
  memset(&req, 0, sizeof(req));
  stop_ = false;
  int iResult = ctp_->ReqQryInstrument(&req, ++nRequestID_);
  mylog(logpath_, L_INFO, "query instrument %d", iResult);
}

void InstramentGeter::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout,
                                      CThostFtdcRspInfoField *pRspInfo,
                                      int nRequestID, bool bIsLast) {
  UNUSED(pUserLogout);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);

  mylog(logpath_, L_INFO, "OnRspUserLogout");
}

int InstramentGeter::clear_instrument_files() {
  char filepath[MAX_PATH];
  snprintf(filepath, MAX_PATH, "%s/instruments.json", inst_path_);
  if (remove(filepath))
    return -1;


  snprintf(filepath, MAX_PATH, "%s/all-instruments.json", inst_path_);
  if (remove(filepath))
    return -1;
  //if (fopen_s(&fp, filepath, "a+"))
  //  return (-1);
  //fprintf(fp, "{\n  \"instrument\" : [\n");
  //fclose(fp);

  return 0;
}

int InstramentGeter::save_instruments_to_json() {
  char filepath[MAX_PATH];
  snprintf(filepath, MAX_PATH, "%s/all-instruments.json", inst_path_);

  FILE *fp = nullptr;
  if (fopen_s(&fp, filepath, "a+"))
    return (-1);

  fprintf(fp, "{\n  \"instrument\" : [\n");
  for(size_t i = 0; i < vecAllInst.size() - 1; ++i)
    fprintf(fp, "    \"%s\",\n", vecAllInst[i].c_str());
  //last instrument
  fprintf(fp, "    \"%s\"\n", vecAllInst.back().c_str());
  fprintf(fp, "  ]\n}");

  fclose(fp);


  snprintf(filepath, MAX_PATH, "%s/instruments.json", inst_path_);
  if (fopen_s(&fp, filepath, "a+"))
    return (-1);

  fprintf(fp, "{\n  \"instrument\" : [\n");

  // for (size_t i = 0; i < ctp_param->instrument.size(); ++i) {
  //   std::string inst = minString(mapInst, getInstCode(ctp_param->instrument[i]));
  //   if (i < ctp_param->instrument.size() - 1)
  //     fprintf(fp, "    \"%s\",\n", inst.c_str());
  //   else
  //     fprintf(fp, "    \"%s\"\n", inst.c_str());
  // }


  size_t k = 0;
  for (auto& p : mapInst) {
    if (k) fprintf(fp, ",\n");
    fprintf(fp, "    \"%s\"", p.second.c_str());
    ++k;
  }
  //for(size_t i = 0; i < vecAllInst.size() - 1; ++i) {
  //  if (match_index(vecRegex, vecAllInst[i].c_str())) {
  //  }
  //}
  fprintf(fp, "\n  ]\n}");

  return (0);
}

int InstramentGeter::save_to_binary(CThostFtdcInstrumentField *pInstrument) {
  if (1 != fwrite(pInstrument, sizeof(CThostFtdcInstrumentField), 1, fpBinary))
    return (-2);

  return (0);
}

void InstramentGeter::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument,
                                         CThostFtdcRspInfoField *pRspInfo,
                                         int nRequestID, bool bIsLast) {
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);

  mylog(logpath_, L_INFO, "%s\n", pInstrument->InstrumentID);

  //save to binary file
  save_to_binary(pInstrument);

  //add into vector of
  vecAllInst.push_back(pInstrument->InstrumentID);

  if (match_index(vecRegex, pInstrument->InstrumentID) > -1)
    mapInst.insert(std::make_pair(getInstCode(pInstrument->InstrumentID), pInstrument->InstrumentID));

  if (bIsLast) try_stop();
}
