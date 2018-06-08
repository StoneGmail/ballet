from ctypes import *
import numpy as np
import pandas as pd
import ctpif

# load dll
ctpif_cdll = cdll.LoadLibrary("libctpif.so")

# the Candle Bar
class CandleBar(Structure):
    _fields_ = [("key", c_double),
                ("open", c_double),
                ("high", c_double),
                ("low", c_double),
                ("close", c_double),
                ("vol", c_double),
                ("interest", c_double)]



def got_new_data_fun(inst, curpos, count, last_pos, last_total, its_new):
    print inst, curpos, count, last_pos, last_total, its_new



# initialize a reader with a json configure file
# reader = ctpif_cdll.cc_init_reader('/app/sean/bin/gom/conf/gmd-simnow-test.json')
# reader = ctpif_cdll.cc_init_reader('/app/sean/bin/gom/conf/gmd-simnow.json')
reader = ctpif_cdll.cc_init_reader('/app/sean/bin/gom/conf/gmd-9-lyj.json')

# get max data length
datalen = ctpif_cdll.cc_get_maxlen(reader)

# get interface of buffer-pointer
cc_get_bufptr = ctpif_cdll.cc_get_bufptr
cc_get_bufptr.restype = POINTER(c_double)

# get buffer pointer to rb1801
bufptr = cc_get_bufptr(reader, 'rb1801')

# convert to numpy matrix
# naptr = cast(bufptr, )
na = np.ctypeslib.as_array(bufptr,shape=(datalen,7))

# convert to dataframe
df = pd.DataFrame(na)

# converto to array of CandleBar
CandleArray = CandleBar * datalen;
cc_get_bufptr.restype = POINTER(CandleArray)
candle_ptr = cc_get_bufptr(reader, 'rb1801')
ca = candle_ptr.contents


ctpif.cc_got_new_data(long(reader), got_new_data_fun, "", 0L, 0L, 0L, 0L, 0)

#destroy reader
ctpif_cdll.cc_destroy_reader(reader)


