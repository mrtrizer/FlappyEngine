# FlappyEngine
## Tests:
| Module | Status |
| --- | --- |
| FlappyEngine | [![status](https://travis-ci.org/mrtrizer/FlappyEngine.svg?branch=master)](https://travis-ci.org/mrtrizer/FlappyEngine) [![Build status](https://ci.appveyor.com/api/projects/status/4xxkhek40a57m8mx?svg=true)](https://ci.appveyor.com/project/mrtrizer/flappyengine) [![codecov](https://codecov.io/gh/mrtrizer/FlappyEngine/branch/master/graph/badge.svg)](https://codecov.io/gh/mrtrizer/FlappyEngine)  |
| FlappyTools2 | [![status](https://travis-ci.org/mrtrizer/FlappyTools2.svg?branch=master)](https://travis-ci.org/mrtrizer/FlappyEngine) [![Build status](https://ci.appveyor.com/api/projects/status/9bqsa6v6fgrvcbaw?svg=true)](https://ci.appveyor.com/project/mrtrizer/flappytools2) | 

## Next development steps
1. ~~Finish new component design (NewCore module)~~
2. ~~Port all existing modules to a new component system~~
3. Reflection generation
4. Editor

## Installation
The engine itself is not an application, it's just a list of modules for FlappyTools2 template processor. To start using of FlappyEngine, install FlappyTools2 first. Follow instructions here https://github.com/mrtrizer/FlappyTools2

Generate default FlappyEngine project with command `flappy init flappy-2d <Project name>`. Enter to the created project directory and run `git submodule update --init`. By default, FlappyEngine repository will be connected to your project. 

You can find more details in FlappyTools2 documentation.

## Linux (Ubuntu)
1. Install libvorbis-dev, libopenal-dev, glew, sdl2 and sdl2_image using standart package manager.
Also install libcairo2-dev libjpeg8-dev libpango1.0-dev libgif-dev build-essential g++ required for atlas and font texture generation.
```
sudo apt-get install libegl1-mesa-dev libgles2-mesa-dev
sudo apt-get install libsdl2-dev libsdl2-image-dev
sudo apt-get install libvorbis-dev libopenal-dev
sudo apt-get install libglew-dev mesa-common-dev freeglut3-dev libxi-dev
sudo apt-get install libcairo2-dev libjpeg8-dev libpango1.0-dev libgif-dev build-essential
```
2. Install llvm 5.0 + clang 5.0
```
sudo add-apt-repository ppa:jonathonf/llvm
sudo apt-get update
sudo apt-get install llvm-5.0-dev
sudo apt-get install clang-5.0-dev
```
3. Make sure that you have gcc-8 and g++-8 libs installed. If you have previous version, update it:
```
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install gcc-8 g++-8
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 60 --slave /usr/bin/g++ g++ /usr/bin/g++-8
```

## OS X
1. Install glew and libvorbis using brew. `brew install glew libvorbis`

2. llvm 5.0 using brew. `brew install llvm`

3. Install libraries for font generation and image processing `brew install pkg-config cairo libpng jpeg giflib graphicsmagick`

4. Download SDL2 and SDL2_image here: https://www.libsdl.org/download-2.0.php#source and https://www.libsdl.org/projects/SDL_image/.

5. Copy SDL2.framework and SDL2_image.framework to ~/Library/Frameworks (create the folder if needed)

## Windows
I'm only in the very beginning of porting engine to Windows. It was developed only with Unix systems in mind just to check ideas. Now project generator FlappyTools2 works on windows and provieds oppotunities to build everything under it.
1. Install Node.js, Cmake, VS17 Build Tools
2. Install FlappyTools2
3. Follow instructions on FlappyTools2 to build modules.
