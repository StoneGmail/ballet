#include "tick_bin.h"
#include <time.h>
#include <memory.h>
#include <map>

#ifdef UTILITY_LIBRARY
#define STANDE_ALONG 0
#else //UTILITY_LIBRARY
#define STANDE_ALONG 1
#endif //UTILITY_LIBRARY

#define BLOCK_SIZE 2000


#ifdef MAX_PATH
#undef MAX_PATH
#endif //MAX_APTH

#ifndef MAX_PATH
#define MAX_PATH 260 * 3
#endif //MAX_PATH

typedef std::map<uint64_t, uint64_t> MapValuePos;

void update_pos(MapValuePos& mvp, double ftime, uint64_t pos) {
  uint64_t utime = (uint64_t)(ftime * 10 / BLOCK_SIZE);
  auto it = mvp.find(utime);
  if (it == mvp.end()) {
    mvp.insert(std::make_pair(utime, pos));
    // printf("MVP inserted %lu %lu\n", utime, pos);
  }
}

void update_pos(MapValuePos& mvp, uint64_t utime, uint64_t pos) {
  auto it = mvp.find(utime);
  if (it == mvp.end()) {
    mvp.insert(std::make_pair(utime, pos));
    // printf("MVP inserted %lu %lu\n", utime, pos);
  }
}


typedef struct TICK_DATA_INDEX {
  uint64_t count = 0;
  MapValuePos mvp_10k;

  void make_index(double ftime, uint64_t pos) {
    update_pos(mvp_10k,  ftime, pos);
    count = mvp_10k.size();
  }

  int write_index_file(const char* filepath) {
    FILE* fp = fopen(filepath, "wb");
    if (!fp) return -1000;

    printf("Save index %lu\n", count);
    fwrite(&count, sizeof(uint64_t), 1, fp);
    write_mvp(mvp_10k, fp);
    fclose(fp);
    return 0;
  }

  int read_index_file(const char* filepath) {
    FILE* fp = fopen(filepath, "rb");
    if (!fp) return 0;

    fread(&count, sizeof(uint64_t), 1, fp);
    int ret = read_mvp(mvp_10k, fp);
    fclose(fp);
    printf("index file count got %lu\n", count);
    return ret;
  }

  int find_pos_by_date(const char* strdate, uint64_t& pos) {
    uint64_t date = atoi(strdate);
    struct tm ttt;
    memset(&ttt, 0, sizeof(struct tm));
    ttt.tm_year = date / 10000 - 1900;
    ttt.tm_mon = (date - (ttt.tm_year + 1900) * 10000) / 100 - 1;
    ttt.tm_mday = date - (ttt.tm_year + 1900) * 10000 -(ttt.tm_mon + 1) * 100;
    ttt.tm_hour = 14;
    ttt.tm_min = 59;
    ttt.tm_sec = 59;
    time_t tt = mktime(&ttt);
    printf("Input %s -> %lu -> %ld, I will find %lu\n",
           strdate, date, tt, (uint64_t)(tt * 10 / BLOCK_SIZE));
     auto it = mvp_10k.find((uint64_t)(tt * 10 / BLOCK_SIZE));
    if (it == mvp_10k.end()) return -1;
    pos = (*it).second;
    return 0;
  }

private:
  int write_mvp(MapValuePos& mvp, FILE* fp) {
    for (auto it = mvp.begin(); it != mvp.end(); ++it) {
      fwrite(&(*it).first, sizeof(uint64_t), 1, fp);
      fwrite(&(*it).second, sizeof(uint64_t), 1, fp);
    }
    return 0;
  }

  int read_mvp(MapValuePos& mvp, FILE* fp) {
    uint64_t v = 0, p = 0;

    while(true) {
      if (1 != fread(&v, sizeof(uint64_t), 1, fp))
        break;
      if (1 != fread(&p, sizeof(uint64_t), 1, fp))
        break;
      update_pos(mvp, v, p);
    }
    return 0;
  }

} TICK_DATA_INDEX;


