// VectorPro vs std::vector Benchmark Suite
// Measures core dynamic array operations:
//
// - push_back (amortized growth)
// - emplace_back (in-place construction efficiency)
// - pop_back (O(1))
// - insert (O(n) shifting)
// - erase front (O(n) shifting)
// - remove_if (O(n) filtering + compaction)

#include <iostream>
#include <chrono>
#include <cstddef>

#include "VectorPro.h"
#include "Table.h"

// Timing utility (returns milliseconds)
template<typename F>
auto duration(F func) {
	auto start = std::chrono::steady_clock::now();
	func();
	auto end = std::chrono::steady_clock::now();

	return std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
}

// Push Back Benchmark
// tests amortized append performance
void push_back() {
	VectorPro<int> vp;
	std::vector<int> v;

	VectorPro<long> vp_durations;
	VectorPro<long> v_durations;

	// workload sizes
	VectorPro<std::size_t> counts = {
		1'000'000,
		2'000'000,
		4'000'000,
		8'000'000
	};

	for (std::size_t i = 0; i < counts.size(); ++i) {

		auto vp_duration = duration([&]() {
			for (std::size_t j = 0; j < counts[i]; ++j)
				vp.push_back(j);
		});

		auto v_duration = duration([&]() {
			for (std::size_t j = 0; j < counts[i]; ++j)
				v.push_back(j);
		});

		vp_durations.push_back(vp_duration.count());
		v_durations.push_back(v_duration.count());
	}

	VectorPro<VectorPro<std::string>> data{
		Table::convert(counts),
		Table::convert(vp_durations, "ms"),
		Table::convert(v_durations, "ms")
	};

	Table::table(
	    "Push Back Benchmarks",
	{"Count", "VectorPro", "std::vector"},
	data,
	50
	);
}

// Pop Back Benchmark
// tests O(1) removal from end
void pop_back() {
	VectorPro<int> vp;
	std::vector<int> v;

	VectorPro<long> vp_durations;
	VectorPro<long> v_durations;

	VectorPro<std::size_t> counts = {
		100'000,
		200'000,
		400'000,
		800'000
	};

	for (std::size_t i = 0; i < counts.size(); ++i) {

		for (std::size_t j = 0; j < counts[i]; ++j) {
			vp.push_back(j);
			v.push_back(j);
		}

		auto vp_duration = duration([&]() {
			for (std::size_t j = 0; j < counts[i]; ++j)
				vp.pop_back();
		});

		auto v_duration = duration([&]() {
			for (std::size_t j = 0; j < counts[i]; ++j)
				v.pop_back();
		});

		vp_durations.push_back(vp_duration.count());
		v_durations.push_back(v_duration.count());
	}

	VectorPro<VectorPro<std::string>> data{
		Table::convert(counts),
		Table::convert(vp_durations, "ms"),
		Table::convert(v_durations, "ms")
	};

	Table::table(
	    "Pop Back Benchmarks",
	{"Count", "VectorPro", "std::vector"},
	data,
	50
	);
}

// Insert Benchmark
// tests O(n) shifting during insertion
void insert() {
	VectorPro<int> vp;
	std::vector<int> v;

	VectorPro<long> vp_durations;
	VectorPro<long> v_durations;

	VectorPro<std::size_t> counts = {
		10'000,
		20'000,
		40'000,
		80'000
	};

	for (std::size_t i = 0; i < counts.size(); ++i) {

		for (std::size_t j = 0; j < counts[i]; ++j) {
			vp.push_back(j);
			v.push_back(j);
		}

		auto vp_duration = duration([&]() {
			for (std::size_t j = 0; j < counts[i]; ++j)
				vp.insert(j, j + 1);
		});

		auto v_duration = duration([&]() {
			for (std::size_t j = 0; j < counts[i]; ++j)
				v.insert(v.begin() + j, j + 1);
		});

		vp_durations.push_back(vp_duration.count());
		v_durations.push_back(v_duration.count());
	}

	VectorPro<VectorPro<std::string>> data{
		Table::convert(counts),
		Table::convert(vp_durations, "ms"),
		Table::convert(v_durations, "ms")
	};

	Table::table(
	    "Insert Benchmarks",
	{"Count", "VectorPro", "std::vector"},
	data,
	50
	);
}

