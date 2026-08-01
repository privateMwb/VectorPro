/**
 * @file            Vector.hpp
 *
 * @date            2026-07-14
 *
 * @version         1.0.0
 *
 * @copyright       Copyright (c) 2026 MWB
 *                  All rights reserved.
 *                  https://github.com/privateMwb/VectorPro
 *
 * @attention       This source is released under the MIT license
 *                  SPDX-License-Identifier: MIT
 *                  <http://opensource.org/licenses/MIT>
 */

#pragma once

#include "Iterator.h"

// clang-format off
#include <algorithm>        // std::copy, std::move, std::equal, std::lexicographical_compare_three_way
#include <compare>          // std::strong_ordering / operator<=>
#include <concepts>         // std::invocable, std::input_iterator (Listener concept, iterator-pair ctor)
#include <cstddef>          // std::size_t, std::ptrdiff_t
#include <cstring>          // std::memcpy (trivial-type fast path in insert/erase/push_back)
#include <functional>       // std::function (ListenerFn storage)
#include <initializer_list> // std::initializer_list ctor/assign
#include <iterator>         // std::reverse_iterator, iterator tag dispatch
#include <limits>           // std::numeric_limits (overflow checks in growCapacity)
#include <memory>           // std::allocator, std::allocator_traits
#include <optional>         // (reserved: nullable return values)
#include <span>             // std::span (as_span())
#include <stdexcept>        // std::out_of_range, std::length_error
#include <type_traits>      // std::is_trivially_copyable, etc.
#include <utility>          // std::move, std::forward, std::exchange
// clang-format on

// A std::vector-like dynamic array with configurable growth policy,
// custom allocator support, and optional modification-event notifications.
// Requires C++20 (concepts, <=>, std::span, [[no_unique_address]]).

namespace VectorPro {

/**
 * @brief Constrains listener callbacks usable with Vector:subscribe().
 * @tparam F The callable type being constrained.
 * @tparam VectorType The Vector specialization the listener will observe.
 * @details Requires the callable to accept the vector instance and the
 * associated event information, i.e. `F(const VectorType&, VectorType::EventData)`.
 */
template <typename F, typename VectorType>
concept Listener = std::invocable<F, const VectorType&, typename VectorType::EventData>;

namespace detail {

/**
 * @brief Listener bookkeeping storage, conditionally present.
 * @tparam ListenerFn Callback type stored per listener.
 * @tparam Enabled Whether event support is turned on for the owning Vector.
 * @details When `Enabled` is `false`, this is an empty type. Combined with
 * `[[no_unique_address]]` on the Vector member that holds it, this means a
 * Vector with events disabled pays zero storage cost for listener
 * bookkeeping — the fields simply don't exist in that instantiation, rather
 * than existing but always being zero.
 */
template <typename ListenerFn, bool Enabled> struct ListenerStore {
    ListenerFn* listeners_ = nullptr;
    std::size_t lsize_ = 0;
    std::size_t lcap_ = 0;
};

/// @brief Empty specialization used when event support is disabled.
template <typename ListenerFn> struct ListenerStore<ListenerFn, false> {};

} // namespace detail

/**
 * @brief A dynamically resizable contiguous array.
 * @tparam T Element type. Must satisfy `std::destructible`.
 * @tparam Allocator Allocator type used for storage. Defaults to `std::allocator<T>`.
 * @tparam GrowthNum Numerator of the capacity growth factor (`GrowthNum / GrowthDen`).
 * @tparam GrowthDen Denominator of the capacity growth factor.
 * @tparam EnableEvents Whether modification-event notifications (subscribe()/
 * unsubscribe()) are compiled in. Defaults to `false`; when disabled, no
 * listener storage exists and no notification code is emitted anywhere in
 * the class — see VectorPro::ObservableVector for the enabled alias.
 * @details Provides std::vector-like semantics with configurable growth,
 * custom allocator support, iterator support, and modification event
 * notifications. `GrowthNum` must be greater than `GrowthDen` so that
 * capacity strictly increases on each reallocation.
 */
template <typename T, typename Allocator = std::allocator<T>, std::size_t GrowthNum = 2,
          std::size_t GrowthDen = 1, bool EnableEvents = false>
    requires std::destructible<T>
class Vector {
  public:
    // Standard container type aliases
    using allocator_type = Allocator;
    using value_type = T;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    // Validates the compile-time growth policy.
    static_assert(GrowthNum != 0, "VectorPro::Vector: GrowthNum must not be zero");
    static_assert(GrowthDen != 0, "VectorPro::Vector: GrowthDen must not be zero");
    static_assert(GrowthNum > GrowthDen,
                  "VectorPro::Vector: GrowthNum must be greater than GrowthDen to ensure growth");

