# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.2

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/trizer/Hobby/FlappyEngine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/trizer/Hobby/FlappyEngine

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/trizer/Hobby/FlappyEngine/CMakeFiles /home/trizer/Hobby/FlappyEngine/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/trizer/Hobby/FlappyEngine/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named FlappyEngine

# Build rule for target.
FlappyEngine: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 FlappyEngine
.PHONY : FlappyEngine

# fast build rule for target.
FlappyEngine/fast:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/build
.PHONY : FlappyEngine/fast

src/core/animation.o: src/core/animation.cpp.o
.PHONY : src/core/animation.o

# target to build an object file
src/core/animation.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/animation.cpp.o
.PHONY : src/core/animation.cpp.o

src/core/animation.i: src/core/animation.cpp.i
.PHONY : src/core/animation.i

# target to preprocess a source file
src/core/animation.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/animation.cpp.i
.PHONY : src/core/animation.cpp.i

src/core/animation.s: src/core/animation.cpp.s
.PHONY : src/core/animation.s

# target to generate assembly for a file
src/core/animation.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/animation.cpp.s
.PHONY : src/core/animation.cpp.s

src/core/atlas.o: src/core/atlas.cpp.o
.PHONY : src/core/atlas.o

# target to build an object file
src/core/atlas.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/atlas.cpp.o
.PHONY : src/core/atlas.cpp.o

src/core/atlas.i: src/core/atlas.cpp.i
.PHONY : src/core/atlas.i

# target to preprocess a source file
src/core/atlas.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/atlas.cpp.i
.PHONY : src/core/atlas.cpp.i

src/core/atlas.s: src/core/atlas.cpp.s
.PHONY : src/core/atlas.s

# target to generate assembly for a file
src/core/atlas.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/atlas.cpp.s
.PHONY : src/core/atlas.cpp.s

src/core/camera.o: src/core/camera.cpp.o
.PHONY : src/core/camera.o

# target to build an object file
src/core/camera.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/camera.cpp.o
.PHONY : src/core/camera.cpp.o

src/core/camera.i: src/core/camera.cpp.i
.PHONY : src/core/camera.i

# target to preprocess a source file
src/core/camera.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/camera.cpp.i
.PHONY : src/core/camera.cpp.i

src/core/camera.s: src/core/camera.cpp.s
.PHONY : src/core/camera.s

# target to generate assembly for a file
src/core/camera.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/camera.cpp.s
.PHONY : src/core/camera.cpp.s

src/core/circleshape.o: src/core/circleshape.cpp.o
.PHONY : src/core/circleshape.o

# target to build an object file
src/core/circleshape.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/circleshape.cpp.o
.PHONY : src/core/circleshape.cpp.o

src/core/circleshape.i: src/core/circleshape.cpp.i
.PHONY : src/core/circleshape.i

# target to preprocess a source file
src/core/circleshape.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/circleshape.cpp.i
.PHONY : src/core/circleshape.cpp.i

src/core/circleshape.s: src/core/circleshape.cpp.s
.PHONY : src/core/circleshape.s

# target to generate assembly for a file
src/core/circleshape.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/circleshape.cpp.s
.PHONY : src/core/circleshape.cpp.s

src/core/entitymgr.o: src/core/entitymgr.cpp.o
.PHONY : src/core/entitymgr.o

# target to build an object file
src/core/entitymgr.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/entitymgr.cpp.o
.PHONY : src/core/entitymgr.cpp.o

src/core/entitymgr.i: src/core/entitymgr.cpp.i
.PHONY : src/core/entitymgr.i

# target to preprocess a source file
src/core/entitymgr.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/entitymgr.cpp.i
.PHONY : src/core/entitymgr.cpp.i

src/core/entitymgr.s: src/core/entitymgr.cpp.s
.PHONY : src/core/entitymgr.s

# target to generate assembly for a file
src/core/entitymgr.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/entitymgr.cpp.s
.PHONY : src/core/entitymgr.cpp.s

src/core/flappyapp.o: src/core/flappyapp.cpp.o
.PHONY : src/core/flappyapp.o

