#include "ctpif.h"
#include "utility.h"
#include "mylog.h"
#include "str.h"

#include <fstream>
#include <iostream>
#include <thread>

#define ORDERREF SessionInfo_.MaxOrderRef

//CtpTradeSpi* gtrade = 0;


/*------------------------------------------------------------------
 *
 *  class CtpTradeSpi
 *
 * -----------------------------------------------------------------*/

CtpTradeSpi::CtpTradeSpi(GmdParam *gmd_param, const char *logpath) : ctp_(0), nRequestID_(-1), gmd_param_(gmd_param) {
  strcpy_s(logpath_, MAX_PATH, logpath);
}

CtpTradeSpi::~CtpTradeSpi() {
  clear_map(map_inst_);
  clear_map(map_cms_);
  clear_map(map_mgn_);
}

int CtpTradeSpi::run() {
  if (!(ctp_ = CThostFtdcTraderApi::CreateFtdcTraderApi(
            gmd_param_->ctp_param->ctpmd_logpath.c_str())))
    return -1;

  ctp_->RegisterSpi(this);
  ctp_->SubscribePublicTopic(THOST_TERT_RESTART); // 注册公有流
  // THOST_TE_RESUME_TYPE::THOST_TERT_RESTART
  ctp_->SubscribePrivateTopic(THOST_TERT_RESTART); // 注册私有流
  // THOST_TE_RESUME_TYPE::THOST_TERT_RESTART
  for (int i = (int)gmd_param_->ctp_param->tduri.size() - 1; i >= 0; --i)
    ctp_->RegisterFront((char *)gmd_param_->ctp_param->tduri[i].c_str());

  ctp_->Init();
  ctp_->Join();

  if (ctp_)
    ctp_->Release();
  ctp_ = 0;

  return (0);
}

#define CHECK_INITIAL

int CtpTradeSpi::stop() {
  CHECK_INITIAL

  logout();
  return (0);
}

int CtpTradeSpi::login() {
  CHECK_INITIAL

  CThostFtdcReqUserLoginField req;
  memset(&req, 0, sizeof(req));
  strcpy_s(req.BrokerID, sizeof(req.BrokerID), gmd_param_->ctp_param->brokerid.c_str());
  strcpy_s(req.UserID, sizeof(req.UserID), gmd_param_->ctp_param->clientid.c_str());
  strcpy_s(req.Password, sizeof(req.Password), gmd_param_->ctp_param->password.c_str());

  int iResult = ctp_->ReqUserLogin(&req, ++nRequestID_);
  mylog(logpath_, L_INFO, "send login request");

  return (iResult);
}

int CtpTradeSpi::logout() {
  CHECK_INITIAL

  CThostFtdcUserLogoutField req;
  memset(&req, 0, sizeof(req));
  strcpy_s(req.BrokerID, sizeof(req.BrokerID), gmd_param_->ctp_param->brokerid.c_str());
  strcpy_s(req.UserID, sizeof(req.UserID), gmd_param_->ctp_param->clientid.c_str());

  int iResult = ctp_->ReqUserLogout(&req, ++nRequestID_);
  mylog(logpath_, L_INFO, "send logout request");

  return (iResult);
}

int CtpTradeSpi::prepare_environment() {
  // set_session(pUserLogin);

  if (confirm_settlement_info())
    return (1);

  std::this_thread::sleep_for(std::chrono::milliseconds(10000));
  if (query_instrument())
    return (2);
  std::this_thread::sleep_for(std::chrono::milliseconds(10000));
  if (query_margin())
    return (3);
  std::this_thread::sleep_for(std::chrono::milliseconds(10000));
  if (query_commision())
    return (4);
  std::this_thread::sleep_for(std::chrono::milliseconds(10000));
  if (query_account())
    return (5);
  std::this_thread::sleep_for(std::chrono::milliseconds(10000));
  if (query_investor_position())
    return (6);
  std::this_thread::sleep_for(std::chrono::milliseconds(10000));
  if (query_order())
    return (7);
  std::this_thread::sleep_for(std::chrono::milliseconds(10000));
  if (query_trade())
    return (8);
  std::this_thread::sleep_for(std::chrono::milliseconds(10000));
  return (0);
}

time_t make_time_in_same_day(const char *day, const char *strt) {
  long llt = atol(day);
  if (!llt)
    return (0);

  struct tm lt;
  memset(&lt, 0, sizeof(struct tm));

  lt.tm_year = (int)(llt / 10000) - 1900;
  lt.tm_mon = (int)(llt - (lt.tm_year + 1900) * 10000) / 100 - 1;
  lt.tm_mday = (int)(llt - (lt.tm_year + 1900) * 10000 - (lt.tm_mon + 1) * 100);
#ifdef _WINDOWS
  sscanf_s(strt, "%d:%d:%d", &lt.tm_hour, &lt.tm_min, &lt.tm_sec);
#else
  sscanf(strt, "%d:%d:%d", &lt.tm_hour, &lt.tm_min, &lt.tm_sec);
#endif

  return mktime(&lt);
}

void CtpTradeSpi::set_session(CThostFtdcRspUserLoginField *pUserLogin) {
  strcpy_s(SessionInfo_.BrokerID, sizeof(SessionInfo_.BrokerID),
           pUserLogin->BrokerID);
  SessionInfo_.FrontID = pUserLogin->FrontID;
  strcpy_s(SessionInfo_.LoginTime, sizeof(SessionInfo_.LoginTime),
           pUserLogin->LoginTime);
  SessionInfo_.SessionID = pUserLogin->SessionID;
  strcpy_s(SessionInfo_.SystemName, sizeof(SessionInfo_.SystemName),
           pUserLogin->SystemName);
  strcpy_s(SessionInfo_.TradingDay, sizeof(SessionInfo_.TradingDay),
           pUserLogin->TradingDay);
  strcpy_s(SessionInfo_.UserID, sizeof(SessionInfo_.UserID),
           pUserLogin->UserID);

  time_t now = time(0);
  SessionInfo_.m_nDeltaSHFETime =
      (int)(make_time_in_same_day(pUserLogin->TradingDay,
                                  pUserLogin->SHFETime) -
            now);
  SessionInfo_.m_nDeltaCZCETime =
      (int)(make_time_in_same_day(pUserLogin->TradingDay,
                                  pUserLogin->CZCETime) -
            now);
  SessionInfo_.m_nDeltaDCETime =
      (int)(make_time_in_same_day(pUserLogin->TradingDay, pUserLogin->DCETime) -
            now);
  SessionInfo_.m_nDeltaFFEXTime =
      (int)(make_time_in_same_day(pUserLogin->TradingDay,
                                  pUserLogin->FFEXTime) -
            now);
  SessionInfo_.MaxOrderRef = atoi(pUserLogin->MaxOrderRef);
}

