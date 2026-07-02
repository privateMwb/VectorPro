#pragma once

#include "Iterator.h"

#include <iterator>
#include <type_traits>
#include <initializer_list>
#include <algorithm>
#include <cstddef>
#include <utility>
#include <cstring>
#include <span>
#include <concepts>
#include <compare>
#include <memory>
#include <functional>
#include <stdexcept>
#include <optional>

namespace VectorPro {

// Constrains listener callbacks.
// Requires the callable to accept the vector instance and
// the associated event information.
template<typename F, typename VectorType>
concept Listener = std::invocable<F, const VectorType&, typename VectorType::EventData>;

// A dynamically resizable contiguous array.
// Provides std::vector-like semantics with configurable growth,
// custom allocator support, iterator support, and modification
// event notifications.
template<typename T,
         typename Allocator = std::allocator<T>,
         std::size_t GrowthNum = 2,
         std::size_t GrowthDen = 1>
requires std::destructible<T>
class Vector {
public:

	// Standard container type aliases
	using allocator_type  = Allocator;
	using value_type      = T;
	using pointer         = typename std::allocator_traits<Allocator>::pointer;
	using const_pointer   = typename std::allocator_traits<Allocator>::const_pointer;
	using reference       = T&;
	using const_reference = const T&;
	using size_type       = std::size_t;
	using difference_type = std::ptrdiff_t;

    // Validates the compile-time growth policy.
    static_assert(GrowthNum != 0,         "VectorPro::Vector: GrowthNum must not be zero");
    static_assert(GrowthDen != 0,         "VectorPro::Vector: GrowthDen must not be zero");
    static_assert(GrowthNum > GrowthDen,  "VectorPro::Vector: GrowthNum must be greater than GrowthDen to ensure growth");

	// Event types emitted when the vector is modified.
	enum class EventType {
		PUSHBACK,
		EMPLACEBACK,
		POPBACK,
		INSERT,
		REMOVE,
		ERASE,
		CLEAR,
		SHRINK,
		RESERVE
	};

        // Describes a vector modification event.
	struct EventData {
		EventType    type;
		std::size_t  index;
		std::size_t  oldSize;
		std::size_t  newSize;
	};

        // Listener callback and subscription handle types.
	using ListenerFn = std::function<void(const Vector&, EventData)>;
	using ListenerHandle = std::size_t;

	// Iterator type aliases.
	using iterator                = VectorPro::Iterator<T>;
	using const_iterator          = VectorPro::Iterator<const T>;
	using reverse_iterator        = std::reverse_iterator<iterator>;
	using const_reverse_iterator  = std::reverse_iterator<const_iterator>;

private:

	// Core storage state.
	[[no_unique_address]] Allocator  alloc_;
	pointer      data_   = nullptr;
	std::size_t  vsize_     = 0;
	std::size_t  vcap_      = 0;

        // Registered event listeners.
	ListenerFn*  listeners_  = nullptr;
	std::size_t  lsize_     = 0;
	std::size_t  lcap_      = 0;

        // Default capacity used for the first allocation.
	static constexpr std::size_t INITIAL_CAP = 8;

public:

	// Constructors and destructor.
	explicit Vector(std::size_t count, const T& value = {});
	Vector(std::initializer_list<T> init);

	template<std::input_iterator It>
	Vector(It first, It last);

    explicit Vector(const Allocator& alloc = Allocator{});

	~Vector() noexcept;

	Vector(const Vector& other);
	Vector& operator=(const Vector& other);

	Vector(Vector&& other) noexcept;
	Vector& operator=(Vector&& other) noexcept;

	// Element insertion and removal.
	void push_back(const T& value);
	void push_back(T&& value);

	template<typename... Args>
	requires std::constructible_from<T, Args...>
	void emplace_back(Args&&... args);

	[[nodiscard]] iterator insert(const_iterator pos, const T& value);
	[[nodiscard]] iterator insert(const_iterator pos, T&& value);

	template<std::input_iterator It>
	iterator insert(const_iterator pos, It first, It last);

	template<typename... Args>
	requires std::constructible_from<T, Args...>
	[[nodiscard]] iterator emplace(const_iterator pos, Args&&... args);

