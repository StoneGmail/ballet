#include "ctpmem.h"
#include <thread>
/*-------------------------------------------------------------*/
/*              CTP Marketing Data Reader                      */
/*-------------------------------------------------------------*/

CtpMdReader::CtpMdReader(bool has_ctpmmd)
    : rtBuffer_(0), mdPtr_(0), cur_tick_buf_(0), max_size_tick_buf_(0),
      ctpbuf_(0), buf_ctpmmd_reader_(0), has_ctpmmd_(has_ctpmmd),
      ctpdata_(0), ctpmmd_(0) {}

CtpMdReader::~CtpMdReader() { destroyReader(); }

int CtpMdReader::initReader() {
  // create the SharedMemory for CtpData
  rtBuffer_ = new SharedMemory *[gmd_param->tick_buffer_num];
  memset(rtBuffer_, 0, sizeof(SharedMemory*) * gmd_param->tick_buffer_num);
  ctpbuf_ = new CtpDataBuffer *[gmd_param->tick_buffer_num];
  memset(ctpbuf_, 0, sizeof(CtpDataBuffer *) * gmd_param->tick_buffer_num);
  ctpdata_ = new CtpMarketData *[gmd_param->tick_buffer_num];
  memset(ctpdata_, 0, sizeof(CtpMarketData *) * gmd_param->tick_buffer_num);


  max_size_tick_buf_ = static_cast<int>(ctp_param->instrument.size() *
                                        gmd_param->max_tick_per_sec *
                                        gmd_param->save_tickbuf_time * 60);

  // create mapping for CtpData
  for (uint32_t i = 0; i < gmd_param->tick_buffer_num; ++i) {
    char rtBufferName[MAX_PATH];
    snprintf(rtBufferName, MAX_PATH, "rtbuffer_%d", i);
    rtBuffer_[i] = SharedMemory::getMemMapping();
    if (!rtBuffer_[i])
      return (-1);
    int res = rtBuffer_[i]->OpenRead(
        rtBufferName,
        sizeof(CtpDataBuffer) + max_size_tick_buf_ * sizeof(CtpMarketData));
    if (res) {
      // cout << "read open ctp rt buffer " << rtBufferName << " bad\n";
      destroyReader();
      return res;
    } else { // init CtpDataBuffer
      // cout << "read open ctp rt buffer " << rtBufferName << " ok\n";
      ctpbuf_[i] =
          reinterpret_cast<CtpDataBuffer *>(rtBuffer_[i]->GetDataPtr());
      ctpdata_[i] = (CtpMarketData*)((char*)ctpbuf_[i] + sizeof(CtpDataBuffer));
#ifdef _DEBUG_
      //printf("%lx ctpbuf_ %u\n", ctpbuf_[i], i);
#endif //_DEBUG_
    }
  }

  // initial GETNEWTICK
  memset(&get_new_tick_, 0xff, sizeof(get_new_tick_));
  get_new_tick_.cur_tick_buf = 0, get_new_tick_.has_new_tick = false;

  if (has_ctpmmd_) {
    // Create Ctpmmd mapping
    mdPtr_ = new SharedMemory *[ctp_param->instrument.size()];
    memset(mdPtr_, 0, sizeof(SharedMemory*) * ctp_param->instrument.size());
    // ctpmmd_ = new CTPMMD *[ctp_param->instrument.size()];
    // memset(ctpmmd_, 0, sizeof(CTPMMD*) * ctp_param->instrument.size());

    buf_ctpmmd_reader_ = new CtpmmdDataBuffer[ctp_param->instrument.size()];

    if (!mdPtr_)
      return (-3);
    size_t datasize_ = sizeof(CtpmmdDataBuffer) + 2 * gmd_param->max_time_long * sizeof(CTPMMD);
    for (uint32_t i = 0; i < ctp_param->instrument.size(); ++i) {
      mdPtr_[i] = SharedMemory::getMemMapping();
      if (!mdPtr_[i])
        return (-4);
      int res = mdPtr_[i]->OpenRead(ctp_param->instrument[i].c_str(),
                                    datasize_);
      if (res) {
        // cout << ctp_param->instrument[i] << " reader opened BAD\n";
        destroyReader();
        return res;
      } else {
        // cout << ctp_param->instrument[i] << " reader opened ok\n";
        CtpmmdDataBuffer *buf =
            reinterpret_cast<CtpmmdDataBuffer *>(mdPtr_[i]->GetDataPtr());
        PairStrCtpmmdDataPtr pair(ctp_param->instrument[i], buf);
        maps2p_.insert(pair);
        buf_ctpmmd_reader_[i].insind = i;
        buf_ctpmmd_reader_[i].pos = -1;
        buf_ctpmmd_reader_[i].ctpmmd = (CTPMMD*)((char*)buf + sizeof(CtpmmdDataBuffer));
#ifdef _DEBUG_
        //printf("ctpmmd ptr %s, %lx\n", ctp_param->instrument[i].c_str(), buf_ctpmmd_reader_[i].ctpmmd);
#endif //_DEBUG
        // buf_ctpmmd_reader_[i].ctpmmd = buf->ctpmmd;
      }
    }
  }
  return (0);
}