    /// @brief Event types emitted when the vector is modified.
    enum class EventType {
        PUSHBACK,    ///< Emitted by push_back().
        EMPLACEBACK, ///< Emitted by emplace_back().
        POPBACK,     ///< Emitted by pop_back().
        INSERT,      ///< Emitted by insert() and emplace().
        REMOVE,      ///< Emitted by remove_if().
        ERASE,       ///< Emitted by erase().
        CLEAR,       ///< Emitted by clear().
        SHRINK,      ///< Emitted by shrink_to_fit().
        RESERVE      ///< Emitted by reserve().
    };

    /**
     * @brief Describes a vector modification event.
     * @details Passed to subscribed listeners after a modifying operation.
     * `index` is the affected position when applicable (e.g. insertion or
     * erase index); its meaning is operation-dependent and may be `0` for
     * events like CLEAR that don't target a single index.
     */
    struct EventData {
        EventType type;      ///< The kind of modification that occurred.
        std::size_t index;   ///< The index associated with the event, if any.
        std::size_t oldSize; ///< Vector size before the operation.
        std::size_t newSize; ///< Vector size after the operation.
    };

    /// @brief Callback type invoked with the vector and the event that occurred.
    using ListenerFn = std::function<void(const Vector&, EventData)>;
    /// @brief Opaque handle returned by subscribe(), used to unsubscribe() later.
    using ListenerHandle = std::size_t;

    // Iterator type aliases.
    using iterator = VectorPro::Iterator<T>;
    using const_iterator = VectorPro::Iterator<const T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  private:
    // Core storage state.
    [[no_unique_address]] Allocator alloc_;
    pointer data_ = nullptr;
    std::size_t vsize_ = 0;
    std::size_t vcap_ = 0;

    // Registered event listeners. Empty (zero size) when EnableEvents is false.
    [[no_unique_address]] detail::ListenerStore<ListenerFn, EnableEvents> listenerStore_;

    // Default capacity used for the first allocation.
    static constexpr std::size_t INITIAL_CAP = 8;

  public:
    /**
     * @brief Constructs a vector with `count` copies of `value`.
     * @param count Number of elements to construct.
     * @param value Value used to initialize each element. Defaults to `T{}`.
     */
    explicit Vector(std::size_t count, const T& value = {});

    /**
     * @brief Constructs a vector from an initializer list.
     * @param init Elements to copy into the vector, in order.
     */
    Vector(std::initializer_list<T> init);

    /**
     * @brief Constructs a vector from an iterator range via repeated push_back().
     * @tparam It Input iterator type.
     * @param first Iterator to the first element to copy.
     * @param last Iterator one past the last element to copy.
     */
    template <std::input_iterator It> Vector(It first, It last);

    /**
     * @brief Constructs an empty vector with no allocated storage.
     * @param alloc Allocator instance to use. Defaults to `Allocator{}`.
     */
    explicit Vector(const Allocator& alloc = Allocator{});

    /// @brief Destroys all elements and releases storage.
    ~Vector() noexcept;

    /**
     * @brief Copy-constructs a vector, deep-copying `other`'s elements.
     * @param other Vector to copy from.
     */
    Vector(const Vector& other);

    /**
     * @brief Copy-assigns from `other`, replacing this vector's contents.
     * @param other Vector to copy from.
     * @return Reference to `*this`.
     * @details Reuses existing capacity when sufficient; otherwise falls back
     * to copy-and-swap for the strong exception guarantee.
     */
    Vector& operator=(const Vector& other);

    /**
     * @brief Move-constructs a vector, taking ownership of `other`'s storage.
     * @param other Vector to move from, left empty afterward.
     */
    Vector(Vector&& other) noexcept;

    /**
     * @brief Move-assigns from `other`, replacing this vector's contents.
     * @param other Vector to move from, left empty afterward.
     * @return Reference to `*this`.
     */
    Vector& operator=(Vector&& other) noexcept;

    /**
     * @brief Appends a copy of `value` to the end, growing capacity if needed.
     * @param value Value to append. Safe to pass a reference into this vector.
     */
    void push_back(const T& value);

