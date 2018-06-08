cd /work/sean/projects/build/md/gcc/debug
cd utility
make -j24
sudo make install
cd ../gom
make -j24
sudo make install
cd ../ctpdi
make -j24
sudo make install

cd ..
make -j24
sudo make install


cd ../../release
cd utility
make -j24
sudo make install
cd ../gom
make -j24
sudo make install
cd ../ctpdi
make -j24
sudo make install

cd ..
make -j24
sudo make install
