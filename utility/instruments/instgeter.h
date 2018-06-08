#ifndef INSTRMENTGETER_H
#define INSTRMENTGETER_H

#include "ctpif_global.h"
#include <vector>
#include <map>
#include <regex>

#include "ctptdspi.h"

class InstramentGeter : public CtpTradeSpi {
public:
  InstramentGeter(GmdParam *param, const char *logpath, const char *inst_path);
  virtual ~InstramentGeter();

public:
  // InstramentGeter interface
  bool try_stop();
  bool is_stoped();
  int  init();

  // CThostFtdcTraderSpi interface
public:
  void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
                      CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                      bool bIsLast);
  void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout,
                       CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                       bool bIsLast);
  void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument,
                          CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                          bool bIsLast);

private:
  bool stop_;
  char inst_path_[MAX_PATH];
  FILE* fpBinary;
  std::multimap<std::string, std::string> mapInst;
  std::vector<std::string> vecAllInst;
  std::vector<std::string> vecFocusedInst;
  std::vector<std::regex> vecRegex;

private:
  int save_instruments_to_json();
  int save_to_binary(CThostFtdcInstrumentField *pInstrument);
  int clear_instrument_files();
};

#endif // INSTRMENTGETER_H
