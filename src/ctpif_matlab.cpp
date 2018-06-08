#include "ctpif.h"
#include "ctpif_matlab.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief get ctp-parameters from the file
 * @param filepath, the config file in json
 * @return zero for suceess, non-zero for error
 */
CONFIGPTR mexInitConfig(const char *filepath) {
  return initConfig(filepath);
}

/**
 * @brief releaseCtpMdIf Release the Ctp Interface for market data
 */
void mexReleaseConfig(CONFIGPTR param) {
  return releaseConfig((GmdParam*)param);
}

void mexConnectToCtpTdif(INTERFACE interface) {
  return connect_to_ctp_tdif((CtpTradeSpi*)interface);
}

INTERFACE mexGetCtpMdif(CONFIGPTR param) {
  return get_ctp_mdif((GmdParam*)param);
}

INTERFACE mexGetCtpTdif(CONFIGPTR param, const char *logpath) {
  return get_ctp_tdif((GmdParam*)param, logpath);
}

void mexConnectToCtpMdif(INTERFACE interface) {
  return connect_to_ctp_mdif((CtpMdSpi*) interface);
}


void mexReleaseCtpMdif(INTERFACE mdif) {
  if (mdif) delete ((CtpMdSpi*)mdif);
}


void mexReleaseCtpTdif(INTERFACE tdif) {
  if (tdif) delete ((CtpTradeSpi*)tdif);
}




int mexStop(INTERFACE tdif) {
    return ((CtpTradeSpi*)tdif)->stop();
}

int mexLogin(INTERFACE tdif) {
    return ((CtpTradeSpi*)tdif)->login();
}

int mexLogout(INTERFACE tdif) {
    return ((CtpTradeSpi*)tdif)->logout();
}

int mexQueryInstrument(INTERFACE tdif, const char *instrument) {
    return ((CtpTradeSpi*)tdif)->query_instrument(instrument);
}

int mexQueryAccount(INTERFACE tdif) {
    return ((CtpTradeSpi*)tdif)->query_account();
}

int mexQueryInvestorPosition(INTERFACE tdif, const char *instrument) {
    return ((CtpTradeSpi*)tdif)->query_investor_position(instrument);
}

int mexQueryOrder(INTERFACE tdif, const char *instrument, const char *exchange,
                const char *ordersysid, const char *begintime,
                const char *endtime) {
    return ((CtpTradeSpi*)tdif)->query_order(instrument, exchange,
                                             ordersysid, begintime,
                                             endtime);
}

int mexQueryTrade(INTERFACE tdif, const char *instrument, const char *exchange,
                const char *tradeid, const char *begintime,
                const char *endtime) {
    return ((CtpTradeSpi*)tdif)->query_trade(instrument, exchange,
                                             tradeid, begintime,
                                             endtime);
}

int mexOrderInsertLimitPrice(INTERFACE tdif, const char *instrument, char cDirection,
            double fPrice, int nVolume,
            char cVolumeCondition, const char *strGTDdate,
            char cTimeCondition, int nMinVolume,
            char cTrigerCondition, double fStopPrice) {
    return ((CtpTradeSpi*)tdif)->order_insert_limit_price(instrument, cDirection,
                                                          fPrice, nVolume,
                                                          cVolumeCondition, strGTDdate,
                                                          cTimeCondition, nMinVolume,
                                                          cTrigerCondition, fStopPrice);
}

int mexOrderInsertLimitd(INTERFACE tdif, const char *instrument, char cDirection,
                        double fPrice, int nVolume) {
    return ((CtpTradeSpi*)tdif)->order_insert_limitd(instrument, cDirection,
                                                     fPrice, nVolume);
}

int mexOrderInsertCondtn(INTERFACE tdif, const char *instrument, char cDirection,
                        double fPrice, int nVolume, double fConditionPrice,
                        char cTrigerCondition, char cPriceCondition) {
    return ((CtpTradeSpi*)tdif)->order_insert_condtn(instrument, cDirection,
                                                     fPrice, nVolume, fConditionPrice,
                                                     cTrigerCondition, cPriceCondition);
}

int mexOrderCorverCondtn(INTERFACE tdif, const char *instrument, char cDirection,
                        double fPrice, int nVolume, double fConditionPrice,
                        char cTrigerCondition, char cPriceCondition) {
    return ((CtpTradeSpi*)tdif)->order_corver_condtn(instrument, cDirection,
                                                     fPrice, nVolume, fConditionPrice,
                                                     cTrigerCondition, cPriceCondition);
}

int mexOrderInsertParked(INTERFACE tdif) {
    return ((CtpTradeSpi*)tdif)->order_insert_parked();
}

int mexOrderCorver(INTERFACE tdif, const char *instrument, char cDirection, double fPrice,
                 int nVolume) {
    return ((CtpTradeSpi*)tdif)->order_corver(instrument, cDirection, fPrice, nVolume);
}

int mexOrderRecall(INTERFACE tdif, Order *order) {
    // return ((CtpTradeSpi*)tdif)->order_recall(order);
    return (0);
}

char *mexGetOrderStatus(INTERFACE tdif, char status, char *buf, int len) {
    return ((CtpTradeSpi*)tdif)->GetOrderStatus(status, buf, len);
}

#ifdef __cplusplus
}
#endif
