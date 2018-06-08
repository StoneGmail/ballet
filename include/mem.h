#ifndef MEMMAPPING_H
#define MEMMAPPING_H

#include "utility_global.h"
#include "dataset.h"

#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#ifndef NAME_MAX
#define NAME_MAX 255
#endif // name_max

#define MEMORY_SHARED_FILE_READ_WRITE 0
#define MEMORY_SHARED_FILE_READ_ONLY 1

#define RECEIVER_STATUS_NONE -1
#define RECEIVER_STATUS_WAITING_FOR_DATA 0
#define RECEIVER_STATUS_RECEIVEING_DATA 1
#define RECEIVER_STATUS_FINISH_ONE_DAY_DATA 2


typedef std::map<std::string, DATAPTR> MapStr2Ptr;
typedef std::pair<std::string, DATAPTR> PairStrPtr;


typedef int (*convert_2_simple_tick) (DATAPTR dest, DATAPTR src);
typedef int (*write_data_file) (DATAPTR, int len);

#ifdef _WINDOWS
typedef uint64_t key_t;
#endif //_WINDOWS

class UTILITY_API SharedMemory {
public:
  SharedMemory() : is_writble_(false) {}
  virtual ~SharedMemory() {}

public:
  static SharedMemory *getMemMapping();
  virtual void Delete() = 0;

  virtual int OpenRead(const char *memname, size_t nFileSize = 0) = 0;
  virtual int OpenWrite(const char *filepath, const char *memname,
                        size_t nFileSize) = 0;

  virtual int CloseFile() = 0;
  virtual bool IsValidFile() = 0;

  virtual void *GetDataPtr() = 0;
  virtual key_t getKey() = 0;

protected:
  bool is_writble_ ;
};



#ifdef _WINDOWS
#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

class UTILITY_API MemMapping : public SharedMemory {
public:
  MemMapping();
  ~MemMapping();

  void Delete();

  int OpenRead(const char *memname, size_t nFileSize = 0);
  int OpenWrite(const char *filepath, const char *memname, size_t nFileSize);

  int CloseFile();
  bool IsValidFile() { return (m_pView != 0); }

  void *GetDataPtr() { return m_pView; }
  key_t getKey() { return 0; }

private:
  HANDLE m_hFile;
  HANDLE m_hMapping;
  LPVOID m_pView;
  TCHAR m_strPath[MAX_PATH];
  int m_nMode;
  size_t m_nFileSize;
};
#endif //_WINDOWS

#ifdef _UNIX

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

class UTILITY_API MemMapping : public SharedMemory {
public:
  MemMapping();
  ~MemMapping();

  void Delete();

  int OpenRead(const char *memname, size_t nFileSize = 0);
  int OpenWrite(const char *filepath, const char *memname, size_t nFileSize);

  int CloseFile();
  bool IsValidFile();

  void *GetDataPtr();

  key_t getKey() { return _key; }

private:
  int _shfd;
  char _name[NAME_MAX];
  void *_dataptr;
  size_t _size;
  key_t _key;
};
#endif //_UNIX


#include "dataset.h"

template <typename T, typename TInfo>
class DataSetSharedMemory {
public:
    size_t maxsize_ = 0;
    size_t memsize_ = 0;
    MemMapping* shared_mem_ = 0;
    bool writeble_ = true;
    DataSet<T, TInfo>* dataset_ = 0;
    char name_[DATA_NAME_LENGTH];

    DataSetSharedMemory(bool writeble = true)
        : writeble_(writeble) {
    }

    ~DataSetSharedMemory() {
      destroyMemory();
    }

    int initMemory(size_t size, const char* shared_mem_path, const char* shared_mem_name) {
      maxsize_ = size;
      memsize_ = DATASET(T)::get_memory_size(maxsize_);
      shared_mem_ = SharedMemory::getMemMapping();
      strcpy(name_, shared_mem_name);

      int ret = 0;

      if (writeble_) {
        ret = shared_mem_->OpenWrite(shared_mem_path,
                                     shared_mem_name,
                                     memsize_);
      } else {
        ret = shared_mem_->OpenRead(shared_mem_name, memsize_);
      }

      if (!ret)
        ret = mapping_memory();

      if (ret)
        destroyMemory();

      return ret;
    }

    int destroyMemory() {
      if (dataset_ && writeble_) {
        SafeDeletePtr(dataset_);
      }

      if (shared_mem_) {
        shared_mem_->CloseFile();
        SafeDeletePtr(shared_mem_);
      }

      return 0;
    }

