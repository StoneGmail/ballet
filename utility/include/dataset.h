#ifndef DATASET_H
#define DATASET_H

#include "utility_global.h"

#include "mderr.h"
#include "def.h"
#include "str.h"

#include <assert.h>
#include <list>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <thread>
#include <time.h>
#include <vector>

typedef bool (*got_new_data_cb)(DATAPTR dataobj, int64_t pos, bool isnew);
typedef bool (*is_new_data_cb)(int64_t pos, DATAPTR d1, DATAPTR d2);
typedef void (*merge_data_cb)(DATAPTR d1, DATAPTR d2);

typedef struct DataInfo {
  size_t cbsize = 0;
  size_t headsize = 0;
  size_t maxsize = 0;
  int64_t index = 0;  /// index of something
  int64_t pos = -1;    /// the amount number of data, start at -1
  bool isworking = true; /// indicate this buffer is working for now
  int64_t count = 0;     /// the cout >= pos + 1, for candle data, the pos will plus one when a new candle generated
  DATAPTR data = nullptr;

  DataInfo() {}

  DataInfo(size_t cbs, size_t hs)
      : cbsize(cbs), headsize(hs) {}

  DataInfo(size_t cbs, size_t hs, size_t ms, int64_t ind, int64_t amount,
           bool working, DATAPTR dp)
      : cbsize(cbs), headsize(hs), maxsize(ms), index(ind), pos(amount - 1),
        isworking(working), data(dp) { }

  void init(size_t cbs, size_t hs, size_t ms, DATAPTR ptr) {
    cbsize = (cbs), headsize = (hs), maxsize = (ms), index = (0), pos = (-1),
    isworking = (true), count = (0), data = ptr;
  }

} DataInfo;

//
// Data Set
//
template <typename T, typename TInfo>
class DataSet {
public:
  size_t  cbsize = sizeof(T);   // sizeof(T)
  size_t  infosize = sizeof(TInfo); // TInfo is a descript of T
  int64_t maxsize = 0;  // capacity of row
  int64_t nrow = 0;     // current rows of df
  TInfo *info = nullptr;
  T *data = nullptr;
  bool myown_memory_ = false;
  char mode_ = 'w'; ///'r' or 'w'
  int64_t sleep_interval = 0;
  got_new_data_cb newdata_cb_ = nullptr;
  is_new_data_cb is_new_ = nullptr;
  merge_data_cb merge_ = nullptr;


public:
  DataSet() {}
  virtual ~DataSet() { free_data_set(); }

  DataSet(char mode, bool own_memory)
      : myown_memory_(own_memory), mode_(mode) {
  }

  DataSet(int64_t data_size, char mode, bool own_memory, TInfo *tinfo)
    : mode_(mode) {
    init(data_size, mode, own_memory, tinfo);
  }

  void init(int64_t max_size, char mode, bool own_memory, TInfo *tinfo) {
    mode_ = mode;
    myown_memory_ = own_memory;

    if (mode_ == 'w') {
      free_data_set();
      cbsize = sizeof(T), maxsize = (max_size), nrow = (0), infosize = sizeof(TInfo);
      sleep_interval = (0), newdata_cb_ = nullptr, is_new_ = nullptr;

      if (myown_memory_) {
        info = reinterpret_cast<TInfo *>(new char[cbsize * max_size + infosize]);
        memset(info, 0, cbsize * max_size + infosize);
        if (tinfo)
          memcpy(info, tinfo, infosize);
        data = reinterpret_cast<T *>((char *)info + infosize);
      }
    } else {
      cbsize = sizeof(T), maxsize = (max_size), nrow = (0), infosize = sizeof(TInfo);
      sleep_interval = (0), info = nullptr, data = nullptr;
      newdata_cb_ = nullptr, is_new_ = nullptr;
    }
  }


  TInfo *getInfo() { return info; }
  int64_t get_data_amount() { return info->di.count; }
  int get_cur_pos() { return info->di.pos; }

  void free_data_set() {
    if (myown_memory_) {
      if (info)
        delete[]((char *)info);
      cbsize = (sizeof(T)), maxsize = (0), nrow = (0), infosize = sizeof(TInfo),
      info = nullptr;
      data = nullptr;
      myown_memory_ = false;
    }
  }

  int64_t rand_id() {
    if (!maxsize)
      return -1;
    return (int64_t)(rand() % maxsize);
  }

