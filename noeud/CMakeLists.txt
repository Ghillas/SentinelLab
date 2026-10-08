cmake_minimum_required(VERSION 3.20)
project(sentinellab_jour2 CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

set(AVERTISSEMENTS -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wno-sign-conversion)

add_executable(noeud main.cpp)

target_compile_options(noeud PRIVATE ${AVERTISSEMENTS} -fstack-usage)

