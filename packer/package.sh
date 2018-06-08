#!/bin/sh

#sh ./package.sh /home/yht/projects/build/interface/gcc730/release

if [ ! -n "$1" ] ;then
   echo "please input project build directory as param"
   exit
fi

parent_path=$(cd `dirname $0`; cd "../"; pwd)
current_path=$(cd `dirname $0`; pwd)
spath="$current_path"
bpath=$1

mkdir -p $parent_path/installer
mkdir -p $spath/files
mkdir -p $spath/files/lib
mkdir -p $spath/files/bin
mkdir -p $spath/files/config
mkdir -p $spath/files/data
mkdir -p $spath/files/data/rt
mkdir -p $spath/files/data/cal
mkdir -p $spath/files/data/tick
mkdir -p $spath/files/log/logs

cp -f $bpath/datasaver/datasaver $spath/files/bin
cp -f $bpath/marketdata/marketdata $spath/files/bin
cp -f $bpath/datasimu/datasimu $spath/files/bin

cp -f $bpath/utility/libutility*.so.1.0.0 $spath/files/lib
cp -f $bpath/ctpif/libctpif*.so.1.0.0 $spath/files/lib

cp -rf $parent_path/ctp-api/20160606_tradeapi_linux64/*.so* $spath/files/lib
cp -rf /usr/local/lib/libboost_python* $spath/files/lib

cp -f $parent_path/marketdata/*.json $spath/files/config/

rm -rf $parent_path/installer/md-installer.tar.gz
tar cvf $parent_path/installer/md-installer.tar.gz -C $spath/files bin config data log

echo "output package to $parent_path/installer/md-installer.tar"
rm -rf $spath/files

cd $parent_path/installer
#cp -f $dpath/data-1m/*_1m.csv $tpath/gom/data/kindle/data-1m
#cp -f $spath/ctpdi/py*.py $tpath/scripts
#cp -f ./install.sh $tpath/

#cd $tpath
#cd ..

#rm -f gmd-package.tar.gz
#tar cvf gmd-package.tar gmd
#gzip -9 gmd-package.tar
