#include "datasaver.h"
#include "mderr.h"
#include "str.h"
#include "utility.h"
#include <thread>

DataSaver::DataSaver(bool has_ctpmmd) : TickReader::TickReader(has_ctpmmd) {}

DataSaver::~DataSaver() {}

int DataSaver::onNewTick(const CtpMarketData *cmd) {
  char filepath[MAX_PATH];

  // save the Ctp-Market-Data
  snprintf(filepath, MAX_PATH, "%s/%s-%s.tick", gmd_param->data_store_path,
           cmd->InstrumentID, cmd->TradingDay);
  // cout << filepath << endl;
  FILE *fp = 0;
  int result = fopen_s(&fp, filepath, "a+b");
  if (result)
    return MDERR_CANNOT_OPEN_INSTICKFILE;
  if (1 !=
      fwrite(cmd, sizeof(CtpMarketData), 1, fp))
    return MDERR_WRITE_TICK_CTPMARKETDATA;
  fclose(fp);

  return 0;
}

int DataSaver::onNewCtpmmd(const char *instrument, const char *tradingday,
                           const CTPMMD *mmd, uint32_t spos, uint32_t epos) {
  char filepath[MAX_PATH];

  // cout << "DataSaver " << instrument << tradingday << endl;
  // save the Ctp-Mmd-Data
  snprintf(filepath, MAX_PATH, "%s/%s-%s.mmd", gmd_param->data_rtpath,
           instrument, tradingday);
  FILE *fp = 0;
  int result = fopen_s(&fp, filepath, "a+b");
  if (result)
    return MDERR_CANNOT_OPEN_INSTICKFILE;
  if ((epos - spos + 1) !=
      fwrite(mmd + spos, sizeof(CTPMMD), epos - spos + 1, fp))
    return MDERR_WRITE_TICK_CTPMARKETDATA;
  fclose(fp);

  return 0;
}
