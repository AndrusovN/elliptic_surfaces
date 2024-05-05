#pragma once
#include <string>
#include <exception>

class MathException : public std::exception {
public:
    MathException();
    MathException(std::string text);
    MathException(std::string& text);

    virtual void show();
    virtual std::string what();
private:
    std::string message;
};
