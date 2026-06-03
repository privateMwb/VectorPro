#include <iostream>
#include <vector>
#include <chrono>

#include "VectorPro.h"

using size_type = std::size_t;
namespace global {
constexpr size_type reserve = 10'000'000;
constexpr size_type num = 1'000'000;
constexpr size_type index = 56700;
constexpr size_type value = 999999;
constexpr size_type inserts[] = {
    456, 
    2364, 
    878, 
    135, 
    8};
constexpr size_type erases[] = {
	999999,
	89873,
	5673,
	53100,
	3};
constexpr auto removeNums = [](int x) {
	return x % 5 == 0; };
}

auto t() {
	return std::chrono::high_resolution_clock::now();
}

template<typename T>
void push_back_benchmark(VectorPro<T>& vp, std::vector<T>& v) {
	auto vp_start = t();
	for(size_type i = 0; i<global::num; ++i) {
		vp.push_back(i);
	}
	auto vp_end = t();

	auto v_start = t();
	for(size_type j = 0; j<global::num; ++j) {
		v.push_back(j);
	}
	auto v_end = t();

	std::cout << "PushBack Benchmark\n";

	std::cout << "VectorPro: " <<
	          std::chrono::duration_cast<std::chrono::milliseconds>(vp_end-vp_start).count() <<
	          "ms\n";

	std::cout << "std::vector: " <<
	          std::chrono::duration_cast<std::chrono::milliseconds>(v_end-v_start).count() <<
	          "ms\n\n";
}

template<typename T>
void emplace_back_benchmark(VectorPro<T>& vp, std::vector<T>& v) {
	auto vp_start = t();
	for(size_type i = 0; i<global::num; ++i) {
		vp.emplace_back(i);
	}
	auto vp_end = t();

	auto v_start = t();
	for(size_type j = 0; j<global::num; ++j) {
		v.emplace_back(j);
	}
	auto v_end = t();

	std::cout << "EmplaceBack Benchmark\n";

	std::cout << "VectorPro: " <<
	          std::chrono::duration_cast<std::chrono::milliseconds>(vp_end-vp_start).count() <<
	          "ms\n";

	std::cout << "std::vector: " <<
	          std::chrono::duration_cast<std::chrono::milliseconds>(v_end-v_start).count() <<
	          "ms\n\n";
}

template<typename T>
void pop_back_benchmark(VectorPro<T>& vp, std::vector<T>& v) {
	auto vp_start = t();
	for(size_type i = 0; i<global::num; ++i) {
		vp.pop_back();
	}
	auto vp_end = t();

	auto v_start = t();
	for(size_type j = 0; j<global::num; ++j) {
		v.pop_back();
	}
	auto v_end = t();

	std::cout << "PopBack Benchmark\n";

	std::cout << "VectorPro: " <<
	          std::chrono::duration_cast<std::chrono::milliseconds>(vp_end-vp_start).count() <<
	          "ms\n";

	std::cout << "std::vector: " <<
	          std::chrono::duration_cast<std::chrono::milliseconds>(v_end-v_start).count() <<
	          "ms\n\n";
}

template<typename T>
void insert_benchmark(VectorPro<T>& vp, std::vector<T>& v) {
	auto vp_start = t();
	for(size_type x : global::inserts) {
		vp.insert(x, global::value);
	}
	auto vp_end = t();

	auto v_start = t();
	for(size_type x : global::inserts) {
		v.insert(v.begin() + x, global::value);
	}
	auto v_end = t();

	std::cout << "Insert Benchmark\n";

	std::cout << "VectorPro: " <<
	          std::chrono::duration_cast<std::chrono::milliseconds>(vp_end-vp_start).count() <<
	          "ms\n";

	std::cout << "std::vector: " <<
	          std::chrono::duration_cast<std::chrono::milliseconds>(v_end-v_start).count() <<
	          "ms\n\n";
}

template<typename T>
void erase_benchmark(VectorPro<T>& vp, std::vector<T>& v) {
	auto vp_start = t();
	for(size_type x : global::erases) {
		vp.erase(x);
	}
	auto vp_end = t();

	auto v_start = t();
	for(size_type x : global::erases) {
		v.erase(v.begin() + x);
	}
	auto v_end = t();

	std::cout << "Erase Benchmark\n";

	std::cout << "VectorPro: " <<
	          std::chrono::duration_cast<std::chrono::milliseconds>(vp_end-vp_start).count() <<
	          "ms\n";

	std::cout << "std::vector: " <<
	          std::chrono::duration_cast<std::chrono::milliseconds>(v_end-v_start).count() <<
	          "ms\n\n";
}

template<typename T>
void reserve_benchmark(VectorPro<T>& vp, std::vector<T>& v) {
	auto vp_start = t();
	vp.reserve(global::reserve);
	auto vp_end = t();

	auto v_start = t();
	v.reserve(global::reserve);
	auto v_end = t();

	std::cout << "Reserve Benchmark\n";

	std::cout << "VectorPro: " <<
	          std::chrono::duration_cast<std::chrono::milliseconds>(vp_end-vp_start).count() <<
	          "ms\n";

	std::cout << "std::vector: " <<
	          std::chrono::duration_cast<std::chrono::milliseconds>(v_end-v_start).count() <<
	          "ms\n\n";
}

