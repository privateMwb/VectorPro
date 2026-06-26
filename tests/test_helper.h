#pragma once

#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>
#include <memory_resource>

#include "Vector.h"

inline int total  = 0;
inline int pass   = 0;
inline int fail   = 0;

inline void stats() {
    std::cout << "T: " << total << "\n";
    std::cout << "P: " << pass << "\n";
    std::cout << "F: " << fail << "\n";
}

inline void setTitle(std::string_view title){
    std::cout << "\033[96m" << title << "\033[0m\n";
}

inline std::string prettify(const char* name) {
	std::string result{name};
	bool firstLetter = true;

	for (char& c : result) {
		if (firstLetter) {
			c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
			firstLetter = false;
		}
		if (c == '_') {
			c = ' ';
			firstLetter = true;
		}
	}
    
	return result;
}

#define RUN(name) do {                        \
    name();                                   \
    std::cout << "\033[92m[PASS]\033[0m "     \
              << prettify(#name)              \
              << "\n";                        \
} while (0)

#define CHK(expr) do {                        \
    if(!(expr)) {                             \
        ++fail;                               \
        std::cout << "\033[91m[FAIL]\033[0m " \
                  << #expr                    \
                  << " (" << __FILE__         \
                  << ":"  << __LINE__         \
                  << ")\n";                   \
    } else {                                  \
        ++pass;                               \
    }                                         \
    ++total;                                  \
} while (0)

// Shared Callable
inline int g_eventCount = 0;

inline void countListener(
    const VectorPro::Vector<int>&,
    VectorPro::Vector<int>::EventData) {
	++g_eventCount;
}

inline void resetEventCount() {
	g_eventCount = 0;
}
