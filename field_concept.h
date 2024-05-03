#pragma once

template<typename T>
concept Field = requires(T a, T b, const T c, const T d, bool e) {
    a = c + d;
    a = c - d;
    a = c * d;
    a = c / d;
    b = c.inverse();
    b = -c;
    a += c;
    a -= c;
    a *= c;
    a /= c;
    e = a == b;
};
