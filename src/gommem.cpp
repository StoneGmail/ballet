#include "gommem.h"

CtpMdMemory *gmdmem = NULL;

/*-------------------------------------------------------------*/
/*              CTP Marketing Data Memory                      */
/*-------------------------------------------------------------*/

CtpMdMemory::CtpMdMemory(bool has_ctpmmd)
    : rtBuffer_(0), mdPtr_(0), cur_tick_buf_(0), max_size_tick_buf_(0),
      ctpbuf_(0), has_ctpmmd_(has_ctpmmd), amountTick_(0), amountCtpmmd_(0) {}

CtpMdMemory::~CtpMdMemory() { destroyMemory(); }

int CtpMdMemory::initMemory() {
  // create the SharedMemory for CtpData
  rtBuffer_ = new SharedMemory *[gmd_param->tick_buffer_num];
  memset(rtBuffer_, 0, sizeof(SharedMemory *) * gmd_param->tick_buffer_num);
  ctpbuf_ = new CtpDataBuffer *[gmd_param->tick_buffer_num];
  memset(ctpbuf_, 0, sizeof(CtpDataBuffer *) * gmd_param->tick_buffer_num);

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
    int res = rtBuffer_[i]->OpenWrite(
        gmd_param->tick_data_path, rtBufferName,
        sizeof(CtpDataBuffer) + max_size_tick_buf_ * sizeof(CtpMarketData));
    if (res) {
      destroyMemory();
      return res;
    }
    { // init CtpDataBuffer
      CtpDataBuffer *buf =
          reinterpret_cast<CtpDataBuffer *>(rtBuffer_[i]->GetDataPtr());
      buf->pos = -1;
      buf->isworking = false;
      buf->ctpdata = reinterpret_cast<CtpMarketData *>(buf + 1);
      ctpbuf_[i] = buf;
    }
    {
      if (!i) {
        cur_tick_buf_ = 0;
        ctpbuf_[0]->isworking = true;
      }
    }
  }

  if (has_ctpmmd_) {
    // Create Ctpmmd mapping
    mdPtr_ = new SharedMemory *[ctp_param->instrument.size()];
    if (!mdPtr_)
      return (-3);
    size_t datasize_ = sizeof(CtpmmdDataBuffer) +
                       2 * gmd_param->max_time_long * sizeof(CTPMMD);
    for (uint32_t i = 0; i < ctp_param->instrument.size(); ++i) {
      mdPtr_[i] = SharedMemory::getMemMapping();
      if (!mdPtr_[i])
        return (-4);
      int res = mdPtr_[i]->OpenWrite(
          gmd_param->data_rtpath, ctp_param->instrument[i].c_str(), datasize_);
      if (res) {
        destroyMemory();
        return res;
      } else {
        CtpmmdDataBuffer *buf =
            reinterpret_cast<CtpmmdDataBuffer *>(mdPtr_[i]->GetDataPtr());
        buf->insind = i;
        buf->pos = -1;
        buf->ctpmmd = reinterpret_cast<CTPMMD *>(buf + 1);

        PairStrCtpmmdDataPtr pair(ctp_param->instrument[i], buf);
        maps2p_.insert(pair);
      }
    }
  }

  amountTick_ = amountCtpmmd_ = 0;
  return (0);
}

int CtpMdMemory::destroyMemory() {
  if (rtBuffer_) {
    for (uint32_t i = 0; i < gmd_param->tick_buffer_num; ++i) {
      if (rtBuffer_[i]) {
        rtBuffer_[i]->CloseFile();
        rtBuffer_[i]->Delete();
      }
    }

    SafeDeleteAry(rtBuffer_);
  }

  if (ctpbuf_)
    SafeDeleteAry(ctpbuf_);

  if (mdPtr_) {
    for (uint32_t i = 0; i < ctp_param->instrument.size(); ++i) {
      if (mdPtr_[i]) {
        mdPtr_[i]->CloseFile();
        mdPtr_[i]->Delete();
        maps2p_.erase(ctp_param->instrument[i]);
      }
    }

    SafeDeleteAry(mdPtr_);
    maps2p_.clear();
  }

  amountTick_ = amountCtpmmd_ = 0;
  return (0);
}

