#!/bin/bash

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

#check process eixst key in cmd
MD_PROCESS_KEY="/marketdata"
DATASAVE_PROCESS_KEY="/datasaver"

#function kill process
kill_process() {
    echo "about to kill process $1..."
    ps -ef | grep $1  | grep -v grep | awk '{print $2}' | xargs kill -s USR1
    echo "killed process $1"
}

#function check process exist
is_process_exist() {
    alive=`ps aux|grep $1|grep -v grep|wc -l`
    if [ $alive -eq 0 ]
    then
        echo "process $1 not exist"
        return 0;
    else
        echo "process $1 exist"
        return 1
    fi
}

#function sleep
my_sleep() {
    echo "about to sleep $1 seconds"
    sleep $1
    echo "sleep $1 funished"
}

#check md process 
is_process_exist $MD_PROCESS_KEY
if [ $? -ne 1 ]
then
    my_sleep 30
    kill_process $DATASAVE_PROCESS_KEY
    my_sleep 30
    cd $GOM_PATH
    $GOM_PATH/bin/marketdata $GOM_PATH/conf/gmd-9-lyj.json &
    my_sleep 30
fi

is_process_exist $DATASAVE_PROCESS_KEY
if [ $? -ne 1 ]
then
    $GOM_PATH/bin/datasaver $GOM_PATH/conf/gmd-9-lyj.json &
fi
