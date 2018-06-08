#ifndef _TICK_DATA_H_INCLUDE_
#define _TICK_DATA_H_INCLUDE_

#include <cstddef>
#include <iostream>
#include <fstream>
#include <vector>

#ifdef UTILITY_LIBRARY
#include "utility_global.h"
#else //UTILITY_LIBRARY
#  ifndef UTILITY_API
#    define UTILITY_API
#  endif //UTILITY_API
#endif //UTILITY_LIBRARY


typedef struct TICK_DATA {
  double ftime = 0;
  double price = 0;
} TICK_DATA;

extern UTILITY_API int tick_csv_to_bin(const char *csv_filepath,
                                       const char *bin_filepath);

extern UTILITY_API int find_pos_by_date(const char* strdate,
                                      const char* bin_filepath,
                                      const char* cdi_filepath,
                                      uint64_t& pos);

extern UTILITY_API int load_tick_to_pos(const char* bin_filepath,
                                        uint64_t pos,
                                        uint64_t size,
                                        std::vector<double>* price,
                                        std::vector<double>* ftime);

extern UTILITY_API int load_tick_to_last(const char* bin_filepath,
                        uint64_t size, std::vector<double>* price,
                                       std::vector<double>* ftime);

extern UTILITY_API int save_tick_bin(const char* datapath, const char *inst,
                                     TICK_DATA* td, int size);

extern UTILITY_API int gen_cdi(const char *bin_filepath,
                               const char *cdi_filepath);

#endif // _TICK_DATA_H_INCLUDE_