int CtpTradeSpi::confirm_settlement_info() {
  CHECK_INITIAL

  CThostFtdcSettlementInfoConfirmField req;
  memset(&req, 0, sizeof(req));
  strcpy_s(req.BrokerID, sizeof(req.BrokerID), gmd_param_->ctp_param->brokerid.c_str());
  strcpy_s(req.InvestorID, sizeof(req.InvestorID), gmd_param_->ctp_param->clientid.c_str());
  time_t t = time(0);
  struct tm tt;
  localtime_s(&tt, &t);
  snprintf(req.ConfirmDate, sizeof(req.ConfirmDate),
           "%04d%02d%02d",
           1900 + tt.tm_year,
           1 + tt.tm_mon,
           tt.tm_mday);
  snprintf(req.ConfirmTime, sizeof(req.ConfirmTime),
           "%02d:%02d:%03d",
           tt.tm_hour,
           tt.tm_min,
           tt.tm_sec);

  int iResult = ctp_->ReqSettlementInfoConfirm(&req, ++nRequestID_);

  mylog(logpath_, L_INFO, "request settlement info confirm %d", iResult);
  return iResult;
}


int CtpTradeSpi::query_margin(const char *instrument) {
  CThostFtdcQryInstrumentMarginRateField mr;
  memset(&mr, 0, sizeof(mr));
  strcpy_s(mr.BrokerID, sizeof(mr.BrokerID), gmd_param_->ctp_param->brokerid.c_str());
  strcpy_s(mr.InvestorID, sizeof(mr.InvestorID), gmd_param_->ctp_param->clientid.c_str());
  mr.HedgeFlag = '1'; //touji
  if (nullptr != instrument)
      strcpy_s(mr.InstrumentID, sizeof(mr.InstrumentID), instrument);
  int iResult = ctp_->ReqQryInstrumentMarginRate(
                &mr,
                ++nRequestID_);
  mylog(logpath_, L_INFO, "query commision %d", iResult);
  return iResult;
}

int CtpTradeSpi::query_commision(const char* instrument) {
    CThostFtdcQryInstrumentCommissionRateField qrycms;
    memset(&qrycms, 0, sizeof(qrycms));
    strcpy_s(qrycms.BrokerID, sizeof(qrycms.BrokerID), gmd_param_->ctp_param->brokerid.c_str());
    strcpy_s(qrycms.InvestorID, sizeof(qrycms.InvestorID), gmd_param_->ctp_param->clientid.c_str());
    if (nullptr != instrument)
        strcpy_s(qrycms.InstrumentID, sizeof(qrycms.InstrumentID), instrument);
    int iResult = ctp_->ReqQryInstrumentCommissionRate(
                  &qrycms,
                  ++nRequestID_);
    mylog(logpath_, L_INFO, "query commision %d", iResult);
    return iResult;
}

int CtpTradeSpi::query_instrument(const char *instrument) {
  CHECK_INITIAL

  //clean instument vector
  if (instrument == nullptr)
    clear_map(map_inst_);

  CThostFtdcQryInstrumentField req;
  memset(&req, 0, sizeof(req));
  if (instrument != nullptr)
    strcpy_s(req.InstrumentID, sizeof(req.InstrumentID), instrument);

  int iResult = ctp_->ReqQryInstrument(&req, ++nRequestID_);

  mylog(logpath_, L_INFO, "query instrument %d", iResult);
  return iResult;
}

int CtpTradeSpi::query_account() {
  CHECK_INITIAL

  CThostFtdcQryTradingAccountField req;
  memset(&req, 0, sizeof(req));
  strcpy_s(req.BrokerID, sizeof(req.BrokerID), gmd_param_->ctp_param->brokerid.c_str());
  strcpy_s(req.InvestorID, sizeof(req.InvestorID), gmd_param_->ctp_param->clientid.c_str());
  int iResult = ctp_->ReqQryTradingAccount(&req, ++nRequestID_);

  mylog(logpath_, L_INFO, "query trading account %d", iResult);
  return iResult;
}

int CtpTradeSpi::query_investor_position(const char *instrument) {
  CHECK_INITIAL

  CThostFtdcQryInvestorPositionField req;
  memset(&req, 0, sizeof(req));
  strcpy_s(req.BrokerID, sizeof(req.BrokerID), gmd_param_->ctp_param->brokerid.c_str());
  strcpy_s(req.InvestorID, sizeof(req.InvestorID), gmd_param_->ctp_param->clientid.c_str());
  if (instrument)
    strcpy_s(req.InstrumentID, sizeof(req.InstrumentID), instrument);

  int iResult = ctp_->ReqQryInvestorPosition(&req, ++nRequestID_);
  //	if (!iResult)
  //		m_nInvPos = 0;

  mylog(logpath_, L_INFO, "query investor position %d", iResult);
  return iResult;
}

int CtpTradeSpi::query_order(const char *instrument, const char *exchange,
                             const char *ordersysid, const char *begintime,
                             const char *endtime) {
  CThostFtdcQryOrderField QryOrder;
  strcpy_s(QryOrder.BrokerID, sizeof(TThostFtdcBrokerIDType),
           SessionInfo_.BrokerID);
  strcpy_s(QryOrder.InvestorID, sizeof(TThostFtdcInvestorIDType),
           SessionInfo_.UserID);
  if (instrument)
    strcpy_s(QryOrder.InstrumentID, sizeof(TThostFtdcInstrumentIDType),
             instrument);
  if (exchange)
    strcpy_s(QryOrder.ExchangeID, sizeof(TThostFtdcExchangeIDType), exchange);
  if (ordersysid)
    strcpy_s(QryOrder.OrderSysID, sizeof(TThostFtdcOrderSysIDType), ordersysid);
  if (begintime)
    strcpy_s(QryOrder.InsertTimeStart, sizeof(TThostFtdcTimeType), begintime);
  if (endtime)
    strcpy_s(QryOrder.InsertTimeEnd, sizeof(TThostFtdcTimeType), endtime);

  int iResult = ctp_->ReqQryOrder(&QryOrder, ++nRequestID_);
  mylog(logpath_, L_INFO, "query order %d", iResult);
  return (0);
}

