#pragma once

#include <iostream>
#include <string>
#include <iomanip>

inline void setTitle(std::string_view title) {
    std::cout << "\033[96m" << title << "\033[0m\n";
}

template<typename T>
inline void dataFormat(std::string_view left, T value) {
    std::cout << std::left
              << std::setw(30) << left << ":"
              << std::setw(15) << value
              << "\n";
}