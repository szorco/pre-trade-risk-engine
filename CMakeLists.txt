cmake_minimum_required(VERSION 3.20)
project(PreTradeRiskEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-O2 -Wall -Wextra -pthread)
endif()

include_directories(include)

# Add your own .cpp files to this list as you build the engine out.
add_executable(risk_engine_bin
    src/main.cpp
)

find_package(Threads REQUIRED)
target_link_libraries(risk_engine_bin PRIVATE Threads::Threads)
