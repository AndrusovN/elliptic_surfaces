cmake_minimum_required(VERSION 3.28)
project(elliptic_surfaces)

set(CMAKE_CXX_STANDARD 23)

add_executable(elliptic_surfaces main.cpp
        field_concept.h
        math_exception.h
        math_exception.cpp
        prime_field.h
        polynom.h)