  static size_t get_memory_size(int64_t maxsize) {
    return (sizeof(T) * maxsize + sizeof(TInfo));
  }

  void set_memory(DATAPTR data_memptr, TInfo *tinfo, int64_t max_data_size,
                  char mode = 'w') {
    free_data_set();

    cbsize = (sizeof(T)), maxsize = (max_data_size), nrow = (0),
    infosize = sizeof(TInfo);
    mode_ = mode;

    info = reinterpret_cast<TInfo *>(data_memptr);
    data = reinterpret_cast<T *>((char *)info + infosize);
    if (mode == 'w') {
      if (tinfo)
        memcpy(info, tinfo, infosize);
      else {
        // memset(info, 0, infosize);
        info->di.init(cbsize, infosize, maxsize, data);
      }

      memset(data, 0, cbsize * max_data_size);
    }
    myown_memory_ = false;
  }

  T *get_dataptr(int64_t *s) {
    *s = maxsize;
    return data;
  }

  T &operator[](int64_t row) {
    return data[row];
  }

  T* at(int64_t row) {
    return data + row;
  }

  DATAPTR get_bufptr() {
    return (data);
  }

  bool is_writble() { return mode_ == 'w'; }

  int append(const T *v) {
    if (mode_ != 'w')
      return MDERR_MEMORY_IS_READONLY;
    if (nrow >= maxsize)
      return MDERR_MEMORY_IS_FULL;
    if (info->di.pos + 1 != (int64_t)nrow)
      return MDERR_MEMORY_WRITE_ERROR;

    bool its_new = true;
    if (is_new_ != nullptr)
      its_new = is_new_(info->di.pos, data + info->di.pos, (T*) v);

    if (its_new) {
      nrow++;
      info->di.pos++;
      memcpy(data + info->di.pos, v, sizeof(T));
    } else {
      if (merge_) {
        if (info->di.pos < 0) {
          info->di.pos++, nrow++;
        }
        merge_(data + info->di.pos, (DATAPTR)(v));
      }
    }
    info->di.count++;

    if (newdata_cb_ != nullptr)
      newdata_cb_(this, info->di.pos, its_new);

    return 0;
  }

  void set_working(bool working = true) {
    info->di.isworking = working;
    info->di.pos = -1;
    nrow = 0;
  }

  void is_working() { return info->di.isworking; }

  void set_newdata_cb(got_new_data_cb cb) { newdata_cb_ = cb; }

  inline void start_data_reader(int sleep_interval);

  DataSet<T, TInfo>* sample(size_t interval, size_t offset) {
      if (offset > nrow) return nullptr;
      size_t datasize = (nrow - offset) / interval + 1;
      auto newset = new DataSet<T, TInfo>(datasize);
      for (size_t i = offset; i < nrow; i += interval)
        newset->append(data + i);
      return newset;
  }
};

#define DATASET(T) DataSet<T, T##Info>
#define DS(T) DataSet<T, T##Info>

template <typename T, typename TInfo>
class TsDataFrame : public DataSet<T, TInfo> {
public:
  int64_t ncol = 0; // the amount of column, = sizeof(T) / sizeof(double)
  ftime_t *key = 0;
  char mode_ = 'w';
  bool own_memory_ = false;
  double **series = nullptr;

  TsDataFrame() { }
  virtual ~TsDataFrame() { free_data_set(); }

  TsDataFrame(const char mode, bool own_memory)
      : DataSet<T, TInfo>(mode, own_memory) {  }

  TsDataFrame(int64_t data_size, const char mode, bool own_memory, TInfo *tinfo)
      : DataSet<T, TInfo>(mode, own_memory, tinfo) {
    init(data_size, mode, own_memory, tinfo);
  }


