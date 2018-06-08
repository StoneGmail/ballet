#include "ctpdi.h"
#include "utility.h"

#include <thread>

static Ctpdi *gctpdi = 0;
static bool grun = false;

static char kindel_memfile[] = {"kindle-mem-file"};
// static char ta_memfile[] = { "" };
typedef std::map<std::string, int> MapKPeriodStringIndex;

static MapKPeriodStringIndex mapP2I;

static uint32_t one_day_minutes =
    15 * 60; // asumme one trading day has 15 hours trading time
static const size_t addtion_size[] = {
    one_day_minutes + 1,
    one_day_minutes / 5 + 10,
    one_day_minutes / 10 + 10,
    one_day_minutes / 15 + 10,
    one_day_minutes / 30 + 10,
    one_day_minutes / 60 + 10,
    one_day_minutes / (4 * 60) + 10,
    one_day_minutes / one_day_minutes + 10,
    one_day_minutes / (5 * one_day_minutes) + 10,
    one_day_minutes / (one_day_minutes * 24) + 10};

#define AMOUNT_KINDLE_PERIOD 10 // 10 periods

int open_data_interface() {
  int ret = -1;
  if (!gctpdi && ctp_param && gmd_param) {
    printf("openning data interface ... ");
    gctpdi = new Ctpdi();

    ret = gctpdi->open_data_interface();
  }

  if (ret) {
    printf(" error!\n");
    return ret;
  } else
    printf(" ok!\n");

  mapP2I["m1"] = 1;
  mapP2I["m5"] = 2;
  mapP2I["m10"] = 3;
  mapP2I["m15"] = 4;
  mapP2I["m30"] = 5;
  mapP2I["h1"] = 6;
  mapP2I["h4"] = 7;
  mapP2I["D1"] = 8;
  mapP2I["W1"] = 9;
  mapP2I["M1"] = 10;

  return ret;
}

int close_data_interface() {
  if (gctpdi) {
    printf("close data interface ... \n");
    gctpdi->close_data_interface();
  }

  SafeDeletePtr(gctpdi);
  return 0;
}

KINDLE *get_kindle_data(const char *inst, const char *period, size_t *len,
                        size_t *oldlen, size_t *newlen) {
  int pi = mapP2I[period] - 1;
  if (pi < 0)
    return 0;

  if (gctpdi)
    return gctpdi->get_kindle_data(inst, gkperiod[pi + 4], len, oldlen, newlen);

  return 0;
}

KINDLE *search_kindle_data(const char *inst, size_t *len,
                           CANDLE_PERIOD_TYPE period, uint32_t stime,
                           uint32_t etime) {
  if (gctpdi)
    return gctpdi->search_kindle_data(inst, len, period, stime, etime);

  return 0;
}

int start_listen(NEW_KINDLE_CALLBACK kindle_callback) {
  if (gctpdi)
    return gctpdi->start_listen(kindle_callback);
  return -100;
}

void stop_listen() { grun = false; }

bool is_listening() { return grun; }

Ctpdi::Ctpdi() : kindle_index_(0), datasize_(0), curpos_(0) {}

Ctpdi::~Ctpdi() {}

int Ctpdi::open_data_interface() {
  // get all size of kindle data in one trading day
  // the memeory size if 1m + 5m + 10m + 15m + 30m + 1h + 4h + 1d + 7d + 1month
  curpos_ =
      (size_t *)new size_t[ctp_param->instrument.size() * AMOUNT_KINDLE_PERIOD];
  // cout << "new indexes_ is " << indexes_ << endl;

  {
    datasize_ = 0;

    for (size_t k = 0; k < ctp_param->instrument.size(); ++k) {
      itmemMap_[getInstCode(ctp_param->instrument[k])] =
          static_cast<int>(k + 1);

      for (int i = 0; i <= 9; ++i) {
        size_t this_size = 0;
        int ret =
            get_kindle_size(ctp_param->instrument[k].c_str(), gkperiod[i + 4],
                            gmd_param->cal_store_path, &this_size);
        if (ret)
          return ret;
        datasize_ += (this_size + addtion_size[i]) * sizeof(KINDLE);
      }
    }
    datasize_ += sizeof(KindleMemIndex) * ctp_param->instrument.size();
#ifdef _DEBUG_
    cout << "datasize " << datasize_ << endl;
#endif
  }

  {
    if (mmKindle_.OpenRead(kindel_memfile, datasize_))
      return -1;
    if (nullptr == (kindle_index_ = reinterpret_cast<KindleMemIndex *>(
                        mmKindle_.GetDataPtr())))
      return -2;
  }

  { set_lengths(); }

  return 0;
}

