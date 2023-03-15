cmake_minimum_required(VERSION 3.14...3.25)

set(CMAKE_CXX_COMPILER "clang++")
set(BOOST_ENABLE_CMAKE ON)

project(
  tcpChat
  VERSION 0.1
  LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
  Boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_TAG boost-1.81.0)
FetchContent_MakeAvailable(Boost)

add_subdirectory(app)
add_subdirectory(src)
