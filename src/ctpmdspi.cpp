#include "ctpmdspi.h"
#include "ctpifio.h"
#include "ctpif_data.h"
#include "mylog.h"
#include "str.h"
#include "utility.h"
#include <locale>
#include "ctpif_data_fio.h"

CtpMdSpi::CtpMdSpi(GmdParam *param) : ctp_(0), iRequestID(-1),
    datacb_(0), writer_(0), gmd_param_(param) {
}

CtpMdSpi::~CtpMdSpi() {
  if (ctp_) {
    ctp_->Release();
    //wait for ctp_mdapi exit
    std::this_thread::sleep_for(std::chrono::seconds(5));
    ctp_ = 0;
  }
}

void CtpMdSpi::OnFrontConnected() {
  ctp_status_.status = CTPMD_STATUS_CONNECTED;
  mylog(gmd_param_->gmdlog, L_INFO, "ctp data interface connected.");
  user_login();
}

void CtpMdSpi::OnFrontDisconnected(int nReason) {
  ctp_status_.status = CTPMD_STATUS_DISCONNECTED;
  ctp_status_.data = nReason;
  mylog(gmd_param_->gmdlog, L_INFO, "ctp data interface disconnected.");

  if(writer_ != 0) {
      writer_->close_interface();
      sleep(10);
      exit(-1);
  }
}

void CtpMdSpi::OnHeartBeatWarning(int nTimeLapse) {
  ctp_status_.status = CTPMD_STATUS_HEARTBEAT;
  ctp_status_.data = nTimeLapse;
  mylog(gmd_param_->gmdlog, L_INFO, "ctp heart beat %d.", nTimeLapse);
}

void CtpMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
                              CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                              bool bIsLast) {
  ctp_status_.status = CTPMD_STATUS_USER_LOGIN;
  mylog(gmd_param_->gmdlog, L_INFO, "response: user login");
  if (pRspUserLogin != nullptr && pRspInfo != nullptr) {
    strcpy_s(trade_day_, MAX_PATH, pRspUserLogin->TradingDay);
    ofstream out;
    getLogfile(out, gmd_param_->gmdlog);
    out << *pRspUserLogin << *pRspInfo << " request id " << nRequestID << " last "
        << bIsLast;
    if (pRspInfo->ErrorID)
      processError(pRspInfo->ErrorID);
    else
      ctp_status_.data = 0, ctp_status_.status = CTPMD_STATUS_USER_LOGIN;
  }
  return;
}
void CtpMdSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout,
                               CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                               bool bIsLast) {
  ctp_status_.status = CTPMD_STATUS_USER_LOGOUT;
  mylog(gmd_param_->gmdlog, L_INFO, "response: user logout");
  ofstream out;
  getLogfile(out, gmd_param_->gmdlog);
  out << *pUserLogout << *pRspInfo << " request id " << nRequestID << " last "
      << bIsLast;
  processError(pRspInfo->ErrorID);
  ctp_status_.status = CTPMD_STATUS_USER_LOGOUT;
}

void CtpMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                          bool bIsLast) {
  ctp_status_.status = CTPMD_STATUS_ERROR;
  mylog(gmd_param_->gmdlog, L_ERR, "OnRspError!");
  ofstream out;
  getLogfile(out, gmd_param_->gmdlog);
  out << *pRspInfo << " request id " << nRequestID << " last " << bIsLast;
  processError(pRspInfo->ErrorID);
}

void CtpMdSpi::OnRspSubMarketData(
    CThostFtdcSpecificInstrumentField *pSpecificInstrument,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  ctp_status_.status = CTPMD_STATUS_SUBSCRIPT;
  mylog(gmd_param_->gmdlog, L_INFO, "OnRspSubMarketData");
  ofstream out;
  getLogfile(out, gmd_param_->gmdlog);
  out << *pSpecificInstrument << *pRspInfo << " request id " << nRequestID
      << " last " << bIsLast;
  processError(pRspInfo->ErrorID);
}

void CtpMdSpi::OnRspUnSubMarketData(
    CThostFtdcSpecificInstrumentField *pSpecificInstrument,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  ctp_status_.status = CTPMD_STATUS_UNSUBSCRIPT;
  mylog(gmd_param_->gmdlog, L_INFO, "OnRspUnSubMarketData");
  ofstream out;
  getLogfile(out, gmd_param_->gmdlog);
  out << *pSpecificInstrument << *pRspInfo << " request id " << nRequestID
      << " last " << bIsLast;
  processError(pRspInfo->ErrorID);
}

void CtpMdSpi::OnRspSubForQuoteRsp(
    CThostFtdcSpecificInstrumentField *pSpecificInstrument,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  ctp_status_.status = CTPMD_STATUS_SUB_QUOTERSP;
  mylog(gmd_param_->gmdlog, L_INFO, "OnRspSubForQuoteRsp");
  ofstream out;
  getLogfile(out, gmd_param_->gmdlog);
  out << *pSpecificInstrument << *pRspInfo << " request id " << nRequestID
      << " last " << bIsLast;
  processError(pRspInfo->ErrorID);
}