    /**
     * @brief Appends `value` to the end by move, growing capacity if needed.
     * @param value Value to move-append. Safe to pass a reference into this vector.
     */
    void push_back(T&& value);

    /**
     * @brief Constructs a new element in place at the end of the vector.
     * @tparam Args Constructor argument types for `T`.
     * @param args Arguments forwarded to `T`'s constructor.
     */
    template <typename... Args>
        requires std::constructible_from<T, Args...>
    void emplace_back(Args&&... args);

    /**
     * @brief Inserts a copy of `value` before `pos`.
     * @param pos Position to insert before.
     * @param value Value to insert. Safe to pass a reference into this vector.
     * @return Iterator pointing to the newly inserted element.
     */
    [[nodiscard]] iterator insert(const_iterator pos, const T& value);

    /**
     * @brief Inserts `value` before `pos` by move.
     * @param pos Position to insert before.
     * @param value Value to move-insert. Safe to pass a reference into this vector.
     * @return Iterator pointing to the newly inserted element.
     */
    [[nodiscard]] iterator insert(const_iterator pos, T&& value);

    /**
     * @brief Inserts a copy of each element in `[first, last)` before `pos`.
     * @tparam It Input iterator type.
     * @param pos Position to insert before.
     * @param first Iterator to the first element to insert.
     * @param last Iterator one past the last element to insert.
     * @return Iterator pointing to the first newly inserted element.
     */
    template <std::input_iterator It> iterator insert(const_iterator pos, It first, It last);

    /**
     * @brief Constructs a new element in place before `pos`.
     * @tparam Args Constructor argument types for `T`.
     * @param pos Position to insert before.
     * @param args Arguments forwarded to `T`'s constructor.
     * @return Iterator pointing to the newly constructed element.
     */
    template <typename... Args>
        requires std::constructible_from<T, Args...>
    [[nodiscard]] iterator emplace(const_iterator pos, Args&&... args);

    /**
     * @brief Removes all elements for which `pred` returns `true`.
     * @tparam Predicate Unary predicate type invocable on `const T&`.
     * @param pred Predicate used to test each element.
     * @return The number of elements removed.
     * @details Relative order of retained elements is preserved.
     */
    template <typename Predicate>
        requires std::predicate<Predicate, const T&>
    [[nodiscard]] std::size_t remove_if(Predicate pred);

    /// @brief Removes the last element. No-op if the vector is empty.
    void pop_back();

    /**
     * @brief Erases the element at `pos`.
     * @param pos Position of the element to erase.
     * @return Iterator to the element that followed the erased one.
     */
    [[nodiscard]] iterator erase(const_iterator pos);

    /**
     * @brief Erases the elements in `[first, last)`.
     * @param first Iterator to the first element to erase.
     * @param last Iterator one past the last element to erase.
     * @return Iterator to the element that followed the erased range.
     */
    [[nodiscard]] iterator erase(const_iterator first, const_iterator last);

    /// @brief Destroys all elements, leaving the vector empty. Capacity is unchanged.
    void clear() noexcept;

    /**
     * @brief Ensures capacity is at least `newCap`, reallocating if needed.
     * @param newCap Minimum capacity to reserve. No-op if not greater than current capacity.
     */
    void reserve(std::size_t newCap);

    /// @brief Reduces capacity to match the current size, reallocating if needed.
    void shrink_to_fit();

    /**
     * @brief Registers a listener to be notified of modification events.
     * @tparam F Callable type satisfying the Listener concept.
     * @param listeners Callable to invoke with `(*this, EventData)` on each modification.
     * @return A handle that can be passed to unsubscribe() to remove the listener.
     * @details Only callable when `EnableEvents` is `true`; a Vector with events
     * disabled has no listener storage, so subscribing is a compile error rather
     * than a silent no-op.
     */
    template <typename F>
        requires EnableEvents &&
                 Listener<F, Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>>
    [[nodiscard]] ListenerHandle subscribe(F&& listeners);

    /**
     * @brief Removes a previously registered listener.
     * @param handle Handle returned by subscribe(). No-op if out of range.
     * @details Only callable when `EnableEvents` is `true`.
     */
    void unsubscribe(ListenerHandle handle)
        requires EnableEvents;

    /**
     * @brief Compares two vectors for equality.
     * @param other Vector to compare against.
     * @return `true` if both vectors have equal size and equal elements in order.
     */
    [[nodiscard]] bool operator==(const Vector& other) const
        noexcept(noexcept(std::declval<const T&>() == std::declval<const T&>()));

