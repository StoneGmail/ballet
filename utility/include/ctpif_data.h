#ifndef GOMMEM_H
#define GOMMEM_H


#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"
#include "ThostFtdcMdApi.h"
#include "ThostFtdcTraderApi.h"

#include "ctpif_global.h"
#include "utility.h"
#include "mem.h"
#include "dataset.h"

#include "ctpif_data.h"
#include "ctpifio.h"

typedef CThostFtdcDepthMarketDataField CtpMarketData;
typedef struct CtpMarketDataInfo {
  DataInfo di;
  char trade_day[9];
  CtpMarketDataInfo() : di(sizeof(CtpMarketData), sizeof(CtpMarketDataInfo)) {}
} CtpMarketDataInfo;

typedef vector<string> CtpServers;
typedef vector<string> CtpInstruments;

typedef TThostFtdcDateType DateType;

/// struct for CTP Parameters
typedef struct CtpParam {
  string dif_name;           /// data-interface name, must be "ctp"
  string ctpmd_logpath;      /// ctp marketing data log path
  string brokerid;           /// broker id
  string clientid;           /// client id
  string password;           /// password
  bool udp;                  /// using udp protocol
  CtpServers mduri;          /// URI of Market Data servers
  CtpServers tduri;          /// URI of Trade servers
  string inst_file;          /// instruments list file
  CtpInstruments instrument; /// instruments
  string shred_mem_name;     /// shared memrory name
} CtpParam;


typedef struct GmdParam {
  uint32_t max_tick_per_sec;  /// max amount of tick data per second
  uint32_t max_time_long;     /// max trading duringtion in one day, in second
  uint32_t tick_buffer_num;   /// amout number of tick buffer
  uint32_t save_tickbuf_time; /// interval time for save tick buffer to
                              /// disk(persistent), in minutes

  char gmdlog[MAX_PATH];          /// the log filepath for go-markting-data
  char gtdlog[MAX_PATH];          /// the log filepath for trade
  char data_store_path[MAX_PATH]; /// persistent all data, include kindle data,
                                  /// etc.
  char cal_store_path[MAX_PATH];  /// calculate result for MA, MACD or something
                                  /// etc.
  char tick_data_path[MAX_PATH];  /// buffer for tick_data_path
  char data_rtpath[MAX_PATH];     /// realtime CTPMMD struct for calculate or
                                  /// analysis
  char tick_bin_path[MAX_PATH];   /// The path to save tick-binary file and
                                  /// tick-cdi file
  uint32_t save_loop_interval;    /// the interval time for save tick data loop

  string exit_time; /// exit time

  CtpParam* ctp_param;
} GmdParam;

typedef enum CTPMD_STATUS {
  CTPMD_STATUS_DISCONNECTED = -2,
  CTPMD_STATUS_NO_STATUS = -1,
  CTPMD_STATUS_CONNECTED = 0,
  CTPMD_STATUS_USER_LOGIN,
  CTPMD_STATUS_USER_LOGOUT,
  CTPMD_STATUS_HEARTBEAT,
  CTPMD_STATUS_ERROR,
  CTPMD_STATUS_SUBSCRIPT,
  CTPMD_STATUS_UNSUBSCRIPT,
  CTPMD_STATUS_DEPTH_MD,
  CTPMD_STATUS_UNSUB_QUOTERSP,
  CTPMD_STATUS_SUB_QUOTERSP,
  CTPMD_STATUS_QUOTERSP,
} CTPMD_STATUS;

typedef struct CtpStatus {
    CTPMD_STATUS status;
    uint32_t data;
} CtpStatus;

typedef struct CTPMMD {
  TThostFtdcPriceType LastPrice;
  TThostFtdcPriceType BidPrice1;
  TThostFtdcPriceType AskPrice1;
  // TThostFtdcMoneyType Turnover;
  TThostFtdcLargeVolumeType OpenInterest;
  time_t UpdateTime;
  TThostFtdcVolumeType Volume;
  TThostFtdcVolumeType BidVolume1;
  TThostFtdcVolumeType AskVolume1;
  TThostFtdcMillisecType UpdateMillisec;
} CTPMMD;