# target to build an object file
src/core/flappyapp.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/flappyapp.cpp.o
.PHONY : src/core/flappyapp.cpp.o

src/core/flappyapp.i: src/core/flappyapp.cpp.i
.PHONY : src/core/flappyapp.i

# target to preprocess a source file
src/core/flappyapp.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/flappyapp.cpp.i
.PHONY : src/core/flappyapp.cpp.i

src/core/flappyapp.s: src/core/flappyapp.cpp.s
.PHONY : src/core/flappyapp.s

# target to generate assembly for a file
src/core/flappyapp.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/flappyapp.cpp.s
.PHONY : src/core/flappyapp.cpp.s

src/core/gamemgr.o: src/core/gamemgr.cpp.o
.PHONY : src/core/gamemgr.o

# target to build an object file
src/core/gamemgr.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/gamemgr.cpp.o
.PHONY : src/core/gamemgr.cpp.o

src/core/gamemgr.i: src/core/gamemgr.cpp.i
.PHONY : src/core/gamemgr.i

# target to preprocess a source file
src/core/gamemgr.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/gamemgr.cpp.i
.PHONY : src/core/gamemgr.cpp.i

src/core/gamemgr.s: src/core/gamemgr.cpp.s
.PHONY : src/core/gamemgr.s

# target to generate assembly for a file
src/core/gamemgr.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/gamemgr.cpp.s
.PHONY : src/core/gamemgr.cpp.s

src/core/inputmgr.o: src/core/inputmgr.cpp.o
.PHONY : src/core/inputmgr.o

# target to build an object file
src/core/inputmgr.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/inputmgr.cpp.o
.PHONY : src/core/inputmgr.cpp.o

src/core/inputmgr.i: src/core/inputmgr.cpp.i
.PHONY : src/core/inputmgr.i

# target to preprocess a source file
src/core/inputmgr.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/inputmgr.cpp.i
.PHONY : src/core/inputmgr.cpp.i

src/core/inputmgr.s: src/core/inputmgr.cpp.s
.PHONY : src/core/inputmgr.s

# target to generate assembly for a file
src/core/inputmgr.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/inputmgr.cpp.s
.PHONY : src/core/inputmgr.cpp.s

src/core/manager.o: src/core/manager.cpp.o
.PHONY : src/core/manager.o

# target to build an object file
src/core/manager.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/manager.cpp.o
.PHONY : src/core/manager.cpp.o

src/core/manager.i: src/core/manager.cpp.i
.PHONY : src/core/manager.i

# target to preprocess a source file
src/core/manager.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/manager.cpp.i
.PHONY : src/core/manager.cpp.i

src/core/manager.s: src/core/manager.cpp.s
.PHONY : src/core/manager.s

# target to generate assembly for a file
src/core/manager.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/manager.cpp.s
.PHONY : src/core/manager.cpp.s

src/core/presenter.o: src/core/presenter.cpp.o
.PHONY : src/core/presenter.o

# target to build an object file
src/core/presenter.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/presenter.cpp.o
.PHONY : src/core/presenter.cpp.o

src/core/presenter.i: src/core/presenter.cpp.i
.PHONY : src/core/presenter.i

# target to preprocess a source file
src/core/presenter.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/presenter.cpp.i
.PHONY : src/core/presenter.cpp.i

src/core/presenter.s: src/core/presenter.cpp.s
.PHONY : src/core/presenter.s

# target to generate assembly for a file
src/core/presenter.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/presenter.cpp.s
.PHONY : src/core/presenter.cpp.s

src/core/quad.o: src/core/quad.cpp.o
.PHONY : src/core/quad.o

# target to build an object file
src/core/quad.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/quad.cpp.o
.PHONY : src/core/quad.cpp.o

src/core/quad.i: src/core/quad.cpp.i
.PHONY : src/core/quad.i

# target to preprocess a source file
src/core/quad.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/quad.cpp.i
.PHONY : src/core/quad.cpp.i

src/core/quad.s: src/core/quad.cpp.s
.PHONY : src/core/quad.s