    /**
     * @brief Lexicographically compares two vectors.
     * @param other Vector to compare against.
     * @return The three-way comparison result of the element sequences.
     */
    [[nodiscard]] auto operator<=>(const Vector& other) const
        noexcept(noexcept(std::declval<const T&>() <=> std::declval<const T&>()));

    /// @brief Returns a mutable span over the vector's current elements.
    [[nodiscard]] std::span<T> as_span() noexcept;
    /// @brief Returns a read-only span over the vector's current elements.
    [[nodiscard]] std::span<const T> as_span() const noexcept;

    /// @brief Returns a pointer to the underlying contiguous storage.
    [[nodiscard]] pointer data_ptr() noexcept;
    /// @brief Returns a const pointer to the underlying contiguous storage.
    [[nodiscard]] const_pointer data_ptr() const noexcept;

    /**
     * @brief Returns a reference to the element at `index`, with bounds checking.
     * @param index Zero-based index of the element to access.
     * @return Reference to the element at `index`.
     * @throws std::out_of_range if `index >= size()`.
     */
    [[nodiscard]] reference at(std::size_t index);

    /**
     * @brief Returns a const reference to the element at `index`, with bounds checking.
     * @param index Zero-based index of the element to access.
     * @return Const reference to the element at `index`.
     * @throws std::out_of_range if `index >= size()`.
     */
    [[nodiscard]] const_reference at(std::size_t index) const;

    /// @brief Returns a reference to the first element. Undefined behavior if empty.
    [[nodiscard]] reference front();
    /// @brief Returns a const reference to the first element. Undefined behavior if empty.
    [[nodiscard]] const_reference front() const;

    /// @brief Returns a reference to the last element. Undefined behavior if empty.
    [[nodiscard]] reference back();
    /// @brief Returns a const reference to the last element. Undefined behavior if empty.
    [[nodiscard]] const_reference back() const;

    /**
     * @brief Returns a reference to the element at `index`, without bounds checking.
     * @param index Zero-based index of the element to access.
     * @return Reference to the element at `index`.
     */
    [[nodiscard]] reference operator[](std::size_t index) noexcept;

    /**
     * @brief Returns a const reference to the element at `index`, without bounds checking.
     * @param index Zero-based index of the element to access.
     * @return Const reference to the element at `index`.
     */
    [[nodiscard]] const_reference operator[](std::size_t index) const noexcept;

    /**
     * @brief Checks whether `value` is present in the vector.
     * @param value Value to search for.
     * @return `true` if an equal element is found.
     */
    [[nodiscard]] bool contains(const T& value) const
        noexcept(noexcept(std::declval<const T&>() == std::declval<const T&>()));

    /**
     * @brief Finds the first element equal to `value`.
     * @param value Value to search for.
     * @return Iterator to the found element, or end() if not found.
     */
    [[nodiscard]] iterator find(const T& value) noexcept(noexcept(std::declval<const T&>() ==
                                                                  std::declval<const T&>()));

    /**
     * @brief Finds the first element equal to `value`.
     * @param value Value to search for.
     * @return Const iterator to the found element, or end() if not found.
     */
    [[nodiscard]] const_iterator find(const T& value) const
        noexcept(noexcept(std::declval<const T&>() == std::declval<const T&>()));

    /**
     * @brief Exchanges the contents of two vectors, including allocators.
     * @param other Vector to swap with.
     */
    void swap(Vector& other) noexcept;

    /// @brief Returns whether the vector has no elements.
    [[nodiscard]] bool empty() const noexcept;
    /// @brief Returns the number of elements currently stored.
    [[nodiscard]] std::size_t size() const noexcept;
    /// @brief Returns the number of elements the vector can hold without reallocating.
    [[nodiscard]] std::size_t capacity() const noexcept;

