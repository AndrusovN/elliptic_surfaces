#include <iostream>
#include "math_exception.h"

MathException::MathException() : message("Unspecified math exception occurred") {}
MathException::MathException(std::string text) : message(std::move(text)) {}
MathException::MathException(std::string& text) : message(text) {}

void MathException::show() {
    std::cerr << message << std::endl;
}

std::string MathException::what() {
    return message;
}
