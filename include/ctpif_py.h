#ifndef CTPIF_PY_H
#define CTPIF_PY_H

#include "ctpif_global.h"
#include "ctpif.h"
#include "ctpif_data.h"
#include "ctpif_data_fio.h"

// #ifdef PYTHON_INTERFACE
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/python/stl_iterator.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <vector>
#include <set>

namespace bp = boost::python;


template<typename T>
void pylist_to_vector(bp::object& o, std::vector<T>& v) {
    bp::stl_input_iterator<T> begin(o), end;
    v.clear();
    v.insert(v.end(), begin, end);
}

template<typename T>
T* pylist_to_array(bp::object& o) {
    bp::stl_input_iterator<T> begin(o), end;

    T* v = new T[bp::len(o)];
    T* p = v;
    for (auto i = begin; i != end; i++)
      *p++ = *i;

#ifdef _DEBUG
    p = v;
    for (int i = 0; i < bp::len(o); ++i) {
        std::cout << p[i] << ", ";
    }
    std::cout << std::endl;
#endif //_DEBUG

    return v;
}


template<typename T>
int pylist_to_array(const bp::object& o, T* v, int length) {
    if (bp::len(o) != length) return -1;
    bp::stl_input_iterator<T> begin(o), end;
    T* p = v;
    for (auto i = begin; i != end; i++)
      *p++ = *i;

#ifdef _DEBUG
    p = v;
    for (int i = 0; i < bp::len(o); ++i) {
        std::cout << p[i] << ", ";
    }
    std::cout << std::endl;
#endif //_DEBUG

    return 0;
}

template<typename T>
bp::list std_vector_to_py_list(const std::vector<T>& v)
{
   bp::list l;
   for (auto &p : v)
       l.append(p);
   return l;
}

template<typename T>
bp::list array_to_py_list(const T* v, int len)
{
    assert(v != nullptr);

    bp::list l;
   if (v) {
     for (int i = 0; i < len; i ++)
       l.append(v[i]);
   }

   return l;
}

template<typename T>
int array_to_py_list(bp::list& l, const T* v, int len)
{
  assert(v != nullptr);
  assert(bp::len(l) == 0);

  for (int i = 0; i < len; i ++)
    l.append(v[i]);

  return 0;
}

template<typename T>
int vector_to_pylist(bp::list& l, std::vector<T>& v)
{
  assert(bp::len(l) == 0);

  for (auto& p : v)
    l.append(p);

  return 0;
}


#ifdef __cplusplus
extern "C" {
#endif
  extern CTPIF_API double* get_array();
  extern CTPIF_API void free_array();
  extern CTPIF_API DATAPTR cc_init_reader(const char* config);
  extern CTPIF_API double *cc_get_dfptr(DATAPTR reader, const char* instrument);
  extern CTPIF_API void *cc_get_bufptr(DATAPTR reader, const char* instrument);
  extern CTPIF_API void cc_destroy_reader(DATAPTR reader);
  extern CTPIF_API int64_t cc_get_maxlen(DATAPTR reader);
#ifdef __cplusplus
}
#endif //

#endif // CTPIF_PY_H