# target to generate assembly for a file
src/core/quad.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/quad.cpp.s
.PHONY : src/core/quad.cpp.s

src/core/rectshape.o: src/core/rectshape.cpp.o
.PHONY : src/core/rectshape.o

# target to build an object file
src/core/rectshape.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/rectshape.cpp.o
.PHONY : src/core/rectshape.cpp.o

src/core/rectshape.i: src/core/rectshape.cpp.i
.PHONY : src/core/rectshape.i

# target to preprocess a source file
src/core/rectshape.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/rectshape.cpp.i
.PHONY : src/core/rectshape.cpp.i

src/core/rectshape.s: src/core/rectshape.cpp.s
.PHONY : src/core/rectshape.s

# target to generate assembly for a file
src/core/rectshape.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/rectshape.cpp.s
.PHONY : src/core/rectshape.cpp.s

src/core/resourcemgr.o: src/core/resourcemgr.cpp.o
.PHONY : src/core/resourcemgr.o

# target to build an object file
src/core/resourcemgr.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/resourcemgr.cpp.o
.PHONY : src/core/resourcemgr.cpp.o

src/core/resourcemgr.i: src/core/resourcemgr.cpp.i
.PHONY : src/core/resourcemgr.i

# target to preprocess a source file
src/core/resourcemgr.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/resourcemgr.cpp.i
.PHONY : src/core/resourcemgr.cpp.i

src/core/resourcemgr.s: src/core/resourcemgr.cpp.s
.PHONY : src/core/resourcemgr.s

# target to generate assembly for a file
src/core/resourcemgr.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/resourcemgr.cpp.s
.PHONY : src/core/resourcemgr.cpp.s

src/core/scenemgr.o: src/core/scenemgr.cpp.o
.PHONY : src/core/scenemgr.o

# target to build an object file
src/core/scenemgr.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/scenemgr.cpp.o
.PHONY : src/core/scenemgr.cpp.o

src/core/scenemgr.i: src/core/scenemgr.cpp.i
.PHONY : src/core/scenemgr.i

# target to preprocess a source file
src/core/scenemgr.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/scenemgr.cpp.i
.PHONY : src/core/scenemgr.cpp.i

src/core/scenemgr.s: src/core/scenemgr.cpp.s
.PHONY : src/core/scenemgr.s

# target to generate assembly for a file
src/core/scenemgr.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/scenemgr.cpp.s
.PHONY : src/core/scenemgr.cpp.s

src/core/screenmgr.o: src/core/screenmgr.cpp.o
.PHONY : src/core/screenmgr.o

# target to build an object file
src/core/screenmgr.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/screenmgr.cpp.o
.PHONY : src/core/screenmgr.cpp.o

src/core/screenmgr.i: src/core/screenmgr.cpp.i
.PHONY : src/core/screenmgr.i

# target to preprocess a source file
src/core/screenmgr.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/screenmgr.cpp.i
.PHONY : src/core/screenmgr.cpp.i

src/core/screenmgr.s: src/core/screenmgr.cpp.s
.PHONY : src/core/screenmgr.s

# target to generate assembly for a file
src/core/screenmgr.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/screenmgr.cpp.s
.PHONY : src/core/screenmgr.cpp.s

src/core/sprite.o: src/core/sprite.cpp.o
.PHONY : src/core/sprite.o

# target to build an object file
src/core/sprite.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/sprite.cpp.o
.PHONY : src/core/sprite.cpp.o

src/core/sprite.i: src/core/sprite.cpp.i
.PHONY : src/core/sprite.i

# target to preprocess a source file
src/core/sprite.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/sprite.cpp.i
.PHONY : src/core/sprite.cpp.i

src/core/sprite.s: src/core/sprite.cpp.s
.PHONY : src/core/sprite.s

# target to generate assembly for a file
src/core/sprite.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/sprite.cpp.s
.PHONY : src/core/sprite.cpp.s

src/core/tools.o: src/core/tools.cpp.o
.PHONY : src/core/tools.o

# target to build an object file
src/core/tools.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/tools.cpp.o
.PHONY : src/core/tools.cpp.o

