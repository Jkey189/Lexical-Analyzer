cmake_minimum_required(VERSION 3.29)
project(Lexical-Analyzer)

set(CMAKE_CXX_STANDARD 20)

add_executable(Lexical-Analyzer main.cpp
        includes/includes.h
        tokens.h)