  void init(int64_t s, const char mode, bool own_memory, TInfo *tinfo) {
    mode_ = mode;
    DataSet<T, TInfo>::myown_memory_ = own_memory_ = own_memory;
    ncol = sizeof(T) / sizeof(double);

    free_data_set();

    DataSet<T, TInfo>::cbsize = (sizeof(T)), DataSet<T, TInfo>::maxsize = (s),
               DataSet<T, TInfo>::nrow = (0),
               DataSet<T, TInfo>::infosize = sizeof(TInfo);

    DataSet<T, TInfo>::newdata_cb_ = (0), DataSet<T, TInfo>::sleep_interval = (0);

    if (own_memory_ && mode_ == 'w') {
        DataSet<T, TInfo>::info = reinterpret_cast<TInfo *>(
                   new char[DataSet<T, TInfo>::cbsize * s * 2 +
                            DataSet<T, TInfo>::infosize]);
        memset(DataSet<T, TInfo>::info, 0,
               DataSet<T, TInfo>::cbsize * s * 2 + DataSet<T, TInfo>::infosize);
        if (tinfo)
          memcpy(DataSet<T, TInfo>::info, tinfo, DataSet<T, TInfo>::infosize);
        else
          DataSet<T, TInfo>::info->di.init(sizeof(T), sizeof(TInfo), s, DataSet<T, TInfo>::data);

        DataSet<T, TInfo>::data = reinterpret_cast<T *>(
            (char *)DataSet<T, TInfo>::info + DataSet<T, TInfo>::infosize);

        series = new double *[ncol];
        memset(series, 0, sizeof(double *) * ncol);

        series[0] = (double *)(DataSet<T, TInfo>::data + s);
        for (int64_t i = 1; i < ncol; ++i)
          series[i] = series[0] + i * DataSet<T, TInfo>::maxsize;

        key = reinterpret_cast<ftime_t *>(series[0]);
    } else {
        DataSet<T, TInfo>::info = nullptr;
        DataSet<T, TInfo>::data = nullptr;
        series = nullptr;
        key = nullptr;
    }
  }

  static size_t get_memory_size(int64_t maxsize) {
    return (sizeof(T) * maxsize * 2 + sizeof(TInfo));
  }

  void free_data_set() {
//    DataSet<T, TInfo>::free_data_set();
    if (DataSet<T, TInfo>::myown_memory_) {
      if (DataSet<T, TInfo>::info)
        delete[]((char *)DataSet<T, TInfo>::info);
      DataSet<T, TInfo>::cbsize = (sizeof(T)), DataSet<T, TInfo>::maxsize = (0), DataSet<T, TInfo>::nrow = (0), DataSet<T, TInfo>::infosize = sizeof(TInfo),
      DataSet<T, TInfo>::info = nullptr;
      DataSet<T, TInfo>::data = nullptr;
      DataSet<T, TInfo>::myown_memory_ = false;
    }
    if (nullptr != series)
      delete[] series;

    series = nullptr;
    key = nullptr;
    ncol = (sizeof(T) / sizeof(double));
  }

  void set_memory(DATAPTR data_memptr, TInfo *tinfo, int64_t max_data_size,
                  char mode = 'w') {
      ncol = sizeof(T) / sizeof(double);
      this->free_data_set();

      DataSet<T, TInfo>::set_memory(data_memptr, tinfo, max_data_size, mode);

      series = new double *[ncol];
      series[0] = (double *)(DataSet<T, TInfo>::data + max_data_size);
      for (int64_t i = 1; i < ncol; ++i)
        series[i] = series[0] + i * DataSet<T, TInfo>::maxsize;
      key = reinterpret_cast<ftime_t *>(series[0]);

      if (mode == 'w')
        memset(series[0], 0, sizeof(T) * max_data_size);
  }

  T &get_data(ftime_t ft) {
      return DataSet<T, TInfo>::data[0];
  }

  T &operator[](double key) {
    return get_data(key);
  }

  T& get_data(int64_t row) {
    return DataSet<T, TInfo>::data[row];
  }

  T &operator[](int64_t row) {
      return DataSet<T, TInfo>::data[row];
  }

  double *operator[](std::string col_name) {
      return maps2dp[col_name];
  }

  double* get_dfptr() {
    return key;
  }

  ftime_t *get_key() { return key; }
  double *get_col(size_t col) {
    if (col > ncol)
      return nullptr;
    return reinterpret_cast<double *>(series) +
           col * DataSet<T, TInfo>::maxsize;
  }
  double *get_col(std::string col_name) { return maps2dp[col_name]; }

  double get_data(int64_t row, int64_t col) {
    return *(series[0] + col * DataSet<T, TInfo>::maxsize + row);
  }

  size_t get_row_amount() {return this->nrow;}
  size_t get_clo_amount() {return this->ncol;}

  int get_row() { return DataSet<T, TInfo>::nrow; }


