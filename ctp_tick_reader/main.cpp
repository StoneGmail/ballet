#include <iostream>
#include "ctpif_data.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;


CtpMarketData* read_tick_from_file(const char* filepath, size_t* size) {
  FILE* fp = fopen(filepath, "rb");
  if (!fp) return nullptr;
  CtpMarketData* ptr = nullptr;

  fseek(fp, 0, SEEK_END);
  *size = ((size_t)ftell(fp)) / sizeof(CtpMarketData);
  fseek(fp, 0, SEEK_SET);

  ptr = new CtpMarketData[*size];
  if (*size != fread(ptr, sizeof(CtpMarketData), *size, fp)) {
    delete[] ptr;
    ptr = nullptr;
  }

  fclose(fp);
  return ptr;
}


int main(int argc, char *argv[])
{
  if (argc != 2) {
    printf("Usage ctp_tick_reader <tick file>\n");
    exit(0);
  }

  size_t size = 0;
  CtpMarketData* data = read_tick_from_file(argv[1], &size);
  char buf[1024];
  if (data != nullptr) {
    for (size_t i = 0; i < size; ++i) {
      printf("%s, %s | %s - %s - %d | %.02lf, %d\n", data[i].InstrumentID, data[i].TradingDay, data[i].ActionDay, data[i].UpdateTime, data[i].UpdateMillisec, data[i].LastPrice, data[i].Volume);
      if (0 == i % 10) {
        fgets(buf, 1024, stdin);
        if (*buf == 'q') break;
      }
    }

    delete[] data;
  }
  return 0;
}
