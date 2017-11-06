# CMakeLists.txt --- CMake project settings
#    ex) cmake -G "Visual Studio 9 2008"
#    ex) cmake -DCMAKE_BUILD_TYPE=Release -G "MSYS Makefiles"
##############################################################################

# CMake minimum version
cmake_minimum_required(VERSION 2.4)

# Enable testing
enable_testing()

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

# project name and language
project(BoostWaveExample CXX)

# check build type
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Debug")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# make uppercase string of build type
string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)

# check build type again
if (NOT uppercase_CMAKE_BUILD_TYPE MATCHES "^(DEBUG|RELEASE|RELWITHDEBINFO|MINSIZEREL)$")
    message(FATAL_ERROR "Invalid value for CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
endif()

# define NDEBUG
string(CONCAT CMAKE_C_FLAGS_RELEASE " -DNDEBUG")
string(CONCAT CMAKE_C_FLAGS_RELWITHDEBINFO " -DNDEBUG")
string(CONCAT CMAKE_C_FLAGS_MINSIZEREL " -DNDEBUG")
string(CONCAT CMAKE_CXX_FLAGS_RELEASE " -DNDEBUG")
string(CONCAT CMAKE_CXX_FLAGS_RELWITHDEBINFO " -DNDEBUG")
string(CONCAT CMAKE_CXX_FLAGS_MINSIZEREL " -DNDEBUG")

# set output directory (build/)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

##############################################################################
# Use libiconv?

if(CMAKE_HOST_WIN32)
    option(USE_ICONV "Use libiconv" OFF)
else()
    option(USE_ICONV "Use libiconv" ON)
endif()

# Use libiconv?
message("USE_ICONV is ${USE_ICONV}")
if(USE_ICONV)
    add_definitions(-DUSE_ICONV)
endif()

# To load "FindIconv.cmake"
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/.)

##############################################################################

# Boost
find_package(Boost REQUIRED COMPONENTS system filesystem wave)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

##############################################################################

# subdirectories
subdirs(
    No1_Basic
    No2_IgnoreDirectiveHook
    No3_OutputHook
    No4_MakeIncludeTree
    No5_JapaneseEncoding)

##############################################################################
