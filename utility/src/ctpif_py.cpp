#include "ctpif_py.h"


bp::object run_pyfun(bp::object& pyfun, int a, int b) {
  return pyfun(a, b);
}


void test_char(bp::object& pyfun, const char* s) {
  pyfun(s);
}


class DataAmount {
public:
  int has_new = 0;
  int curpos = -1;
  int count = -1;
  int isnew = 0;
};

int cc_new_data(int64_t reader, const char* inst, DataAmount& da) {
/*  CtpCandleReader* ptr = (CtpCandleReader*)reader;
  auto map_inst_ptr = ptr->get_map_inst_ptr();
  if (ptr->writer_is_exited()) return -1;

  auto p = ptr->get_map_pos()[inst];

  da.has_new = 0;
  TSDATAFRAME(CandleBar) *dfCandle =
    reinterpret_cast<TSDATAFRAME(CandleBar) *>(map_inst_ptr[inst]);
  if (dfCandle) {
    int64_t count = dfCandle->get_data_amount();
    int64_t curpos = dfCandle->get_cur_pos();
    if (count > p.last_total || (curpos >= 0 && p.last_pos < curpos)) {
      printf("%ld, %ld, %ld, %ld\n", count, curpos, p.last_total, p.last_pos);
      da.has_new = 1, da.count = count, da.curpos = curpos, da.last_pos = p.last_pos, da.last_total = p.last_total,
      da.isnew = (int)(p.last_pos != curpos);
#ifdef _DEBUG_
        {
          if (curpos >= 0 && p.last_pos < curpos)
            printf("Got %s candle data from %ld to %ld\n", inst, p.last_pos, curpos);
          else if (count > p.last_total && p.last_pos == curpos)
            printf("Update candle data %s %ld\n", inst, p.last_pos);
        }
#endif //_DEBUG_
      p.last_pos = curpos;
      p.last_total = count;
    }
  } else {
    printf("hahaha no cadle\n");
  }

  return 0;*/
  CtpCandleReader* ptr = (CtpCandleReader*)reader;
  da.has_new = 0;
  auto map_inst_ptr = ptr->get_map_inst_ptr();
  if (!ptr->writer_is_exited()) {
    for(auto& p : ptr->get_map_pos()) {
      if (strcmp(p.first.c_str(), inst)) continue;
      TSDATAFRAME(CandleBar) *dfCandle =
        reinterpret_cast<TSDATAFRAME(CandleBar) *>(map_inst_ptr[p.first]);

      if (dfCandle) {
        int64_t count = dfCandle->get_data_amount();
        int64_t curpos = dfCandle->get_cur_pos();
        if (count > p.second.last_total || (curpos >= 0 && p.second.last_pos < curpos)) {
          // fun(p.first.c_str(), curpos, count, p.second.last_pos, p.second.last_total, (int)(p.second.last_pos != curpos));
          da.has_new = 1;
          da.count = (int)(count);
          da.curpos = (int)(curpos);
          da.isnew = (int)(p.second.last_pos != curpos);
          // printf("%d, %d, %d, %d\n", da.has_new, da.count, da.curpos, da.isnew);
#ifdef _DEBUG_
          {
            if (curpos >= 0 && p.second.last_pos < curpos)
              printf("Got %s candle data from %ld to %ld\n", p.first.c_str(), p.second.last_pos, curpos);
            else if (count > p.second.last_total && p.second.last_pos == curpos)
              printf("Update candle data %s %ld\n", p.first.c_str(), p.second.last_pos);
          }
#endif //_DEBUG_
          p.second.last_pos = curpos;
          p.second.last_total = count;
        }
      }
    }
    return 0;
  } else {
    return -1;
  }
}

void cc_got_new_data(int64_t reader, bp::object& fun, const char* /*inst*/, int64_t /*curpos*/,
                  int64_t /*count*/, int64_t /*last_pos*/, int64_t /*last_count*/, int /*isnew*/) {


  CtpCandleReader* ptr = (CtpCandleReader*)reader;
  auto map_inst_ptr = ptr->get_map_inst_ptr();
  while(!ptr->writer_is_exited()) {
    for(auto& p : ptr->get_map_pos()) {
      TSDATAFRAME(CandleBar) *dfCandle =
        reinterpret_cast<TSDATAFRAME(CandleBar) *>(map_inst_ptr[p.first]);
      if (dfCandle) {
        int64_t count = dfCandle->get_data_amount();
        int64_t curpos = dfCandle->get_cur_pos();
        if (count > p.second.last_total || (curpos >= 0 && p.second.last_pos < curpos)) {
          fun(p.first.c_str(), curpos, count, p.second.last_pos, p.second.last_total, (int)(p.second.last_pos != curpos));
#ifdef _DEBUG_
          {
            if (curpos >= 0 && p.second.last_pos < curpos)
              printf("Got %s candle data from %ld to %ld\n", p.first.c_str(), p.second.last_pos, curpos);
            else if (count > p.second.last_total && p.second.last_pos == curpos)
              printf("Update candle data %s %ld\n", p.first.c_str(), p.second.last_pos);
          }
#endif //_DEBUG_
          p.second.last_pos = curpos;
          p.second.last_total = count;
        }
      }
    }
    usleep(10);
  }
}



BOOST_PYTHON_MODULE(ctpif) {
  using namespace boost::python;

  class_<DataAmount>("DataAmount")
    .def_readwrite("has_new", &DataAmount::has_new)
    .def_readwrite("curpos", &DataAmount::curpos)
    .def_readwrite("count", &DataAmount::count)
    .def_readwrite("isnew", &DataAmount::isnew)
    ;
  def("run_pyfun", run_pyfun);
  def("cc_got_new_data", cc_got_new_data);
  def("test_char", test_char);
  def("cc_new_data", cc_new_data);
}

#ifdef __cplusplus
extern "C" {
#endif

static double* ppp = 0;

double* get_array() {
  ppp =  new double[10];
  return ppp;
}

void free_array() {
  delete ppp;
}


DATAPTR cc_init_reader(const char* config) {
  GmdParam* param = initConfig(config);
  if (param == nullptr) return (0);

  return new CtpCandleReader(param);
}


double* cc_get_dfptr(DATAPTR reader, const char* instrument) {
  return ((CtpCandleReader*)reader)->get_dfptr(instrument);
}

void *cc_get_bufptr(DATAPTR reader, const char* instrument) {
  return ((CtpCandleReader*)reader)->get_bufptr(instrument);
}

void cc_destroy_reader(DATAPTR reader) {
  ((CtpCandleReader*)reader)->delete_param();
}

int64_t cc_get_maxlen(DATAPTR reader) {
  return ((CtpCandleReader*)reader)->getDataSize();
}


#ifdef __cplusplus
}
#endif