int CtpTradeSpi::query_trade(const char *instrument, const char *exchange,
                             const char *tradeid, const char *begintime,
                             const char *endtime) {
  CThostFtdcQryTradeField QryTrade;
  strcpy_s(QryTrade.BrokerID, sizeof(TThostFtdcBrokerIDType),
           SessionInfo_.BrokerID);
  strcpy_s(QryTrade.InvestorID, sizeof(TThostFtdcInvestorIDType),
           SessionInfo_.UserID);
  if (instrument)
    strcpy_s(QryTrade.InstrumentID, sizeof(TThostFtdcInstrumentIDType),
             instrument);
  if (exchange)
    strcpy_s(QryTrade.ExchangeID, sizeof(TThostFtdcExchangeIDType), exchange);
  if (tradeid)
    strcpy_s(QryTrade.TradeID, sizeof(TThostFtdcTradeIDType), tradeid);
  if (begintime)
    strcpy_s(QryTrade.TradeTimeStart, sizeof(TThostFtdcTimeType), begintime);
  if (endtime)
    strcpy_s(QryTrade.TradeTimeEnd, sizeof(TThostFtdcTimeType), endtime);

  int iResult = ctp_->ReqQryTrade(&QryTrade, ++nRequestID_);
  mylog(logpath_, L_INFO, "query trade %d", iResult);
  return (0);
}

int CtpTradeSpi::order_insert_limit_price(
    const char *instrument, char cDirection, double fPrice, int nVolume,
    char cVolumeCondition, const char *strGTDdate, char cTimeCondition,
    int nMinVolume, char cTrigerCondition, double fStopPrice) {
  CHECK_INITIAL

  CThostFtdcInputOrderField req;
  memset(&req, 0, sizeof(req));

  strcpy_s(req.BrokerID, sizeof(req.BrokerID), SessionInfo_.BrokerID);
  strcpy_s(req.InvestorID, sizeof(req.InvestorID), SessionInfo_.UserID);
  strcpy_s(req.InstrumentID, sizeof(req.InstrumentID), instrument);
  itoa_s(ORDERREF++, req.OrderRef, sizeof(req.OrderRef), 10);
  strcpy_s(req.UserID, sizeof(TThostFtdcUserIDType), SessionInfo_.UserID);
  req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
  req.Direction = cDirection;
  req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
  req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
  req.LimitPrice = fPrice;
  req.VolumeTotalOriginal = nVolume;
  req.TimeCondition = cTimeCondition;
  if (cTimeCondition == '4')
    strcpy_s(req.GTDDate, sizeof(req.GTDDate), strGTDdate);
  req.VolumeCondition = cVolumeCondition;
  req.MinVolume = nMinVolume;
  req.ContingentCondition = cTrigerCondition;
  req.StopPrice = fStopPrice;
  req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
  req.IsAutoSuspend = 0;
  req.UserForceClose = 0;

  int iResult = ctp_->ReqOrderInsert(&req, ++nRequestID_);
  mylog(logpath_, L_INFO, "insert a limit price order");

  return iResult;
}

int CtpTradeSpi::order_insert_limitd(const char *instrument, char cDirection,
                                     double fPrice, int nVolume) {
  CHECK_INITIAL

  CThostFtdcInputOrderField req;
  memset(&req, 0, sizeof(req));

  strcpy_s(req.BrokerID, sizeof(req.BrokerID), SessionInfo_.BrokerID);
  strcpy_s(req.InvestorID, sizeof(req.InvestorID), SessionInfo_.UserID);
  strcpy_s(req.InstrumentID, sizeof(req.InstrumentID), instrument);
  itoa_s(ORDERREF++, req.OrderRef, sizeof(req.OrderRef), 10);
  strcpy_s(req.UserID, sizeof(TThostFtdcUserIDType), SessionInfo_.UserID);
  req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
  req.Direction = cDirection;
  req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
  req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
  req.LimitPrice = fPrice;
  req.VolumeTotalOriginal = nVolume;
  req.TimeCondition = THOST_FTDC_TC_GFD;
  req.VolumeCondition = THOST_FTDC_VC_AV;
  req.MinVolume = 0;
  req.ContingentCondition = THOST_FTDC_CC_Immediately;
  req.StopPrice = 0;
  req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
  req.IsAutoSuspend = 0;
  req.UserForceClose = 0;

  int iResult = ctp_->ReqOrderInsert(&req, ++nRequestID_);
  mylog(logpath_, L_INFO, "insert a limit price order %d", iResult);

  return iResult;
}

int CtpTradeSpi::order_insert_condtn(const char *instrument, char cDirection,
                                     double fPrice, int nVolume,
                                     double fConditionPrice,
                                     char cTrigerCondition,
                                     char cPriceCondition) {
  CHECK_INITIAL

  CThostFtdcInputOrderField req;
  memset(&req, 0, sizeof(req));

  strcpy_s(req.BrokerID, sizeof(req.BrokerID), SessionInfo_.BrokerID);
  strcpy_s(req.InvestorID, sizeof(req.InvestorID), SessionInfo_.UserID);
  strcpy_s(req.InstrumentID, sizeof(req.InstrumentID), instrument);
  itoa_s(ORDERREF++, req.OrderRef, sizeof(req.OrderRef), 10);
  strcpy_s(req.UserID, sizeof(TThostFtdcUserIDType), SessionInfo_.UserID);
  req.OrderPriceType = cPriceCondition;
  req.Direction = cDirection;
  req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
  req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
  req.LimitPrice = fPrice;
  req.VolumeTotalOriginal = nVolume;
  req.TimeCondition = THOST_FTDC_TC_GFD;
  req.VolumeCondition = THOST_FTDC_VC_AV;
  req.MinVolume = 0;
  req.ContingentCondition = cTrigerCondition;
  req.StopPrice = fConditionPrice;
  req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
  req.IsAutoSuspend = 0;
  req.UserForceClose = 0;

  int iResult = ctp_->ReqOrderInsert(&req, ++nRequestID_);
  mylog(logpath_, L_INFO, "insert a condition price order %d", iResult);

  return iResult;
}