  int append(const T *v) {
    if (DataSet<T, TInfo>::mode_ != 'w')
      return MDERR_MEMORY_IS_READONLY;
    if (DataSet<T, TInfo>::info->di.pos >=
        (int64_t)DataSet<T, TInfo>::info->di.maxsize)
      return MDERR_MEMORY_IS_FULL;
    if (DataSet<T, TInfo>::info->di.pos + 1 != (int64_t)DataSet<T, TInfo>::nrow)
      return MDERR_MEMORY_WRITE_ERROR;

    // printf("Add a new data heheh\n");

    bool b_new_data = true;
    if (isnew_ != nullptr)
      b_new_data = isnew_(DataSet<T, TInfo>::info->di.pos,
                          DataSet<T, TInfo>::data + DataSet<T, TInfo>::info->di.pos,
                          (T*) v);
    if (b_new_data) {
      ++DataSet<T, TInfo>::nrow;
      ++DataSet<T, TInfo>::info->di.pos;
      memcpy(DataSet<T, TInfo>::data + DataSet<T, TInfo>::info->di.pos, v, sizeof(T));
      for (int64_t i = 0; i < ncol; i++)
        *(series[i] + DataSet<T, TInfo>::info->di.pos) = *((double *)(v) + i);
    } else {
      if (merge_) {
        if (DataSet<T, TInfo>::info->di.pos < 0) {
          DataSet<T, TInfo>::info->di.pos++, DataSet<T, TInfo>::nrow++;
        }
        merge_(DataSet<T, TInfo>::data + DataSet<T, TInfo>::info->di.pos, (DATAPTR)(v));
      }
      for (int64_t i = 0; i < ncol; i++)
        *(series[i] + DataSet<T, TInfo>::info->di.pos)
              = *((double *)(DataSet<T, TInfo>::data + DataSet<T, TInfo>::info->di.pos) + i);
    }
    ++DataSet<T, TInfo>::info->di.count;

    if (new_data_cb_ != nullptr)
      new_data_cb_(this, DataSet<T, TInfo>::info->di.pos, b_new_data);

    return 0;
  }

  void set_columns(VecColName &cols) {
    for (size_t i = 0; i < cols.size(); ++i) {
      maps2dp.insert(PairColDfPtr(cols[i], series[i]));
    }
  }

  int sample(TsDataFrame<T, TInfo>&& df, size_t interval, size_t offset) {
      if (offset > DS(T)::nrow) return -1;
      size_t datasize = (DS(T)::nrow - offset) / interval + 1;
      auto newset = new TsDataFrame<T, TInfo>(datasize);
      for (size_t i = offset; i < DS(T)::nrow; i += interval)
        newset->append(DS(T)::data + i);

      df = *newset;
      return 0;
  }

  std::shared_ptr<TsDataFrame<T, TInfo>> sample(size_t interval, size_t offset) {
      if (offset > DS(T)::nrow) return nullptr;
      size_t datasize = (DS(T)::nrow - offset) / interval + 1;
      std::shared_ptr<TsDataFrame<T, TInfo>> newset(new TsDataFrame<T, TInfo>(datasize));
      for (size_t i = offset; i < DS(T)::nrow; i += interval)
        (*newset).append(DS(T)::data + i);
      return newset;
  }

  void set_isnew_cb(is_new_data_cb isnew) { isnew_ = isnew; }
  void set_got_new_data_cb(got_new_data_cb new_data_cb) { new_data_cb_ = new_data_cb; }
  void set_merge(merge_data_cb merge) {merge_ = merge;}

private:
  Col2DfPtr maps2dp;
  is_new_data_cb isnew_ = nullptr;
  got_new_data_cb new_data_cb_ = nullptr;
  merge_data_cb merge_ = nullptr;
};

#define TSDATAFRAME(T) TsDataFrame<T, T##Info>

extern int g_run;
//
// template<typename T, typename TInfo>
// inline void DataSet<T, TInfo>::start_data_reader(int sleep_interval) {
//
// }
//
//
// template<typename T, typename TInfo>
// inline void TsDataFrame<T, TInfo>::start_data_reader(int sleep_interval) {
//
// }
//
template <template <typename, typename> class DataContainer, typename T,
          typename TInfo>
int start_data_reader(DataContainer<T, TInfo> *dc, int sleep_interval) {
  if (!dc)
    return 0;
  while (g_run) {
    if (dc->newdata_cb) {
      int64_t spos = 0, epos = 0;
      dc->newdata_cb((void **)(&dc->data), &spos, &epos);
    }
    std::this_thread::sleep_for(std::chrono::seconds(sleep_interval));
  }

  return 0;
}


