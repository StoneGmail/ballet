#ifndef CTPIF_H
#define CTPIF_H

#include "ctpif_global.h"
#include "ctpif_data.h"
#include "ctpifio.h"
#include "ctpmdspi.h"
#include "ctptdspi.h"


/**
 * @brief get ctp-parameters from the file
 * @param filepath, the config file in json
 * @return zero for suceess, non-zero for error
 */
extern CTPIF_API GmdParam* initConfig(const char *filepath);
/**
 * @brief releaseCtpMdIf Release the Ctp Interface for market data
 */
extern CTPIF_API void releaseConfig(GmdParam* param);


extern CTPIF_API bool invalid_data(CtpMarketData& data);
/**
 * @brief convert_ctpmd_2_ctpmmd convert the CtpMarketData format to CTPMMD
 */
extern void CTPIF_API convert_ctpmd_2_ctpmmd(CTPMMD *mmd, CtpMarketData *data);
extern void CTPIF_API convert_ctpmd_2_ctpmd(DATAPTR d1, DATAPTR d2);


/**
 * @brief tb_2_ctpmmd convete TB-tick-data to CTPMMD format
 * @param tbcsv the TB-tick-data file
 * @param mmdfile the CTPMMD file
 * @return zero for good
 */
extern CTPIF_API int sample_tick_data(const char *tick_csv,
                                        const char *mdfile, const char *csvfile,
                                        const int interval);

extern CTPIF_API int connect_to_ctp_tdif(CtpTradeSpi* trade_spi);

extern CTPIF_API CtpMdSpi* get_ctp_mdif(GmdParam *param);

extern CTPIF_API CtpTradeSpi* get_ctp_tdif(GmdParam *param, const char *logpath);

extern CTPIF_API void connect_to_ctp_mdif(CtpMdSpi* mdspi);

extern CTPIF_API void output_inst_summary(ostream& out, const MapInstrument& map_inst);


extern CTPIF_API int ctp_2_candle(DATAPTR bar_ptr, DATAPTR ctpdata_ptr);
extern CTPIF_API bool its_new_candle_data(int64_t pos, DATAPTR old, DATAPTR bar_data);
extern CTPIF_API void merge_candle_data(DATAPTR bar_old, DATAPTR bar_new);


#endif // CTPIF_H
