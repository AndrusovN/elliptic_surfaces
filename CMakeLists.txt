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
add_executable( runUnitTests src/tests.cpp
        src/prime_field_tests.hpp
        src/math_exception.cpp
        src/polynomial_tests.hpp
        src/galois_field_tests.hpp
        src/multivariable_tests.hpp)
# Link test executable against gtest & gtest_main
target_link_libraries(runUnitTests gtest gtest_main)
add_test( runUnitTests runUnitTests )
add_executable(elliptic_surfaces src/main.cpp
        src/field_concept.h
        src/math_exception.h
        src/math_exception.cpp
        src/prime_field.h
        src/polynom.h
        src/galois_field.h
        src/multivariable_monomial.h
        src/multivariable_polynomial.h
        src/surface.h)


set_property(TARGET runUnitTests PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

set_property(TARGET elliptic_surfaces PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")