typedef int (*udp_reply_callback) (SOCKET sockfd, void* buf, size_t buflen, struct sockaddr_in* clientaddr, socklen_t clientlen);

class UTILITY_API UdpDataTransfer {
private:
  char addr[MAX_PATH];
  short port = 0;
  SOCKET sockfd = -1;
  socklen_t socklen = 0;
  struct sockaddr_in serveraddr;
  struct hostent *hostp = 0;

  char buf[BUFSIZE];
  bool this_run = false;
  udp_reply_callback reply_cb = 0;

public:
  UdpDataTransfer(const char* addr, short port) : port(port){
    strcpy(this->addr, addr);
  }

  ~UdpDataTransfer() {
    if (sockfd > 0) closesocket(sockfd);
  }

  int init_client();
  int start_server();
  void stop_server();

  template <typename T, typename TInfo>
  int send(DATASET(T)& ds);
  template <typename T, typename TInfo>
  int send(TSDATAFRAME(T)& df);

  int send(void *ptr, size_t len);

  template <typename T, typename TInfo>
  int recv(DATASET(T)& ds);
  template <typename T, typename TInfo>
  int recv(TSDATAFRAME(T)& df);

  int recv();

  int close();
};

extern UTILITY_API void show_cstring(const char *str, int len);

template<typename T, typename TInfo>
int load_data_from_file(const char* filepath, T** data, TInfo** info) {
    FILE* fp = 0;
    long s = 0;
    size_t size = 0;

    int ret = fopen_s(&fp, filepath, "rb");
    if (ret)
      printf("Error occured when open %s with error %d %s\n", filepath, errno, strerror(errno));
    else {
      fseek(fp, 0, SEEK_END);
      s = ftell(fp);
      fseek(fp, 0, SEEK_SET);
      ret = s < 1;
    }

    if (!ret)
      size = ((size_t)s) / sizeof(T);

    if (!ret) {
      *info = new TInfo;
      *data = new T[size];

      ret = !(*info && *data);
    }

    if (!ret)
      ret = (size != fread(*data, sizeof(T), size, fp));

    if (fp)
      fclose(fp);

    if (ret) {
      delete *info;
      delete[] *data;
    } else {
      (*info)->di.data = *data;
      (*info)->di.maxsize = size;
      (*info)->di.count = (int64_t)size;
    }

    return ret;
}


#ifndef ORDER_DEFINE
#define ORDER_DEFINE

//字符字段长度定义
const unsigned int LEN_ID       = 40;
const unsigned int LEN_SYMBOL   = 32;
const unsigned int LEN_USERNAME = 32;
const unsigned int LEN_PASSWORD = 32;
const unsigned int LEN_INFO     = 64;
const unsigned int LEN_SECID    = 24;
const unsigned int LEN_EXCHANGE = 8;
const unsigned int LEN_ISO_DATATIME = 36;
const unsigned int LEN_SEC_NAME = 64;
const unsigned int LEN_PUB_DATE = 16;
const unsigned int LEN_TRADE_DATE = 16;


enum ExecType {
  ExecType_New = 1,
  ExecType_DoneForDay = 4,
  ExecType_Canceled = 5,
  ExecType_PendingCancel = 6,
  ExecType_Stopped = 7,
  ExecType_Rejected = 8,
  ExecType_Suspended = 9,
  ExecType_PendingNew = 10,
  ExecType_Calculated = 11,
  ExecType_Expired = 12,
  ExecType_Restated = 13,
  ExecType_PendingReplace = 14,
  ExecType_Trade = 15,
  ExecType_TradeCorrect = 16,
  ExecType_TradeCancel = 17,
  ExecType_OrderStatus = 18,
  ExecType_CancelRejected = 19
};


enum OrderStatus {
  OrderStatus_New = 1,
  OrderStatus_PartiallyFilled = 2,
  OrderStatus_Filled = 3,
  OrderStatus_DoneForDay = 4,
  OrderStatus_Canceled = 5,
  OrderStatus_PendingCancel = 6,
  OrderStatus_Stopped = 7,
  OrderStatus_Rejected = 8,
  OrderStatus_Suspended = 9,
  OrderStatus_PendingNew = 10,
  OrderStatus_Calculated = 11,
  OrderStatus_Expired = 12,
  OrderStatus_AcceptedForBidding = 13,
  OrderStatus_PendingReplace = 14
};


