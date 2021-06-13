cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)
project(bamAnalyzer)

set(src src/Analyzer.cpp
        src/BamReader.cpp
        src/BamRead.cpp
        src/FastaReader.cpp
        src/main.cpp)
include_directories(inc)

find_package(ZLIB REQUIRED)

add_executable(bamAnalyzer ${src})

target_link_libraries(bamAnalyzer z)

