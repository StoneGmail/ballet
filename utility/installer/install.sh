#!/bin/sh

if [ ! -n "$1" ] ;then
   echo "please input to installed path as param"
   exit
fi

current_path=$(cd `dirname $0`; pwd)
target_path=$1

echo "$current_path"

mkdir -p $target_path
tar -xvf $current_path/md-installer.tar -C $target_path

sudo rm -rf /usr/local/lib/libgom*
sudo rm -rf /usr/local/lib/libutility*
sudo rm -rf /usr/local/lib/libctpdi*

echo 'install libs ...'

sudo rm -rf /usr/local/lib/libthostmduserapi.so*
sudo rm -rf /usr/local/lib/libthosttraderapi.so*
sudo rm -rf /usr/local/lib/libboost_python27.so*
sudo rm -rf /usr/local/lib/libboost_python27.a
sudo rm -rf /usr/local/lib/libctpif.so*
sudo rm -rf /usr/local/lib/libutility.so*

sudo cp $target_path/lib/*.so.1.0.0 /usr/local/lib

sudo ln -sf /usr/local/lib/libboost_python27.so.1.67.0 /usr/local/lib/libboost_python27.so

sudo ln -sf /usr/local/lib/libctpif.so.1.0.0 /usr/local/lib/libctpif.so
sudo ln -sf /usr/local/lib/libctpif.so.1.0.0 /usr/local/lib/libctpif.so.1
sudo ln -sf /usr/local/lib/libctpif.so.1.0.0 /usr/local/lib/libctpif.so.1.0

#sudo ln -sf /usr/local/lib/libgom.so.1.0.0 /usr/local/lib/libgom.so
#sudo ln -sf /usr/local/lib/libgom.so.1.0.0 /usr/local/lib/libgom.so.1
#sudo ln -sf /usr/local/lib/libgom.so.1.0.0 /usr/local/lib/libgom.so.1.0

sudo ln -sf /usr/local/lib/libutility.so.1.0.0 /usr/local/lib/libutility.so
sudo ln -sf /usr/local/lib/libutility.so.1.0.0 /usr/local/lib/libutility.so.1
sudo ln -sf /usr/local/lib/libutility.so.1.0.0 /usr/local/lib/libutility.so.1.0

echo 'libs Done.'
echo 'install bin ...'

#mkdir -p $target_path
#cp -rf $current_path/bin $target_path
#cp -rf $current_path/config $target_path
#cp -rf $current_path/data $target_path
#cp -rf $current_path/log $target_path

#cp -rf ./scripts /work/znl
#cp -rf ./gom /work/znl
echo 'bin Done.'

#tar zxvf /home/znl/downloads/data-1m.tar.gz
#mv -f ./data-1m /work/znl/gom/data/kindle/
