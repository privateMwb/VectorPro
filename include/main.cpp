#include <iostream>
#include "VectorPro.h"

int main() {
	VectorPro<int> vec;

	// Subscribe listener
	vec.subscribe([](const VectorPro<int>& vec, EventType type) {
		switch(type) {
		case EventType::PushBack:
			std::cout << "[Notify] PushBack\n";
			break;

		case EventType::PopBack:
			std::cout << "[Notify] PopBack\n";
			break;

		case EventType::Insert:
			std::cout << "[Notify] Insert\n";
			break;

		case EventType::Erase:
			std::cout << "[Notify] Erase\n";
			break;

		case EventType::Remove:
			std::cout << "[Notify] Remove\n";
			break;

		case EventType::Clear:
			std::cout << "[Notify] Clear\n";
			break;

		case EventType::Shrink:
			std::cout << "[Notify] Shrink\n";
			break;

		case EventType::Reserve:
			std::cout << "[Notify] Reserve\n";
			break;
		}

		std::cout << "Current size: "
		          << vec.size() << "\n";
	});

	// push_back
	vec.push_back(10);
	vec.push_back(20);
	vec.push_back(30);

	std::cout << "\nAfter push_back:\n";
	for(auto it = vec.begin(); it != vec.end(); ++it) {
		std::cout << *it << ' ';
	}
	std::cout << "\n";

	// insert
	vec.insert(1, 99);

	std::cout << "\nAfter insert:\n";
	for(auto it = vec.begin(); it != vec.end(); ++it) {
		std::cout << *it << ' ';
	}
	std::cout << "\n";

	// erase
	vec.erase(2);

	std::cout << "\nAfter erase:\n";
	for(auto it = vec.begin(); it != vec.end(); ++it) {
		std::cout << *it << ' ';
	}
	std::cout << "\n";

	// remove_if
	vec.remove_if([](int value) {
		return value > 15;
	});

	std::cout << "\nAfter remove_if (>15):\n";
	for(auto it = vec.begin(); it != vec.end(); ++it) {
		std::cout << *it << ' ';
	}
	std::cout << "\n";

	// reserve
	vec.reserve(20);

	std::cout << "\nCapacity after reserve(20): "
	          << vec.capacity() << "\n";

	// shrink_to_fit
	vec.shrink_to_fit();

	std::cout << "Capacity after shrink_to_fit(): "
	          << vec.capacity() << "\n";

	// front/back
	if(!vec.empty()) {
		std::cout << "\nFront: " << vec.front() << "\n";
		std::cout << "Back : " << vec.back() << "\n";
	}

	// copy constructor
	VectorPro<int> copy(vec);

	std::cout << "\nCopied vector:\n";
	for(auto it = copy.begin(); it != copy.end(); ++it) {
		std::cout << *it << ' ';
	}
	std::cout << "\n";

	// move constructor
	VectorPro<int> moved(std::move(copy));

	std::cout << "\nMoved vector:\n";
	for(auto it = moved.begin(); it != moved.end(); ++it) {
		std::cout << *it << ' ';
	}
	std::cout << "\n";

	// pop_back
	moved.pop_back();

	std::cout << "\nAfter pop_back:\n";
	for(auto it = moved.begin(); it != moved.end(); ++it) {
		std::cout << *it << ' ';
	}
	std::cout << "\n";

	// clear
	moved.clear();

	std::cout << "\nAfter clear:\n";
	std::cout << "Size  : " << moved.size() << "\n";
	std::cout << "Empty : "
	          << std::boolalpha
	          << moved.empty() << "\n";

	// at() exception test
	try {
		std::cout << moved.at(0) << "\n";
	} catch(const std::exception& e) {
		std::cout << "\nException caught: "
		          << e.what() << "\n";
	}

	return 0;
}