typedef struct CTPMD {
  ftime_t key;
  PriceType LastPrice;
  PriceType BidPrice1;
  PriceType AskPrice1;
  VolumeType Volume;
  VolumeType BidVolume1;
  VolumeType AskVolume1;
  VolumeType OpenInterest;
  MoneyType Turnover;
} CTPMD;

extern CTPIF_API std::ostream& operator << (std::ostream&, CTPMD& ctpmd);

typedef struct CTPMDInfo {
  DataInfo di;

  ///浜ゆ槗鏃�
  TThostFtdcDateType TradingDay;
  ///鍚堢害浠ｇ爜
  TThostFtdcInstrumentIDType InstrumentID;
  ///浜ゆ槗鎵€浠ｇ爜
  TThostFtdcExchangeIDType ExchangeID;
  ///鍚堢害鍦ㄤ氦鏄撴墍鐨勪唬鐮�
  TThostFtdcExchangeInstIDType ExchangeInstID;
  ///涓婃缁撶畻浠�
  TThostFtdcPriceType PreSettlementPrice;
  ///鏄ㄦ敹鐩�
  TThostFtdcPriceType PreClosePrice;
  ///鏄ㄦ寔浠撻噺
  TThostFtdcLargeVolumeType PreOpenInterest;
  ///浠婂紑鐩�
  TThostFtdcPriceType OpenPrice;
  ///鏈缁撶畻浠�
  TThostFtdcPriceType SettlementPrice;
  ///娑ㄥ仠鏉夸环
  TThostFtdcPriceType UpperLimitPrice;
  ///璺屽仠鏉夸环
  TThostFtdcPriceType LowerLimitPrice;
  ///鏄ㄨ櫄瀹炲害
  TThostFtdcRatioType PreDelta;
  ///浠婅櫄瀹炲害
  TThostFtdcRatioType CurrDelta;
  ///褰撴棩鍧囦环
  TThostFtdcPriceType AveragePrice;
  ///涓氬姟鏃ユ湡
  TThostFtdcDateType ActionDay;

  CTPMDInfo() : di(sizeof(CTPMD), sizeof(CTPMDInfo)) {
  }
} CTPMDInfo;

//
// typedef struct CtpmmdDataBuffer {
//   int insind;     /// index of the instrument in vector
//   int pos;        /// the amount number of received data, start at -1
//   CTPMMD *ctpmmd; /// pointer to memory of CTPMMD
// } CtpmmdDataBuffer;
//
// typedef struct CtpDataBuffer {
//   int pos;                /// the amount number of received data, start at -1
//   bool isworking;         /// indicate this buffer is working for now
//   CtpMarketData *ctpdata; /// pointer to memory of storage of ctp-market-data
// } CtpDataBuffer;
//
// typedef std::map<std::string, CtpmmdDataBuffer *> MapStr2CtpmmdDataPtr;
// typedef MapStr2CtpmmdDataPtr::iterator MsdIter;
// typedef std::pair<std::string, CtpmmdDataBuffer *> PairStrCtpmmdDataPtr;
//
// class CTPIF_API CtpMdMemory {
// public:
//   CtpMdMemory(bool has_ctpmmd);
//   ~CtpMdMemory();
//
//   int initMemory();
//   int destroyMemory();
//
//   CtpDataBuffer *getTickbuf(int index);
//   CtpmmdDataBuffer *getRealtimeMmd(std::string instrument);
//   CtpmmdDataBuffer *getRealtimeMmd(int index);
//
//   int addNewTick(CtpMarketData *data);
//
//   int getTickbufIndex();
//
//   size_t getTickAmount() { return amountTick_; }
//   size_t getCtpmmdAmount() { return amountCtpmmd_; }
//
// private:
//   SharedMemory **rtBuffer_; /// those buffer to save realtime tick data
//   SharedMemory *
//       *mdPtr_; /// those pointer of market data for calculate and analysis
//   /// each pointer indicate one instrument of ctp
//   MapStr2CtpmmdDataPtr maps2p_;
//   int cur_tick_buf_;
//   int max_size_tick_buf_;
//
//   CtpDataBuffer **ctpbuf_;
//
//
//   bool has_ctpmmd_;
//   size_t amountTick_;
//   size_t amountCtpmmd_;
//
// };
// extern CTPIF_API CtpMdMemory *gmdmem;

