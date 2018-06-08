#ifndef CTPMDSPI_H
#define CTPMDSPI_H


#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"
#include "ThostFtdcMdApi.h"
#include "ThostFtdcTraderApi.h"

#include "ctpif_global.h"
#include "utility.h"
#include "ThostFtdcMdApi.h"
#include "ctpif_data.h"

class CtpCandleWriter;

typedef int (*market_data_callback) (CThostFtdcDepthMarketDataField *pDepthMarketData);

class CTPIF_API CtpMdSpi : public CThostFtdcMdSpi {
public:
  CtpMdSpi(GmdParam* param);
  virtual ~CtpMdSpi();

    void setCtpCandleWriter(CtpCandleWriter* writer) { writer_ = writer; }

  void setMdApi(CThostFtdcMdApi *ctp) { ctp_ = ctp; }
  CThostFtdcMdApi *getCtpApi() { return ctp_; }

  void clear_mdapi() {ctp_ = nullptr;}

  const char* get_trade_day() {return trade_day_; }
  // CThostFtdcMdSpi interface
public:
  void OnFrontConnected();
  void OnFrontDisconnected(int nReason);
  void OnHeartBeatWarning(int nTimeLapse);
  void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
                      CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                      bool bIsLast);
  void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout,
                       CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                       bool bIsLast);
  void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                  bool bIsLast);
  void
  OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument,
                     CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                     bool bIsLast);
  void
  OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument,
                       CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                       bool bIsLast);
  void
  OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument,
                      CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                      bool bIsLast);
  void
  OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument,
                        CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                        bool bIsLast);
  void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
  void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp);

public:
  void user_login();
  void user_logout();
  void close();
  void sub_instruments();
  void unsub_instruments();
  void set_market_data_cb(market_data_callback datacb);

  CTPMD_STATUS get_status() {return ctp_status_.status;}
private:
  void processError(int errid);

private:
  CThostFtdcMdApi *ctp_ = nullptr;
  int iRequestID;
  market_data_callback datacb_ = nullptr;
  CtpStatus ctp_status_ = {CTPMD_STATUS_NO_STATUS, 0};

  char trade_day_[MAX_PATH];
  bool started = false;
  CtpCandleWriter* writer_;

public:
  GmdParam* gmd_param_ = nullptr;
};


#endif // CTPMDSPI_H