src/core/tools.i: src/core/tools.cpp.i
.PHONY : src/core/tools.i

# target to preprocess a source file
src/core/tools.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/tools.cpp.i
.PHONY : src/core/tools.cpp.i

src/core/tools.s: src/core/tools.cpp.s
.PHONY : src/core/tools.s

# target to generate assembly for a file
src/core/tools.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/tools.cpp.s
.PHONY : src/core/tools.cpp.s

src/core/transform.o: src/core/transform.cpp.o
.PHONY : src/core/transform.o

# target to build an object file
src/core/transform.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/transform.cpp.o
.PHONY : src/core/transform.cpp.o

src/core/transform.i: src/core/transform.cpp.i
.PHONY : src/core/transform.i

# target to preprocess a source file
src/core/transform.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/transform.cpp.i
.PHONY : src/core/transform.cpp.i

src/core/transform.s: src/core/transform.cpp.s
.PHONY : src/core/transform.s

# target to generate assembly for a file
src/core/transform.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/transform.cpp.s
.PHONY : src/core/transform.cpp.s

src/core/view.o: src/core/view.cpp.o
.PHONY : src/core/view.o

# target to build an object file
src/core/view.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/view.cpp.o
.PHONY : src/core/view.cpp.o

src/core/view.i: src/core/view.cpp.i
.PHONY : src/core/view.i

# target to preprocess a source file
src/core/view.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/view.cpp.i
.PHONY : src/core/view.cpp.i

src/core/view.s: src/core/view.cpp.s
.PHONY : src/core/view.s

# target to generate assembly for a file
src/core/view.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/view.cpp.s
.PHONY : src/core/view.cpp.s

src/core/viewmgr.o: src/core/viewmgr.cpp.o
.PHONY : src/core/viewmgr.o

# target to build an object file
src/core/viewmgr.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/viewmgr.cpp.o
.PHONY : src/core/viewmgr.cpp.o

src/core/viewmgr.i: src/core/viewmgr.cpp.i
.PHONY : src/core/viewmgr.i

# target to preprocess a source file
src/core/viewmgr.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/viewmgr.cpp.i
.PHONY : src/core/viewmgr.cpp.i

src/core/viewmgr.s: src/core/viewmgr.cpp.s
.PHONY : src/core/viewmgr.s

# target to generate assembly for a file
src/core/viewmgr.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/core/viewmgr.cpp.s
.PHONY : src/core/viewmgr.cpp.s

src/gl/glattribarray.o: src/gl/glattribarray.cpp.o
.PHONY : src/gl/glattribarray.o

# target to build an object file
src/gl/glattribarray.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/glattribarray.cpp.o
.PHONY : src/gl/glattribarray.cpp.o

src/gl/glattribarray.i: src/gl/glattribarray.cpp.i
.PHONY : src/gl/glattribarray.i

# target to preprocess a source file
src/gl/glattribarray.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/glattribarray.cpp.i
.PHONY : src/gl/glattribarray.cpp.i

src/gl/glattribarray.s: src/gl/glattribarray.cpp.s
.PHONY : src/gl/glattribarray.s

# target to generate assembly for a file
src/gl/glattribarray.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/glattribarray.cpp.s
.PHONY : src/gl/glattribarray.cpp.s

src/gl/glshaderprogram.o: src/gl/glshaderprogram.cpp.o
.PHONY : src/gl/glshaderprogram.o

# target to build an object file
src/gl/glshaderprogram.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/glshaderprogram.cpp.o
.PHONY : src/gl/glshaderprogram.cpp.o

src/gl/glshaderprogram.i: src/gl/glshaderprogram.cpp.i
.PHONY : src/gl/glshaderprogram.i

# target to preprocess a source file
src/gl/glshaderprogram.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/glshaderprogram.cpp.i
.PHONY : src/gl/glshaderprogram.cpp.i

src/gl/glshaderprogram.s: src/gl/glshaderprogram.cpp.s
.PHONY : src/gl/glshaderprogram.s

# target to generate assembly for a file
src/gl/glshaderprogram.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/glshaderprogram.cpp.s
.PHONY : src/gl/glshaderprogram.cpp.s

