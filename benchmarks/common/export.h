#pragma once

#include "registry.h"

#include <fstream>
#include <iomanip>
#include <string>

inline void exportJson(const std::string& filename) {
    std::ofstream out(filename);

    if (!out)
        return;

    out << "[\n";

    const auto& results = benchmark_results();

    for (std::size_t i = 0; i < results.size(); ++i) {
        const auto& r = results[i];

        out << "  {\n";
        out << "    \"name\": \"" << r.name << "\",\n";
        out << "    \"library\": \"" << r.library << "\",\n";
        out << "    \"operation\": \"" << r.operation << "\",\n";
        out << "    \"total_ns\": " << r.total_ns << ",\n";
        out << "    \"iterations\": " << r.iterations << ",\n";
        out << "    \"ns_per_op\": " << std::fixed << std::setprecision(2) << r.ns_per_op << "\n";
        out << "  }";

        if (i + 1 != results.size())
            out << ",";

        out << "\n";
    }

    out << "]\n";
}