cmake_minimum_required(VERSION 3.28)
project(elliptic_surfaces)

set(CMAKE_CXX_STANDARD 23)

################################
# GTest
################################
ADD_SUBDIRECTORY (googletest-1.14.0)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

################################
# Unit Tests
################################
# Add test cpp file
add_executable( runUnitTests tests.cpp
        prime_field_tests.hpp
        math_exception.cpp
        polynomial_tests.hpp
        galois_field_tests.hpp
        multivariable_tests.hpp)
# Link test executable against gtest & gtest_main
target_link_libraries(runUnitTests gtest gtest_main)
add_test( runUnitTests runUnitTests )
add_executable(elliptic_surfaces main.cpp
        field_concept.h
        math_exception.h
        math_exception.cpp
        prime_field.h
        polynom.h
        galois_field.h
        multivariable_monomial.h
        multivariable_polynomial.h
        surface.h)


set_property(TARGET runUnitTests PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

set_property(TARGET elliptic_surfaces PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")