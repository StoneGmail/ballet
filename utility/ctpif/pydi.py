#!/usr/bin/python

import threading
import time
from ctypes import *

print "!!!\nbefore run this script, please ensure the datasimu is running\n!!!\n"

ctpdi = cdll.LoadLibrary("libctpdid.so") #debug version library
# ctpdi = cdll.LoadLibrary("libctpdi.so") #release version librayr

print ctpdi

class KINDLE(Structure):
    _fields_ = [("update_time", c_longlong),
                ("open", c_double),
                ("high", c_double),
                ("low", c_double),
                ("close", c_double),
                ("vol", c_uint),
                ("interest", c_double)]


'''
C interfaces:

//initial ctp configuration, filepath is a file of json config
int initCtpMdIf(const char *filepath);

//release ctp config
void releaseCtpMdIf();

//open interface
int open_data_interface();

//close interface
int close_data_interface();
//get kindel data

KINDLE *get_kindle_data(const char *inst, const char *period,
                      size_t *len, size_t *oldlen,
                      size_t *newlen);

'''

open_data_interface = ctpdi.open_data_interface
close_data_interface = ctpdi.close_data_interface
get_kindle_data = ctpdi.get_kindle_data
get_kindle_data.restype = POINTER(KINDLE)

init_ctp_if = ctpdi.initCtpIf
release_ctp_if = ctpdi.releaseCtpIf

KINDLE_CALLBACK_FUNC = CFUNCTYPE(c_char_p, POINTER(KINDLE), POINTER(c_int))



# ### for test##
# UINT64_P = POINTER(c_ulonglong)
# klen = c_ulonglong(44)
# addr = addressof(klen)
# print 'address:', addr, type(addr)
# byref(klen) = cast(addr, UINT64_P)
# print 'pointer:', byref(klen)
# print 'value:', byref(klen)[0]
#

def c2p_get_kindel_array(kindle_ptr, len):
    KindleArray = KINDLE * len;
    kap = cast(kindle_ptr, POINTER(KindleArray))
    Array = kap.contents
    return Array

def print_kindle(kindle):
    print ("Kindle: ", kindle.update_time, kindle.open, kindle.high, kindle.low, kindle.close, kindle.vol, kindle.interest)

def print_kindle_array(Array):
    for k in Array:
        print_kindle(k)

# config_json = "/work/sean/projects/md/md/gmd-linux.json"
config_json = "/work/znl/gom/config/gmd-linux.json"
if (0 == init_ctp_if(config_json)):
    if (0 == open_data_interface()):
        klen = c_ulonglong(0)
        kolen = c_ulonglong(0)
        knlen = c_ulonglong(0)

        n = knlen.value
        while (n < 181) :
            kindle = get_kindle_data( "CF701", "m1", byref(klen), byref(kolen), byref(knlen))
            Array = c2p_get_kindel_array(kindle, knlen.value)
            n = klen.value
            print("CF701 , m1: ", klen.value, Array, n)
            # print_kindle_array(Array)

            kindle = get_kindle_data( "IF1611","m5", byref(klen), byref(kolen), byref(knlen))
            Array = c2p_get_kindel_array(kindle, knlen.value)
            print("IF1611, m5: ", klen.value, Array)

            kindle = get_kindle_data( "SR701", "m10", byref(klen), byref(kolen), byref(knlen))
            Array = c2p_get_kindel_array(kindle, knlen.value)
            print("SR701 , m10: ", klen.value, Array)

            kindle = get_kindle_data( "ag1701","m15", byref(klen), byref(kolen), byref(knlen))
            Array = c2p_get_kindel_array(kindle, knlen.value)
            print("ag1701, m15: ", klen.value, Array)

            kindle = get_kindle_data( "au1701","m30", byref(klen), byref(kolen), byref(knlen))
            Array = c2p_get_kindel_array(kindle, knlen.value)
            print("au1701, m30: ", klen.value, Array)

            kindle = get_kindle_data( "i1710", "h1", byref(klen), byref(kolen), byref(knlen))
            Array = c2p_get_kindel_array(kindle, knlen.value)
            print("i1710 , h1: ", klen.value, Array)

            print
            time.sleep(10)

        close_data_interface()
    release_ctp_if()



import ctpif

def myadd(a, b):
    return a + b

def mystr(s):
    print s

c = ctpif.run_pyfun(myadd, 1, 2)

ctpif.test_char(mystr, "hello")

# c = ctpif.ctpreader(hello, '/app/sean/bin/gom/conf/gmd-simnow.json')

import ctpif
import ctypes
import numpy as np
import pandas as pd

def hello():
    print 'hello'

ctpreader = ctpif.PyCandleReader('/app/sean/bin/gom/conf/gmd-simnow.json')
maxsize = ctpreader.get_max_data_size()



from ctypes import *
import numpy as np
import pandas as pd

# load dll
ctpif = cdll.LoadLibrary("libctpif.so")

# the Candle Bar
class CandleBar(Structure):
    _fields_ = [("key", c_double),
                ("open", c_double),
                ("high", c_double),
                ("low", c_double),
                ("close", c_double),
                ("vol", c_double),
                ("interest", c_double)]


# initialize a reader with a json configure file
reader = ctpif.cc_init_reader('/app/sean/bin/gom/conf/gmd-simnow.json')

# get max data length
datalen = ctpif.cc_get_maxlen(reader)

# get interface of buffer-pointer
cc_get_bufptr = ctpif.cc_get_bufptr
cc_get_bufptr.restype = POINTER(c_double)

# get buffer pointer to ru1701
bufptr = cc_get_bufptr(reader, 'ru1701')

# convert to numpy matrix
# naptr = cast(bufptr, )
na = np.ctypeslib.as_array(bufptr,shape=(datalen,7))

# convert to dataframe
df = pd.DataFrame(na)

# converto to array of CandleBar
CandleArray = CandleBar * datalen;
cc_get_bufptr.restype = POINTER(CandleArray)
candle_ptr = cc_get_bufptr(reader, 'ru1701')
ca = candle_ptr.contents
ca = np.array(ca)
df2 = pd.DataFrame(ca)


ctpif.cc_destroy_reader(reader)
