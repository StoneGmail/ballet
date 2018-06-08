#ifndef CTPMEM_H
#define CTPMEM_H

#include "ctpif_global.h"
#include "ctpif_data.h"
#include "utility_global.h"
#include "utility.h"
#include "mem.h"

typedef struct GETNEWTICK {
  bool has_new_tick;
  int cur_tick_buf;
  int cur_start_pos;
  int cur_end_pos;
  int pre_tick_buf;
  int pre_start_pos;
  int pre_end_pos;
  int amount;
} GETNEWTICK;

class CTPIF_API CtpMdReader {
public:
  CtpMdReader(bool has_ctpmmd);
  ~CtpMdReader();

  int initReader();
  int destroyReader();

  CtpMarketData *getTickbuf(int index);
  CtpmmdDataBuffer *getRealtimeMmd(std::string instrument);
  CtpmmdDataBuffer *getRealtimeMmd(int index);

  /**
   * @brief getNewCtpmmd if instrument has new CTPMMD data, the retuen the
   * pointeer and start position and end position
   * @param instrument the name of instrument
   * @param spos out, the start position
   * @param epos out, the end position
   * @return if has new data, then returen the pointer, else return 0
   */
  CTPMMD *getNewCtpmmd(string instrument, uint32_t &spos, uint32_t &epos);

  bool hasNewTick();
  GETNEWTICK *getNewTick();

  int getTickbufIndex();

private:
  SharedMemory **rtBuffer_; /// those buffer to save realtime tick data
  SharedMemory *
      *mdPtr_; /// those pointer of market data for calculate and analysis
  /// each pointer indicate one instrument of ctp
  MapStr2CtpmmdDataPtr maps2p_;
  int cur_tick_buf_;
  int max_size_tick_buf_;

  CtpDataBuffer **ctpbuf_;

  GETNEWTICK get_new_tick_;

  CtpmmdDataBuffer *buf_ctpmmd_reader_;
  bool has_ctpmmd_;
  CtpMarketData **ctpdata_;
  CTPMMD **ctpmmd_;
};

class CTPIF_API TickReader {
public:
  TickReader(bool has_ctpmmd);
  virtual ~TickReader();

  virtual int initServer();
  virtual void onStopServer() {}

  int start();
  void stop();

  uint32_t getCtpmmdAmount();
  uint32_t getTickAmount();

  virtual int onNewTick(const CtpMarketData *cmd) = 0;
  virtual int onNewCtpmmd(const char *instrument, const char *tradingday,
                          const CTPMMD *mmd, uint32_t spos, uint32_t epos) = 0;

protected:
  CtpMdReader *tickReader_;
  bool run_;
  GETNEWTICK *NewTick_;
  uint32_t tick_amount_;
  uint32_t mmd_amount_;
  bool has_ctpmmd_;

private:
  int on_new_tick_(const CtpMarketData *cmd, uint32_t spos, uint32_t epos);
};

#endif // CTPMEM_H
