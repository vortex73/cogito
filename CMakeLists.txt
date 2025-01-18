cmake_minimum_required(VERSION 3.20)
project(COGITO LANGUAGES CXX)

set(CMAKE_STANDARD 20)
set(CMAKE_STANDARD_REQUIRED True)

add_executable(cogito
	src/lexer.cpp
	src/lexer.h
	src/token.h
)