    DATASET(T)* getDataSet() {
      return dataset_;
    }

    int addNewData(T *data) {
      if (!writeble_) return -1;
      return dataset_->append(data);
    }

    size_t getDataAmount() { if (dataset_) return dataset_->get_data_amount(); }

  private:

    int mapping_memory() {
      int ret = 0;
      char *memptr = (char *)(shared_mem_->GetDataPtr());

      char mode = (writeble_) ? 'w' : 'r';
      dataset_ = new DataSet<T, TInfo>(name_, mode);
      dataset_->set_memory(memptr, nullptr, maxsize_, mode);

      return ret;
    }
};


/*
template <typename T, typename TInfo>
class DataSetMemory {
public:
  DataSetMemory(const char* name, char mode = 'w') : mode_(mode) {
    mem_ = SharedMemory::getMemMapping();
    strcpy(name_, name);
  }
  ~DataSetMemory() { destroy(); }

  int init(const char* path, const char* memname, size_t maxsize) {
    int ret = 0;
    maxsize_ = maxsize;

    if (mode_ == 'w')
      ret = mem_->OpenWrite(path, memname, DataSet<T, TInfo>::get_memory_size(maxsize));
    else
      ret = mem_->OpenRead(memname, DataSet<T, TInfo>::get_memory_size(maxsize));

    if (!ret)
      ret = (NULL == new DataSet<T, TInfo>(name_, mode_));

    if (!ret)
      ret = data_set_->set_memory(mem_->GetDataPtr(), nullptr, maxsize_, mode_);

    return ret;
  }

  int destroy() {
    SafeDeletePtr(data_set_);
    mem_->CloseFile();
    SafeDeletePtr(mem_);

    return (0);
  }

  DataSet<T, TInfo>* get_data_set() { return data_set_; }
  SharedMemory* get_memory() { return mem_; }

  int append(T* t) {
    return data_set_->append(t);
  }

  bool got_new_data() {
    return data_set_->got_new_data();
  }

  void set_newdata_cb(got_new_data_callback cb) { data_set_->set_newdata_cb(cb); }

private:
  size_t maxsize_ = 0;
  char mode_ = 'w';
  SharedMemory* mem_ = nullptr;
  DataSet<T, TInfo>* data_set_ = nullptr;
  char name_[DATA_NAME_LENGTH];
}; */

/*! \class DataInterfaceWriter
  \brief store the ctp raw datad and CTPMD raw data
  the struct of memory block are
  +---------------------------------------------+
  | Tick Raw Data Buffer 1                      |
  +---------------------------------------------+
  | Tick Raw Data Buffer 2                      |
  +---------------------------------------------+
  | ...                                         |
  +---------------------------------------------+
  | Tick Raw Data Buffer N                      |
  +---------------------------------------------+
  | Simple Raw Data, Instrument 0               |
  +---------------------------------------------+
  | Simple Raw Data, Instrument 1               |
  +---------------------------------------------+
  | Simple Raw Data, Instrument 2               |
  +---------------------------------------------+
  | ...                                         |
  +---------------------------------------------+
  | Simple  Raw Data, Instrument N              |
  +---------------------------------------------+

  each Raw block has DataSet struct:
  +---------------------------------------------+
  | DataInfo | DataSet ...                      |
  +---------------------------------------------+
  each Simple Raw block has TsDataFrame struct:
  +---------------------------------------------+
  | DataInfo | DataFrame ...                    |
  +---------------------------------------------+
*/

typedef struct InterfaceBufferInfo {
    bool writeble_ = false;
    bool has_simple_tick_ = true;
    bool is_exited_ = false;
    int64_t cur_tick_buf_ = 0;
    size_t tick_buf_size_ = 0;
    size_t stick_buf_size_ = 0;
    size_t tick_buf_num_ = 0;
    size_t memsize_ = 0;
    size_t amounTRawData_ = 0;
    size_t amounTData_ = 0;
} InterfaceBufferInfo;

template <typename TRawData, typename TRawDataInfo, typename TData, typename TDataInfo>
class DataInterfaceMemory {
public:
    DataInterfaceMemory() {}

  void init(std::vector<std::string>& instrument, bool writeble = true, bool has_simple_tick = true) {
    instrument_ = instrument;
    ibi_.writeble_ = writeble;
    ibi_.has_simple_tick_ = has_simple_tick;
  }

  DataInterfaceMemory(std::vector<std::string>& instrument, bool writeble = true, bool has_simple_tick = true)
      : instrument_(instrument) {
      ibi_.writeble_ = writeble;
      ibi_.has_simple_tick_ = has_simple_tick;
  }

