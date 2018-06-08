#ifndef CTPDI_H
#define CTPDI_H

#include "ctpif_global.h"
#include "ctpif_data.h"
#include "ctpmem.h"
#include "utility.h"
#include "utility_global.h"

typedef void (*NEW_KINDLE_CALLBACK)(const char *instrument, KINDLE *kindle,
                                    int period);

class CTPIF_API Ctpdi {
public:
  Ctpdi();
  ~Ctpdi();

  int open_data_interface();
  int close_data_interface();
  bool has_new_kindle(const char *inst, CANDLE_PERIOD_TYPE period);
  KINDLE *get_kindle_data(const char *inst, CANDLE_PERIOD_TYPE period,
                          size_t *len, size_t *oldlen, size_t *newlen);
  KINDLE *search_kindle_data(const char *inst, size_t *len,
                             CANDLE_PERIOD_TYPE period, uint32_t stime,
                             uint32_t etime);

  int start_listen(NEW_KINDLE_CALLBACK kindle_callback);

private:
  void set_lengths();

private:
  MemMapping mmKindle_;
  MemMapping mmTA_;

  INTMEMMAP itmemMap_;
  KindleMemIndex *kindle_index_;

  size_t datasize_;
  size_t *curpos_;
};

// typedef char TThostFtdcInstrumentIDType[31]

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

extern CTPIF_API int open_data_interface();
extern CTPIF_API int start_listen(NEW_KINDLE_CALLBACK kindle_callback);
extern CTPIF_API void stop_listen();
extern CTPIF_API bool is_listening();
extern CTPIF_API int close_data_interface();
extern CTPIF_API KINDLE *get_kindle_data(const char *inst, const char *period,
                                         size_t *len, size_t *oldlen,
                                         size_t *newlen);

extern CTPIF_API KINDLE *search_kindle_data(const char *inst, size_t *len,
                                            CANDLE_PERIOD_TYPE period,
                                            uint32_t stime, uint32_t etime);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // CTPDI_H
