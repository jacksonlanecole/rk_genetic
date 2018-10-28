CMAKE_MINIMUM_REQUIRED (VERSION 3.12.1)
SET (CMAKE_CXX_STANDARD 17)
SET (CMAKE_BUILD_TYPE Release)
PROJECT (rk_genetic)
# --------------------------------------------------------------------------- #
# MAKE CHANGES HERE (except for project specific information below)
# --------------------------------------------------------------------------- #
SET (SRCDIR
	"src"
	)
SET (localPythonLibrary
	"/usr/local/Cellar/python3/3.7.0/Frameworks/Python.framework/Versions/3.7/lib/libpython3.7m.dylib"
	)
SET (localPythonIncludeDir
	"/usr/local/Cellar/python/3.7.0/Frameworks/Python.framework/Versions/3.7/include"
	)
SET (boostRoot
	"/usr/local/Cellar/boost/1.67.0_1"
	)
SET (boostPython3Libs
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
IF (APPLE)
	SET (CMAKE_SHARED_LIBRARY_SUFFIX ".so")
ENDIF (APPLE)
# --------------------------------------------------------------------------- #

# --------------------------------------------------------------------------- #
# PYTHON STUFF
# --------------------------------------------------------------------------- #
SET (PYTHON_LIBRARY
	${localPythonLibrary}
	CACHE FILEPATH
	"The path to Python libraries"
	)
SET (PYTHON_INCLUDE_DIR
	${localPythonIncludeDir}
	CACHE PATH
	"Path to Python headers"
	)

FIND_PACKAGE (PythonInterp 3.6 REQUIRED)
FIND_PACKAGE (PythonLibs 3.6 REQUIRED)

IF(PYTHONLIBS_FOUND)
	INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})
ELSE()
	MESSAGE(FATAL_ERROR "Unable to find PythonLibs.")
ENDIF()
# --------------------------------------------------------------------------- #

# --------------------------------------------------------------------------- #
# BOOST STUFF
# --------------------------------------------------------------------------- #
SET (BOOST_ROOT ${boostRoot})
SET (BOOST_INCLUDEDIR "${BOOST_ROOT}/include")
SET (BOOST_LIBRARYDIR ${boostPython3Libs})
SET (Boost_DEBUG                 OFF)
SET (Boost_USE_STATIC_LIBS       OFF) # only find static libs
SET (Boost_USE_MULTITHREADED      ON)
SET (Boost_USE_STATIC_RUNTIME    OFF)

find_package(Boost COMPONENTS python37 REQUIRED)
message(STATUS "Setting up BOOST")
message(STATUS "> Boost Includes : ${Boost_INCLUDE_DIRS}")
message(STATUS "> Boost Libraries: ${Boost_LIBRARY_DIRS}")
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
# --------------------------------------------------------------------------- #

# --------------------------------------------------------------------------- #
# Linking everything
# --------------------------------------------------------------------------- #
add_library(
	greet
	SHARED
	${SRCDIR}/greet.cpp
	)
add_library(
	greet_ext
	SHARED
	${SRCDIR}/greet_ext.cpp
	)
target_link_libraries(
	greet_ext
	greet
	${Boost_LIBRARIES}
	${PYTHON_LIBRARIES}
	)
set_target_properties(
	greet_ext
	PROPERTIES
	PREFIX
	""
	)
# --------------------------------------------------------------------------- #