int CtpTradeSpi::order_corver_condtn(const char *instrument, char cDirection,
                                     double fPrice, int nVolume,
                                     double fConditionPrice,
                                     char cTrigerCondition,
                                     char cPriceCondition) {
  CHECK_INITIAL

  CThostFtdcInputOrderField req;
  memset(&req, 0, sizeof(req));

  strcpy_s(req.BrokerID, sizeof(req.BrokerID), SessionInfo_.BrokerID);
  strcpy_s(req.InvestorID, sizeof(req.InvestorID), SessionInfo_.UserID);
  strcpy_s(req.InstrumentID, sizeof(req.InstrumentID), instrument);
  itoa_s(ORDERREF++, req.OrderRef, sizeof(req.OrderRef), 10);
  strcpy_s(req.UserID, sizeof(TThostFtdcUserIDType), SessionInfo_.UserID);
  req.OrderPriceType = cPriceCondition;
  req.Direction = cDirection;
  req.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
  req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
  req.LimitPrice = fPrice;
  req.VolumeTotalOriginal = nVolume;
  req.TimeCondition = THOST_FTDC_TC_GFD;
  req.VolumeCondition = THOST_FTDC_VC_AV;
  req.MinVolume = 0;
  req.ContingentCondition = cTrigerCondition;
  req.StopPrice = fConditionPrice;
  req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
  req.IsAutoSuspend = 0;
  req.UserForceClose = 0;

  int iResult = ctp_->ReqOrderInsert(&req, ++nRequestID_);
  mylog(logpath_, L_INFO, "corver a condition price order %d", iResult);

  return iResult;
}

int CtpTradeSpi::order_insert_parked() { return (0); }

int CtpTradeSpi::order_corver(const char *instrument, char cDirection,
                              double fPrice, int nVolume) {
  CHECK_INITIAL

  CThostFtdcInputOrderField req;
  memset(&req, 0, sizeof(req));

  strcpy_s(req.BrokerID, sizeof(req.BrokerID), SessionInfo_.BrokerID);
  strcpy_s(req.InvestorID, sizeof(req.InvestorID), SessionInfo_.UserID);
  strcpy_s(req.InstrumentID, sizeof(req.InstrumentID), instrument);
  itoa_s(ORDERREF++, req.OrderRef, sizeof(req.OrderRef), 10);
  strcpy_s(req.UserID, sizeof(TThostFtdcUserIDType), SessionInfo_.UserID);
  req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
  req.Direction = cDirection;
  req.CombOffsetFlag[0] = THOST_FTDC_OF_CloseYesterday;
  req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
  req.LimitPrice = fPrice;
  req.VolumeTotalOriginal = nVolume;
  req.TimeCondition = THOST_FTDC_TC_GFD;
  req.VolumeCondition = THOST_FTDC_VC_AV;
  req.MinVolume = 0;
  req.ContingentCondition = THOST_FTDC_CC_Immediately;
  req.StopPrice = 0;
  req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
  req.IsAutoSuspend = 0;
  req.UserForceClose = 0;

  int iResult = ctp_->ReqOrderInsert(&req, ++nRequestID_);
  mylog(logpath_, L_INFO, "cover an limit price order %d", iResult);

  return iResult;
}

int CtpTradeSpi::order_corver_today(const char *instrument, char cDirection,
                              double fPrice, int nVolume) {
  CHECK_INITIAL

  CThostFtdcInputOrderField req;
  memset(&req, 0, sizeof(req));

  strcpy_s(req.BrokerID, sizeof(req.BrokerID), SessionInfo_.BrokerID);
  strcpy_s(req.InvestorID, sizeof(req.InvestorID), SessionInfo_.UserID);
  strcpy_s(req.InstrumentID, sizeof(req.InstrumentID), instrument);
  itoa_s(ORDERREF++, req.OrderRef, sizeof(req.OrderRef), 10);
  strcpy_s(req.UserID, sizeof(TThostFtdcUserIDType), SessionInfo_.UserID);
  req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
  req.Direction = cDirection;
  req.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
  req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
  req.LimitPrice = fPrice;
  req.VolumeTotalOriginal = nVolume;
  req.TimeCondition = THOST_FTDC_TC_GFD;
  req.VolumeCondition = THOST_FTDC_VC_AV;
  req.MinVolume = 0;
  req.ContingentCondition = THOST_FTDC_CC_Immediately;
  req.StopPrice = 0;
  req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
  req.IsAutoSuspend = 0;
  req.UserForceClose = 0;

  int iResult = ctp_->ReqOrderInsert(&req, ++nRequestID_);
  mylog(logpath_, L_INFO, "cover an limit price order %d", iResult);

  return iResult;
}


int CtpTradeSpi::order_recall(CTPORDER *pOrder) {
  CHECK_INITIAL

  CThostFtdcInputOrderActionField req;
  memset(&req, 0, sizeof(req));

  strcpy_s(req.BrokerID, sizeof(req.BrokerID), pOrder->BrokerID);
  strcpy_s(req.InvestorID, sizeof(req.InvestorID), pOrder->InvestorID);
  strcpy_s(req.InstrumentID, sizeof(req.InstrumentID), pOrder->InstrumentID);
  strcpy_s(req.UserID, sizeof(TThostFtdcUserIDType), pOrder->UserID);
  req.OrderActionRef = ORDERREF++;
  strcpy_s(req.OrderRef, sizeof(req.OrderRef), pOrder->OrderRef);
  req.RequestID = pOrder->RequestID;
  req.FrontID = pOrder->FrontID;
  req.SessionID = pOrder->SessionID;
  strcpy_s(req.ExchangeID, sizeof(TThostFtdcExchangeIDType),
           pOrder->ExchangeID);
  strcpy_s(req.OrderSysID, sizeof(TThostFtdcOrderSysIDType),
           pOrder->OrderSysID);
  req.ActionFlag = THOST_FTDC_AF_Delete;
  req.LimitPrice = pOrder->LimitPrice;
  // TThostFtdcVolumeType	VolumeChange;

  int iResult = ctp_->ReqOrderAction(&req, ++nRequestID_);
  mylog(logpath_, L_INFO, "recall a normal order %d", iResult);

  return iResult;
}

