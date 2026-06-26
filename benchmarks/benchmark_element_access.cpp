// Vector Element Access & Search Benchmark
// Measures indexed access, bounds-checked access, convenience
// accessors, raw pointer access, span access, and value search
// performance against std::vector where applicable.
//
// Covers:
// - operator[]
// - at()
// - front() and back()
// - data_ptr()
// - as_span()
// - contains()
// - find()

#include "benchmark_helper.h"

#include <vector>
#include <algorithm>

using namespace VectorPro;

static void operator_index() {
    Vector<int>      v;
    std::vector<int> stdV;
    fillVector(v);
    fillStd(stdV);

    BENCH("VectorPro operator[]",   ITERATIONS, (void)v[50]);
    BENCH("std::vector operator[]", ITERATIONS, (void)stdV[50]);
}

static void at_access() {
    Vector<int>      v;
    std::vector<int> stdV;
    fillVector(v);
    fillStd(stdV);

    BENCH("VectorPro at()",   ITERATIONS, (void)v.at(50));
    BENCH("std::vector at()", ITERATIONS, (void)stdV.at(50));
}

static void front_back() {
    Vector<int>      v;
    std::vector<int> stdV;
    fillVector(v);
    fillStd(stdV);

    BENCH("VectorPro front()",   ITERATIONS, (void)v.front());
    BENCH("std::vector front()", ITERATIONS, (void)stdV.front());
    BENCH("VectorPro back()",    ITERATIONS, (void)v.back());
    BENCH("std::vector back()",  ITERATIONS, (void)stdV.back());
}

static void data_ptr() {
    Vector<int>      v;
    std::vector<int> stdV;
    fillVector(v);
    fillStd(stdV);

    BENCH("VectorPro data_ptr()", ITERATIONS, (void)v.data_ptr());
    BENCH("std::vector data()",   ITERATIONS, (void)stdV.data());
}

static void as_span() {
    Vector<int> v;
    fillVector(v);

    BENCH("VectorPro as_span()", ITERATIONS, (void)v.as_span());
}

static void contains() {
    Vector<int>      v;
    std::vector<int> stdV;
    fillVector(v);
    fillStd(stdV);

    // best case — element at front
    BENCH("VectorPro contains() best case",    ITERATIONS, (void)v.contains(0));
    BENCH("VectorPro contains() worst case",   ITERATIONS, (void)v.contains(-1));

    // std::vector equivalent via std::find
    BENCH("std::vector find() best case",  ITERATIONS, (void)(std::find(stdV.begin(), stdV.end(), 0) != stdV.end()));
    BENCH("std::vector find() worst case", ITERATIONS, (void)(std::find(stdV.begin(), stdV.end(), -1) != stdV.end()));
}

static void find() {
    Vector<int>      v;
    std::vector<int> stdV;
    fillVector(v);
    fillStd(stdV);

    // best case — element at front
    BENCH("VectorPro find() best case",    ITERATIONS, (void)v.find(0));
    BENCH("VectorPro find() worst case",   ITERATIONS, (void)v.find(-1));

    BENCH("std::find() best case",  ITERATIONS, (void)std::find(stdV.begin(), stdV.end(), 0));
    BENCH("std::find() worst case", ITERATIONS, (void)std::find(stdV.begin(), stdV.end(), -1));
}

void run_element_access_benchmarks() {
    bench_header("Element Access & Search");

    operator_index();
    std::cout << "\n";

    at_access();
    std::cout << "\n";

    front_back();
    std::cout << "\n";

    data_ptr();
    std::cout << "\n";

    as_span();
    std::cout << "\n";

    contains();
    std::cout << "\n";

    find();

    bench_footer();
}