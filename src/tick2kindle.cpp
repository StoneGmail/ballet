#include "tick2kindle.h"
#include "str.h"
#include <sys/stat.h>

Tick2Kindle::Tick2Kindle(bool has_ctpmmd)
    : TickReader(has_ctpmmd), dataptr_(0), datasize_(0), indexes_(0) {}

Tick2Kindle::~Tick2Kindle() { mmKindle_.CloseFile(); }

static char mempath[] = {"/tmp/kindle-mem-file"};
static char memfile[] = {"kindle-mem-file"};
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

int Tick2Kindle::initServer() {
  {
    int ret = TickReader::initServer();
    if (ret) {
#ifdef _DEBUG_
      printf("TickReader::initServer() error\n");
#endif //_DEBUG
      return ret;
    }
  }

  // get all size of kindle data in one trading day
  // the memeory size if 1m + 5m + 10m + 15m + 30m + 1h + 4h + 1d + 7d + 1month
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
        if (ret) {
#ifdef _DEBUG_
          printf("get_kindle_size error %d\n", ret);
#endif //_DEBUG
          return ret;
        }
        datasize_ += (this_size + addtion_size[i]) * sizeof(KINDLE);
      }
    }
    datasize_ += sizeof(KindleMemIndex) * ctp_param->instrument.size();
#ifdef _DEBUG_
    cout << "datasize " << datasize_ << endl;
#endif
  }

#ifdef _DEBUG_
// printf("%s %d\n", __FILE__, __LINE__);
#endif

  { // generate shared memory
    int ret = mmKindle_.OpenWrite(mempath, memfile, datasize_);
#ifdef _DEBUG_
// printf("%s %d\n", __FILE__, __LINE__);
#endif
    if (ret) {
#ifdef _DEBUG_
      printf("mmKindle_.OpenWrite error\n");
#endif //_DEBUG
      return ret;
    }
#ifdef _DEBUG_
// printf("%s %d\n", __FILE__, __LINE__);
#endif
    dataptr_ = mmKindle_.GetDataPtr();
#ifdef _DEBUG_
// printf("%s %d\n", __FILE__, __LINE__);
#endif
  }

#ifdef _DEBUG_
// printf("%s %d\n", __FILE__, __LINE__);
#endif
  { indexes_ = reinterpret_cast<KindleMemIndex *>(dataptr_); }
#ifdef _DEBUG_
// printf("%s %d\n", __FILE__, __LINE__);
#endif

  { // read kindle data
    char *ptr = (char *)dataptr_ +
                sizeof(KindleMemIndex) * ctp_param->instrument.size();
#ifdef _DEBUG_
// printf("%s %d\n", __FILE__, __LINE__);
#endif
    for (size_t k = 0; k < ctp_param->instrument.size(); ++k) {
      indexes_[k].index = (uint32_t)k;
      indexes_[k].offset = ptr - (char *)dataptr_;
      for (int i = 0; i <= 9; ++i) {
        size_t this_size = 0;
        int ret = read_kindle_file(ctp_param->instrument[k].c_str(),
                                   gkperiod[i + 4], gmd_param->cal_store_path,
                                   (KINDLE *)ptr, &this_size);
        if (ret) {
#ifdef _DEBUG_
          printf("read_kindle_file error\n");
#endif //_DEBUG
          return ret;
        }
        // cout << ctp_param->instrument[k] << kperiod_postfix[i + 4] << ", size
        // " << this_size << endl;
        indexes_[k].kptr[i].olen = indexes_[k].kptr[i].len = this_size;
        indexes_[k].kptr[i].ptr = reinterpret_cast<KINDLE *>(ptr);
#ifdef _DEBUG_
// cout << "\tptr(" << k << ", " << i << ")->" << (char*)ptr - (char*)indexes_
// << endl;
#endif //_DEBUG_
        indexes_[k].kptr[i].maxlen = this_size + addtion_size[i];
        indexes_[k].kptr[i].period = gkperiod[i + 4];
        indexes_[k].kptr[i].offset = ptr - (char *)dataptr_;
        ptr += indexes_[k].kptr[i].maxlen * sizeof(KINDLE);
      }
    }
  }
#ifdef _DEBUG_
//  printf("%s %d\n", __FILE__, __LINE__);
#endif

  return (0);
}

int Tick2Kindle::onNewCtpmmd(const char *instrument, const char *tradingday,
                             const CTPMMD *mmd, uint32_t spos, uint32_t epos) {
  UNUSED(tradingday);
  // cout << "onNewCtpmmd " << mmd_amount_ << ", " << instrument << ", "
  //      << tradingday << ", " << spos << " ~ " << epos << endl;

  // find the index of instrument

  int k = itmemMap_[getInstCode(instrument)] - 1;
  if (k < 0)
    return (-1);

  int ret = 0;
  for (uint32_t i = spos; !ret && i <= epos; ++i) {
    ret = convert_ctpmmd_2_kindle(instrument, k, (CTPMMD *)mmd, i);
    if (ret)
      cout << i << " Tick2Kindle convert_ctpmmd_2_kindle return " << ret
           << endl;
  }
  return ret;
}