src/gl/gltexture.o: src/gl/gltexture.cpp.o
.PHONY : src/gl/gltexture.o

# target to build an object file
src/gl/gltexture.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/gltexture.cpp.o
.PHONY : src/gl/gltexture.cpp.o

src/gl/gltexture.i: src/gl/gltexture.cpp.i
.PHONY : src/gl/gltexture.i

# target to preprocess a source file
src/gl/gltexture.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/gltexture.cpp.i
.PHONY : src/gl/gltexture.cpp.i

src/gl/gltexture.s: src/gl/gltexture.cpp.s
.PHONY : src/gl/gltexture.s

# target to generate assembly for a file
src/gl/gltexture.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/gltexture.cpp.s
.PHONY : src/gl/gltexture.cpp.s

src/gl/gltools.o: src/gl/gltools.cpp.o
.PHONY : src/gl/gltools.o

# target to build an object file
src/gl/gltools.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/gltools.cpp.o
.PHONY : src/gl/gltools.cpp.o

src/gl/gltools.i: src/gl/gltools.cpp.i
.PHONY : src/gl/gltools.i

# target to preprocess a source file
src/gl/gltools.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/gltools.cpp.i
.PHONY : src/gl/gltools.cpp.i

src/gl/gltools.s: src/gl/gltools.cpp.s
.PHONY : src/gl/gltools.s

# target to generate assembly for a file
src/gl/gltools.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/gltools.cpp.s
.PHONY : src/gl/gltools.cpp.s

src/gl/glviewcircle.o: src/gl/glviewcircle.cpp.o
.PHONY : src/gl/glviewcircle.o

# target to build an object file
src/gl/glviewcircle.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/glviewcircle.cpp.o
.PHONY : src/gl/glviewcircle.cpp.o

src/gl/glviewcircle.i: src/gl/glviewcircle.cpp.i
.PHONY : src/gl/glviewcircle.i

# target to preprocess a source file
src/gl/glviewcircle.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/glviewcircle.cpp.i
.PHONY : src/gl/glviewcircle.cpp.i

src/gl/glviewcircle.s: src/gl/glviewcircle.cpp.s
.PHONY : src/gl/glviewcircle.s

# target to generate assembly for a file
src/gl/glviewcircle.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/glviewcircle.cpp.s
.PHONY : src/gl/glviewcircle.cpp.s

src/gl/glviewfactory.o: src/gl/glviewfactory.cpp.o
.PHONY : src/gl/glviewfactory.o

# target to build an object file
src/gl/glviewfactory.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/glviewfactory.cpp.o
.PHONY : src/gl/glviewfactory.cpp.o

src/gl/glviewfactory.i: src/gl/glviewfactory.cpp.i
.PHONY : src/gl/glviewfactory.i

# target to preprocess a source file
src/gl/glviewfactory.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/glviewfactory.cpp.i
.PHONY : src/gl/glviewfactory.cpp.i

src/gl/glviewfactory.s: src/gl/glviewfactory.cpp.s
.PHONY : src/gl/glviewfactory.s

# target to generate assembly for a file
src/gl/glviewfactory.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/glviewfactory.cpp.s
.PHONY : src/gl/glviewfactory.cpp.s

src/gl/glviewmgr.o: src/gl/glviewmgr.cpp.o
.PHONY : src/gl/glviewmgr.o

# target to build an object file
src/gl/glviewmgr.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/glviewmgr.cpp.o
.PHONY : src/gl/glviewmgr.cpp.o

src/gl/glviewmgr.i: src/gl/glviewmgr.cpp.i
.PHONY : src/gl/glviewmgr.i

# target to preprocess a source file
src/gl/glviewmgr.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/glviewmgr.cpp.i
.PHONY : src/gl/glviewmgr.cpp.i

src/gl/glviewmgr.s: src/gl/glviewmgr.cpp.s
.PHONY : src/gl/glviewmgr.s

# target to generate assembly for a file
src/gl/glviewmgr.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/glviewmgr.cpp.s
.PHONY : src/gl/glviewmgr.cpp.s