int CtpMdReader::destroyReader() {
  SafeDeleteAry(ctpmmd_);
  SafeDeleteAry(ctpdata_);

  if (rtBuffer_) {
    for (uint32_t i = 0; i < gmd_param->tick_buffer_num; ++i) {
      if (rtBuffer_[i]) {
        rtBuffer_[i]->CloseFile();
        rtBuffer_[i]->Delete();
      }
    }

    delete[] rtBuffer_;
    rtBuffer_ = 0;
  }

  SafeDeleteAry(ctpbuf_);

  if (mdPtr_) {
    for (uint32_t i = 0; i < ctp_param->instrument.size(); ++i) {
      if (mdPtr_[i]) {
        mdPtr_[i]->CloseFile();
        mdPtr_[i]->Delete();
        maps2p_.erase(ctp_param->instrument[i]);
      }
    }

    delete[] mdPtr_;
    mdPtr_ = 0;

    maps2p_.clear();
  }

  SafeDeleteAry(buf_ctpmmd_reader_);

  return (0);
}

CtpMarketData *CtpMdReader::getTickbuf(int index) {
  //if (!ctpbuf_ || index < 0 || (uint32_t)index > gmd_param->tick_buffer_num ||
  //    !(ctpbuf_[index]))
  //  return (0);
  //else
  //  //return ctpbuf_[index];
  return ctpdata_[index];
}

CTPMMD *CtpMdReader::getNewCtpmmd(std::string instrument, uint32_t &spos,
                                  uint32_t &epos) {
  CtpmmdDataBuffer *buf = maps2p_.find(instrument)->second;
  if (buf && buf_ctpmmd_reader_[buf->insind].pos < buf->pos) {
    spos = buf_ctpmmd_reader_[buf->insind].pos + 1;
    epos = buf->pos;
    buf_ctpmmd_reader_[buf->insind].pos = buf->pos;
#ifdef _DEBUG_
    //printf("new ctpmmd %s %lx\n", instrument.c_str(), buf_ctpmmd_reader_[buf->insind].ctpmmd);
#endif //_DEBUG_
    return buf_ctpmmd_reader_[buf->insind].ctpmmd;
  }
  return 0;
}

CtpmmdDataBuffer *CtpMdReader::getRealtimeMmd(std::string instrument) {
  if (!mdPtr_)
    return 0;
  else
    return maps2p_.find(instrument)->second;
}

CtpmmdDataBuffer *CtpMdReader::getRealtimeMmd(int index) {
  if (!mdPtr_ || index < 0 || (uint32_t)index > ctp_param->instrument.size() ||
      !(mdPtr_[index]) || !(mdPtr_[index]->IsValidFile()))
    return (0);
  else
    return (CtpmmdDataBuffer *)mdPtr_[index]->GetDataPtr();
}

int CtpMdReader::getTickbufIndex() { return cur_tick_buf_; }