static void set_first_updatetime(int i, KINDLE *kindle, CTPMMD *mmd,
                                 struct tm *new_tm) {
  kindle->open = kindle->low = kindle->high = kindle->close = mmd->LastPrice;
  kindle->interest = mmd->OpenInterest;
  kindle->volume = mmd->Volume;

  if (i < 11 - 4) { // m1, m5, m15, ...... h4
    kindle->UpdateTime = mmd->UpdateTime - mmd->UpdateTime % gkperiod[i + 4];

    if (i == 10 - 4) // KINDLE_PERIOD_TYPE_h4
      fix_open_time_to_trading_time(kindle, new_tm);
  } else
    kindle->UpdateTime = mmd->UpdateTime;
}

#define NOMINMAX
#ifdef min
#undef min
#endif //min

#ifdef max
#undef max
#endif //max

int Tick2Kindle::convert_ctpmmd_2_kindle(const char * instrument, int k,
                                         CTPMMD *mmdptr, uint32_t curpos) {
  // cout << "mmd_2_kindle " << instrument
  //      << " ->k= " << k << ", " << (mmdptr + curpos)->UpdateTime << ", " <<
  //      (mmdptr + curpos)->UpdateMillisec
  //      << endl;
  struct tm new_tm, old_tm;
  memset(&new_tm, 0, sizeof(new_tm)); // make valgrind happy
  memset(&old_tm, 0, sizeof(old_tm)); // make valgrind happy

  CTPMMD *mmd = (CTPMMD *)mmdptr + curpos;
  localtime_s(&new_tm, &mmd->UpdateTime);

  for (int i = 0; i <= 9; ++i) {
    if (indexes_[k].kptr[i].len >= indexes_[k].kptr[i].maxlen) {
      cout << instrument << ": " << k << ", " << i << ", "
           << indexes_[k].kptr[i].len << ",, " << indexes_[k].kptr[i].maxlen
           << endl;
      return (-2);
    }

    KINDLE *oldk = indexes_[k].kptr[i].ptr + indexes_[k].kptr[i].len - 1;
    if (oldk->volume == 0)
      set_first_updatetime(i, oldk, mmd, &new_tm);

    localtime_s(&old_tm, &oldk->UpdateTime);
    bool kindle_close = false;

    if (i < 11 - 4) { // m1, m5, m10, .... h4
      kindle_close = over_period(mmd->UpdateTime, oldk->UpdateTime, &new_tm,
                                 &old_tm, gkperiod[i + 4]);
    } else if (i == 11 - 4) {
      kindle_close = over_one_day(&old_tm, mmd->UpdateTime);
    } else if (i == 12 - 4) {
      kindle_close =
          over_one_week(&new_tm, &old_tm, mmd->UpdateTime, oldk->UpdateTime);
    } else if (i == 13 - 4) {
      CTPMMD *prev = (curpos > 0) ? mmd - 1 : mmd;
      kindle_close = over_one_month(&new_tm, mmd->UpdateTime, &old_tm,
                                    oldk->UpdateTime, prev->UpdateTime);
    }

    if (kindle_close) {
      // cout << "Got a new kindle of " << instrument << kperiod_postfix[i + 4]
      // << endl;
      indexes_[k].kptr[i].len++;
      oldk++;
      set_first_updatetime(i, oldk, mmd, &new_tm);
    } else {
      oldk->low = std::min(oldk->low, mmd->LastPrice);
      oldk->high = std::max(oldk->high, mmd->LastPrice);
      oldk->interest = mmd->OpenInterest;
      oldk->volume += mmd->Volume;
      oldk->close = mmd->LastPrice;
    }
  }

  return 0;
}

int Tick2Kindle::saveData() {
  for (size_t k = 0; k < ctp_param->instrument.size(); ++k) {
    for (int i = 0; i <= 9; ++i) {
      int ret = 0;
#ifdef _DEBUG_
      if (0 != (ret = save_kindle_csv(
                    ctp_param->instrument[k].c_str(), gkperiod[i + 4],
                    gmd_param->cal_store_path, indexes_[k].kptr[i].ptr,
                    indexes_[k].kptr[i].len))) {
        cout << "save kindle csv error" << ret << endl;
        return ret;
      }
#endif
      if (0 != (ret = save_kindle_file(
                    ctp_param->instrument[k].c_str(), gkperiod[i + 4],
                    gmd_param->cal_store_path, indexes_[k].kptr[i].ptr,
                    indexes_[k].kptr[i].len))) {
        cout << "save kindle file error" << ret << endl;
        return ret;
      }
    }
  }
  return (0);
}