enum CancelOrderRejectReason {
  CancelOrderRejectReason_OrderFinalized = 101,
  CancelOrderRejectReason_UnknownOrder = 102,
  CancelOrderRejectReason_BrokerOption = 103,
  CancelOrderRejectReason_AlreadyInPendingCancel = 104
};


enum OrderSide {
  OrderSide_Bid = 1,
  OrderSide_Ask = 2
};


enum OrderType {
    OrderType_LMT = 0,
    OrderType_BOC = 1,
    OrderType_BOP = 2,
    OrderType_B5TC = 3,
    OrderType_B5TL = 4,
    OrderType_IOC = 5,
    OrderType_FOK = 6,
    OrderType_AON = 7,
    OrderType_MTL = 8,
    OrderType_EXE = 9
};


enum PositionEffect {
    PositionEffect_Open = 1,
    PositionEffect_Close = 2,
    PositionEffect_CloseToday = 3,
    PositionEffect_CloseYesterday = 4
};


enum OrderRejectReason {
  OrderRejectReason_UnknownReason = 1,
  OrderRejectReason_RiskRuleCheckFailed = 2,
  OrderRejectReason_NoEnoughCash = 3,
  OrderRejectReason_NoEnoughPosition = 4,
  OrderRejectReason_IllegalStrategyID = 5,
  OrderRejectReason_IllegalSymbol = 6,
  OrderRejectReason_IllegalVolume = 7,
  OrderRejectReason_IllegalPrice = 8,
  OrderRejectReason_NoMatchedTradingChannel = 9,
  OrderRejectReason_AccountForbidTrading = 10,
  OrderRejectReason_TradingChannelNotConnected = 11,
  OrderRejectReason_StrategyForbidTrading = 12,
  OrderRejectReason_NotInTradingSession = 13
};


/**
* @brief 委托数据定义
*
*/
typedef struct Order
{
    char strategy_id[LEN_ID];       ///策略id
    char account_id[LEN_ID];        ///帐号id, 系统自动根据配置补充

    char cl_ord_id[LEN_ID];         ///委托订单的客户方id
    char order_id[LEN_ID];          ///委托订单的柜台生成id
    char ex_ord_id[LEN_ID];         ///委托订单的交易所接受后生成id

    char exchange[LEN_EXCHANGE];    ///交易所代码
    char sec_id[LEN_SECID];         ///交易标的代码，本地代码如IF1502

    int position_effect;            //开平标志，取值参考enum PositionEffect
    int side;                       //买卖方向，取值参考enum OrderSide
    int order_type;                 //委托类型，取值参考enum OrderType
    int order_src;                  //委托来源，取值参考enum OrderSrc
    int status;                     //委托状态，取值参考enum OrderStatus
    int ord_rej_reason;             //委托拒绝原因，取值参考enum OrderRejectReason
    char ord_rej_reason_detail[LEN_INFO];   ///委托被拒绝原因描述

    double price;                   //委托价格
    double stop_price;              //止损价
    double volume;                  //委托量
    double filled_volume;           //已成量
    double filled_vwap;             //已成均价
    double filled_amount;           //已成金额

    double sending_time;            //委托发送时间
    double transact_time;           //委托处理时间
} Order;


typedef struct OrderInfo {
  DataInfo di;
  int OrderSomething;
}OrderInfo;


/**
* @brief 委托回报数据定义
*
*/
typedef struct ExecRpt
{
    char strategy_id[LEN_ID];       ///策略id

    char cl_ord_id[LEN_ID];         ///委托订单的客户方id
    char order_id[LEN_ID];          ///委托订单的柜台生成id
    char exec_id[LEN_ID];           ///委托订单的交易所生成id

    char exchange[LEN_EXCHANGE];    ///交易所代码
    char sec_id[LEN_SECID];         ///交易标的代码，本地代码如IF1502

    int position_effect;            ///开平仓标识
    int side;                       ///买卖方向
    int ord_rej_reason;             ///委托被拒绝原因类型
    char ord_rej_reason_detail[LEN_INFO];  ///委托被拒绝原因描述
    int exec_type;

    double price;                   ///成交价格
    double volume;                  ///成交数量
    double amount;                  ///成交金额

    double transact_time;           ///成交时间
}ExecRpt;