// 20171229,0.09002,9,104,28,129,677,3812,3813,3812,2652460
typedef struct CSV_TICK_DATA {
  int date;
  double ftt;
  int u1;
  int volume;
  int u2;
  int av;
  int bv;
  double price;
  double ap;
  double bp;
  int interest;

  double ftime;
  uint64_t itime;
  double ms;

  void set_ftime() {
    double b = ftt * 1000000;
    itime = uint64_t(b + 0.4);
    ms = b - double(itime);

    if (ms < 0) {
        --itime;
        ms = b - double(itime);
    }
    if (ms > 0.999) {
       ++itime;
       ms = b - double(itime);
    }

    struct tm ttt;
    memset(&ttt, 0, sizeof(struct tm));
    ttt.tm_year = date / 10000 - 1900;
    ttt.tm_mon = (date - (ttt.tm_year + 1900) * 10000) / 100 - 1;
    ttt.tm_mday = date - (ttt.tm_year + 1900) * 10000 -(ttt.tm_mon + 1) * 100;
    ttt.tm_hour = itime / 10000;
    ttt.tm_min = (itime - ttt.tm_hour * 10000) / 100;
    ttt.tm_sec = itime - ttt.tm_hour * 10000 - ttt.tm_min * 100;
    time_t tt = mktime(&ttt);
    ftime = double(tt) + ms;
  }

  void print(FILE* fp) {
    fprintf(fp, "%d / %lf / %10lf / %06lu / %10lf\n", date, ftime, price, itime, ms);
  }
} CSV_TICK_DATA;


int tick_csv_to_bin(const char* csv_filepath, const char* bin_filepath) {
  int ret = 0;
  int64_t count = 0, new_data = 0;
  double last_time = 0;
  FILE* fp_csv = nullptr;

  //get last time
  FILE* fp_bin = fopen(bin_filepath, "rb");
  if (fp_bin != nullptr) {
    fseeko(fp_bin, 0, SEEK_END);
    int pos = ftello(fp_bin);
    count = pos / 16;
    if (pos > 16) {
      fseeko(fp_bin,  -16, SEEK_CUR);
      fread(&last_time, sizeof(double), 1, fp_bin);
    } else {
      printf("Error Format of %s @ tick_csv_to_bin\n", bin_filepath);
      ret = -1;
    }
    fclose(fp_bin);
    fp_bin = nullptr;
  } else {
    printf("Error open %s to read @ tick_csv_to_bin\n", bin_filepath);
  }

  if (!ret) {
    printf("Last Time: %lf, count is %ld @ tick_csv_to_bin\n", last_time, count);
  }

  //open csv file to read
  if (!ret) {
    fp_csv = fopen(csv_filepath, "r");
    if (fp_csv == nullptr) {
      ret = -2;
      printf("Error open %s to read @ tick_csv_to_bin\n", csv_filepath);
    }
  }

  if (!ret) {
    //open bin file to append
    fp_bin = fopen(bin_filepath, "ab");
    if (fp_bin == nullptr) {
      ret = -3;
      printf("Error open %s to write @ tick_csv_to_bin\n", bin_filepath);
    }
  }

  if (!ret) {
    CSV_TICK_DATA ctd;
    TICK_DATA td;
    char line[1024];

    while (fgets(line, 1024, fp_csv)) {
      if (*line == 0 || *line == '\r' || *line == '\n' || *line == '#')
        continue;

      sscanf(line, "%d,%lf,%d,%d,%d,%d,%d,%lf", &ctd.date, &ctd.ftt, &ctd.u1,
             &ctd.volume, &ctd.u2, &ctd.av, &ctd.bv, &ctd.price);
      ctd.set_ftime();
      if (ctd.ftime > last_time) {
        last_time = ctd.ftime;
        ++count;
        ++new_data;
        // ctd.print(stdout);

        td.ftime = ctd.ftime, td.price = ctd.price;
        if (1 != fwrite(&td, sizeof(TICK_DATA), 1, fp_bin)) {
          printf("Error to write count %ld, count %ld @ tick_csv_to_bin", count, new_data);
          ret = -4;
          break;
        }
      }
    }
  }

  if (fp_bin != nullptr) fclose(fp_bin);
  if (fp_csv != nullptr) fclose(fp_csv);

  if (!ret)
    printf("Count is %ld, new data is %ld, last time is %lf\n", count, new_data, last_time);

  return ret;
}