char *CtpTradeSpi::GetOrderStatus(char status, char *buf, int len) {
  switch (status) {
  case '0':
    strcpy_s(buf, len, "all dealt");
    break;
  case '1':
    strcpy_s(buf, len, "part dealt in queue");
    break;
  case '2':
    strcpy_s(buf, len, "part dealt not in queue");
    break;
  case '3':
    strcpy_s(buf, len, "no deal in queue");
    break;
  case '4':
    strcpy_s(buf, len, "no deal in queue");
    break;
  case '5':
    strcpy_s(buf, len, "cancel");
    break;
  case 'a':
    strcpy_s(buf, len, "unknow");
    break;
  case 'b':
    strcpy_s(buf, len, "not touched");
    break;
  case 'c':
    strcpy_s(buf, len, "touched");
    break;
  default:
    strcpy_s(buf, len, "unknow status");
    break;
  }
  return buf;
}

void CtpTradeSpi::OnFrontConnected() {
  mylog(logpath_, L_INFO, "Trade Server was connected");
  int r = login();
  mylog(logpath_, L_INFO, "login return %d", r);
}

void CtpTradeSpi::OnFrontDisconnected(int nReason) {
  mylog(logpath_, L_INFO, "Trade Server was disconnected with code %d",
        nReason);
  ctp_->RegisterSpi(NULL);

  if (ctp_)
    ctp_->Release();
  ctp_ = 0;

  mylog(logpath_, L_INFO, "released user api");
}

void CtpTradeSpi::OnHeartBeatWarning(int nTimeLapse) { UNUSED(nTimeLapse); }
void CtpTradeSpi::OnRspAuthenticate(
    CThostFtdcRspAuthenticateField *pRspAuthenticateField,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  mylog(logpath_, L_INFO, "OnRspAuthenticate");
  ofstream out;
  getLogfile(out, logpath_);
  if (pRspAuthenticateField)
    out << *pRspAuthenticateField;
  if (pRspInfo)
    out << *pRspInfo;
  out << ", request " << nRequestID
      << ", last " << bIsLast << endl;
}

void CtpTradeSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
                                 CThostFtdcRspInfoField *pRspInfo,
                                 int nRequestID, bool bIsLast) {
  mylog(logpath_, L_INFO, "OnRspUserLogin");
  ofstream out;
  getLogfile(out, logpath_);
  out << *pRspUserLogin << *pRspInfo << ", request " << nRequestID << ", last "
      << bIsLast << endl;

  if (!pRspInfo->ErrorID) {
    set_session(pRspUserLogin);
    logined_ = true;
  }
  //prepare_environment(pRspUserLogin);
}

void CtpTradeSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout,
                                  CThostFtdcRspInfoField *pRspInfo,
                                  int nRequestID, bool bIsLast) {
  mylog(logpath_, L_INFO, "OnRspUserLogout");
  ofstream out;
  getLogfile(out, logpath_);
  if (pUserLogout)
    out << *pUserLogout;
  if (pRspInfo)
    out << *pRspInfo;
  out << ", request " << nRequestID << ", last "
      << bIsLast << endl;
}

