cmake_minimum_required(VERSION 3.12.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)
project(rkev)
# --------------------------------------------------------------------------- #
# MAKE CHANGES HERE(except for project specific information below)
# --------------------------------------------------------------------------- #
set(SRCDIR
	"src"
	)
set(localPythonLibrary
	"/usr/local/Cellar/python3/3.7.0/Frameworks/Python.framework/Versions/3.7/lib/libpython3.7m.dylib"
	)
set(localPythonIncludeDir
	"/usr/local/Cellar/python/3.7.0/Frameworks/Python.framework/Versions/3.7/include/python3.7m"
	)
set(boostRoot
	"/usr/local/Cellar/boost/1.67.0_1"
	)
set(boostPython3Libs
	"/usr/local/Cellar/boost-python3/1.67.0_1/lib"
	)
# --------------------------------------------------------------------------- #

# --------------------------------------------------------------------------- #
# Local Stuff
# bring project headers into the project
# --------------------------------------------------------------------------- #
include_directories(include)
#file(GLOB SOURCES "src/*.cpp")
# --------------------------------------------------------------------------- #

# --------------------------------------------------------------------------- #
# Deals with actually creating the shared libraries rather than dylib on macOS
# --------------------------------------------------------------------------- #
if(APPLE)
	set(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
endif(APPLE)
# --------------------------------------------------------------------------- #

# --------------------------------------------------------------------------- #
# PYTHON STUFF
# --------------------------------------------------------------------------- #
set(PYTHON_LIBRARY
	${localPythonLibrary}
	CACHE FILEPATH
	"The path to Python libraries"
	)
set(PYTHON_INCLUDE_DIR
	${localPythonIncludeDir}
	CACHE PATH
	"Path to Python headers"
	)

find_package(PythonInterp 3.7 REQUIRED)
find_package(PythonLibs 3.7 REQUIRED)

if(PYTHONLIBS_FOUND)
	message(STATUS "Setting up Python Libraries")
	message(STATUS "\tPython Includes : ${PYTHON_INCLUDE_DIRS}")
	message(STATUS "\tPython Libraries: ${PYTHON_LIBRARIES}")
	include_directories(${PYTHON_INCLUDE_DIRS})
else()
	message(FATAL_ERROR "Unable to find PythonLibs.")
endif()
# --------------------------------------------------------------------------- #

# --------------------------------------------------------------------------- #
# BOOST STUFF
# --------------------------------------------------------------------------- #
set(BOOST_ROOT ${boostRoot})
set(BOOST_INCLUDEDIR "${BOOST_ROOT}/include")
set(BOOST_LIBRARYDIR ${boostPython3Libs})
set(Boost_DEBUG                 OFF)
set(Boost_USE_STATIC_LIBS       OFF) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

find_package(Boost COMPONENTS python37 REQUIRED)
message(STATUS "Setting up BOOST")
message(STATUS "\tBoost Includes : ${Boost_INCLUDE_DIRS}")
message(STATUS "\tBoost Libraries: ${Boost_LIBRARY_DIRS}")
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
# --------------------------------------------------------------------------- #

# --------------------------------------------------------------------------- #
# Linking everything
# --------------------------------------------------------------------------- #
add_library(
	rk
	SHARED
	${SRCDIR}/rk.cpp
	${SRCDIR}/tableau.cpp
	)
add_library(
	rk_ext
	SHARED
	${SRCDIR}/rk_ext.cpp
	)
target_link_libraries(
	rk_ext
	rk
	${Boost_LIBRARIES}
	${PYTHON_LIBRARIES}
	)
set_target_properties(
	rk_ext
	PROPERTIES
	PREFIX
	""
	)
# --------------------------------------------------------------------------- #