src/gl/glviewrect.o: src/gl/glviewrect.cpp.o
.PHONY : src/gl/glviewrect.o

# target to build an object file
src/gl/glviewrect.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/glviewrect.cpp.o
.PHONY : src/gl/glviewrect.cpp.o

src/gl/glviewrect.i: src/gl/glviewrect.cpp.i
.PHONY : src/gl/glviewrect.i

# target to preprocess a source file
src/gl/glviewrect.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/glviewrect.cpp.i
.PHONY : src/gl/glviewrect.cpp.i

src/gl/glviewrect.s: src/gl/glviewrect.cpp.s
.PHONY : src/gl/glviewrect.s

# target to generate assembly for a file
src/gl/glviewrect.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/glviewrect.cpp.s
.PHONY : src/gl/glviewrect.cpp.s

src/gl/glviewshape.o: src/gl/glviewshape.cpp.o
.PHONY : src/gl/glviewshape.o

# target to build an object file
src/gl/glviewshape.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/glviewshape.cpp.o
.PHONY : src/gl/glviewshape.cpp.o

src/gl/glviewshape.i: src/gl/glviewshape.cpp.i
.PHONY : src/gl/glviewshape.i

# target to preprocess a source file
src/gl/glviewshape.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/glviewshape.cpp.i
.PHONY : src/gl/glviewshape.cpp.i

src/gl/glviewshape.s: src/gl/glviewshape.cpp.s
.PHONY : src/gl/glviewshape.s

# target to generate assembly for a file
src/gl/glviewshape.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/glviewshape.cpp.s
.PHONY : src/gl/glviewshape.cpp.s

src/gl/glviewsprite.o: src/gl/glviewsprite.cpp.o
.PHONY : src/gl/glviewsprite.o

# target to build an object file
src/gl/glviewsprite.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/glviewsprite.cpp.o
.PHONY : src/gl/glviewsprite.cpp.o

src/gl/glviewsprite.i: src/gl/glviewsprite.cpp.i
.PHONY : src/gl/glviewsprite.i

# target to preprocess a source file
src/gl/glviewsprite.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/glviewsprite.cpp.i
.PHONY : src/gl/glviewsprite.cpp.i

src/gl/glviewsprite.s: src/gl/glviewsprite.cpp.s
.PHONY : src/gl/glviewsprite.s

# target to generate assembly for a file
src/gl/glviewsprite.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/gl/glviewsprite.cpp.s
.PHONY : src/gl/glviewsprite.cpp.s

src/glm/detail/dummy.o: src/glm/detail/dummy.cpp.o
.PHONY : src/glm/detail/dummy.o

# target to build an object file
src/glm/detail/dummy.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/glm/detail/dummy.cpp.o
.PHONY : src/glm/detail/dummy.cpp.o

src/glm/detail/dummy.i: src/glm/detail/dummy.cpp.i
.PHONY : src/glm/detail/dummy.i

# target to preprocess a source file
src/glm/detail/dummy.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/glm/detail/dummy.cpp.i
.PHONY : src/glm/detail/dummy.cpp.i

src/glm/detail/dummy.s: src/glm/detail/dummy.cpp.s
.PHONY : src/glm/detail/dummy.s

# target to generate assembly for a file
src/glm/detail/dummy.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/glm/detail/dummy.cpp.s
.PHONY : src/glm/detail/dummy.cpp.s

src/glm/detail/glm.o: src/glm/detail/glm.cpp.o
.PHONY : src/glm/detail/glm.o

# target to build an object file
src/glm/detail/glm.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/glm/detail/glm.cpp.o
.PHONY : src/glm/detail/glm.cpp.o

src/glm/detail/glm.i: src/glm/detail/glm.cpp.i
.PHONY : src/glm/detail/glm.i

# target to preprocess a source file
src/glm/detail/glm.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/glm/detail/glm.cpp.i
.PHONY : src/glm/detail/glm.cpp.i

src/glm/detail/glm.s: src/glm/detail/glm.cpp.s
.PHONY : src/glm/detail/glm.s

