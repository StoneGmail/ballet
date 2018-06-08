#ifndef TICK2KINDLE_H
#define TICK2KINDLE_H

#include "ctpif_global.h"
#include "ctpif_data.h"
#include "ctpmem.h"

#include <map>
#include <string>

class CTPIF_API Tick2Kindle : public TickReader {
public:
  Tick2Kindle(bool has_ctpmmd);
  ~Tick2Kindle();

  // TickReader interface
public:
  int initServer();

  int onNewTick(const CtpMarketData * /*cmd*/) { return (0); }
  int onNewCtpmmd(const char *instrument, const char *tradingday,
                  const CTPMMD *mmd, uint32_t spos, uint32_t epos);

  int convert_ctpmmd_2_kindle(const char *instrument, int k, CTPMMD *mmdptr,
                              uint32_t curpos);

  DATAPTR getDataPtr() { return dataptr_; }
  size_t getDataSize() { return datasize_; }

  int saveData();

protected:
  MemMapping mmKindle_;
  DATAPTR dataptr_;
  size_t datasize_;
  INTMEMMAP itmemMap_;
  KindleMemIndex *indexes_;
};

#endif // TICK2KINDLE_H
