#include "utility.h"

#include <iostream>
#include <cmath>
#include <cfloat>
#include <fstream>
#include <iostream>
#include <string>

#include "rapidjson/document.h"

using namespace std;
/**
 * @brief TimeTrans::make_the_time
 * @param TradeDate in yyyymmmdd(20100906) fromat
 * @param TradeTime in hh:mm:ss(17:17:27) format if format_type is 1
 *        in hhmmss format if format_type is 2
 * @param format_type
 * @return the time in time_t format
 */
time_t make_the_time(char *TradeDate, char *TradeTime) { //, int format_type) {
  int llt = atoi(TradeDate);
  struct tm lt;
  memset(&lt, 0, sizeof(lt));

  lt.tm_year = (int)(llt / 10000) - 1900;
  lt.tm_mon = (int)(llt - (lt.tm_year + 1900) * 10000) / 100 - 1;
  lt.tm_mday = (int)(llt - (lt.tm_year + 1900) * 10000 - (lt.tm_mon + 1) * 100);

  // if (format_type == 1) {
  lt.tm_hour = (TradeTime[0] - '0') * 10 + (TradeTime[1] - '0');
  lt.tm_min = (TradeTime[3] - '0') * 10 + (TradeTime[4] - '0');
  lt.tm_sec = (TradeTime[6] - '0') * 10 + (TradeTime[7] - '0');
  //} else {
  //  // format_type == 2
  //  lt.tm_hour = (TradeTime[0] - '0') * 10 + (TradeTime[1] - '0');
  //  lt.tm_min =  (TradeTime[2] - '0') * 10 + (TradeTime[3] - '0');
  //  lt.tm_sec =  (TradeTime[4] - '0') * 10 + (TradeTime[5] - '0');
  //}

  return mktime(&lt);
}

time_t make_the_time(char *TradeDate, uint32_t TradeTime) {
  uint32_t llt = atoi(TradeDate);
  struct tm lt;
  memset(&lt, 0, sizeof(lt));

  lt.tm_year = (int)(llt / 10000) - 1900;
  lt.tm_mon = (int)(llt - (lt.tm_year + 1900) * 10000) / 100 - 1;
  lt.tm_mday = (int)(llt - (lt.tm_year + 1900) * 10000 - (lt.tm_mon + 1) * 100);

  lt.tm_hour = (int)(TradeTime / 10000);
  lt.tm_min = (int)((TradeTime - lt.tm_hour * 10000) / 100);
  lt.tm_sec = (int)(TradeTime - (lt.tm_hour * 10000) - lt.tm_min * 100);
  return mktime(&lt);
}

double make_the_time(char *ActionDay, char* uptime, int32_t millisec) {
  uint32_t llt = atoi(ActionDay);
  struct tm lt;
  memset(&lt, 0, sizeof(lt));

  lt.tm_year = (int)(llt / 10000) - 1900;
  lt.tm_mon = (int)(llt - (lt.tm_year + 1900) * 10000) / 100 - 1;
  lt.tm_mday = (int)(llt - (lt.tm_year + 1900) * 10000 - (lt.tm_mon + 1) * 100);

  //21:51:18 500
  lt.tm_hour = atoi(uptime);
  lt.tm_min = atoi(uptime + 3);
  lt.tm_sec = atoi(uptime + 6);
  return (double)(mktime(&lt)) + (double)millisec / 1000.0;
}

time_t make_the_time(uint32_t TradeDate, uint32_t TradeTime) {
  uint32_t llt = TradeDate;
  struct tm lt;

  lt.tm_year = (int)(llt / 10000) - 1900;
  lt.tm_mon = (int)(llt - (lt.tm_year + 1900) * 10000) / 100 - 1;
  lt.tm_mday = (int)(llt - (lt.tm_year + 1900) * 10000 - (lt.tm_mon + 1) * 100);

  lt.tm_hour = (int)(TradeTime / 10000);
  lt.tm_min = (int)((TradeTime - lt.tm_hour * 10000) / 100);
  lt.tm_sec = (int)(TradeTime - (lt.tm_hour * 10000) - lt.tm_min * 100);
  return mktime(&lt);
}

time_t make_the_time(time_t tt, uint32_t TradeTime) {
  struct tm lt;
  localtime_s(&lt, &tt);

  lt.tm_hour = (int)(TradeTime / 10000);
  lt.tm_min = (int)((TradeTime - lt.tm_hour * 10000) / 100);
  lt.tm_sec = (int)(TradeTime - (lt.tm_hour * 10000) - lt.tm_min * 100);
  return mktime(&lt);
}


