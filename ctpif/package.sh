#!/bin/sh

# echo 'please enter the source path:'
# read spath
#
# echo 'please enther the bin path:'
# read bpath
#
# echo 'please enter the libs path:'
# read lpath
#
# echo 'please enter the target path to packaging:'
# read tpath

spath="/work/sean/projects/md"
bpath="/usr/local/bin/gmd"
lpath="/usr/local/lib"
tpath="/home/sean/install/gmd"
dpath="/home/sean/Downloads"

mkdir -p $tpath/lib
mkdir -p $tpath/bin
mkdir -p $tpath/scripts
mkdir -p $tpath/gom/config
mkdir -p $tpath/gom/data
mkdir -p $tpath/gom/data/rt
mkdir -p $tpath/gom/data/cal
mkdir -p $tpath/gom/data/tick
mkdir -p $tpath/gom/data/store
mkdir -p $tpath/gom/data/kindle/data-1m
mkdir -p $tpath/gom/log/logs
mkdir -p $tpath/gom/log/instgeter

cp -f $bpath/* $tpath/bin

cp -f $lpath/libgom*.so.1.0.0 $tpath/lib
cp -f $lpath/libutility*.so.1.0.0 $tpath/lib
cp -f $lpath/libctpdi*.so.1.0.0 $tpath/lib

cp -rf $spath/ctp-api/20160606_tradeapi_linux64 $tpath
cp -f $spath/md/*.json $tpath/gom/config
#cp -f $dpath/data-1m/*_1m.csv $tpath/gom/data/kindle/data-1m
cp -f $spath/ctpdi/py*.py $tpath/scripts
cp -f ./install.sh $tpath/

cd $tpath
cd ..

rm -f gmd-package.tar.gz
tar cvf gmd-package.tar gmd
gzip -9 gmd-package.tar