int Ctpdi::close_data_interface() {
  mmKindle_.CloseFile();

  SafeDeleteAry(curpos_);
  return 0;
}

KINDLE *Ctpdi::get_kindle_data(const char *inst, CANDLE_PERIOD_TYPE period,
                               size_t *len, size_t *oldlen, size_t *newlen) {
  int k = itmemMap_[getInstCode(inst)] - 1;
  if (k < 0)
    return 0;
  int period_index = get_period_index(period) - 4;
  if (period_index < 0 || period_index > 9)
    return 0;

  *newlen = kindle_index_[k].kptr[period_index].len;
  *oldlen = kindle_index_[k].kptr[period_index].olen;
  *len = *newlen - *oldlen;
#ifdef _DEBUG_
  printf("total %lu, (k, j) is (%d, %d), offset %lu, good ? %d\n", datasize_, k,
         period_index, kindle_index_[k].kptr[period_index].offset,
         (int)(kindle_index_[k].kptr[period_index].offset < datasize_));
#endif //_DEBUG_
  return (KINDLE *)((char *)kindle_index_ +
                    kindle_index_[k].kptr[period_index].offset);
}

void Ctpdi::set_lengths() {
  for (size_t k = 0; k < ctp_param->instrument.size(); ++k)
    for (size_t j = 0; j < AMOUNT_KINDLE_PERIOD; ++j)
      curpos_[k * AMOUNT_KINDLE_PERIOD + j] = kindle_index_[k].kptr[j].len;
}

int Ctpdi::start_listen(NEW_KINDLE_CALLBACK kindle_callback) {
  grun = true;
  while (grun) {
    for (size_t k = 0; k < ctp_param->instrument.size(); ++k) {
      for (size_t j = 0; j < AMOUNT_KINDLE_PERIOD; ++j) {
        if (curpos_[k * AMOUNT_KINDLE_PERIOD + j] <
            kindle_index_[k].kptr[j].len) {
          for (size_t i = curpos_[k * AMOUNT_KINDLE_PERIOD + j];
               i < kindle_index_[k].kptr[j].len - 1; ++i) {
            kindle_callback(ctp_param->instrument[k].c_str(),
                            kindle_index_[k].kptr[j].ptr + i, j);
          }
        }
      }
    }

    std::this_thread::sleep_for(std::chrono::microseconds(10));
    set_lengths();
  }
  return 0;
}

KINDLE *Ctpdi::search_kindle_data(const char *inst, size_t *len,
                                  CANDLE_PERIOD_TYPE period, uint32_t stime,
                                  uint32_t etime) {
  int k = itmemMap_[getInstCode(inst)] - 1;
  if (k < 0)
    return 0;
  int period_index = get_period_index(period) - 4;
  if (period_index < 0 || period_index > 9)
    return 0;
  if (etime < stime)
    return 0;

  size_t s = 0, e = 0;
  KINDLE *kindle = (KINDLE *)((char *)kindle_index_ +
                              kindle_index_[k].kptr[period_index].offset);

  if (stime == 0)
    s = 1;
  if (etime == 0)
    e = kindle_index_[k].kptr[period_index].len;
  if (s && e) {
    *len = e;
    return kindle;
  }

  time_t st = make_the_time(stime, 0);
  time_t et = make_the_time(etime, 0);

  for (size_t i = 0; i < kindle_index_[k].kptr[period_index].len; ++i) {
    if (!s) {
      if (kindle[i].UpdateTime >= st)
        s = i + 1;
    } else {
      if (!e) {
        if (kindle[i].UpdateTime > et) {
          e = i; // i - 1
          break;
          ;
        }
      } else
        break;
    }
  }

  if (!s && !e) {
    *len = 0;
    return 0;
  }
  *len = e - s + 1;
  return kindle + s - 1;
}