# target to generate assembly for a file
src/glm/detail/glm.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/glm/detail/glm.cpp.s
.PHONY : src/glm/detail/glm.cpp.s

src/glut/glutmgr.o: src/glut/glutmgr.cpp.o
.PHONY : src/glut/glutmgr.o

# target to build an object file
src/glut/glutmgr.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/glut/glutmgr.cpp.o
.PHONY : src/glut/glutmgr.cpp.o

src/glut/glutmgr.i: src/glut/glutmgr.cpp.i
.PHONY : src/glut/glutmgr.i

# target to preprocess a source file
src/glut/glutmgr.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/glut/glutmgr.cpp.i
.PHONY : src/glut/glutmgr.cpp.i

src/glut/glutmgr.s: src/glut/glutmgr.cpp.s
.PHONY : src/glut/glutmgr.s

# target to generate assembly for a file
src/glut/glutmgr.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/glut/glutmgr.cpp.s
.PHONY : src/glut/glutmgr.cpp.s

src/png/viewfactorylibpng.o: src/png/viewfactorylibpng.cpp.o
.PHONY : src/png/viewfactorylibpng.o

# target to build an object file
src/png/viewfactorylibpng.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/png/viewfactorylibpng.cpp.o
.PHONY : src/png/viewfactorylibpng.cpp.o

src/png/viewfactorylibpng.i: src/png/viewfactorylibpng.cpp.i
.PHONY : src/png/viewfactorylibpng.i

# target to preprocess a source file
src/png/viewfactorylibpng.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/png/viewfactorylibpng.cpp.i
.PHONY : src/png/viewfactorylibpng.cpp.i

src/png/viewfactorylibpng.s: src/png/viewfactorylibpng.cpp.s
.PHONY : src/png/viewfactorylibpng.s

# target to generate assembly for a file
src/png/viewfactorylibpng.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/png/viewfactorylibpng.cpp.s
.PHONY : src/png/viewfactorylibpng.cpp.s

src/sdl/viewfactorysdl.o: src/sdl/viewfactorysdl.cpp.o
.PHONY : src/sdl/viewfactorysdl.o

# target to build an object file
src/sdl/viewfactorysdl.cpp.o:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/sdl/viewfactorysdl.cpp.o
.PHONY : src/sdl/viewfactorysdl.cpp.o

src/sdl/viewfactorysdl.i: src/sdl/viewfactorysdl.cpp.i
.PHONY : src/sdl/viewfactorysdl.i

# target to preprocess a source file
src/sdl/viewfactorysdl.cpp.i:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/sdl/viewfactorysdl.cpp.i
.PHONY : src/sdl/viewfactorysdl.cpp.i

src/sdl/viewfactorysdl.s: src/sdl/viewfactorysdl.cpp.s
.PHONY : src/sdl/viewfactorysdl.s

# target to generate assembly for a file
src/sdl/viewfactorysdl.cpp.s:
	$(MAKE) -f CMakeFiles/FlappyEngine.dir/build.make CMakeFiles/FlappyEngine.dir/src/sdl/viewfactorysdl.cpp.s