// Erase Benchmark
// tests O(n) shifting from front
void erase() {
	VectorPro<int> vp;
	std::vector<int> v;

	VectorPro<long> vp_durations;
	VectorPro<long> v_durations;

	VectorPro<std::size_t> counts = {
		10'000,
		20'000,
		40'000,
		80'000
	};

	for (std::size_t i = 0; i < counts.size(); ++i) {

		for (std::size_t j = 0; j < counts[i]; ++j) {
			vp.push_back(j);
			v.push_back(j);
		}

		auto vp_duration = duration([&]() {
			for (std::size_t j = 0; j < counts[i]; ++j)
				vp.erase(0);
		});

		auto v_duration = duration([&]() {
			for (std::size_t j = 0; j < counts[i]; ++j)
				v.erase(v.begin());
		});

		vp_durations.push_back(vp_duration.count());
		v_durations.push_back(v_duration.count());
	}

	VectorPro<VectorPro<std::string>> data{
		Table::convert(counts),
		Table::convert(vp_durations, "ms"),
		Table::convert(v_durations, "ms")
	};

	Table::table(
	    "Erase Benchmarks",
	{"Count", "VectorPro", "std::vector"},
	data,
	50
	);
}

// Emplace Back Benchmark
// tests in-place construction vs push_back overhead
void emplace_back() {
	VectorPro<int> vp;
	std::vector<int> v;

	VectorPro<long> vp_durations;
	VectorPro<long> v_durations;

	VectorPro<std::size_t> counts = {
		1'000'000,
		2'000'000,
		4'000'000,
		8'000'000
	};

	for (std::size_t i = 0; i < counts.size(); ++i) {

		auto vp_duration = duration([&]() {
			for (std::size_t j = 0; j < counts[i]; ++j)
				vp.emplace_back(j);
		});

		auto v_duration = duration([&]() {
			for (std::size_t j = 0; j < counts[i]; ++j)
				v.emplace_back(j);
		});

		vp_durations.push_back(vp_duration.count());
		v_durations.push_back(v_duration.count());
	}

	VectorPro<VectorPro<std::string>> data{
		Table::convert(counts),
		Table::convert(vp_durations, "ms"),
		Table::convert(v_durations, "ms")
	};

	Table::table(
		"Emplace Back Benchmarks",
		{"Count", "VectorPro", "std::vector"},
		data,
		50
	);
}

// Remove If Benchmark
// tests filtering performance (O(n) traversal + compaction)
void remove_if() {
	VectorPro<int> vp;
	std::vector<int> v;

	VectorPro<long> vp_durations;
	VectorPro<long> v_durations;

	VectorPro<std::size_t> counts = {
		100'000,
		200'000,
		400'000,
		800'000
	};

	for (std::size_t i = 0; i < counts.size(); ++i) {

		for (std::size_t j = 0; j < counts[i]; ++j) {
			vp.push_back(j);
			v.push_back(j);
		}

		auto vp_duration = duration([&]() {
			vp.remove_if([](int x) {
				return x % 2 == 0;
			});
		});

		auto v_duration = duration([&]() {
			v.erase(
				std::remove_if(v.begin(), v.end(),
					[](int x) { return x % 2 == 0; }),
				v.end()
			);
		});

		vp_durations.push_back(vp_duration.count());
		v_durations.push_back(v_duration.count());
	}

	VectorPro<VectorPro<std::string>> data{
		Table::convert(counts),
		Table::convert(vp_durations, "ms"),
		Table::convert(v_durations, "ms")
	};

	Table::table(
		"Remove If Benchmarks",
		{"Count", "VectorPro", "std::vector"},
		data,
		50
	);
}

int main() {
	push_back();
	pop_back();
	insert();
	erase();
	emplace_back();
	remove_if();
	
	return 0;
}



