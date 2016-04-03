Main Page {#mainpage}
=========
# FlappyEngine
##Install
To install:

	git clone https://github.com/mrtrizer/FlappyEngine.git flappy
	cd dviglo
	cmake -G "Unix Makefiles"
	make
	make install

##Create own project
To init new project:

	mkdir MyProject
	cd MyProject
	flappy init
	flappy gentarget cmake

##Build project
To build project:

	cd MyProject/targets/cmake/
	cmake -G "Unix Makefiles"
	make

# Documentation
It is Doxyfile in a doc folder. You need doxygen and graphviz installed to generate full documentation. Run doxygen in a project root and generated docs will be placed in doc/doxydoc/html.

# Project structure
##File tree
Every project has flappy.json file in a root. It defines configuration of project for different targets.
