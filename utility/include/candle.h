#ifndef CANDLE_H
#define CANDLE_H

#include <vector>
#include <map>
#include "dataset.h"

#ifndef DATELEN
#define DATELEN 9
#define TIMELEN 9
#define INSTRUMENT_LENGTH 9
#define PERIOD_LENGTH 4
#endif //DATELEN

typedef struct MongoTick {
  double lastPrice;
  double askPrice;
  double bidPrice;
  double volume;
  double askvol;
  double bidvol;
  double interest;
  double fltTime;
  char date[DATELEN];
  char strttime[TIMELEN];
}MongoTick;


typedef struct CandleData {
    double key;
    double Open;
    double High;
    double Low;
    double Close;
    double Volume;
    double openInterest;
    char instrument[INSTRUMENT_LENGTH];
    char period[PERIOD_LENGTH];
    char date[DATELEN];
    char strTime[TIMELEN];

    CandleData() {}
    CandleData(
        double fltTime,
        double Open,
        double High,
        double Low,
        double Close,
        double Volume,
        double openInterest,
        const char* inst,
        const char* str_period,
        const char* str_date,
        const char* strtime)
        : key    (fltTime    )
        , Open       (Open       )
        , High       (High       )
        , Low        (Low        )
        , Close      (Close      )
        , Volume     (Volume     )
        , openInterest(openInterest)
    {
      strcpy(instrument, inst);
      strcpy(period, str_period);
      strcpy(date, str_date);
      strcpy(strTime, strtime);
    }
public:
    bool operator==(const CandleData& bar) {
        return (key == bar.key);
    }
}CandleData;

class CandleBar {
public:
    double key;
    double Open;
    double High;
    double Low;
    double Close;
    double Volume ;
    double openInterest;

    CandleBar() {}

    CandleBar(
        double fltTime,
        double Open,
        double High,
        double Low,
        double Close,
        double Volume,
        double openInterest)
        : key    (fltTime    )
        , Open       (Open       )
        , High       (High       )
        , Low        (Low        )
        , Close      (Close      )
        , Volume     (Volume     )
        , openInterest(openInterest)
    {
    }


public:
    bool operator==(const CandleBar& bar) {
        return (key == bar.key);
    }

    void operator=(const CandleData& data) {
        memcpy(this, &data, sizeof(CandleBar));
    }
};



typedef struct CandleBarInfo {
  DataInfo di;
  char trade_date[DATELEN];
  char instrument[INSTRUMENT_LENGTH];
  char period[PERIOD_LENGTH];
} CandleBarInfo;





#endif // CANDLE_H