CtpDataBuffer *CtpMdMemory::getTickbuf(int index) {
  if (!ctpbuf_ || index < 0 || (uint32_t)index > gmd_param->tick_buffer_num ||
      !(ctpbuf_[index]))
    return (0);
  else
    return ctpbuf_[index];
}

CtpmmdDataBuffer *CtpMdMemory::getRealtimeMmd(std::string instrument) {
  if (!mdPtr_)
    return 0;
  else
    return maps2p_.find(instrument)->second;
}

CtpmmdDataBuffer *CtpMdMemory::getRealtimeMmd(int index) {
  if (!mdPtr_ || index < 0 || (uint32_t)index > ctp_param->instrument.size() ||
      !(mdPtr_[index]) || !(mdPtr_[index]->IsValidFile()))
    return (0);
  else
    return (CtpmmdDataBuffer *)mdPtr_[index]->GetDataPtr();
}

int CtpMdMemory::addNewTick(CtpMarketData *data) {
  ++amountTick_;
  // convert 2 ctpmmd
  CtpmmdDataBuffer *buf = maps2p_.find(data->InstrumentID)->second;
  if (buf) {
    if (has_ctpmmd_) {
      ++amountCtpmmd_;
      convert_ctpmd_2_ctpmmd(buf->ctpmmd + (buf->pos + 1), data);
      buf->pos++;
    }

    // current buffer is full
    if (ctpbuf_[cur_tick_buf_]->pos + 1 >= max_size_tick_buf_) {
      // cout << "change buffer " << cur_tick_buf_ << " to ";
      ctpbuf_[cur_tick_buf_]->isworking = false;
      cur_tick_buf_++;
      cur_tick_buf_ %= gmd_param->tick_buffer_num;
      // cout << cur_tick_buf_ << ", amount " << amountTick_ << endl;
      ctpbuf_[cur_tick_buf_]->pos = -1;
      ctpbuf_[cur_tick_buf_]->isworking = true;
    }

    // copy new tick to buffer
    memcpy(ctpbuf_[cur_tick_buf_]->ctpdata + (ctpbuf_[cur_tick_buf_]->pos + 1),
           data, sizeof(CtpMarketData));
    // cout << "put to pos @" << ctpbuf_[cur_tick_buf_]->pos + 1;
    // add new tick count
    ctpbuf_[cur_tick_buf_]->pos++;
    // cout << ", and now pos is " << ctpbuf_[cur_tick_buf_]->pos << endl;
  }

  return 0;
}

int CtpMdMemory::getTickbufIndex() { return cur_tick_buf_; }

// ////////////////////////////////////////////////////////////////////
// CtpMdSharedMemory
// ////////////////////////////////////////////////////////////////////
CtpMdWriter::CtpMdWriter(bool has_ctpmmd)
    : shared_mem_(0), ctp_data_set_(0), ctpmd_df_(0), cur_tick_buf_(0),
      tick_buf_size_(0), ctpmd_buf_size_(0), memsize_(0),
      has_ctpmmd_(has_ctpmmd), amountTick_(0), amountCtpmmd_(0) {}
CtpMdWriter::~CtpMdWriter() { destroyMemory(); }

int CtpMdWriter::mapping_memory() {
  int ret = 0;
  char *memptr = (char *)(shared_mem_->GetDataPtr());
  // DATASET CtpMarketData
  ctp_data_set_ = new DATASET(CtpMarketData) *[gmd_param->tick_buffer_num];
  for (uint32_t i = 0; i < gmd_param->tick_buffer_num; ++i) {
    ctp_data_set_[i] = new DATASET(CtpMarketData);
    ctp_data_set_[i]->set_memory(memptr, nullptr, tick_buf_size_);
    ctp_data_set_[i]->info->di.pos = -1;
    ctp_data_set_[i]->info->di.isworking = (i) ? false : true;

    memptr += DATASET(CtpMarketData)::get_memory_size(tick_buf_size_);
  }

  cur_tick_buf_ = 0;

  if (!has_ctpmmd_)
    return ret;

  ctpmd_df_ = new TSDATAFRAME(CTPMD) *[ctp_param->instrument.size()];
  for (size_t i = 0; i < ctp_param->instrument.size(); ++i) {
    ctpmd_df_[i] = new TSDATAFRAME(CTPMD);
    ctpmd_df_[i]->set_memory(memptr, nullptr, ctpmd_buf_size_);
    ctpmd_df_[i]->info->di.index = i;
    ctpmd_df_[i]->info->di.pos = -1;

    maps2p_.insert(PairStrPtr(ctp_param->instrument[i], ctpmd_df_[i]));
    memptr += TSDATAFRAME(CTPMD)::get_memory_size(ctpmd_buf_size_);
  }

  return ret;
}

