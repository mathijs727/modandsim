========================================================================
    CONSOLE APPLICATION : boltzmann Project Overview
========================================================================

AppWizard has created this boltzmann application for you.

This file contains a summary of what you will find in each of the files that
make up your boltzmann application.


boltzmann.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

boltzmann.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

boltzmann.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named boltzmann.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
Install on Linux (Ubuntu 15.10):

sudo apt-get install git
sudo apt-get install cmake
sudo apt-get install libglew-dev
sudo apt-get install libfreeimage-dev

git clone https://github.com/glfw/glfw.git
cd glfw
sudo apt-get build-dep glfw3
cmake -G "Unix Makefiles"
make
sudo make install

[IN BOLTZMANN FOLDER]
mkdir build
cd build
cmake -G "Unix Makefiles" ..
make
./boltzmann-openmp

/////////////////////////////////////////////////////////////////////////////
Install on Windows:
Open boltzmann.sln in VS2015
Running in debug mode will be sequential
Running in release mode will use openmp

/////////////////////////////////////////////////////////////////////////////
Install on OS X:
Install macports...
sudo port install git
sudo port install cmake
sudo port install glew
sudo port install glfw
sudo port install freeimage

[IN BOLTZMANN FOLDER]
mkdir build
cd build
cmake -G "Unix Makefiles" ..
make
./boltzmann-openmp