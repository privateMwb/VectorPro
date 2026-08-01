/**
 * @file Iterator.h
 * @brief Iterator implementation for VectorPro.
 *
 * Contains the iterator types used by VectorPro for traversing
 * elements stored within the container.
 */

#pragma once

// clang-format off
#include <cassert>     // assert (bounds check in operator[])
#include <compare>     // std::strong_ordering / operator<=>
#include <cstddef>     // std::ptrdiff_t
#include <iterator>    // std::contiguous_iterator_tag, std::random_access_iterator_tag
#include <type_traits> // std::remove_cv_t, std::is_same_v
// clang-format on

namespace VectorPro {

/**
 * @brief A raw-pointer-backed contiguous random-access iterator.
 * @tparam T Element type pointed to. May be `const`-qualified for a const iterator.
 * @details Satisfies `std::contiguous_iterator`. Used as VectorPro::Vector's
 * `iterator` and `const_iterator` (with `T` and `const T` respectively).
 */
template <typename T> class Iterator {
  public:
    // Types
    using iterator_concept = std::contiguous_iterator_tag;
    using iterator_category = std::random_access_iterator_tag;
    using value_type = std::remove_cv_t<T>;
    using element_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

  private:
    // Data
    pointer ptr_ = nullptr;

  public:
    /// @brief Constructs a null (singular) iterator.
    constexpr Iterator() noexcept = default;

    /**
     * @brief Constructs an iterator pointing to `ptr`.
     * @param ptr Pointer to the element the iterator should reference.
     */
    constexpr explicit Iterator(pointer ptr) noexcept : ptr_(ptr) {}

    /**
     * @brief Converting constructor from a non-const iterator to a const iterator.
     * @tparam U Source element type, must be `std::remove_cv_t<T>`.
     * @param other Iterator to convert from.
     * @details Enables implicit `iterator` -> `const_iterator` conversion.
     */
    template <typename U>
        requires std::is_same_v<U, std::remove_cv_t<T>>
    constexpr Iterator(const Iterator<U>& other) noexcept : ptr_(other.operator->()) {}

    /// @brief Dereferences the iterator.
    /// @return Reference to the pointed-to element.
    [[nodiscard]] constexpr reference operator*() const noexcept {
        return *ptr_;
    }

    /// @brief Returns the underlying pointer.
    [[nodiscard]] constexpr pointer operator->() const noexcept {
        return ptr_;
    }

    /**
     * @brief Accesses the element `n` positions away.
     * @param n Offset from the current position.
     * @return Reference to the element at the offset position.
     */
    [[nodiscard]] constexpr reference operator[](difference_type n) const noexcept {
        assert(ptr_ != nullptr);
        return ptr_[n];
    }

    /// @brief Pre-increments the iterator to point to the next element.
    /// @return Reference to `*this`.
    constexpr Iterator& operator++() noexcept {
        ++ptr_;
        return *this;
    }

    /// @brief Post-increments the iterator to point to the next element.
    /// @return A copy of the iterator's value before incrementing.
    [[nodiscard]] constexpr Iterator operator++(int) noexcept {
        auto t = *this;
        ++ptr_;
        return t;
    }

    /// @brief Pre-decrements the iterator to point to the previous element.
    /// @return Reference to `*this`.
    constexpr Iterator& operator--() noexcept {
        --ptr_;
        return *this;
    }

    /// @brief Post-decrements the iterator to point to the previous element.
    /// @return A copy of the iterator's value before decrementing.
    [[nodiscard]] constexpr Iterator operator--(int) noexcept {
        auto t = *this;
        --ptr_;
        return t;
    }

    /**
     * @brief Advances the iterator by `n` positions.
     * @param n Number of positions to advance (may be negative).
     * @return Reference to `*this`.
     */
    constexpr Iterator& operator+=(difference_type n) noexcept {
        ptr_ += n;
        return *this;
    }

    /**
     * @brief Moves the iterator back by `n` positions.
     * @param n Number of positions to move back (may be negative).
     * @return Reference to `*this`.
     */
    constexpr Iterator& operator-=(difference_type n) noexcept {
        ptr_ -= n;
        return *this;
    }

    /**
     * @brief Returns an iterator advanced by `n` positions.
     * @param i Base iterator.
     * @param n Number of positions to advance.
     * @return A new iterator, offset from `i` by `n`.
     */
    [[nodiscard]] friend constexpr Iterator operator+(Iterator i, difference_type n) noexcept {
        i += n;
        return i;
    }

    /// @copydoc operator+(Iterator, difference_type)
    [[nodiscard]] friend constexpr Iterator operator+(difference_type n, Iterator i) noexcept {
        i += n;
        return i;
    }

    /**
     * @brief Returns an iterator moved back by `n` positions.
     * @param i Base iterator.
     * @param n Number of positions to move back.
     * @return A new iterator, offset from `i` by `-n`.
     */
    [[nodiscard]] friend constexpr Iterator operator-(Iterator i, difference_type n) noexcept {
        i -= n;
        return i;
    }

    /**
     * @brief Computes the distance between two iterators.
     * @param a First iterator.
     * @param b Second iterator.
     * @return The number of elements between `b` and `a` (`a - b`).
     */
    [[nodiscard]] friend constexpr difference_type operator-(Iterator a, Iterator b) noexcept {
        return a.ptr_ - b.ptr_;
    }

    /**
     * @brief Compares two iterators for equality.
     * @param a First iterator.
     * @param b Second iterator.
     * @return `true` if both point to the same element.
     */
    [[nodiscard]] friend constexpr bool operator==(const Iterator& a, const Iterator& b) noexcept {
        return a.ptr_ == b.ptr_;
    }

    /**
     * @brief Three-way compares two iterators by position.
     * @param a First iterator.
     * @param b Second iterator.
     * @return The strong ordering between `a` and `b`.
     */
    [[nodiscard]] friend constexpr auto operator<=>(const Iterator& a, const Iterator& b) noexcept {
        return a.ptr_ <=> b.ptr_;
    }
};

} // namespace VectorPro
