#pragma once

#include "helper.h"

#include <fstream>
#include <iomanip>
#include <string>

inline void exportJson(const std::string& filename) {
    std::ofstream out(filename);

    if (!out)
        return;

    out << "[\n";

    const auto& results = regression_results();

    for (std::size_t i = 0; i < results.size(); ++i) {
        const auto& r = results[i];

        out << "  {\n";
        out << "    \"suite\": \"" << r.suite << "\",\n";
        out << "    \"operation\": \"" << r.operation << "\",\n";
        out << "    \"iteration\": " << r.iteration << ", \n";
        out << "    \"baseline_ns\": " << std::fixed << std::setprecision(2) << r.baseline_ns
            << ",\n";
        out << "    \"current_ns\": " << std::fixed << std::setprecision(2) << r.current_ns
            << ",\n";
        out << "    \"pct_change\": " << std::fixed << std::setprecision(2) << r.pct_change << "\n";
        out << "  }";

        if (i + 1 != results.size())
            out << ",";

        out << "\n";
    }

    out << "]\n";
}

// Writes the markdown transcript accumulated in markdown_buffer() (built up
// by setHeader()/printComparisonRow() during the run) to a file.
inline void exportMarkdown(const std::string& filename) {
    std::ofstream out(filename);

    if (!out)
        return;

    out << "# VectorPro Regression Report\n";
    out << markdown_buffer();
}