typedef struct ExecRptInfo {
  DataInfo di;
  int ExecRptSomething;
} ExecRptInfo;

/**
* @brief 资金帐户的数据定义
*
*/
typedef struct Cash
{
    char strategy_id[LEN_ID];       ///策略id
    char account_id[LEN_ID];        ///账户id
    int currency;                   ///货币类别

    double nav;                     ///帐户净值
    double fpnl;                    ///浮动赢亏
    double pnl;                     ///总赢亏
    double profit_ratio;            ///收益率
    double frozen;                  ///持仓冻结金额
    double order_frozen;            ///挂单冻结金额
    double available;               ///可用资金

    double cum_inout;               ///累计出入金总额
    double cum_trade;               ///累计交易额
    double cum_pnl;                 ///累计赢亏
    double cum_commission;          ///累计交易费

    double last_trade;              ///上次交易金额
    double last_pnl;                ///上次总赢亏
    double last_commission;         ///上次交易费
    double last_inout;              ///上次出入金
    int change_reason;              ///帐户变动类别

    double transact_time;           ///帐户变动时间戳
}Cash;


/**
* @brief 持仓数据定义
*
*/
typedef struct Position
{
    char strategy_id[LEN_ID];           ///策略id
    char account_id[LEN_ID];            ///账户id
    char exchange[LEN_EXCHANGE];        ///交易所代码
    char sec_id[LEN_SECID];             ///交易标的代码，本地代码如IF1502
    int  side;                          ///买卖方向
    double volume;                      ///持仓数量
    double volume_today;                ///今仓量
    double amount;                      ///持仓市值
    double vwap;                        ///持仓成本

    double price;                       ///当前价格
    double fpnl;                        ///浮动赢动
    double cost;                        ///持仓成本
    double order_frozen;                ///挂单冻结仓位量
    double order_frozen_today;          ///挂单冻结今仓仓位
    double available;                   ///可平仓位量
    double available_today;             ///可平今仓位(volume_today-order_frozen_today)
    double available_yesterday;         ///可平昨仓位(available - available_today)

    double last_price;                  ///上一次成交价
    double last_volume;                 ///上一次成交量

    double init_time;                   ///建仓时间
    double transact_time;               ///仓位变更时间
}Position;


typedef struct PositionInfo {
  DataInfo di;
  int PositionSomething;
} PositionInfo;

/**
* @brief 绩效指标定义
*
*/
typedef struct Indicator
{
    char  strategy_id[LEN_ID];                   //策略ID
    char  account_id[LEN_ID];                    //账号ID

    double nav;                                  //净值(cum_inout + cum_pnl + fpnl - cum_commission)
    double pnl;                                  //净收益(nav-cum_inout)
    double profit_ratio;                         //收益率(pnl/cum_inout)
    double profit_ratio_bench;                   //基准收益率

    double sharp_ratio;                          //夏普比率
    double risk_ratio;                           //风险比率
    int open_count;                              //开仓次数
    int trade_count;                             //交易次数
    int win_count;                               //盈利次数
    int lose_count;                              //亏损次数
    double win_ratio;                            //胜率
    double max_profit;                           //最大收益
    double min_profit;                           //最小收益
    double max_single_trade_profit;              //最大单次交易收益
    double min_single_trade_profit;              //最小单次交易收益
    double daily_max_single_trade_profit;        //今日最大单次交易收益
    double daily_min_single_trade_profit;        //今日最小单次交易收益
    double max_position_value;                   //最大持仓市值或权益
    double min_position_value;                   //最小持仓市值或权益
    double max_drawdown;                         //最大回撤
    double daily_pnl;                            //今日收益
    double daily_return;                         //今日收益率
    double annual_return;                        //年化收益率

    double cum_inout;							 //累计出入金
    double cum_trade;                            //累计交易额
    double cum_pnl;                              //累计平仓收益(没扣除手续费)
    double cum_commission;                       //累计手续费

    double transact_time;                        //指标计算时间
}Indicator;

/**
* @brief 柜台账号信息
*
*/
typedef struct BrokerAccount
{
    char account_id[LEN_ID];                      //账号ID
    char username[LEN_USERNAME];                  //用户名
    bool permissible;                             //账号是否允许交易
    int  status;                                  //账户状态
} BrokerAccount;

#endif //ORDER_DEFINE


#endif // DATASET_H