int find_pos_by_date(const char* strdate,
                     const char* bin_filepath,
                     const char* cdi_filepath,
                     uint64_t& pos) {
  printf("Binfile: %s\nCDI file: %s\n", bin_filepath, cdi_filepath);

  TICK_DATA_INDEX tdi;
  tdi.read_index_file(cdi_filepath);
  int result = tdi.find_pos_by_date(strdate, pos);
  if (result < 0) return result;
  printf("Find Pos is %lu\n", pos);

  uint64_t count = BLOCK_SIZE;
  // if (pos + BLOCK_SIZE > tdi.count) count = tdi.count - pos;
  //
  // printf("The Pos is %lu ~ %lu, count is %lu, TDI count %lu\n",
  //        pos, pos + BLOCK_SIZE - 1, count, tdi.count);

  FILE* fp = fopen(bin_filepath, "rb");
  if (!fp) {
    printf("Can not open %s\n", bin_filepath);
    return -1;
  }
  TICK_DATA* td = new TICK_DATA[count];
  fseeko(fp, pos * sizeof(TICK_DATA), SEEK_SET);
  if (count != fread(td, sizeof(TICK_DATA), count, fp)) {
    printf("Read file Error %s\n", bin_filepath);
  }

  printf("First Data is %lf, %lf\n", td[0].ftime, td[0].price);
  printf("Last Data is %lf, %lf\n", td[count -1].ftime, td[count -1].price);

  uint64_t date = atoi(strdate);
  struct tm ttt;
  memset(&ttt, 0, sizeof(struct tm));
  ttt.tm_year = date / 10000 - 1900;
  ttt.tm_mon = (date - (ttt.tm_year + 1900) * 10000) / 100 - 1;
  ttt.tm_mday = date - (ttt.tm_year + 1900) * 10000 -(ttt.tm_mon + 1) * 100;
  ttt.tm_hour = 14;
  ttt.tm_min = 59;
  ttt.tm_sec = 59;
  double ftime = (double)(mktime(&ttt) + 1);

  uint64_t i = 0;
  for (; i < count - 1; ++i) {
    if (td[i].ftime <= ftime && td[i + 1].ftime > ftime)
      break;
  }
  pos += i;
  printf("Found %lu,  %lf, %lf\n", pos, td[i].ftime, td[i].price);

  delete [] td;

  fseeko(fp, pos * sizeof(TICK_DATA), SEEK_SET);
  TICK_DATA mytd;
  fread(&mytd, sizeof(TICK_DATA), 1, fp);
  printf("The ftime is %lf, diff is %lf\n", mytd.ftime, mytd.ftime - ftime);


  fclose(fp);
  return 0;
}


