#!/bin/sh

cd 20160606_tradeapi_linux64
sudo make install
echo 'Done.'


sudo rm -rf /usr/local/lib/libgom*
sudo rm -rf /usr/local/lib/libutility*
sudo rm -rf /usr/local/lib/libctpdi*


cd ../lib
echo 'install libs ...'
sudo cp *.so.1.0.0 /usr/local/lib
sudo ln -sf /usr/local/lib/libctpdid.so.1.0.0 /usr/local/lib/libctpdid.so
sudo ln -sf /usr/local/lib/libctpdid.so.1.0.0 /usr/local/lib/libctpdid.so.1
sudo ln -sf /usr/local/lib/libctpdid.so.1.0.0 /usr/local/lib/libctpdid.so.1.0

sudo ln -sf /usr/local/lib/libctpdi.so.1.0.0 /usr/local/lib/libctpdi.so
sudo ln -sf /usr/local/lib/libctpdi.so.1.0.0 /usr/local/lib/libctpdi.so.1
sudo ln -sf /usr/local/lib/libctpdi.so.1.0.0 /usr/local/lib/libctpdi.so.1.0

sudo ln -sf /usr/local/lib/libgomd.so.1.0.0 /usr/local/lib/libgomd.so
sudo ln -sf /usr/local/lib/libgomd.so.1.0.0 /usr/local/lib/libgomd.so.1
sudo ln -sf /usr/local/lib/libgomd.so.1.0.0 /usr/local/lib/libgomd.so.1.0

sudo ln -sf /usr/local/lib/libgom.so.1.0.0 /usr/local/lib/libgom.so
sudo ln -sf /usr/local/lib/libgom.so.1.0.0 /usr/local/lib/libgom.so.1
sudo ln -sf /usr/local/lib/libgom.so.1.0.0 /usr/local/lib/libgom.so.1.0

sudo ln -sf /usr/local/lib/libutilityd.so.1.0.0 /usr/local/lib/libutilityd.so
sudo ln -sf /usr/local/lib/libutilityd.so.1.0.0 /usr/local/lib/libutilityd.so.1
sudo ln -sf /usr/local/lib/libutilityd.so.1.0.0 /usr/local/lib/libutilityd.so.1.0

sudo ln -sf /usr/local/lib/libutility.so.1.0.0 /usr/local/lib/libutility.so
sudo ln -sf /usr/local/lib/libutility.so.1.0.0 /usr/local/lib/libutility.so.1
sudo ln -sf /usr/local/lib/libutility.so.1.0.0 /usr/local/lib/libutility.so.1.0

echo 'Done.'

echo 'install bin ...'
cd ..
cp -rf ./bin /work/znl
cp -rf ./scripts /work/znl
cp -rf ./gom /work/znl
echo 'Done.'

tar zxvf /home/znl/downloads/data-1m.tar.gz
mv -f ./data-1m /work/znl/gom/data/kindle/