void CtpMdSpi::OnRspUnSubForQuoteRsp(
    CThostFtdcSpecificInstrumentField *pSpecificInstrument,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  ctp_status_.status = CTPMD_STATUS_UNSUB_QUOTERSP;
  mylog(gmd_param_->gmdlog, L_INFO, "OnRspUnSubForQuoteRsp");
  ofstream out;
  getLogfile(out, gmd_param_->gmdlog);
  out << *pSpecificInstrument << *pRspInfo << " request id " << nRequestID
      << " last " << bIsLast;
  processError(pRspInfo->ErrorID);
}

void CtpMdSpi::OnRtnDepthMarketData (
    CThostFtdcDepthMarketDataField *pDepthMarketData) {
  ctp_status_.status = CTPMD_STATUS_DEPTH_MD;
  // mylog(gmd_param_->gmdlog, L_INFO, "OnRtnDepthMarketData");
  // ofstream out(gmd_param_->gmdlog, ios_base::app|ios_base::out);
#ifdef _DEBUG_
  std::cout << *pDepthMarketData;
#endif //_DEBUG_
  if (datacb_)
      datacb_(pDepthMarketData);
  else
      std::cout << "NO data call back function\n";
  // if (gmdmem)
  //   gmdmem->addNewTick(pDepthMarketData);
}

void CtpMdSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) {
  ctp_status_.status = CTPMD_STATUS_QUOTERSP;
  mylog(gmd_param_->gmdlog, L_INFO, "OnRtnForQuoteRsp");
  ofstream out;
  getLogfile(out, gmd_param_->gmdlog);
  out << *pForQuoteRsp;
}

void CtpMdSpi::user_login() {
  CThostFtdcReqUserLoginField req;
  memset(&req, 0, sizeof(req));
  strcpy_s(req.BrokerID, 10, gmd_param_->ctp_param->brokerid.c_str());
  strcpy_s(req.UserID, 15, gmd_param_->ctp_param->clientid.c_str());
  strcpy_s(req.Password, 40, gmd_param_->ctp_param->password.c_str());
  int iResult = ctp_->ReqUserLogin(&req, ++iRequestID);
  mylog(gmd_param_->gmdlog, L_INFO,
        "--> send login request with rid %d, result %d", iRequestID, iResult);
}

void CtpMdSpi::user_logout() {
  CThostFtdcUserLogoutField req;
  memset(&req, 0, sizeof(req));
  strcpy_s(req.BrokerID, 10, gmd_param_->ctp_param->brokerid.c_str());
  strcpy_s(req.UserID, 15, gmd_param_->ctp_param->clientid.c_str());

  {
    // unsubscript
    // unsub_instruments();
    // logout
    int iResult = ctp_->ReqUserLogout(&req, ++iRequestID);
    mylog(gmd_param_->gmdlog, L_INFO,
          "--> send logout request with rid %d, result %d", iRequestID,
          iResult);
  }
}

void CtpMdSpi::close() {
    if (ctp_) {
      ctp_->Release();
    }
}


void CtpMdSpi::sub_instruments() {
  char **pInstrument = new char *[gmd_param_->ctp_param->instrument.size()];
  for (size_t i = 0; i < gmd_param_->ctp_param->instrument.size(); ++i)
    pInstrument[i] = (char *)gmd_param_->ctp_param->instrument[i].c_str();

  ctp_->SubscribeMarketData(pInstrument, (int)gmd_param_->ctp_param->instrument.size());
  ctp_->SubscribeForQuoteRsp(pInstrument, (int)gmd_param_->ctp_param->instrument.size());
  SafeDeleteAry(pInstrument);
}

void CtpMdSpi::unsub_instruments() {
  char **pInstrument = new char *[gmd_param_->ctp_param->instrument.size()];
  for (size_t i = 0; i < gmd_param_->ctp_param->instrument.size(); ++i)
    pInstrument[i] = (char *)gmd_param_->ctp_param->instrument[i].c_str();

  ctp_->UnSubscribeMarketData(pInstrument, (int)gmd_param_->ctp_param->instrument.size());
  ctp_->UnSubscribeForQuoteRsp(pInstrument, (int)gmd_param_->ctp_param->instrument.size());
  SafeDeleteAry(pInstrument);
}

void CtpMdSpi::processError(int errid) {
  switch (errid) {
  case 0:
    break;
  case 68:
    //_ctp->Release();
    break;
  }
}


void CtpMdSpi::set_market_data_cb(market_data_callback datacb) {
  datacb_ = datacb;
}



CtpMdSpi* get_ctp_mdif(GmdParam* param) {
  return new CtpMdSpi(param);
}

CTPIF_API void connect_to_ctp_mdif(CtpMdSpi* mdspi) {
  if (!mdspi) return;

  CThostFtdcMdApi *ctp = CThostFtdcMdApi::CreateFtdcMdApi(
      mdspi->gmd_param_->ctp_param->ctpmd_logpath.c_str(),
              mdspi->gmd_param_->ctp_param->udp);

  mdspi->setMdApi(ctp);

  ctp->RegisterSpi(mdspi);
  for (auto &p : mdspi->gmd_param_->ctp_param->mduri)
    ctp->RegisterFront((char *)p.c_str());
  ctp->Init();

  ctp->Join();
  ctp->Release();

  mdspi->clear_mdapi();
}

