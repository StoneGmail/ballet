#ifndef GOMMEM_H
#define GOMMEM_H

#include "gom_global.h"
#include "utility.h"
#include "mem.h"
#include "dataset.h"

class GOM_API CtpMdMemory {
public:
  CtpMdMemory(bool has_ctpmmd);
  ~CtpMdMemory();

  int initMemory();
  int destroyMemory();

  CtpDataBuffer *getTickbuf(int index);
  CtpmmdDataBuffer *getRealtimeMmd(std::string instrument);
  CtpmmdDataBuffer *getRealtimeMmd(int index);

  int addNewTick(CtpMarketData *data);

  int getTickbufIndex();

  size_t getTickAmount() { return amountTick_; }
  size_t getCtpmmdAmount() { return amountCtpmmd_; }

private:
  SharedMemory **rtBuffer_; /// those buffer to save realtime tick data
  SharedMemory *
      *mdPtr_; /// those pointer of market data for calculate and analysis
  /// each pointer indicate one instrument of ctp
  MapStr2CtpmmdDataPtr maps2p_;
  int cur_tick_buf_;
  int max_size_tick_buf_;

  CtpDataBuffer **ctpbuf_;


  bool has_ctpmmd_;
  size_t amountTick_;
  size_t amountCtpmmd_;

};
extern GOM_API CtpMdMemory *gmdmem;

/*! \class CtpMdSharedMemory
  \brief store the ctp raw datad and CTPMD raw data

  the struct of memory block are
  +---------------------------------------------+
  | Ctp Tick Raw Data Buffer 1                  |
  +---------------------------------------------+
  | Ctp Tick Raw Data Buffer 2                  |
  +---------------------------------------------+
  | ...                                         |
  +---------------------------------------------+
  | Ctp Raw Data Buffer N                       |
  +---------------------------------------------+
  | CTPMD Raw Data, Instrument N                |
  +---------------------------------------------+
  | CTPMD Raw Data, Instrument 1                |
  +---------------------------------------------+
  | CTPMD Raw Data, Instrument 2                |
  +---------------------------------------------+
  | ...                                         |
  +---------------------------------------------+
  | CTPMD Raw Data, Instrument N                |
  +---------------------------------------------+

  each Ctp Raw block has DataSet struct:
  +---------------------------------------------+
  | DataSetInfo | Data ...                      |
  +---------------------------------------------+
  and each CTPMD Raw block has TsDataSet struct:
  +---------------------------------------------+
  | TsDataSetInfo | Data ...                    |
  +---------------------------------------------+
*/

class GOM_API CtpMdWriter {
public:
  CtpMdWriter(bool has_ctpmmd = true);
  ~CtpMdWriter();

  int initMemory();
  int destroyMemory();

  DATASET(CtpMarketData) *getTickbuf(int64_t index);
  TSDATAFRAME(CTPMD) *getRealtimeMd(std::string instrument);
  TSDATAFRAME(CTPMD) *getRealtimeMd(int64_t index);

  int addNewTick(CtpMarketData *data);

  int64_t getTickbufIndex();

  size_t getTickAmount() { return amountTick_; }
  size_t getCtpmmdAmount() { return amountCtpmmd_; }

private:
  int mapping_memory();

private:
  SharedMemory* shared_mem_;
  DATASET(CtpMarketData)** ctp_data_set_;
  TSDATAFRAME(CTPMD)** ctpmd_df_;
  int64_t cur_tick_buf_;
  size_t tick_buf_size_;
  size_t ctpmd_buf_size_;
  size_t memsize_;
  bool has_ctpmmd_;
  size_t amountTick_;
  size_t amountCtpmmd_;
  MapStr2Ptr maps2p_;
};

extern GOM_API CtpMdMemory *gmdmem;


#endif // GOMMEM_H
