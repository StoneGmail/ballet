export PATH=/usr/local/gcc62/bin:$PATH:/home/sean/bin
export LD_LIBRARY_PATH=/usr/lib:/usr/local/lib:/usr/local/gcc62/lib:/usr/local/gcc62/lib64:/usr/local/gcc62/libexec:.

#build_root=/work/sean/projects/build/md/gcc
#build_root=/works/projects/build/md/gcc62
build_root=/data/znl/wangchoi/projects/build/md/gcc

#thread=-j3
thread=-j24

cd $build_root/debug
cd utility
make $thread
sudo make install
cd ../ctpif
make $thread
sudo make install

cd ..
make $thread
sudo make install


cd $build_root/release
cd utility
make $thread
sudo make install
cd ../ctpif
make $thread
sudo make install

cd ..
make $thread
sudo make install