  virtual ~DataInterfaceMemory() {
    destroyMemory();
  }

  void close_interface() {
    ibi_.is_exited_ = true;
    ibiptr_->is_exited_ = true;
  }

  int initMemory(size_t tick_size, size_t simple_tick_buf_size, size_t tick_buf_num,
                 const char* shared_mem_path, const char* shared_mem_name) {
    ibi_.tick_buf_size_ = tick_size * instrument_.size();
    ibi_.stick_buf_size_ = simple_tick_buf_size;
    ibi_.tick_buf_num_ = tick_buf_num;
    ibi_.memsize_ = sizeof(InterfaceBufferInfo)
            + DATASET(TRawData)::get_memory_size(ibi_.tick_buf_size_) * ibi_.tick_buf_num_;
    if (ibi_.has_simple_tick_)
      ibi_.memsize_ += TSDATAFRAME(TData)::get_memory_size(ibi_.stick_buf_size_) * instrument_.size();

    ibi_.is_exited_ = false;
    shared_mem_ = SharedMemory::getMemMapping();

    int ret = 0;

    if (ibi_.writeble_) {
      ret = shared_mem_->OpenWrite(shared_mem_path,
                                   shared_mem_name,
                                   ibi_.memsize_);
    } else {
      ret = shared_mem_->OpenRead(shared_mem_name, ibi_.memsize_);
    }

    if (!ret)
      ret = mapping_memory();

    if (ret)
      destroyMemory();

    return ret;
  }

  int destroyMemory() {
    if (simple_data_df_) {
      for (size_t i = 0; i < instrument_.size(); ++i)
        if (simple_data_df_[i]) delete simple_data_df_[i];
      delete[] simple_data_df_;
      simple_data_df_ = 0;
    }

    if (raw_data_set_) {
      for (size_t i = 0; i < ibi_.tick_buf_num_; ++i)
        if (raw_data_set_[i]) delete raw_data_set_[i];
      delete[] raw_data_set_;
      raw_data_set_ = 0;
    }

    if (shared_mem_) {
      shared_mem_->CloseFile();
      delete shared_mem_;
      shared_mem_ = 0;
    }
    return 0;
  }

  DATASET(TRawData) * getRawDatabuf(int64_t index) {
    if (index >= 0 && index < ibi_.tick_buf_num_ && raw_data_set_)
      return raw_data_set_[index];
    return nullptr;
  }

  TSDATAFRAME(TData) * getData(std::string instrument) {
    return reinterpret_cast<TSDATAFRAME(TData) *>(maps2p_[instrument]);
  }

  TSDATAFRAME(TData) * getData(int64_t index) {
    if (index >= 0 && index < instrument_.size() && simple_data_df_)
      return simple_data_df_[index];
    return nullptr;
  }

  int64_t getTickbufIndex() { return ibi_.cur_tick_buf_; }

  int addNewTick(TRawData *data) {
    if (!ibi_.writeble_) return -1;

    int ret = 0;

    ++ibiptr_->amounTRawData_;
    TData simple_tick;
    if (ibiptr_->has_simple_tick_) {
      TSDATAFRAME(TData) *df_simple_tick =
          reinterpret_cast<TSDATAFRAME(TData) *>(maps2p_[data->InstrumentID]);
      if (df_simple_tick && converter_) {
          if (0 == converter_(&simple_tick, data)) {
            // std::cout << "simple " << typeid(simple_tick).name() <<  ", " << simple_tick << std::endl;
            ret = df_simple_tick->append(&simple_tick);
            if (!ret) ++ibiptr_->amounTData_;
          }
      } else {
        printf("did not find %s\n", data->InstrumentID);
        ret = -2;
      }
    }

    if (!ret) {
      // current buffer is full
      ret = raw_data_set_[ibiptr_->cur_tick_buf_]->append(data);
      if (ret && ret == MDERR_MEMORY_IS_FULL) {

#ifdef _DEBUG_
        printf("writer Change buffer %lu ", ibiptr_->cur_tick_buf_);
#endif
        raw_data_set_[ibiptr_->cur_tick_buf_]->set_working(false);
        ibiptr_->cur_tick_buf_++;
        ibiptr_->cur_tick_buf_ %= ibiptr_->tick_buf_num_;
        raw_data_set_[ibiptr_->cur_tick_buf_]->set_working();

#ifdef _DEBUG_
        printf("to %lu\n", ibiptr_->cur_tick_buf_);
#endif
        ret = raw_data_set_[ibiptr_->cur_tick_buf_]->append(data);
      }
    }

    return ret;
  }