	template<typename Predicate>
	requires std::predicate<Predicate, const T&>
	[[nodiscard]] std::size_t remove_if(Predicate pred);

	void pop_back();

	[[nodiscard]] iterator erase(const_iterator pos);
	[[nodiscard]] iterator erase(const_iterator first, const_iterator last);

	void clear() noexcept;

	void reserve(std::size_t newCap);
	void shrink_to_fit();

	// Event subscription management.
	template<typename F>
	requires Listener<F, Vector>
	[[nodiscard]] ListenerHandle subscribe(F&& listener);

	void unsubscribe(ListenerHandle handle);

	// Comparison operators.
	[[nodiscard]] bool operator==(const Vector& other) const
		noexcept(noexcept(std::declval<const T&>() == std::declval<const T&>()));

	[[nodiscard]] auto operator<=>(const Vector& other) const
		noexcept(noexcept(std::declval<const T&>() <=> std::declval<const T&>()));

	// Span access.
	[[nodiscard]] std::span<T>        as_span()       noexcept;
	[[nodiscard]] std::span<const T>  as_span() const noexcept;

	// Element access.
	[[nodiscard]] pointer          data_ptr()       noexcept;
	[[nodiscard]] const_pointer    data_ptr() const noexcept;

	[[nodiscard]] reference        at(std::size_t index);
	[[nodiscard]] const_reference  at(std::size_t index) const;

	[[nodiscard]] reference        front();
	[[nodiscard]] const_reference  front() const;

	[[nodiscard]] reference        back();
	[[nodiscard]] const_reference  back() const;

	[[nodiscard]] reference        operator[](std::size_t index)       noexcept;
	[[nodiscard]] const_reference  operator[](std::size_t index) const noexcept;

	// Search utilities.
	[[nodiscard]] bool contains(const T& value) const
		noexcept(noexcept(std::declval<const T&>() == std::declval<const T&>()));

	[[nodiscard]] iterator find(const T& value)
		noexcept(noexcept(std::declval<const T&>() == std::declval<const T&>()));

	[[nodiscard]] const_iterator find(const T& value) const
		noexcept(noexcept(std::declval<const T&>() == std::declval<const T&>()));

	// Exchanges the contents of two vectors.
	void swap(Vector& other) noexcept;

	// Capacity queries.
	[[nodiscard]] bool         empty()     const noexcept;
	[[nodiscard]] std::size_t  size()      const noexcept;
	[[nodiscard]] std::size_t  capacity()  const noexcept;

	// Iterators access.
	[[nodiscard]] iterator                begin()          noexcept;
	[[nodiscard]] iterator                end()            noexcept;
	[[nodiscard]] const_iterator          begin()    const noexcept;
	[[nodiscard]] const_iterator          end()      const noexcept;
	[[nodiscard]] const_iterator          cbegin()   const noexcept;
	[[nodiscard]] const_iterator          cend()     const noexcept;
	[[nodiscard]] reverse_iterator        rbegin()         noexcept;
	[[nodiscard]] reverse_iterator        rend()           noexcept;
	[[nodiscard]] const_reverse_iterator  rbegin()   const noexcept;
	[[nodiscard]] const_reverse_iterator  rend()     const noexcept;
	[[nodiscard]] const_reverse_iterator  crbegin()  const noexcept;
	[[nodiscard]] const_reverse_iterator  crend()    const noexcept;

private:

	// Releases all allocated storage and destroys contained elements.
	void release() noexcept;

        // Computes the next capacity according to the configured growth policy.
	[[nodiscard]] std::size_t growCapacity() const noexcept;

        // Reallocates storage while preserving existing elements.
	void reallocate(std::size_t newCap);

        // Notifies all registered listeners about a modification event.
	void notify(EventData data);

};

// Non-member swap overload.
template<typename T,
typename Allocator,
std::size_t GrowthNum,
std::size_t GrowthDen>
void swap(
    Vector<T, Allocator, GrowthNum, GrowthDen>& a,
    Vector<T, Allocator, GrowthNum, GrowthDen>& b) noexcept;

} // namespace VectorPro

#include "Vector.tpp"
