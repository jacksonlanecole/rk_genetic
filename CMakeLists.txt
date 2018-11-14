cmake_minimum_required(VERSION 3.12.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)
project(rkev)
# --------------------------------------------------------------------------- #
# MAKE CHANGES HERE(except for project specific information below)
# --------------------------------------------------------------------------- #
set(SRCDIR "src/cpp")
# --------------------------------------------------------------------------- #

# --------------------------------------------------------------------------- #
# Local Stuff
# bring project headers into the project
# --------------------------------------------------------------------------- #
include_directories(include)
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
set(Boost_DEBUG                 OFF)
set(Boost_USE_STATIC_LIBS       OFF) # only find static libs
set(Boost_USE_MULTITHREADED     OFF)
set(Boost_USE_STATIC_RUNTIME    OFF)

find_package(Boost 1.67 COMPONENTS python37 REQUIRED)
message(STATUS "Setting up BOOST")
message(STATUS "\tBoost Includes : ${Boost_INCLUDE_DIRS}")
message(STATUS "\tBoost Libraries: ${Boost_LIBRARY_DIRS}")
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
# --------------------------------------------------------------------------- #


# --------------------------------------------------------------------------- #
# openmp
# This solution was found verbatim here:
# https://stackoverflow.com/questions/46414660/macos-cmake-and-openmp
# --------------------------------------------------------------------------- #
# Find OpenMP
if(APPLE AND USE_OpenMP)
    if(CMAKE_C_COMPILER_ID MATCHES "Clang")
        set(OpenMP_C "${CMAKE_C_COMPILER}")
        set(OpenMP_C_FLAGS "-fopenmp=libomp -Wno-unused-command-line-argument")
        set(OpenMP_C_LIB_NAMES "libomp" "libgomp" "libiomp5")
        set(OpenMP_libomp_LIBRARY ${OpenMP_C_LIB_NAMES})
        set(OpenMP_libgomp_LIBRARY ${OpenMP_C_LIB_NAMES})
        set(OpenMP_libiomp5_LIBRARY ${OpenMP_C_LIB_NAMES})
    endif()
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      set(OpenMP_CXX "${CMAKE_CXX_COMPILER}")
      set(OpenMP_CXX_FLAGS "-fopenmp=libomp -Wno-unused-command-line-argument")
      set(OpenMP_CXX_LIB_NAMES "libomp" "libgomp" "libiomp5")
      set(OpenMP_libomp_LIBRARY ${OpenMP_CXX_LIB_NAMES})
      set(OpenMP_libgomp_LIBRARY ${OpenMP_CXX_LIB_NAMES})
      set(OpenMP_libiomp5_LIBRARY ${OpenMP_CXX_LIB_NAMES})
    endif()
endif()

if(USE_OpenMP)
  find_package(OpenMP REQUIRED)
endif(USE_OpenMP)

if (OPENMP_FOUND)
    include_directories("${OPENMP_INCLUDES}")
    link_directories("${OPENMP_LIBRARIES}")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    # set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif(OPENMP_FOUND)
# --------------------------------------------------------------------------- #


# --------------------------------------------------------------------------- #
# Linking everything
# --------------------------------------------------------------------------- #
add_library(
	tableau
	SHARED
	${SRCDIR}/tableau.cpp
	)

add_library(
	rk
	SHARED
	${SRCDIR}/rk.cpp
	)

add_library(
	rk_ext
	SHARED
	${SRCDIR}/rk_ext.cpp
	)

target_link_libraries(
	tableau
	${Boost_LIBRARIES}
	${PYTHON_LIBRARIES}
	)

target_link_libraries(
	rk
	tableau
	${PYTHON_LIBRARIES}
	${Boost_LIBRARIES}
	)

target_link_libraries(
	rk_ext
	rk
	tableau
	${PYTHON_LIBRARIES}
	${Boost_LIBRARIES}
	)

set_target_properties(
	rk_ext
	PROPERTIES
	PREFIX
	""
	)
# --------------------------------------------------------------------------- #