  size_t getRawDataAmount() { return ibi_.amounTRawData_; }
  size_t getDataAmount() { return ibi_.amounTData_; }
  size_t getDataSize() { return simple_data_df_[0]->maxsize;}
  double* get_dfptr(const char* inst) {
      return  ((TSDATAFRAME(TData)*)(maps2p_[inst]))->get_dfptr();
  }
  void* get_bufptr(const char* inst) {
     return  ((TSDATAFRAME(TData)*)(maps2p_[inst]))->get_bufptr();
  }

private:

  int mapping_memory() {
    int ret = 0;
    char *memptr = (char *)(shared_mem_->GetDataPtr());
    ibiptr_ = (InterfaceBufferInfo*)memptr;
    if (ibi_.writeble_)
        memcpy(ibiptr_, &ibi_, sizeof(InterfaceBufferInfo));
    memptr += sizeof(InterfaceBufferInfo);


    // DATASET TRawData
    raw_data_set_ = new DATASET(TRawData)* [ibi_.tick_buf_num_];
    for (uint32_t i = 0; i < ibi_.tick_buf_num_; ++i) {
      raw_data_set_[i] = new DATASET(TRawData)();
      if (ibi_.writeble_) {
        raw_data_set_[i]->init(ibi_.memsize_, 'w', false, nullptr);
        raw_data_set_[i]->set_memory(memptr, nullptr, ibi_.tick_buf_size_, 'w');
        raw_data_set_[i]->info->di.pos = -1;
        raw_data_set_[i]->info->di.isworking = (i) ? false : true;
      } else {
        raw_data_set_[i]->init(ibi_.memsize_, 'r', false, nullptr);
        raw_data_set_[i]->set_memory(memptr, nullptr, ibi_.tick_buf_size_, 'r');
      }
      memptr += DATASET(TRawData)::get_memory_size(ibi_.tick_buf_size_);
    }

    ibi_.cur_tick_buf_ = 0;

    if (!ibi_.has_simple_tick_)
      return ret;

    simple_data_df_ = new TSDATAFRAME(TData)* [instrument_.size()];
    for (size_t i = 0; i < instrument_.size(); ++i) {
      simple_data_df_[i] = new TSDATAFRAME(TData)();
      if (ibi_.writeble_) {
        simple_data_df_[i]->init(ibi_.stick_buf_size_, 'w', false, nullptr);
        simple_data_df_[i]->set_memory(memptr, nullptr, ibi_.stick_buf_size_, 'w');
        simple_data_df_[i]->info->di.index = i;
        simple_data_df_[i]->info->di.pos = -1;
      } else {
          simple_data_df_[i]->init(ibi_.stick_buf_size_, 'r', false, nullptr);
          simple_data_df_[i]->set_memory(memptr, nullptr, ibi_.stick_buf_size_, 'r');
      }

      maps2p_.insert(PairStrPtr(instrument_[i], simple_data_df_[i]));
      memptr += TSDATAFRAME(TData)::get_memory_size(ibi_.stick_buf_size_);
    }

    return ret;
  }

public:
  void set_converter(convert_2_simple_tick converter) {converter_ = converter;}
  void set_isnew(is_new_data_cb isnew) {
    isnew_ = isnew;
    for (size_t i = 0; i < instrument_.size(); ++i)
      simple_data_df_[i]->set_isnew_cb(isnew);
  }
  void set_merge(merge_data_cb merge) {
    merge_ = merge;
    for (size_t i = 0; i < instrument_.size(); ++i)
      simple_data_df_[i]->set_merge(merge);
  }
  void set_columns(std::vector<std::string>& cols) {
    for (size_t i = 0; i < instrument_.size(); ++i)
      simple_data_df_[i]->set_columns(cols);
  }

protected:
  InterfaceBufferInfo* ibiptr_ = nullptr;
  DATASET(TRawData)** raw_data_set_ = nullptr;
  TSDATAFRAME(TData)** simple_data_df_ = nullptr;

  MapStr2Ptr maps2p_;

private:
  InterfaceBufferInfo ibi_;
  convert_2_simple_tick  converter_ = nullptr;
  is_new_data_cb isnew_ = nullptr;
  merge_data_cb merge_ = nullptr;

  SharedMemory* shared_mem_ = 0;

  std::vector<std::string> instrument_;
};


#endif // MEMMAPPING_H