template<typename T>
void copy_benchmark(VectorPro<T>& vp, std::vector<T>& v) {
	auto vp_start = t();
	VectorPro<T> vp_copy(vp);
	auto vp_end = t();

	auto v_start = t();
	std::vector<T> v_copy(v);
	auto v_end = t();

	std::cout << "Copy Benchmark\n";

	std::cout << "VectorPro: " <<
	          std::chrono::duration_cast<std::chrono::milliseconds>(vp_end-vp_start).count() <<
	          "ms\n";

	std::cout << "std::vector: " <<
	          std::chrono::duration_cast<std::chrono::milliseconds>(v_end-v_start).count() <<
	          "ms\n\n";
}

template<typename T>
void move_benchmark(VectorPro<T>& vp, std::vector<T>& v) {
	VectorPro<T> vp_copy(vp);
	auto vp_start = t();
	VectorPro<T> vp_moved(std::move(vp_copy));
	auto vp_end = t();

	std::vector<T> v_copy(v);
	auto v_start = t();
	std::vector<T> v_moved(std::move(v_copy));
	auto v_end = t();

	std::cout << "Move Benchmark\n";

	std::cout << "VectorPro: " <<
	          std::chrono::duration_cast<std::chrono::milliseconds>(vp_end-vp_start).count() <<
	          "ms\n";

	std::cout << "std::vector: " <<
	          std::chrono::duration_cast<std::chrono::milliseconds>(v_end-v_start).count() <<
	          "ms\n\n";
}

template<typename T>
void iteration_benchmark(VectorPro<T>& vp, std::vector<T>& v) {
	volatile long long vp_sum = 0;
	auto vp_start = t();
	for(auto it = vp.begin(); it != vp.end(); ++it) {
		vp_sum += *it;
	}
	auto vp_end = t();

	volatile long long v_sum = 0;
	auto v_start = t();
	for(auto it = v.begin(); it != v.end(); ++it) {
		v_sum += *it;
	}
	auto v_end = t();

	if(vp_sum == v_sum) {
		std::cout << "Iteration Benchmark\n";

		std::cout << "VectorPro: " <<
		          std::chrono::duration_cast<std::chrono::milliseconds>(vp_end-vp_start).count() <<
		          "ms\n";

		std::cout << "std::vector: " <<
		          std::chrono::duration_cast<std::chrono::milliseconds>(v_end-v_start).count() <<
		          "ms\n\n";
	}
}

template<typename T>
void remove_if_benchmark(VectorPro<T>& vp, std::vector<T>& v) {
	auto vp_start = t();
	vp.remove_if(global::removeNums);
	auto vp_end = t();

	auto v_start = t();
	v.erase(std::remove_if(v.begin(), v.end(), global::removeNums), v.end());
	auto v_end = t();

	std::cout << "RemoveIf Benchmark\n";

	std::cout << "VectorPro: " <<
	          std::chrono::duration_cast<std::chrono::milliseconds>(vp_end-vp_start).count() <<
	          "ms\n";

	std::cout << "std::vector: " <<
	          std::chrono::duration_cast<std::chrono::milliseconds>(v_end-v_start).count() <<
	          "ms\n\n";
}

template<typename T>
void shrink_to_fit_benchmark(VectorPro<T>& vp, std::vector<T>& v) {
	auto vp_start = t();
	vp.shrink_to_fit();
	auto vp_end = t();

	auto v_start = t();
	v.shrink_to_fit();
	auto v_end = t();

	std::cout << "ShrinkToFit Benchmark\n";

	std::cout << "VectorPro: " <<
	          std::chrono::duration_cast<std::chrono::milliseconds>(vp_end-vp_start).count() <<
	          "ms\n";

	std::cout << "std::vector: " <<
	          std::chrono::duration_cast<std::chrono::milliseconds>(v_end-v_start).count() <<
	          "ms\n\n";
}

template<typename T>
void clear_benchmark(VectorPro<T>& vp, std::vector<T>& v) {
	auto vp_start = t();
	vp.clear();
	auto vp_end = t();

	auto v_start = t();
	v.clear();
	auto v_end = t();

	std::cout << "Clear Benchmark\n";

	std::cout << "VectorPro: " <<
	          std::chrono::duration_cast<std::chrono::milliseconds>(vp_end-vp_start).count() <<
	          "ms\n";

	std::cout << "std::vector: " <<
	          std::chrono::duration_cast<std::chrono::milliseconds>(v_end-v_start).count() <<
	          "ms\n\n";
}


int main() {
	VectorPro<int> vp;
	std::vector<int> v;

	reserve_benchmark(vp, v);

	push_back_benchmark(vp, v);

	iteration_benchmark(vp, v);

	insert_benchmark(vp, v);

	copy_benchmark(vp, v);

	erase_benchmark(vp, v);

	move_benchmark(vp, v);

	emplace_back_benchmark(vp, v);

	remove_if_benchmark(vp, v);

	shrink_to_fit_benchmark(vp, v);

	pop_back_benchmark(vp, v);

	clear_benchmark(vp, v);

	return 0;
}






