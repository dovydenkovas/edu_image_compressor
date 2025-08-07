CMAKE_MINIMUM_REQUIRED(VERSION 3.10)


PROJECT("compressor")
set (CMAKE_EXPORT_COMPILE_COMMANDS On)
set (CMAKE_CXX_STANDARD 23)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23 -O3")

find_package(
  Boost  REQUIRED
  COMPONENTS headers program_options
  )


SET(sources
    src/main.cpp
    src/bwt.cpp
    src/cli.cpp
    src/bmp.cpp
    src/rle.cpp
    src/sort.cpp
    src/profiler.cpp
)

ADD_EXECUTABLE(compressor ${sources})

target_link_libraries(
  compressor
  Boost::headers
  Boost::program_options
)
