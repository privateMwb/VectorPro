// Vector Operator Benchmark
// Measures equality and ordering comparison performance
// against std::vector where applicable.
//
// Covers:
// - operator== (equal vectors)
// - operator== (unequal vectors)
// - operator<=> (less than)
// - operator<=> (greater than)
// - operator<=> (equal)

#include "benchmark_helper.h"

#include <vector>

using namespace VectorPro;

static void equality_equal() {
    Vector<int>      a;
    Vector<int>      b;
    std::vector<int> stdA;
    std::vector<int> stdB;
    fillVector(a);
    fillVector(b);
    fillStd(stdA);
    fillStd(stdB);

    BENCH("VectorPro operator== equal",   ITERATIONS, (void)(a == b));
    BENCH("std::vector operator== equal", ITERATIONS, (void)(stdA == stdB));
}

static void equality_unequal() {
    Vector<int>      a;
    Vector<int>      b;
    std::vector<int> stdA;
    std::vector<int> stdB;
    fillVector(a);
    fillVector(b);
    fillStd(stdA);
    fillStd(stdB);

    b[0]    = -1;
    stdB[0] = -1;

    BENCH("VectorPro operator== unequal",   ITERATIONS, (void)(a == b));
    BENCH("std::vector operator== unequal", ITERATIONS, (void)(stdA == stdB));
}

static void spaceship_less() {
    Vector<int>      a;
    Vector<int>      b;
    std::vector<int> stdA;
    std::vector<int> stdB;
    fillVector(a);
    fillVector(b);
    fillStd(stdA);
    fillStd(stdB);

    b.back()    = 999999;
    stdB.back() = 999999;

    BENCH("VectorPro operator<=> less",   ITERATIONS, (void)(a <=> b));
    BENCH("std::vector operator<=> less", ITERATIONS, (void)(stdA <=> stdB));
}

static void spaceship_greater() {
    Vector<int>      a;
    Vector<int>      b;
    std::vector<int> stdA;
    std::vector<int> stdB;
    fillVector(a);
    fillVector(b);
    fillStd(stdA);
    fillStd(stdB);

    a.back()    = 999999;
    stdA.back() = 999999;

    BENCH("VectorPro operator<=> greater",   ITERATIONS, (void)(a <=> b));
    BENCH("std::vector operator<=> greater", ITERATIONS, (void)(stdA <=> stdB));
}

static void spaceship_equal() {
    Vector<int>      a;
    Vector<int>      b;
    std::vector<int> stdA;
    std::vector<int> stdB;
    fillVector(a);
    fillVector(b);
    fillStd(stdA);
    fillStd(stdB);

    BENCH("VectorPro operator<=> equal",   ITERATIONS, (void)(a <=> b));
    BENCH("std::vector operator<=> equal", ITERATIONS, (void)(stdA <=> stdB));
}

void run_operator_benchmarks() {
    bench_header("Operator");

    equality_equal();
    std::cout << "\n";

    equality_unequal();
    std::cout << "\n";

    spaceship_less();
    std::cout << "\n";

    spaceship_greater();
    std::cout << "\n";

    spaceship_equal();

    bench_footer();
}