// /*! \class CtpMdWriter
//   \brief store the ctp raw datad and CTPMD raw data
//
//   the struct of memory block are
//   +---------------------------------------------+
//   | Ctp Tick Raw Data Buffer 1                  |
//   +---------------------------------------------+
//   | Ctp Tick Raw Data Buffer 2                  |
//   +---------------------------------------------+
//   | ...                                         |
//   +---------------------------------------------+
//   | Ctp Tick Raw Data Buffer N                  |
//   +---------------------------------------------+
//   | CTPMD Raw Data, Instrument N                |
//   +---------------------------------------------+
//   | CTPMD Raw Data, Instrument 1                |
//   +---------------------------------------------+
//   | CTPMD Raw Data, Instrument 2                |
//   +---------------------------------------------+
//   | ...                                         |
//   +---------------------------------------------+
//   | CTPMD Raw Data, Instrument N                |
//   +---------------------------------------------+
//
//   each Ctp Raw block has DataSet struct:
//   +---------------------------------------------+
//   | DataInfo | DataSet ...                      |
//   +---------------------------------------------+
//   and each CTPMD Raw block has TsDataFrame struct:
//   +---------------------------------------------+
//   | DataInfo | DataFrame ...                    |
//   +---------------------------------------------+
// */
// class CTPIF_API CtpMdWriter {
// public:
//   CtpMdWriter(bool has_ctpmmd = true);
//   ~CtpMdWriter();
//
//   int initMemory();
//   int destroyMemory();
//
//   DATASET(CtpMarketData) *getTickbuf(int64_t index);
//   TSDATAFRAME(CTPMD) *getRealtimeMd(std::string instrument);
//   TSDATAFRAME(CTPMD) *getRealtimeMd(int64_t index);
//
//   int addNewTick(CtpMarketData *data);
//
//   int64_t getTickbufIndex();
//
//   size_t getTickAmount() { return amountTick_; }
//   size_t getCtpmmdAmount() { return amountCtpmmd_; }
//
//   friend class CtpMdMemoryManager;
//
// private:
//   int mapping_memory();
//
// private:
//   SharedMemory* shared_mem_;
//   DATASET(CtpMarketData)** ctp_data_set_;
//   TSDATAFRAME(CTPMD)** ctpmd_df_;
//
//   int64_t cur_tick_buf_;
//   size_t tick_buf_size_;
//   size_t ctpmd_buf_size_;
//   size_t memsize_;
//   bool has_ctpmmd_;
//   size_t amountTick_;
//   size_t amountCtpmmd_;
//   MapStr2Ptr maps2p_;
// };
//

typedef TSDATAFRAME(CTPMD) CtpmdDataFrame;
typedef DATASET(CtpMarketData) CtpMarketDataSet;

typedef DataInterfaceMemory<CtpMarketData, CtpMarketDataInfo, CTPMD, CTPMDInfo> CtpWriter;
typedef DataInterfaceMemory<CtpMarketData, CtpMarketDataInfo, CTPMD, CTPMDInfo> CtpReader;

