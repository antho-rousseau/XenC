cmake_minimum_required(VERSION 2.8)
project(XenC)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

file(GLOB SOURCE_FILES 
    src/*.cpp
    src/utils/*.cpp
    src/modes/*.cpp
    src/kenlm/lm/*.cc
    src/kenlm/lm/builder/*.cc
    src/kenlm/lm/common/*.cc
    src/kenlm/util/*.cc
    src/kenlm/util/double-conversion/*.cc
    src/kenlm/util/stream/*.cc
    include/*.h
    include/modes/*.h
    include/utils/*.h
    include/utils/*.hpp
    include/utils/threadpool/*.hpp
    include/utils/threadpool/detail/*.hpp
    include/kenlm/lm/*.hh
    include/kenlm/lm/builder/*.hh
    include/kenlm/lm/common/*.hh
    include/kenlm/util/*.hh
    include/kenlm/util/double-conversion/*.h
    include/kenlm/util/stream/*.hh)

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

option(BOOST "Path to BOOST")
option(DEBUG "Set debug symbols")

if(BOOST)
    set(BOOST_ROOT ${BOOST})
endif()

find_package(Boost 1.57.0 COMPONENTS regex filesystem system iostreams program_options thread REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Boost not found at all, please check your system configuration.")
endif()

find_package(ZLIB)

if (ZLIB_FOUND)
    set(HAVE_ZLIB 1)
    set(ZLIB_INCLUDE_DIRECTORIES ${ZLIB_INCLUDE_DIRECTORIES} ${ZLIB_INCLUDE_DIRS})
endif()

if(HAVE_ZLIB)
    add_definitions(-DHAVE_ZLIB)
endif()

if (DEBUG)
    set(CMAKE_BUILD_TYPE Debug)
    set(CMAKE_DEBUG_POSTFIX "_d" CACHE STRING "postfix applied to debug build")
    add_executable(XenC ${SOURCE_FILES})
    set_target_properties(XenC PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
else()
    set(CMAKE_BUILD_TYPE Release)
    add_executable(XenC ${SOURCE_FILES})
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_link_libraries(XenC ${Boost_LIBRARIES} z pthread bz2)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(XenC ${Boost_LIBRARIES} z pthread bz2 rt)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    message(FATAL_ERROR "XenC is not supported on Windows platforms.")
else()
    message(FATAL_ERROR "Unsupported platform.")
endif()

set(KENLM "${KENLM}" CACHE PATH "Set to path to KENLM Build" FORCE)
set(BOOST "${BOOST}" CACHE PATH "Set to path to BOOST dir" FORCE)
