source ~/emsdk/emsdk_env.sh
rm -rf build && mkdir build && cd build && emcmake cmake .. && emmake make

