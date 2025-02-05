cmake_minimum_required(VERSION 3.20)
project(COGITO LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Simplified optimization flags
set(CMAKE_CXX_FLAGS "-O3 -march=native -mtune=native -static")

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.8.0
)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

FetchContent_MakeAvailable(Catch2)
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")


add_executable(cogito
    src/main.cpp
    src/lexer.cpp
    src/lexer.h
    src/token.h
)

add_executable(cogito_tests
    tests/lexer_tests.cpp
    src/lexer.cpp
)

target_link_libraries(cogito_tests PRIVATE Catch2::Catch2WithMain)

enable_testing()
add_test(NAME LexerTests COMMAND cogito_tests)