//
// enum ExecType {
//   ExecType_New = 1,
//   ExecType_DoneForDay = 4,
//   ExecType_Canceled = 5,
//   ExecType_PendingCancel = 6,
//   ExecType_Stopped = 7,
//   ExecType_Rejected = 8,
//   ExecType_Suspended = 9,
//   ExecType_PendingNew = 10,
//   ExecType_Calculated = 11,
//   ExecType_Expired = 12,
//   ExecType_Restated = 13,
//   ExecType_PendingReplace = 14,
//   ExecType_Trade = 15,
//   ExecType_TradeCorrect = 16,
//   ExecType_TradeCancel = 17,
//   ExecType_OrderStatus = 18,
//   ExecType_CancelRejected = 19
// };
//
//
// enum OrderStatus {
//   OrderStatus_New = 1,
//   OrderStatus_PartiallyFilled = 2,
//   OrderStatus_Filled = 3,
//   OrderStatus_DoneForDay = 4,
//   OrderStatus_Canceled = 5,
//   OrderStatus_PendingCancel = 6,
//   OrderStatus_Stopped = 7,
//   OrderStatus_Rejected = 8,
//   OrderStatus_Suspended = 9,
//   OrderStatus_PendingNew = 10,
//   OrderStatus_Calculated = 11,
//   OrderStatus_Expired = 12,
//   OrderStatus_AcceptedForBidding = 13,
//   OrderStatus_PendingReplace = 14
// };
//
//
// enum CancelOrderRejectReason {
//   CancelOrderRejectReason_OrderFinalized = 101,
//   CancelOrderRejectReason_UnknownOrder = 102,
//   CancelOrderRejectReason_BrokerOption = 103,
//   CancelOrderRejectReason_AlreadyInPendingCancel = 104
// };
//
//
// enum OrderSide {
//   OrderSide_Bid = 1,
//   OrderSide_Ask = 2
// };
//
//
// enum OrderType {
//     OrderType_LMT = 0,
//     OrderType_BOC = 1,
//     OrderType_BOP = 2,
//     OrderType_B5TC = 3,
//     OrderType_B5TL = 4,
//     OrderType_IOC = 5,
//     OrderType_FOK = 6,
//     OrderType_AON = 7,
//     OrderType_MTL = 8,
//     OrderType_EXE = 9
// };
//
//
// enum PositionEffect {
//     PositionEffect_Open = 1,
//     PositionEffect_Close = 2,
//     PositionEffect_CloseToday = 3,
//     PositionEffect_CloseYesterday = 4
// };
//
//
// enum OrderRejectReason {
//   OrderRejectReason_UnknownReason = 1,
//   OrderRejectReason_RiskRuleCheckFailed = 2,
//   OrderRejectReason_NoEnoughCash = 3,
//   OrderRejectReason_NoEnoughPosition = 4,
//   OrderRejectReason_IllegalStrategyID = 5,
//   OrderRejectReason_IllegalSymbol = 6,
//   OrderRejectReason_IllegalVolume = 7,
//   OrderRejectReason_IllegalPrice = 8,
//   OrderRejectReason_NoMatchedTradingChannel = 9,
//   OrderRejectReason_AccountForbidTrading = 10,
//   OrderRejectReason_TradingChannelNotConnected = 11,
//   OrderRejectReason_StrategyForbidTrading = 12,
//   OrderRejectReason_NotInTradingSession = 13
// };
//
//
// /**
// * @brief 濮旀墭鏁版嵁瀹氫箟
// *
// */
// typedef struct Order
// {
//     char strategy_id[LEN_ID];       ///绛栫暐id
//     char account_id[LEN_ID];        ///甯愬彿id, 绯荤粺鑷姩鏍规嵁閰嶇疆琛ュ厖
//
//     char cl_ord_id[LEN_ID];         ///濮旀墭璁㈠崟鐨勫鎴锋柟id
//     char order_id[LEN_ID];          ///濮旀墭璁㈠崟鐨勬煖鍙扮敓鎴恑d
//     char ex_ord_id[LEN_ID];         ///濮旀墭璁㈠崟鐨勪氦鏄撴墍鎺ュ彈鍚庣敓鎴恑d
//
//     char exchange[LEN_EXCHANGE];    ///浜ゆ槗鎵€浠ｇ爜
//     char sec_id[LEN_SECID];         ///浜ゆ槗鏍囩殑浠ｇ爜锛屾湰鍦颁唬鐮佸IF1502
//
//     int position_effect;            //寮€骞虫爣蹇楋紝鍙栧€煎弬鑰僥num PositionEffect
//     int side;                       //涔板崠鏂瑰悜锛屽彇鍊煎弬鑰僥num OrderSide
//     int order_type;                 //濮旀墭绫诲瀷锛屽彇鍊煎弬鑰僥num OrderType
//     int order_src;                  //濮旀墭鏉ユ簮锛屽彇鍊煎弬鑰僥num OrderSrc
//     int status;                     //濮旀墭鐘舵€侊紝鍙栧€煎弬鑰僥num OrderStatus
//     int ord_rej_reason;             //濮旀墭鎷掔粷鍘熷洜锛屽彇鍊煎弬鑰僥num OrderRejectReason
//     char ord_rej_reason_detail[LEN_INFO];   ///濮旀墭琚嫆缁濆師鍥犳弿杩�
//
//     double price;                   //濮旀墭浠锋牸
//     double stop_price;              //姝㈡崯浠�
//     double volume;                  //濮旀墭閲�
//     double filled_volume;           //宸叉垚閲�
//     double filled_vwap;             //宸叉垚鍧囦环
//     double filled_amount;           //宸叉垚閲戦
//
//     double sending_time;            //濮旀墭鍙戦€佹椂闂�
//     double transact_time;           //濮旀墭澶勭悊鏃堕棿
// }Order;
//
//
// /**
// * @brief 濮旀墭鍥炴姤鏁版嵁瀹氫箟
// *
// */
// typedef struct ExecRpt
// {
//     char strategy_id[LEN_ID];       ///绛栫暐id
//
//     char cl_ord_id[LEN_ID];         ///濮旀墭璁㈠崟鐨勫鎴锋柟id
//     char order_id[LEN_ID];          ///濮旀墭璁㈠崟鐨勬煖鍙扮敓鎴恑d
//     char exec_id[LEN_ID];           ///濮旀墭璁㈠崟鐨勪氦鏄撴墍鐢熸垚id
//
//     char exchange[LEN_EXCHANGE];    ///浜ゆ槗鎵€浠ｇ爜
//     char sec_id[LEN_SECID];         ///浜ゆ槗鏍囩殑浠ｇ爜锛屾湰鍦颁唬鐮佸IF1502
//
//     int position_effect;            ///寮€骞充粨鏍囪瘑
//     int side;                       ///涔板崠鏂瑰悜
//     int ord_rej_reason;             ///濮旀墭琚嫆缁濆師鍥犵被鍨�
//     char ord_rej_reason_detail[LEN_INFO];  ///濮旀墭琚嫆缁濆師鍥犳弿杩�
//     int exec_type;
//
//     double price;                   ///鎴愪氦浠锋牸
//     double volume;                  ///鎴愪氦鏁伴噺
//     double amount;                  ///鎴愪氦閲戦
//
//     double transact_time;           ///鎴愪氦鏃堕棿
// }ExecRpt;
//
//
// /**
// * @brief 璧勯噾甯愭埛鐨勬暟鎹畾涔�
// *
// */
// typedef struct Cash
// {
//     char strategy_id[LEN_ID];       ///绛栫暐id
// 	char account_id[LEN_ID];        ///璐︽埛id
//     int currency;                   ///璐у竵绫诲埆
//
//     double nav;                     ///甯愭埛鍑€鍊�
//     double fpnl;                    ///娴姩璧簭
//     double pnl;                     ///鎬昏耽浜�
//     double profit_ratio;            ///鏀剁泭鐜�
//     double frozen;                  ///鎸佷粨鍐荤粨閲戦
//     double order_frozen;            ///鎸傚崟鍐荤粨閲戦
//     double available;               ///鍙敤璧勯噾
//
//     double cum_inout;               ///绱鍑哄叆閲戞€婚
//     double cum_trade;               ///绱浜ゆ槗棰�
//     double cum_pnl;                 ///绱璧簭
//     double cum_commission;          ///绱浜ゆ槗璐�
//
//     double last_trade;              ///涓婃浜ゆ槗閲戦
//     double last_pnl;                ///涓婃鎬昏耽浜�
//     double last_commission;         ///涓婃浜ゆ槗璐�
//     double last_inout;              ///涓婃鍑哄叆閲�
//     int change_reason;              ///甯愭埛鍙樺姩绫诲埆
//
//     double transact_time;           ///甯愭埛鍙樺姩鏃堕棿鎴�
// }Cash;
//
//
// /**
// * @brief 鎸佷粨鏁版嵁瀹氫箟
// *
// */
// typedef struct Position
// {
//     char strategy_id[LEN_ID];           ///绛栫暐id
// 	char account_id[LEN_ID];            ///璐︽埛id
//     char exchange[LEN_EXCHANGE];        ///浜ゆ槗鎵€浠ｇ爜
//     char sec_id[LEN_SECID];             ///浜ゆ槗鏍囩殑浠ｇ爜锛屾湰鍦颁唬鐮佸IF1502
//     int  side;                          ///涔板崠鏂瑰悜
//     double volume;                      ///鎸佷粨鏁伴噺
//     double volume_today;                ///浠婁粨閲�
//     double amount;                      ///鎸佷粨甯傚€�
//     double vwap;                        ///鎸佷粨鎴愭湰
//
//     double price;                       ///褰撳墠浠锋牸
//     double fpnl;                        ///娴姩璧㈠姩
//     double cost;                        ///鎸佷粨鎴愭湰
//     double order_frozen;                ///鎸傚崟鍐荤粨浠撲綅閲�
//     double order_frozen_today;          ///鎸傚崟鍐荤粨浠婁粨浠撲綅
//     double available;                   ///鍙钩浠撲綅閲�
//     double available_today;             ///鍙钩浠婁粨浣�(volume_today-order_frozen_today)
// 	double available_yesterday;         ///鍙钩鏄ㄤ粨浣�(available - available_today)
//
//     double last_price;                  ///涓婁竴娆℃垚浜や环
//     double last_volume;                 ///涓婁竴娆℃垚浜ら噺
//
//     double init_time;                   ///寤轰粨鏃堕棿
//     double transact_time;               ///浠撲綅鍙樻洿鏃堕棿
// }Position;
//
// /**
// * @brief 缁╂晥鎸囨爣瀹氫箟
// *
// */
// typedef struct Indicator
// {
//     char  strategy_id[LEN_ID];                   //绛栫暐ID
//     char  account_id[LEN_ID];                    //璐﹀彿ID
//
//     double nav;                                  //鍑€鍊�(cum_inout + cum_pnl + fpnl - cum_commission)
//     double pnl;                                  //鍑€鏀剁泭(nav-cum_inout)
//     double profit_ratio;                         //鏀剁泭鐜�(pnl/cum_inout)
// 	double profit_ratio_bench;                   //鍩哄噯鏀剁泭鐜�
//
//     double sharp_ratio;                          //澶忔櫘姣旂巼
//     double risk_ratio;                           //椋庨櫓姣旂巼
// 	int open_count;                              //寮€浠撴鏁�
//     int trade_count;                             //浜ゆ槗娆℃暟
//     int win_count;                               //鐩堝埄娆℃暟
//     int lose_count;                              //浜忔崯娆℃暟
//     double win_ratio;                            //鑳滅巼
//     double max_profit;                           //鏈€澶ф敹鐩�
//     double min_profit;                           //鏈€灏忔敹鐩�
//     double max_single_trade_profit;              //鏈€澶у崟娆′氦鏄撴敹鐩�
//     double min_single_trade_profit;              //鏈€灏忓崟娆′氦鏄撴敹鐩�
//     double daily_max_single_trade_profit;        //浠婃棩鏈€澶у崟娆′氦鏄撴敹鐩�
//     double daily_min_single_trade_profit;        //浠婃棩鏈€灏忓崟娆′氦鏄撴敹鐩�
//     double max_position_value;                   //鏈€澶ф寔浠撳競鍊兼垨鏉冪泭
//     double min_position_value;                   //鏈€灏忔寔浠撳競鍊兼垨鏉冪泭
//     double max_drawdown;                         //鏈€澶у洖鎾�
//     double daily_pnl;                            //浠婃棩鏀剁泭
//     double daily_return;                         //浠婃棩鏀剁泭鐜�
//     double annual_return;                        //骞村寲鏀剁泭鐜�
//
// 	double cum_inout;							 //绱鍑哄叆閲�
// 	double cum_trade;                            //绱浜ゆ槗棰�
// 	double cum_pnl;                              //绱骞充粨鏀剁泭(娌℃墸闄ゆ墜缁垂)
// 	double cum_commission;                       //绱鎵嬬画璐�
//
//     double transact_time;                        //鎸囨爣璁＄畻鏃堕棿
// }Indicator;
//
// /**
// * @brief 鏌滃彴璐﹀彿淇℃伅
// *
// */
// typedef struct BrokerAccount
// {
// 	char account_id[LEN_ID];                      //璐﹀彿ID
// 	char username[LEN_USERNAME];                  //鐢ㄦ埛鍚�
// 	bool permissible;                             //璐﹀彿鏄惁鍏佽浜ゆ槗
// 	int  status;                                  //璐︽埛鐘舵€�
// }BrokerAccount;
//



#include "candle.h"

typedef DataInterfaceMemory<CtpMarketData, CtpMarketDataInfo, CandleBar, CandleBarInfo> CandleWriter;
typedef DataInterfaceMemory<CtpMarketData, CtpMarketDataInfo, CandleBar, CandleBarInfo> CandleReader;


#endif // GOMMEM_H
