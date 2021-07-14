cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)
project(bambam)

set(src src/Analyzer.cpp
        src/BamReader.cpp
        src/BamRead.cpp
        src/Base.cpp
        src/Exporter.cpp
        src/FastaReader.cpp
        src/Sequence.cpp
        src/Transcript.cpp
        src/main.cpp)
include_directories(inc)

find_package(ZLIB REQUIRED)
find_package(Threads REQUIRED)

add_executable(bambam ${src})

target_link_libraries(bambam z Threads::Threads)

