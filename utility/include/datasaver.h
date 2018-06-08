#ifndef DATASAVER_H
#define DATASAVER_H

#include "ctpif_global.h"
#include "ctpif_data.h"
#include "ctpmem.h"

class CTPIF_API DataSaver : public TickReader {
public:
  DataSaver(bool has_ctpmmd);
  ~DataSaver();

  virtual int onNewTick(const CtpMarketData *cmd);
  virtual int onNewCtpmmd(const char *instrument, const char *tradingday,
                          const CTPMMD *mmd, uint32_t spos, uint32_t epos);
};

#endif // DATASAVER_H