.PHONY : src/sdl/viewfactorysdl.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... FlappyEngine"
	@echo "... src/core/animation.o"
	@echo "... src/core/animation.i"
	@echo "... src/core/animation.s"
	@echo "... src/core/atlas.o"
	@echo "... src/core/atlas.i"
	@echo "... src/core/atlas.s"
	@echo "... src/core/camera.o"
	@echo "... src/core/camera.i"
	@echo "... src/core/camera.s"
	@echo "... src/core/circleshape.o"
	@echo "... src/core/circleshape.i"
	@echo "... src/core/circleshape.s"
	@echo "... src/core/entitymgr.o"
	@echo "... src/core/entitymgr.i"
	@echo "... src/core/entitymgr.s"
	@echo "... src/core/flappyapp.o"
	@echo "... src/core/flappyapp.i"
	@echo "... src/core/flappyapp.s"
	@echo "... src/core/gamemgr.o"
	@echo "... src/core/gamemgr.i"
	@echo "... src/core/gamemgr.s"
	@echo "... src/core/inputmgr.o"
	@echo "... src/core/inputmgr.i"
	@echo "... src/core/inputmgr.s"
	@echo "... src/core/manager.o"
	@echo "... src/core/manager.i"
	@echo "... src/core/manager.s"
	@echo "... src/core/presenter.o"
	@echo "... src/core/presenter.i"
	@echo "... src/core/presenter.s"
	@echo "... src/core/quad.o"
	@echo "... src/core/quad.i"
	@echo "... src/core/quad.s"
	@echo "... src/core/rectshape.o"
	@echo "... src/core/rectshape.i"
	@echo "... src/core/rectshape.s"
	@echo "... src/core/resourcemgr.o"
	@echo "... src/core/resourcemgr.i"
	@echo "... src/core/resourcemgr.s"
	@echo "... src/core/scenemgr.o"
	@echo "... src/core/scenemgr.i"
	@echo "... src/core/scenemgr.s"
	@echo "... src/core/screenmgr.o"
	@echo "... src/core/screenmgr.i"
	@echo "... src/core/screenmgr.s"
	@echo "... src/core/sprite.o"
	@echo "... src/core/sprite.i"
	@echo "... src/core/sprite.s"
	@echo "... src/core/tools.o"
	@echo "... src/core/tools.i"
	@echo "... src/core/tools.s"
	@echo "... src/core/transform.o"
	@echo "... src/core/transform.i"
	@echo "... src/core/transform.s"
	@echo "... src/core/view.o"
	@echo "... src/core/view.i"
	@echo "... src/core/view.s"
	@echo "... src/core/viewmgr.o"
	@echo "... src/core/viewmgr.i"
	@echo "... src/core/viewmgr.s"
	@echo "... src/gl/glattribarray.o"
	@echo "... src/gl/glattribarray.i"
	@echo "... src/gl/glattribarray.s"
	@echo "... src/gl/glshaderprogram.o"
	@echo "... src/gl/glshaderprogram.i"
	@echo "... src/gl/glshaderprogram.s"
	@echo "... src/gl/gltexture.o"
	@echo "... src/gl/gltexture.i"
	@echo "... src/gl/gltexture.s"
	@echo "... src/gl/gltools.o"
	@echo "... src/gl/gltools.i"
	@echo "... src/gl/gltools.s"
	@echo "... src/gl/glviewcircle.o"
	@echo "... src/gl/glviewcircle.i"
	@echo "... src/gl/glviewcircle.s"
	@echo "... src/gl/glviewfactory.o"
	@echo "... src/gl/glviewfactory.i"
	@echo "... src/gl/glviewfactory.s"
	@echo "... src/gl/glviewmgr.o"
	@echo "... src/gl/glviewmgr.i"
	@echo "... src/gl/glviewmgr.s"
	@echo "... src/gl/glviewrect.o"
	@echo "... src/gl/glviewrect.i"
	@echo "... src/gl/glviewrect.s"
	@echo "... src/gl/glviewshape.o"
	@echo "... src/gl/glviewshape.i"
	@echo "... src/gl/glviewshape.s"
	@echo "... src/gl/glviewsprite.o"
	@echo "... src/gl/glviewsprite.i"
	@echo "... src/gl/glviewsprite.s"
	@echo "... src/glm/detail/dummy.o"
	@echo "... src/glm/detail/dummy.i"
	@echo "... src/glm/detail/dummy.s"
	@echo "... src/glm/detail/glm.o"
	@echo "... src/glm/detail/glm.i"
	@echo "... src/glm/detail/glm.s"
	@echo "... src/glut/glutmgr.o"
	@echo "... src/glut/glutmgr.i"
	@echo "... src/glut/glutmgr.s"
	@echo "... src/png/viewfactorylibpng.o"
	@echo "... src/png/viewfactorylibpng.i"
	@echo "... src/png/viewfactorylibpng.s"
	@echo "... src/sdl/viewfactorysdl.o"
	@echo "... src/sdl/viewfactorysdl.i"
	@echo "... src/sdl/viewfactorysdl.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