void CtpTradeSpi::OnRspUserPasswordUpdate(
    CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pUserPasswordUpdate);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspTradingAccountPasswordUpdate(
    CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pTradingAccountPasswordUpdate);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder,
                                   CThostFtdcRspInfoField *pRspInfo,
                                   int nRequestID, bool bIsLast) {
  UNUSED(pInputOrder);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspParkedOrderInsert(
    CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo,
    int nRequestID, bool bIsLast) {
  UNUSED(pParkedOrder);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspParkedOrderAction(
    CThostFtdcParkedOrderActionField *pParkedOrderAction,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pParkedOrderAction);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspOrderAction(
    CThostFtdcInputOrderActionField *pInputOrderAction,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pInputOrderAction);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQueryMaxOrderVolume(
    CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pQueryMaxOrderVolume);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspSettlementInfoConfirm(
    CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  mylog(logpath_, L_INFO, "OnRspSettlementInfoConfirm");
  ofstream out;
  getLogfile(out, logpath_);
  if (pSettlementInfoConfirm)
    out << *pSettlementInfoConfirm;
  if (pRspInfo)
    out << *pRspInfo;

  out << ", request " << nRequestID
      << ", last " << bIsLast << endl;
}
void CtpTradeSpi::OnRspRemoveParkedOrder(
    CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pRemoveParkedOrder);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspRemoveParkedOrderAction(
    CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pRemoveParkedOrderAction);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspExecOrderInsert(
    CThostFtdcInputExecOrderField *pInputExecOrder,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pInputExecOrder);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspExecOrderAction(
    CThostFtdcInputExecOrderActionField *pInputExecOrderAction,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pInputExecOrderAction);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspForQuoteInsert(
    CThostFtdcInputForQuoteField *pInputForQuote,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pInputForQuote);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQuoteInsert(CThostFtdcInputQuoteField *pInputQuote,
                                   CThostFtdcRspInfoField *pRspInfo,
                                   int nRequestID, bool bIsLast) {
  UNUSED(pInputQuote);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQuoteAction(
    CThostFtdcInputQuoteActionField *pInputQuoteAction,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pInputQuoteAction);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspBatchOrderAction(
    CThostFtdcInputBatchOrderActionField *pInputBatchOrderAction,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pInputBatchOrderAction);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspCombActionInsert(
    CThostFtdcInputCombActionField *pInputCombAction,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pInputCombAction);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryOrder(CThostFtdcOrderField *pOrder,
                                CThostFtdcRspInfoField *pRspInfo,
                                int nRequestID, bool bIsLast) {
  mylog(logpath_, L_INFO, "OnRspQryOrder");
  ofstream out;
  getLogfile(out, logpath_);
  if (pOrder)
    out << *pOrder;
  if (pRspInfo)
    out<< *pRspInfo;

  out << ", request " << nRequestID << ", last "
      << bIsLast << endl;
}
void CtpTradeSpi::OnRspQryTrade(CThostFtdcTradeField *pTrade,
                                CThostFtdcRspInfoField *pRspInfo,
                                int nRequestID, bool bIsLast) {
  mylog(logpath_, L_INFO, "OnRspQryTrade");
  ofstream out;
  getLogfile(out, logpath_);
  if (pTrade)
    out << *pTrade;
  if (pRspInfo)
    out << *pRspInfo;
  out << ", request " << nRequestID << ", last "
      << bIsLast << endl;
}
void CtpTradeSpi::OnRspQryInvestorPosition(
    CThostFtdcInvestorPositionField *pInvestorPosition,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  mylog(logpath_, L_INFO, "OnRspQryInvestorPosition");
  ofstream out;
  getLogfile(out, logpath_);
  if (pInvestorPosition)
      out << *pInvestorPosition;
  if (pRspInfo)
      out << *pRspInfo;

  out << ", request " << nRequestID
      << ", last " << bIsLast << endl;
}
void CtpTradeSpi::OnRspQryTradingAccount(
    CThostFtdcTradingAccountField *pTradingAccount,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  mylog(logpath_, L_INFO, "OnRspQryTradingAccount");
  ofstream out;
  getLogfile(out, logpath_);
  if (pTradingAccount)
    out << *pTradingAccount;
  if (pRspInfo)
    out << *pRspInfo;

  out << ", request " << nRequestID
      << ", last " << bIsLast << endl;
}
void CtpTradeSpi::OnRspQryInvestor(CThostFtdcInvestorField *pInvestor,
                                   CThostFtdcRspInfoField *pRspInfo,
                                   int nRequestID, bool bIsLast) {
  UNUSED(pInvestor);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryTradingCode(CThostFtdcTradingCodeField *pTradingCode,
                                      CThostFtdcRspInfoField *pRspInfo,
                                      int nRequestID, bool bIsLast) {
  UNUSED(pTradingCode);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryInstrumentMarginRate(
  CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate,
  CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  if (pInstrumentMarginRate) {
    auto ptr = new CTPMARGIN();
    memcpy(ptr, pInstrumentMarginRate, sizeof(CTPMARGIN));
    map_mgn_.insert(std::make_pair(ptr->InstrumentID, ptr));


    mylog(logpath_, L_INFO, "OnRspQryInstrumentMarginRate");
    ofstream out;
    getLogfile(out, logpath_);
    out << *pInstrumentMarginRate;
    if (pRspInfo) out << *pRspInfo;
    out << ", request " << nRequestID << ", last " << bIsLast << endl;
  }
}
void CtpTradeSpi::OnRspQryInstrumentCommissionRate(
    CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  if (pInstrumentCommissionRate) {
    CTPCOMMISSION* cms = new CTPCOMMISSION();
    auto ptr = new CThostFtdcInstrumentCommissionRateField();
    memcpy(ptr, pInstrumentCommissionRate, sizeof(CThostFtdcInstrumentCommissionRateField));
    cms->cms_rate = ptr;
    map_cms_.insert(std::make_pair(ptr->InstrumentID, cms));

    mylog(logpath_, L_INFO, "OnRspQryInstrumentCommissionRate");
    ofstream out;
    getLogfile(out, logpath_);
    out << *pInstrumentCommissionRate;
    if (pRspInfo) out << *pRspInfo;
    out << ", request " << nRequestID << ", last " << bIsLast << endl;
  }
}
void CtpTradeSpi::OnRspQryExchange(CThostFtdcExchangeField *pExchange,
                                   CThostFtdcRspInfoField *pRspInfo,
                                   int nRequestID, bool bIsLast) {
  UNUSED(pExchange);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryProduct(CThostFtdcProductField *pProduct,
                                  CThostFtdcRspInfoField *pRspInfo,
                                  int nRequestID, bool bIsLast) {
  UNUSED(pProduct);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}



void CtpTradeSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument,
                                     CThostFtdcRspInfoField* pRspInfo,
                                     int nRequestID, bool bIsLast) {
  if (pInstrument && pRspInfo) {
    CThostFtdcInstrumentField* inst = new CThostFtdcInstrumentField();
    memcpy(inst, pInstrument, sizeof(CThostFtdcInstrumentField));
    map_inst_.insert(std::make_pair(inst->InstrumentID, inst));

    mylog(logpath_, L_INFO, "OnRspQryInstrument");
    ofstream out;
    getLogfile(out, logpath_);
    out << *pInstrument
        << *pRspInfo
        << ", request " << nRequestID << ", last " << bIsLast << endl;
  }
}

void CtpTradeSpi::OnRspQryDepthMarketData(
    CThostFtdcDepthMarketDataField *pDepthMarketData,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pDepthMarketData);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQrySettlementInfo(
    CThostFtdcSettlementInfoField *pSettlementInfo,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pSettlementInfo);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryTransferBank(
    CThostFtdcTransferBankField *pTransferBank,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pTransferBank);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryInvestorPositionDetail(
    CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pInvestorPositionDetail);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryNotice(CThostFtdcNoticeField *pNotice,
                                 CThostFtdcRspInfoField *pRspInfo,
                                 int nRequestID, bool bIsLast) {
  UNUSED(pNotice);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQrySettlementInfoConfirm(
    CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pSettlementInfoConfirm);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryInvestorPositionCombineDetail(
    CThostFtdcInvestorPositionCombineDetailField
        *pInvestorPositionCombineDetail,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pInvestorPositionCombineDetail);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryCFMMCTradingAccountKey(
    CThostFtdcCFMMCTradingAccountKeyField *pCFMMCTradingAccountKey,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pCFMMCTradingAccountKey);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryEWarrantOffset(
    CThostFtdcEWarrantOffsetField *pEWarrantOffset,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pEWarrantOffset);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryInvestorProductGroupMargin(
    CThostFtdcInvestorProductGroupMarginField *pInvestorProductGroupMargin,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pInvestorProductGroupMargin);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryExchangeMarginRate(
    CThostFtdcExchangeMarginRateField *pExchangeMarginRate,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pExchangeMarginRate);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryExchangeMarginRateAdjust(
    CThostFtdcExchangeMarginRateAdjustField *pExchangeMarginRateAdjust,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pExchangeMarginRateAdjust);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryExchangeRate(
    CThostFtdcExchangeRateField *pExchangeRate,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pExchangeRate);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQrySecAgentACIDMap(
    CThostFtdcSecAgentACIDMapField *pSecAgentACIDMap,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pSecAgentACIDMap);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryProductExchRate(
    CThostFtdcProductExchRateField *pProductExchRate,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pProductExchRate);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryProductGroup(
    CThostFtdcProductGroupField *pProductGroup,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pProductGroup);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryMMInstrumentCommissionRate(
    CThostFtdcMMInstrumentCommissionRateField *pMMInstrumentCommissionRate,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pMMInstrumentCommissionRate);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryMMOptionInstrCommRate(
    CThostFtdcMMOptionInstrCommRateField *pMMOptionInstrCommRate,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pMMOptionInstrCommRate);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryInstrumentOrderCommRate(
    CThostFtdcInstrumentOrderCommRateField *pInstrumentOrderCommRate,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pInstrumentOrderCommRate);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryOptionInstrTradeCost(
    CThostFtdcOptionInstrTradeCostField *pOptionInstrTradeCost,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pOptionInstrTradeCost);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryOptionInstrCommRate(
    CThostFtdcOptionInstrCommRateField *pOptionInstrCommRate,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pOptionInstrCommRate);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryExecOrder(CThostFtdcExecOrderField *pExecOrder,
                                    CThostFtdcRspInfoField *pRspInfo,
                                    int nRequestID, bool bIsLast) {
  UNUSED(pExecOrder);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryForQuote(CThostFtdcForQuoteField *pForQuote,
                                   CThostFtdcRspInfoField *pRspInfo,
                                   int nRequestID, bool bIsLast) {
  UNUSED(pForQuote);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryQuote(CThostFtdcQuoteField *pQuote,
                                CThostFtdcRspInfoField *pRspInfo,
                                int nRequestID, bool bIsLast) {
  UNUSED(pQuote);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryCombInstrumentGuard(
    CThostFtdcCombInstrumentGuardField *pCombInstrumentGuard,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pCombInstrumentGuard);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryCombAction(CThostFtdcCombActionField *pCombAction,
                                     CThostFtdcRspInfoField *pRspInfo,
                                     int nRequestID, bool bIsLast) {
  UNUSED(pCombAction);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryTransferSerial(
    CThostFtdcTransferSerialField *pTransferSerial,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pTransferSerial);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryAccountregister(
    CThostFtdcAccountregisterField *pAccountregister,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pAccountregister);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID,
                             bool bIsLast) {
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRtnOrder(CThostFtdcOrderField *pOrder) {
  if (ocb_ && pOrder) {
    ocb_(pOrder);
  }
}
void CtpTradeSpi::OnRtnTrade(CThostFtdcTradeField *pTrade) {
  if (tcb_ && pTrade) {
    tcb_(pTrade);
  }
}
void CtpTradeSpi::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder,
                                      CThostFtdcRspInfoField *pRspInfo) {
  UNUSED(pRspInfo);
  UNUSED(pInputOrder);
}
void CtpTradeSpi::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction,
                                      CThostFtdcRspInfoField *pRspInfo) {
  UNUSED(pRspInfo);
  UNUSED(pOrderAction);
}
void CtpTradeSpi::OnRtnInstrumentStatus(
    CThostFtdcInstrumentStatusField *pInstrumentStatus) {
  UNUSED(pInstrumentStatus);
}
void CtpTradeSpi::OnRtnBulletin(CThostFtdcBulletinField *pBulletin) {
  UNUSED(pBulletin);
}
void CtpTradeSpi::OnRtnTradingNotice(
    CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo) {
  UNUSED(pTradingNoticeInfo);
}
void CtpTradeSpi::OnRtnErrorConditionalOrder(
    CThostFtdcErrorConditionalOrderField *pErrorConditionalOrder) {
  UNUSED(pErrorConditionalOrder);
}
void CtpTradeSpi::OnRtnExecOrder(CThostFtdcExecOrderField *pExecOrder) {
  UNUSED(pExecOrder);
}
void CtpTradeSpi::OnErrRtnExecOrderInsert(
    CThostFtdcInputExecOrderField *pInputExecOrder,
    CThostFtdcRspInfoField *pRspInfo) {
  UNUSED(pRspInfo);
  UNUSED(pInputExecOrder);
}
void CtpTradeSpi::OnErrRtnExecOrderAction(
    CThostFtdcExecOrderActionField *pExecOrderAction,
    CThostFtdcRspInfoField *pRspInfo) {
  UNUSED(pRspInfo);
  UNUSED(pExecOrderAction);
}
void CtpTradeSpi::OnErrRtnForQuoteInsert(
    CThostFtdcInputForQuoteField *pInputForQuote,
    CThostFtdcRspInfoField *pRspInfo) {
  UNUSED(pRspInfo);
  UNUSED(pInputForQuote);
}
void CtpTradeSpi::OnRtnQuote(CThostFtdcQuoteField *pQuote) { UNUSED(pQuote); }
void CtpTradeSpi::OnErrRtnQuoteInsert(CThostFtdcInputQuoteField *pInputQuote,
                                      CThostFtdcRspInfoField *pRspInfo) {
  UNUSED(pRspInfo);
  UNUSED(pInputQuote);
}
void CtpTradeSpi::OnErrRtnQuoteAction(CThostFtdcQuoteActionField *pQuoteAction,
                                      CThostFtdcRspInfoField *pRspInfo) {
  UNUSED(pRspInfo);
  UNUSED(pQuoteAction);
}
void CtpTradeSpi::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) {
  UNUSED(pForQuoteRsp);
}
void CtpTradeSpi::OnRtnCFMMCTradingAccountToken(
    CThostFtdcCFMMCTradingAccountTokenField *pCFMMCTradingAccountToken) {
  UNUSED(pCFMMCTradingAccountToken);
}
void CtpTradeSpi::OnErrRtnBatchOrderAction(
    CThostFtdcBatchOrderActionField *pBatchOrderAction,
    CThostFtdcRspInfoField *pRspInfo) {
  UNUSED(pRspInfo);
  UNUSED(pBatchOrderAction);
}
void CtpTradeSpi::OnRtnCombAction(CThostFtdcCombActionField *pCombAction) {
  UNUSED(pCombAction);
}
void CtpTradeSpi::OnErrRtnCombActionInsert(
    CThostFtdcInputCombActionField *pInputCombAction,
    CThostFtdcRspInfoField *pRspInfo) {
  UNUSED(pRspInfo);
  UNUSED(pInputCombAction);
}
void CtpTradeSpi::OnRspQryContractBank(
    CThostFtdcContractBankField *pContractBank,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pContractBank);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder,
                                      CThostFtdcRspInfoField *pRspInfo,
                                      int nRequestID, bool bIsLast) {
  UNUSED(pParkedOrder);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryParkedOrderAction(
    CThostFtdcParkedOrderActionField *pParkedOrderAction,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pParkedOrderAction);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryTradingNotice(
    CThostFtdcTradingNoticeField *pTradingNotice,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pTradingNotice);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryBrokerTradingParams(
    CThostFtdcBrokerTradingParamsField *pBrokerTradingParams,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pBrokerTradingParams);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQryBrokerTradingAlgos(
    CThostFtdcBrokerTradingAlgosField *pBrokerTradingAlgos,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pBrokerTradingAlgos);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQueryCFMMCTradingAccountToken(
    CThostFtdcQueryCFMMCTradingAccountTokenField
        *pQueryCFMMCTradingAccountToken,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pQueryCFMMCTradingAccountToken);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRtnFromBankToFutureByBank(
    CThostFtdcRspTransferField *pRspTransfer) {
  UNUSED(pRspTransfer);
}
void CtpTradeSpi::OnRtnFromFutureToBankByBank(
    CThostFtdcRspTransferField *pRspTransfer) {
  UNUSED(pRspTransfer);
}
void CtpTradeSpi::OnRtnRepealFromBankToFutureByBank(
    CThostFtdcRspRepealField *pRspRepeal) {
  UNUSED(pRspRepeal);
}
void CtpTradeSpi::OnRtnRepealFromFutureToBankByBank(
    CThostFtdcRspRepealField *pRspRepeal) {
  UNUSED(pRspRepeal);
}
void CtpTradeSpi::OnRtnFromBankToFutureByFuture(
    CThostFtdcRspTransferField *pRspTransfer) {
  UNUSED(pRspTransfer);
}
void CtpTradeSpi::OnRtnFromFutureToBankByFuture(
    CThostFtdcRspTransferField *pRspTransfer) {
  UNUSED(pRspTransfer);
}
void CtpTradeSpi::OnRtnRepealFromBankToFutureByFutureManual(
    CThostFtdcRspRepealField *pRspRepeal) {
  UNUSED(pRspRepeal);
}
void CtpTradeSpi::OnRtnRepealFromFutureToBankByFutureManual(
    CThostFtdcRspRepealField *pRspRepeal) {
  UNUSED(pRspRepeal);
}
void CtpTradeSpi::OnRtnQueryBankBalanceByFuture(
    CThostFtdcNotifyQueryAccountField *pNotifyQueryAccount) {
  UNUSED(pNotifyQueryAccount);
}
void CtpTradeSpi::OnErrRtnBankToFutureByFuture(
    CThostFtdcReqTransferField *pReqTransfer,
    CThostFtdcRspInfoField *pRspInfo) {
  UNUSED(pRspInfo);
  UNUSED(pReqTransfer);
}
void CtpTradeSpi::OnErrRtnFutureToBankByFuture(
    CThostFtdcReqTransferField *pReqTransfer,
    CThostFtdcRspInfoField *pRspInfo) {
  UNUSED(pRspInfo);
  UNUSED(pReqTransfer);
}
void CtpTradeSpi::OnErrRtnRepealBankToFutureByFutureManual(
    CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo) {
  UNUSED(pRspInfo);
  UNUSED(pReqRepeal);
}
void CtpTradeSpi::OnErrRtnRepealFutureToBankByFutureManual(
    CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo) {
  UNUSED(pRspInfo);
  UNUSED(pReqRepeal);
}
void CtpTradeSpi::OnErrRtnQueryBankBalanceByFuture(
    CThostFtdcReqQueryAccountField *pReqQueryAccount,
    CThostFtdcRspInfoField *pRspInfo) {
  UNUSED(pRspInfo);
  UNUSED(pReqQueryAccount);
}
void CtpTradeSpi::OnRtnRepealFromBankToFutureByFuture(
    CThostFtdcRspRepealField *pRspRepeal) {
  UNUSED(pRspRepeal);
}
void CtpTradeSpi::OnRtnRepealFromFutureToBankByFuture(
    CThostFtdcRspRepealField *pRspRepeal) {
  UNUSED(pRspRepeal);
}
void CtpTradeSpi::OnRspFromBankToFutureByFuture(
    CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo,
    int nRequestID, bool bIsLast) {
  UNUSED(pReqTransfer);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspFromFutureToBankByFuture(
    CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo,
    int nRequestID, bool bIsLast) {
  UNUSED(pReqTransfer);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRspQueryBankAccountMoneyByFuture(
    CThostFtdcReqQueryAccountField *pReqQueryAccount,
    CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
  UNUSED(pReqQueryAccount);
  UNUSED(pRspInfo);
  UNUSED(nRequestID);
  UNUSED(bIsLast);
}
void CtpTradeSpi::OnRtnOpenAccountByBank(
    CThostFtdcOpenAccountField *pOpenAccount) {
  UNUSED(pOpenAccount);
}
void CtpTradeSpi::OnRtnCancelAccountByBank(
    CThostFtdcCancelAccountField *pCancelAccount) {
  UNUSED(pCancelAccount);
}
void CtpTradeSpi::OnRtnChangeAccountByBank(
    CThostFtdcChangeAccountField *pChangeAccount) {
  UNUSED(pChangeAccount);
}

//#define CHECK_INITIAL { if (!m_pUser) return (-100); }
#define CHECK_INITIAL


int connect_to_ctp_tdif(CtpTradeSpi * trade_spi) {
  if (!trade_spi)
    return -100;

  return trade_spi->run();
}


CtpTradeSpi* get_ctp_tdif(GmdParam* param, const char* logpath) {
  if (!param) return nullptr;

  return new CtpTradeSpi(param, logpath);
}



void print_margin_json(ostream& out, const MapInstrument &mi) {
  out << "{" ;

  for (auto& inst : mi) {
    if (inst.second)
      out << "\'" << (inst.second)->InstrumentID << "\':" << (inst.second)->LongMarginRatio << ",";
  }
  out << "}" << std::endl;

  out << "{" ;
  for (auto& inst : mi) {
    if (inst.second)
      out << "\'" << (inst.second)->InstrumentID << "\':" << (inst.second)->LongMarginRatio << ",";
  }
  out << "}" << std::endl;

  out << "{" ;
  for (auto& inst : mi) {
    if (inst.second)
      out << "\'" << (inst.second)->InstrumentID << "\':" << (inst.second)->VolumeMultiple << ",";
  }
  out << "}" << std::endl;
}