void outbuf(DATAPTR data, size_t length) {
  unsigned char *ptr = reinterpret_cast<unsigned char *>(data);
  for (size_t i = 0; i < length; ++i) {
    printf("%02x ", ptr[i]); // & 0x000000FF);
    if (!((i + 1) % 16))
      printf("\n");
  }
  printf("\n");
}

//
// bool cmpstr(multimap<string, string>::iterator *map1,
//            multimap<string, string>::iterator *map2) {
//  return (*map1)->second < (*map2)->second;
//}
//
std::string minString(std::multimap<std::string, std::string> &mapInst,
                      std::string codeInst) {
  // pairMap is pair<multimap<string, string>::iterator,
  //                 multimap<string, string>::iterator>
  auto pairMap = mapInst.equal_range(codeInst);

  std::vector<string> vecInstrument;
  for (multimap<string, string>::iterator it2 = pairMap.first;
       it2 != pairMap.second; ++it2)
    vecInstrument.push_back((*it2).second);
  if (vecInstrument.size() > 0)
    return *std::min_element(vecInstrument.begin(), vecInstrument.end());
  else
    return "";

  // return *std::min_element(pairMap.first, pairMap.second, cmpstr);
}

int match_index(std::vector<regex> &regexInstruments, const char *str) {
  for (int i = 0; i < (int)regexInstruments.size(); ++i)
    if (regex_match(str, regexInstruments[i]))
      return i;

  return (-1);
}

/**
 * @brief getInstCode return the code of a instrument
 * @param InstRegex the regex of a instrument
 * @return the code
 */
std::string getInstCode(std::string instrument) {
  std::string code("");
  for (auto c : instrument) {
    if (!isalpha(c))
      break;
    code.push_back(c);
  }
  return code;
}

/**
 * @brief getInstContinuedCode return the continued  code of a instrument
 * @param InstRegex the regex of a instrument
 * @return the continued  code
 */
const std::string getInstContinuedCode(std::string instrument) {
  std::string code("");
  for (auto c : instrument) {
    if (!isalpha(c))
      break;
    code.push_back(c);
  }
  code += (code.length() < 2) ? "9888" : "888";

  return code;
}

time_t get_exit_time(const char *the_time) {
  struct tm lt;
  time_t tt = time(0);
  localtime_s(&lt, &tt);

  char *strtt = strdup(the_time);
  char *s = strchr(strtt, ':');
  *s = 0;
  lt.tm_hour = atoi(strtt);
  *s = ':';
  char *t = s + 1;
  s = strchr(t, ':');
  *s = 0;
  lt.tm_min = atoi(t);
  *s = ':';
  t = s + 1;
  lt.tm_sec = atoi(t);
  free(strtt);
  return mktime(&lt);
}


int compare_files(const char* file1, const char* file2) {
  FILE* fp1 = fopen(file1, "rb");
  if (!fp1)
    return -1001;
  FILE* fp2 = fopen(file2, "rb");
  if (!fp2) {
    fclose(fp1);
    return -1002;
  }

  fseek(fp1, 0, SEEK_END);
  long size1 = ftell(fp1);
  fseek(fp1, 0, SEEK_SET);

  fseek(fp2, 0, SEEK_END);
  long size2 = ftell(fp2);
  fseek(fp2, 0, SEEK_SET);

  int ret = 0;
  if (size1 != size2)
      ret = (size1 > size2) ? 5 : -5;

  if (!ret) {
    char c1, c2;
    for (long i = 0; !ret && i < size1; ++i) {
      if (1 != fread(&c1, 1, 1, fp1))
        ret = -1003;

      if (1 != fread(&c2, 1, 1, fp2))
        ret = -1004;

      if (!ret && (c1 != c2)) {
        ret = (c1 > c2) ? 1 : -1;
        printf("not same at %ld\n", i);
      }
    }
  }

  fclose(fp2);
  fclose(fp1);
  return ret;
}

std::vector<std::string> split(const string& input, const string& regex) {
    // passing -1 as the submatch index parameter performs splitting
    std::regex re(regex);
    std::sregex_token_iterator
        first{input.begin(), input.end(), re, -1},
        last;
    return {first, last};
}
