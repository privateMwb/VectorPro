#pragma once

#include "Iterator.h"

#include <iterator>
#include <cstddef>
#include <type_traits>

template<typename T>
class ReverseIterator {
private:
	Iterator<T> current;

public:
	// Types
	using value_type = std::remove_const_t<T>;
	using pointer = T*;
	using reference = T&;
	using difference_type = std::ptrdiff_t;
	using iterator_category = std::random_access_iterator_tag;
	using iterator_concept = std::random_access_iterator_tag;

	// Constructor
	constexpr ReverseIterator() = default;

	constexpr explicit ReverseIterator(Iterator<T> it) noexcept : current(it) {}

	// Conversion
	template<typename U>
	requires std::is_convertible_v<U*, T*>
	constexpr ReverseIterator(const ReverseIterator<U>& other) noexcept : current(other.base()) {}

	// Base
	[[nodiscard]]
	constexpr Iterator<T> base() const noexcept {
		return current;
	}

	// Dereference
	[[nodiscard]]
	constexpr reference operator*() const noexcept {
		Iterator<T> tmp(current);
		--tmp;

		return *tmp;
	}

	[[nodiscard]]
	constexpr pointer operator->() const noexcept {
		return &(operator*());
	}

	// Increment / Decrement
	constexpr ReverseIterator& operator++() noexcept {
		--current;

		return *this;
	}

	constexpr ReverseIterator& operator--() noexcept {
		++current;

		return *this;
	}

	constexpr ReverseIterator operator++(int) noexcept {
		ReverseIterator tmp(*this);
		--current;

		return tmp;
	}

	constexpr ReverseIterator operator--(int) noexcept {
		ReverseIterator tmp(*this);
		++current;

		return tmp;
	}

	// Arithmetic
	constexpr ReverseIterator& operator+=(difference_type n) noexcept {
		current -= n;

		return *this;
	}

	constexpr ReverseIterator& operator-=(difference_type n) noexcept {
		current += n;

		return *this;
	}

	[[nodiscard]]
	constexpr ReverseIterator operator+(difference_type n) const noexcept {
		return ReverseIterator(current - n);
	}

	[[nodiscard]]
	constexpr ReverseIterator operator-(difference_type n) const noexcept {
		return ReverseIterator(current + n);
	}

	// Iterator Difference
	template<typename U>
	constexpr difference_type operator-(const ReverseIterator<U>& other) const noexcept {
		return other.base() - current;
	}

	// Indexing
	[[nodiscard]]
	constexpr reference operator[](difference_type n) const noexcept {
		return *(*this + n);
	}

	// Comparison
	template<typename U>
	[[nodiscard]]
	constexpr auto operator<=>(const ReverseIterator<U>& other) const noexcept {
		return other.base() <=> current;  // reversed intentionally
	}

	template<typename U>
	[[nodiscard]]
	constexpr bool operator==(const ReverseIterator<U>& other) const noexcept {
		return current == other.base();
	}

};

// Arithmetic (free function)
template<typename T>
[[nodiscard]]
constexpr ReverseIterator<T> operator+(std::ptrdiff_t n, const ReverseIterator<T>& it) noexcept {
    return it + n;
}