int CtpMdWriter::initMemory() {
  tick_buf_size_ = ctp_param->instrument.size() * gmd_param->max_tick_per_sec *
                   gmd_param->save_tickbuf_time * 60;
  ctpmd_buf_size_ = gmd_param->max_time_long * gmd_param->max_tick_per_sec;

  memsize_ = DATASET(CtpMarketData)::get_memory_size(tick_buf_size_) *
             gmd_param->tick_buffer_num;
  if (has_ctpmmd_)
    memsize_ += TSDATAFRAME(CTPMD)::get_memory_size(ctpmd_buf_size_) *
                ctp_param->instrument.size();

  shared_mem_ = SharedMemory::getMemMapping();

  int ret = 0;
  ret = shared_mem_->OpenWrite(gmd_param->data_rtpath, "CtpMdSharedMemory",
                               memsize_);

  if (!ret)
    ret = mapping_memory();

  if (ret)
    destroyMemory();

  return ret;
}

int CtpMdWriter::destroyMemory() {

  if (ctpmd_df_) {
    for (size_t i = 0; i < ctp_param->instrument.size(); ++i)
      if (ctpmd_df_[i])
        delete ctpmd_df_[i];
    delete[] ctpmd_df_;
    ctpmd_df_ = 0;
  }

  if (ctp_data_set_) {
    for (size_t i = 0; i < gmd_param->tick_buffer_num; ++i)
      if (ctp_data_set_[i])
        delete ctp_data_set_[i];
    delete[] ctp_data_set_;
    ctp_data_set_ = 0;
  }

  if (shared_mem_) {
    shared_mem_->CloseFile();
    delete shared_mem_;
    shared_mem_ = 0;
  }
  return 0;
}

DATASET(CtpMarketData) * CtpMdWriter::getTickbuf(int64_t index) {
  if (index >= 0 && index < gmd_param->tick_buffer_num && ctp_data_set_)
    return ctp_data_set_[index];
  return nullptr;
}

TSDATAFRAME(CTPMD) * CtpMdWriter::getRealtimeMd(std::string instrument) {
  return reinterpret_cast<TSDATAFRAME(CTPMD) *>(maps2p_[instrument]);
}

TSDATAFRAME(CTPMD) * CtpMdWriter::getRealtimeMd(int64_t index) {
  if (index >= 0 && index < (int64_t)ctp_param->instrument.size() && ctpmd_df_)
    return ctpmd_df_[index];
  return nullptr;
}

int64_t CtpMdWriter::getTickbufIndex() { return cur_tick_buf_; }

int CtpMdWriter::addNewTick(CtpMarketData *data) {
  int ret = 0;

  ++amountTick_;
  CTPMD ctpmd;
  if (has_ctpmmd_) {
    TSDATAFRAME(CTPMD) *dfCtpmd =
        reinterpret_cast<TSDATAFRAME(CTPMD) *>(maps2p_[data->InstrumentID]);
    if (dfCtpmd) {
      ++amountCtpmmd_;
      // convert 2 ctpmd
      convert_ctpmd_2_ctpmd(&ctpmd, data);
      ret = dfCtpmd->append(&ctpmd);
    } else
      ret = -2;
  }

  if (!ret) {
    // current buffer is full
    ret = ctp_data_set_[cur_tick_buf_]->append(data);
    if (ret && ret == MDERR_MEMORY_IS_FULL) {
      ctp_data_set_[cur_tick_buf_]->set_working(false);
      cur_tick_buf_++;
      cur_tick_buf_ %= gmd_param->tick_buffer_num;
      ctp_data_set_[cur_tick_buf_]->set_working();

      ret = ctp_data_set_[cur_tick_buf_]->append(data);
    }
  }

  return ret;
}
