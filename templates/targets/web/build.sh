emconfigure cmake
emmake make
emcc `find CMakeFiles -name "*.o"` engine/libFlappyEngine.so --preload-file ../../build/res/@/res/ --use-preload-plugins -o {!name!}.js
