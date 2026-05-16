cmake_minimum_required(VERSION 3.20)
project(PreTradeRiskEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-O3 -march=native -Wall -Wextra -pthread)
endif()

include_directories(include)

add_executable(risk_engine_bin
    src/main.cpp
    src/risk_engine.cpp
)

target_link_libraries(risk_engine_bin PRIVATE pthread)