bool CtpMdReader::hasNewTick() {
  get_new_tick_.has_new_tick = false;
  for (int i = 0; i < (int)gmd_param->tick_buffer_num; ++i) {
    if (ctpbuf_[i]->isworking) {
      if (cur_tick_buf_ != i) {
        // has new tick and buffer was changed
        get_new_tick_.has_new_tick = true;
        get_new_tick_.pre_tick_buf = cur_tick_buf_;
        get_new_tick_.pre_start_pos = get_new_tick_.cur_end_pos + 1;
        get_new_tick_.pre_end_pos = max_size_tick_buf_ - 1;
        // cout << "buf was changed, prev is " << cur_tick_buf_ << ", start "
        //      << get_new_tick_.pre_start_pos << ", end " << get_new_tick_.pre_end_pos
        //      << endl;
        get_new_tick_.amount =
            get_new_tick_.pre_end_pos - get_new_tick_.pre_start_pos + 1;
        get_new_tick_.cur_tick_buf = i;
        get_new_tick_.cur_start_pos = 0;
        get_new_tick_.cur_end_pos = ctpbuf_[i]->pos;
        get_new_tick_.amount +=
            (get_new_tick_.cur_end_pos - get_new_tick_.cur_start_pos + 1);
        // change prev-tick-buf to current-buffer
        get_new_tick_.pre_tick_buf = cur_tick_buf_;
        // change current-bufer to i
        cur_tick_buf_ = i;
        // cout << "\tcurrent buf is " << i << ", start "
        //      << get_new_tick_.cur_start_pos << ", end " << get_new_tick_.cur_end_pos
        //      << endl;
      } else {
        if (ctpbuf_[i]->pos > get_new_tick_.cur_end_pos) {
          // cout << "checked, buf is " << i << ", start "
          //      << get_new_tick_.cur_end_pos + 1 << ", end " << ctpbuf_[i]->pos
          //      << endl;
          get_new_tick_.has_new_tick = true;
          get_new_tick_.cur_start_pos = get_new_tick_.cur_end_pos + 1;
          get_new_tick_.cur_end_pos = ctpbuf_[i]->pos;
          get_new_tick_.amount =
              get_new_tick_.cur_end_pos - get_new_tick_.cur_start_pos + 1;
        }
      }
      break;
    }
  }

  return get_new_tick_.has_new_tick;
}

GETNEWTICK *CtpMdReader::getNewTick() { return &get_new_tick_; }

TickReader::TickReader(bool has_ctpmmd)
    : tickReader_(0), run_(false), tick_amount_(0), mmd_amount_(0),
      has_ctpmmd_(has_ctpmmd) {}
TickReader::~TickReader() {
  stop();
  std::this_thread::sleep_for(std::chrono::seconds(5));
  SafeDeletePtr(tickReader_);
}
int TickReader::initServer() {
  tickReader_ = new CtpMdReader(has_ctpmmd_);
  int result = tickReader_->initReader();
  if (!result)
    NewTick_ = tickReader_->getNewTick();
  return result;
}

int TickReader::start() {
  int result = 0;
  run_ = true;
  while (run_ && !result) {
    std::this_thread::sleep_for(
        std::chrono::microseconds(gmd_param->save_loop_interval));
    if (tickReader_->hasNewTick()) {
      tick_amount_ += NewTick_->amount;

      if (NewTick_->amount >
          NewTick_->cur_end_pos - NewTick_->cur_start_pos + 1)
        result = on_new_tick_(
            tickReader_->getTickbuf(NewTick_->pre_tick_buf),
            NewTick_->pre_start_pos, NewTick_->pre_end_pos);

      if (!result)
        result = on_new_tick_(
            tickReader_->getTickbuf(NewTick_->cur_tick_buf),
            NewTick_->cur_start_pos, NewTick_->cur_end_pos);
    }
  }
  if (run_ && result) cout << "TickReader error exit " << result << endl;
  // cout << "received " << tick_amount_ << endl;
  return result;
}

void TickReader::stop() {
  run_ = false;
  onStopServer();
}


#ifdef _DEBUG_
static uint32_t kkk =0;
#endif //_DEBUG
int TickReader::on_new_tick_(const CtpMarketData *cmd, uint32_t spos,
                             uint32_t epos) {
  int result = 0;
  for (uint32_t i = spos; (!result) && i <= epos; ++i) {
#ifdef _DEBUG_
    ++kkk;
    printf("got new tick %d, %s, %0.2f, %d\n",
           kkk,
           (cmd + i)->InstrumentID,
           (cmd + i)->LastPrice,
           (cmd + i)->Volume);
#endif //_DEBUG_
    onNewTick(cmd + i);

    if (has_ctpmmd_ && !result) {
      // save the CTPMMD data
      uint32_t spos, epos;
      CTPMMD *mmd =
          tickReader_->getNewCtpmmd((cmd + i)->InstrumentID, spos, epos);
      if (mmd) {
        mmd_amount_ += (epos - spos) + 1;
        result = onNewCtpmmd((cmd + i)->InstrumentID, (cmd + i)->TradingDay,
                             mmd, spos, epos);
      }
    }
  }

  return result;
}

uint32_t TickReader::getCtpmmdAmount() { return mmd_amount_; }

uint32_t TickReader::getTickAmount() { return tick_amount_; }
