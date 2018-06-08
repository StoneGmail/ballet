#ifndef DEF_H
#define DEF_H

#include "utility_global.h"

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif // UNUSED

#ifndef SafeDeletePtr
#define SafeDeletePtr(p)                                                       \
  if ((p)) {                                                                   \
    delete (p);                                                                \
    (p) = 0;                                                                   \
  }
#endif // SafeDeletePtr

#ifndef SafeDeleteAry
#define SafeDeleteAry(p)                                                       \
  if ((p)) {                                                                   \
    delete[](p);                                                               \
    (p) = 0;                                                                   \
  }
#endif // SafeDeleteAry

#ifndef MAX_JSON_FILE_SIZE
#define MAX_JSON_FILE_SIZE 16384
#endif // MAX_JSON_FILE_SIZE

#ifdef MAX_PATH
#undef MAX_APTH
#define MAX_PATH 260 * 3
#else
#define MAX_PATH 260 * 3
#endif

typedef double ftime_t;
typedef void *DATAPTR;

#define DATA_NAME_LENGTH 32

#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>

#include <sys/types.h>
#ifdef _UNIX
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

typedef int SOCKET;

#define closesocket ::close

#else
#include <WinSock2.h>
typedef int socklen_t;
#endif //_WINDOWS

#ifdef _UNIX
#else
#endif //_WINDOWS


#ifdef BUFSIZE
#undef BUFSIZE
#endif //BUFSIZE
#define BUFSIZE 32768


using namespace std;

typedef double ftime_t;
typedef double PriceType;
typedef double VolumeType;
typedef double MoneyType;

typedef std::string COLNAME;
typedef double *DfPtr;
typedef std::map<COLNAME, double*> Col2DfPtr;
typedef std::vector<COLNAME> VecColName;
typedef std::pair<COLNAME, DfPtr> PairColDfPtr;


typedef std::map<std::string, int> INTMEMMAP;


#endif // DEF_H
