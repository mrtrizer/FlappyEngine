# FlappyEngine
## Tests:
| Module | Status |
| --- | --- |
| FlappyEngine | ![status](https://travis-ci.org/mrtrizer/FlappyEngine.svg?branch=master) [![codecov](https://codecov.io/gh/mrtrizer/FlappyEngine/branch/master/graph/badge.svg)](https://codecov.io/gh/mrtrizer/FlappyEngine) |
| FlappyTools2 | ![status](https://travis-ci.org/mrtrizer/FlappyTools2.svg?branch=master) |

## Installation
The engine itself is not an application, it's just a list of modules for FlappyTools2 template processor. To start using of FlappyEngine, install FlappyTools2 first. Follow instructions here https://github.com/mrtrizer/FlappyTools2

Generate default FlappyEngine project with command `flappy init flappy-2d <Project name>`. Enter to the created project directory and run `git submodule update --init`. By default, FlappyEngine repository will be connected to your project. 

You can find more details in FlappyTools2 documentation.

## Linux
Install libvorbis-dev, libopenal-dev, glew, sdl2 and sdl2_image using standart package manager.
Also install libcairo2-dev libjpeg8-dev libpango1.0-dev libgif-dev build-essential g++ required for atlas and font texture generation.

## OS X
1. Install glew and libvorbis using brew. `brew install glew libvorbis`

2. Install libraries for font generation and image processing `brew install pkg-config cairo libpng jpeg giflib graphicsmagick`

3. Download SDL2 and SDL2_image here: https://www.libsdl.org/download-2.0.php#source and https://www.libsdl.org/projects/SDL_image/.

4. Copy SDL2.framework and SDL2_image.framework to ~/Library/Frameworks (create the folder if needed)
