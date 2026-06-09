// VectorPro Unit Test Suite
// Tests correctness of core dynamic array operations:
//
// - constructor (initial state)
// - push_back (append behavior)
// - pop_back (end removal behavior)
// - insert (middle insertion + shifting)
// - erase (index removal + shifting)
// - emplace_back (in-place construction)
// - remove_if (predicate-based filtering)
// - clear (reset container)
// - reserve (capacity growth)
// - shrink_to_fit (capacity reduction)
// - bounds checking (at() exception safety)
// - initializer_list construction
//
// These tests validate correctness of VectorPro against expected std::vector-like behavior.

#include <iostream>
#include <cstddef>
#include <cassert>

#include "VectorPro.h"

// Constructor Test
// verifies empty vector state on creation
void constructor() {
	VectorPro<int> vp;

	assert(vp.size() == 0);
	assert(vp.capacity() == 0);

	std::cout << "\n[PASS] Constructor Test\n";
}

// Push Back Test
// verifies append correctness and ordering
void push_back() {
	VectorPro<int> v;

	v.push_back(10);
	v.push_back(20);

	assert(v.size() == 2);
	assert(v[0] == 10);
	assert(v[1] == 20);

	std::cout << "\n[PASS] Push Back Test\n";
}

// Pop Back Test
// verifies removal from end (logical pop_back behavior)
void pop_back() {
	VectorPro<int> v{1, 2, 3};

	v.erase(1);

	assert(v.size() == 2);
	assert(v[0] == 1);
	assert(v[1] == 3);

	std::cout << "\n[PASS] Pop Back Test\n";
}

// Insert Test
// verifies shifting and correct placement of elements
void insert() {
	VectorPro<int> v;

	v.push_back(1);
	v.push_back(3);

	v.insert(1, 2);

	assert(v[0] == 1);
	assert(v[1] == 2);
	assert(v[2] == 3);

	std::cout << "\n[PASS] Insert Test\n";
}

// Erase Test
// verifies removal and shifting of elements
void erase() {
	VectorPro<int> v{1, 2, 3};

	v.erase(1);

	assert(v.size() == 2);
	assert(v[0] == 1);
	assert(v[1] == 3);

	std::cout << "\n[PASS] Erase Test\n";
}

// Emplace Back Test
// verifies in-place construction and correct ordering
void emplace_back() {
	VectorPro<std::string> v;

	v.emplace_back("A");
	v.emplace_back("B");
	v.emplace_back("C");

	assert(v.size() == 3);
	assert(v[0] == "A");
	assert(v[1] == "B");
	assert(v[2] == "C");

	std::cout << "\n[PASS] Emplace Back Test\n";
}

// Remove If Test
// verifies predicate-based removal and correct compaction
void remove_if() {
	VectorPro<int> v{1, 2, 3, 4, 5, 6};

	v.remove_if([](int x) {
		return x % 2 == 0; // remove evens
	});

	assert(v.size() == 3);
	assert(v[0] == 1);
	assert(v[1] == 3);
	assert(v[2] == 5);

	std::cout << "\n[PASS] Remove If Test\n";
}

// Clear Test
// verifies full reset of container state
void clear() {
	VectorPro<int> v{1, 2, 3};

	v.clear();

	assert(v.size() == 0);
	assert(v.empty());

	std::cout << "\n[PASS] Clear Test\n";
}

// Reserve Test
// verifies capacity growth without affecting size
void reserve() {
	VectorPro<int> v;

	v.reserve(100);

	assert(v.capacity() >= 100);
	assert(v.size() == 0);

	std::cout << "\n[PASS] Reserve Test\n";
}

// Shrink To Fit Test
// verifies that capacity is reduced to match the current size
void shrink_to_fit() {
    VectorPro<int> v;

    v.reserve(100);

    v.push_back(40);
    v.push_back(0);
    v.push_back(592);
    v.push_back(100);
    v.push_back(33);

    v.shrink_to_fit();

    assert(v.size() == 5);
    assert(v.capacity() == 5);

    std::cout << "\n[PASS] Shrink To Fit Test\n";
}

// Bounds Test
// verifies at() throws on invalid access
void bounds() {
	VectorPro<int> v;
	v.push_back(1);

	bool thrown = false;

	try {
		(void)v.at(2);
	} catch (const std::out_of_range&) {
		thrown = true;
	}

	assert(thrown);
        std::cout << "\n[PASS] Bounds Test\n";
}

// Initializer List Test
// verifies correct construction from initializer list
void initializer_list() {
	VectorPro<int> v{1, 2, 3};

	assert(v.size() == 3);
	assert(v[0] == 1);
	assert(v[1] == 2);
	assert(v[2] == 3);

	std::cout << "\n[PASS] Initializer List Test\n";
}

int main() {
	constructor();
	push_back();
	pop_back();
	insert();
	erase();
	emplace_back();
	remove_if();
	clear();
	reserve();
	shrink_to_fit();
	bounds();
	initializer_list();

	return 0;
}



