#kill process
ps -ef | grep "ctpmdif_gui.py"   | grep -v grep | awk '{print $2}' | xargs kill -9
ps -ef | grep "/md"   | grep -v grep | awk '{print $2}' | xargs kill -9



#start process
export PYTHONHOME="/app/bin/py2714-gcc730"
# export PYTHONHOME="/usr"
export GCCH="/usr/local/gcc730"
export PH=$PYTHONHOME
export PATH="$PH/bin:$GCCH/bin:$PATH"
export LD_LIBRARY_PATH="./:.:$GCCH/lib:$GCCH/lib64:$GCCH/libexec:$PH/lib:/usr/local/lib:/usr/lib"
export PYTHONPATH="$PH"
export PYTHON="$PH/bin/python"

GOM_PATH=/app/sean/bin/gom
PYCTP_PATH=/home/yht/projects/helper/api
DATA_PATH=/app/sean/data/10s_candle_bindata
inst=rb1810       # or somethig elese
str_day=20171108  # it must date of today



cd $GOM_PATH
$GOM_PATH/bin/md $GOM_PATH/conf/gmd-9-lyj.json &

#waiting for md connect to ctp interface
sleep 10

$GOM_PATH/bin/datasaver $GOM_PATH/conf/gmd-9-lyj.json &

cd $PYCTP_PATH
python ./ctpmdif_gui.py $inst $str_day $DATA_PATH $GOM_PATH/conf/gmd-9-lyj.json
