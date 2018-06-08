#ifndef UTILITY_H
#define UTILITY_H

#include "utility_global.h"
#include "def.h"
#include "mderr.h"
#include "str.h"
#include "mylog.h"
#include "mem.h"
#include "dataset.h"


/**
 * @brief make_the_time converte 'yyyymmdd' and 'hh:mm:ss' to time_t
 * @param TradeDate the date
 * @param TradeTime the time
 * @return the time in time_t
 */
time_t UTILITY_API make_the_time(char *TradeDate, char *TradeTime);

/**
 * @brief make_the_time converte 'yyyymmdd' and 'hh:mm:ss' to time_t
 * @param TradeDate the date
 * @param TradeTime the time in 'hhmmss'
 * @return the time in time_t
 */
time_t UTILITY_API make_the_time(char *TradeDate, uint32_t TradeTime);
double UTILITY_API make_the_time(char* ActionDay, char *uptime, int32_t millisec);
/**
 * @brief make_the_time converte 'yyyymmdd' and 'hh:mm:ss' to time_t
 * @param TradeDate the date
 * @param TradeTime the time in 'hhmmss'
 * @return the time in time_t
 */
time_t UTILITY_API make_the_time(uint32_t TradeDate, uint32_t TradeTime);

time_t UTILITY_API make_the_time(time_t tt, uint32_t TradeTime);

/**
 * @brief outbuf output the memory in hex
 * @param data, the pointer to memory
 * @param length the length of data
 */
void UTILITY_API outbuf(DATAPTR data, size_t length);

/**
 * @brief minString return minimal matched string in multimap
 * @param mapInst the multimap of regex to instrument
 * @param codeInst the code of instrument
 * @return the minimal code of instrument
 */
extern UTILITY_API std::string
minString(std::multimap<std::string, std::string> &mapInst,
          std::string codeInst);

/**
 * @brief match_index return str matched index in vector<regex>
 * @param regexInstruments the vector of regex
 * @param str the input string
 * @return the index in vector or -1 for did not match
 */
extern UTILITY_API int match_index(std::vector<regex> &regexInstruments,
                                   const char *str);

/**
 * @brief getInstCode return the code of a instrument
 * @param InstRegex the regex of a instrument
 * @return the code
 */
extern UTILITY_API std::string getInstCode(std::string instrument);

/**
 * @brief getInstContinuedCode return the continued code of a instrument
 * @param InstRegex the regex of a instrument
 * @return the continued  code
 */
const UTILITY_API std::string getInstContinuedCode(std::string instrument);

/**
 * @brief get_exit_time get time from time-string in "hh:mm:ss" format
 * @param the_time time-string
 * @return the exit time
 */
extern UTILITY_API time_t get_exit_time(const char *the_time);

extern UTILITY_API int get_days_in_month(const struct tm *ttm);

extern UTILITY_API int compare_files(const char* file1, const char* file2);




template<typename K, typename T>
void clear_map(std::map<K, T*>& m) {
  for (auto& p : m) {
    if (p.second)
        delete (p.second);
  }
  m.clear();
}

template<typename T>
void clear_vector(std::vector<T*>& v) {
  for (auto&p : v) {
    if (p)
      delete p;
  }
  v.clear();
}

extern UTILITY_API std::vector<std::string> split(const string& input, const string& regex);


#endif // UTILITY_H