    /// @brief Returns an iterator to the first element.
    [[nodiscard]] iterator begin() noexcept;
    /// @brief Returns an iterator one past the last element.
    [[nodiscard]] iterator end() noexcept;
    /// @brief Returns a const iterator to the first element.
    [[nodiscard]] const_iterator begin() const noexcept;
    /// @brief Returns a const iterator one past the last element.
    [[nodiscard]] const_iterator end() const noexcept;
    /// @brief Returns a const iterator to the first element.
    [[nodiscard]] const_iterator cbegin() const noexcept;
    /// @brief Returns a const iterator one past the last element.
    [[nodiscard]] const_iterator cend() const noexcept;
    /// @brief Returns a reverse iterator to the last element.
    [[nodiscard]] reverse_iterator rbegin() noexcept;
    /// @brief Returns a reverse iterator one before the first element.
    [[nodiscard]] reverse_iterator rend() noexcept;
    /// @brief Returns a const reverse iterator to the last element.
    [[nodiscard]] const_reverse_iterator rbegin() const noexcept;
    /// @brief Returns a const reverse iterator one before the first element.
    [[nodiscard]] const_reverse_iterator rend() const noexcept;
    /// @brief Returns a const reverse iterator to the last element.
    [[nodiscard]] const_reverse_iterator crbegin() const noexcept;
    /// @brief Returns a const reverse iterator one before the first element.
    [[nodiscard]] const_reverse_iterator crend() const noexcept;

  private:
    /// @brief Destroys all elements and deallocates storage, resetting size and capacity to 0.
    void release() noexcept;

    /**
     * @brief Allocates a buffer sized `other.vcap_` using this vector's
     * current `alloc_`, and copy-constructs `other`'s elements into it.
     * Shared by the copy constructor and by copy assignment's copy-and-swap
     * fallback, so both paths honor whichever allocator was already chosen
     * for `alloc_` instead of duplicating the allocate/construct/rollback
     * logic.
     * @param other Vector whose elements are copy-constructed into the new buffer.
     */
    void copyBufferFrom(const Vector& other);

    /**
     * @brief Computes the next capacity according to the configured growth policy.
     * @return `INITIAL_CAP` if currently empty of capacity; otherwise
     * `capacity() * GrowthNum / GrowthDen`, saturating at `SIZE_MAX` on overflow.
     */
    [[nodiscard]] std::size_t growCapacity() const noexcept;

    /**
     * @brief Reallocates storage to `newCap`, preserving existing elements.
     * @param newCap New capacity to allocate. Must be at least the current size.
     * @throws Whatever the allocator or T's move/copy constructor may throw;
     * provides the strong exception guarantee (original storage is left intact on failure).
     */
    void reallocate(std::size_t newCap);

    /**
     * @brief Cold path for push_back(const T&): handles the aliasing snapshot,
     * reallocation, and construction when `vsize_ == vcap_`. Kept out of
     * push_back's body so the common (no-growth) path stays small enough to
     * reliably inline at call sites; this rarely-taken path is not.
     * @param value Value to append. Safe to pass a reference into this vector.
     */
    void grow_and_push_back(const T& value);

    /**
     * @brief Cold path for push_back(T&&). See grow_and_push_back(const T&).
     * @param value Value to move-append. Safe to pass a reference into this vector.
     */
    void grow_and_push_back(T&& value);

    /**
     * @brief Invokes every registered listener with the given event data.
     * @param data Event describing the modification that just occurred.
     */
    void notify(EventData data);
};

/**
 * @brief Exchanges the contents of two vectors.
 * @param a First vector.
 * @param b Second vector.
 */
template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
void swap(Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>& a,
          Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>& b) noexcept;

/**
 * @brief Convenience alias for a Vector with event notifications enabled.
 * @details Equivalent to `Vector<T, Allocator, GrowthNum, GrowthDen, true>`.
 * Use this wherever code needs subscribe()/unsubscribe(); a plain `Vector<T>`
 * has events disabled and won't compile against those calls.
 */
template <typename T, typename Allocator = std::allocator<T>, std::size_t GrowthNum = 2,
          std::size_t GrowthDen = 1>
using ObservableVector = Vector<T, Allocator, GrowthNum, GrowthDen, true>;

} // namespace VectorPro

/// @brief Umbrella alias so this library's types are reachable as
/// `rain::Vector`, alongside every other project library, while its true
/// namespace (and all internal diagnostics/static_asserts) remains
/// `VectorPro`. Reopens `rain` rather than aliasing it, since multiple
/// libraries each contribute their own names into the same `rain`
/// namespace -- an alias (`namespace rain = VectorPro;`) can only ever
/// bind to one target and collides the moment a second library declares
/// its own `rain` alias to something else. Declared here only (VectorPro's
/// main header); internal headers like Iterator.h do not redeclare this.
namespace rain {
using namespace VectorPro;
}

#include "Vector.tpp"
