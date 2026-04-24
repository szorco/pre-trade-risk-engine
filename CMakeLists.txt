cmake_minimum_required(VERSION 3.20)

project(lob_signal_engine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(lob_engine STATIC
  src/mmap_file.cpp
)
target_include_directories(lob_engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (MSVC)
  target_compile_options(lob_engine INTERFACE /W4 /permissive-)
else()
  target_compile_options(lob_engine INTERFACE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
endif()

add_executable(lob_cli src/main.cpp)
target_link_libraries(lob_cli PRIVATE lob_engine)