int save_tick_bin(const char* datapath, const char* inst, TICK_DATA* td, int size) {
  // char cdifile[MAX_PATH];
  // snprintf(cdifile, MAX_PATH, "%s/%s_tick.cdi", datapath, inst);
  // TICK_DATA_INDEX tdi;
  // tdi.read_index_file(cdifile);
  // for (int i = 0; i < size; ++i)
  //   tdi.make_index(td[i].ftime, td[i].price);
  // tdi.write_index_file(cdifile);

  int ret = 0;
  char binfile[MAX_PATH];
  snprintf(binfile, MAX_PATH, "%s/%s_tick.bin", datapath, inst);

  //get last time
  FILE* fp_bin = fopen(binfile, "rb");
  if (fp_bin == nullptr) {
    printf("Can not open %s to read @ save_tick_bin\n", binfile);
    ret = -1;
  }

  if (!ret) {
    double last_time = 0;
    fseeko(fp_bin, 0, SEEK_END);
    off64_t pos = ftello(fp_bin);
    int64_t count = pos / 16;
    if (pos > 16) {
      fseeko(fp_bin, -16, SEEK_CUR);
      fread(&last_time, sizeof(double), 1, fp_bin);
      printf("Last Time: %lf, count is %lu @ save_tick_bin\n", last_time, count);
    } else {
      printf("Error Format of %s  @ save_tick_bin\n", binfile);
      ret = -3;
    }
    fclose(fp_bin);
    fp_bin = nullptr;

    if (td[0].ftime <= last_time) {
      printf("Error, the time now %lf is less than last time %lf"
             " @ save_tick_bin", td[0].ftime, last_time);
      ret = -4;
    }
  }

  if (!ret) {
    fp_bin = fopen(binfile, "ab");
    if (fp_bin == nullptr) {
      printf("Can not open %s for write @ save_tick_bin\n", binfile);
      ret = -5;
    }
  }

  if (!ret && ((size_t)size != fwrite(td, sizeof(TICK_DATA), size, fp_bin))) {
    ret = -2;
    printf("Error write %d tidk data to %s\n", size, binfile);
  }

  if (fp_bin != nullptr)
    fclose(fp_bin);

  if (!ret)
    printf("Save %d tick to %s @save_tick_bin\n", size, binfile);

  return ret;
}


int load_tick_to_pos(const char* bin_filepath, uint64_t pos, uint64_t size,
                     std::vector<double>* price, std::vector<double>* ftime) {
  if ((int64_t)((int64_t)pos + 1 - (int64_t)size < 0)) return -1;
  FILE* fp = fopen(bin_filepath, "rb");
  if (!fp) {
    printf("Can not open %s\n", bin_filepath);
    return -1;
  }
  price->resize(size * 2);
  price->clear();
  ftime->resize(size * 2);
  ftime->clear();

  fseeko(fp, (pos + 1 - size) * sizeof(TICK_DATA), SEEK_SET);
  TICK_DATA td;
  for (uint64_t i = 0; i < size; ++i) {
    fread(&td, sizeof(TICK_DATA), 1, fp);
    ftime->emplace_back(td.ftime);
    price->emplace_back(td.price);
  }

  fclose(fp);

  return 0;
}


int load_tick_to_last(const char* bin_filepath, uint64_t size,
                     std::vector<double>* price, std::vector<double>* ftime) {
  printf("load_tick_to_last %s, %lu\n", bin_filepath, size);

  FILE* fp = fopen(bin_filepath, "rb");
  if (!fp) {
    printf("Can not open %s\n", bin_filepath);
    return -1;
  }

  fseeko(fp, 0, SEEK_END);
  off64_t fsize = ftello(fp);

  if ((size_t)fsize < size * sizeof(TICK_DATA)) {
    fclose(fp);
    printf("Wrong size %lu, %lu\n", fsize, size);
    return -2;
  }

  price->resize(size * 2);
  price->clear();
  ftime->resize(size * 2);
  ftime->clear();

  fseeko(fp, fsize - size * sizeof(TICK_DATA), SEEK_SET);
  TICK_DATA td;
  for (uint64_t i = 0; i < size; ++i) {
    fread(&td, sizeof(TICK_DATA), 1, fp);
    ftime->emplace_back(td.ftime);
    price->emplace_back(td.price);
  }

  fclose(fp);

  return 0;
}

int gen_cdi(const char *bin_filepath, const char *cdi_filepath) {
  TICK_DATA_INDEX cdi;
  FILE* fp = fopen(bin_filepath, "rb");
  TICK_DATA td;

  int64_t pos = 0;
  while( 1 == fread(&td, sizeof(TICK_DATA), 1, fp))
    cdi.make_index(td.ftime, pos++);

  fclose(fp);

  cdi.write_index_file(cdi_filepath);

  return 0;
}
