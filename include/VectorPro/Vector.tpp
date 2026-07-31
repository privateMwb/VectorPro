/**
 * @file VectorPro.tpp
 * @brief Vector template implementation.
 *
 * Contains the implementation of VectorPro::Vector template member
 * functions and internal implementation details.
 */

// ============================================================
// Vector.tpp
// Template implementation for VectorPro::Vector.
// ============================================================
//
// Sections:
//   1. Constructors & Destructor
//   2. Copy & Move Semantics
//   3. Modifiers
//   4. Observer
//   5. Comparison
//   6. Span Access
//   7. Element Access
//   8. Lookup
//   9. Swap
//  10. Capacity
//  11. Iterator Access
//  12. Private Helpers
//  13. Non-member Functions
//
// ============================================================

namespace VectorPro {

// ============================================================
// Section 1 — Constructors & Destructor
// ============================================================

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::Vector(std::size_t count,
                                                                 const T& value) {
    if (count == 0)
        return;

    data_ = std::allocator_traits<Allocator>::allocate(alloc_, count);
    vcap_ = count;

    if constexpr (std::is_trivially_copyable_v<T> && std::is_same_v<Allocator, std::allocator<T>>) {
        // std::fill_n on a trivial type vectorizes (or reduces to memset for
        // zero-fill) far better than a per-element placement-new loop does.
        std::fill_n(data_, count, value);
    } else {
        for (std::size_t i = 0; i < count; ++i) {
            std::allocator_traits<Allocator>::construct(alloc_, data_ + i, value);
        }
    }

    vsize_ = count;
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::Vector(std::initializer_list<T> init) {
    if (init.size() == 0)
        return;

    data_ = std::allocator_traits<Allocator>::allocate(alloc_, init.size());
    vcap_ = init.size();

    for (const auto& val : init) {
        std::allocator_traits<Allocator>::construct(alloc_, data_ + vsize_++, val);
    }
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
template <std::input_iterator It>
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::Vector(It first, It last) {
    for (; first != last; ++first) {
        push_back(*first);
    }
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::Vector(const Allocator& alloc)
    : alloc_(alloc) {}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::~Vector() noexcept {
    release();

    if constexpr (EnableEvents) {
        if (listenerStore_.listeners_) {
            delete[] listenerStore_.listeners_;
            listenerStore_.listeners_ = nullptr;
        }
    }
}

// ============================================================
// Section 2 — Copy & Move Semantics
// ============================================================

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::Vector(const Vector& other)
    : alloc_(
          std::allocator_traits<Allocator>::select_on_container_copy_construction(other.alloc_)) {
    copyBufferFrom(other);
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>&
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::operator=(const Vector& other) {
    if (this == &other)
        return *this;

    constexpr bool kPropagate =
        std::allocator_traits<Allocator>::propagate_on_container_copy_assignment::value;

    if constexpr (kPropagate) {
        if (!(alloc_ == other.alloc_)) {
            // Our current storage was obtained from the old allocator instance;
            // it must be released before adopting the new one, since the new
            // allocator may not be able to deallocate memory it didn't allocate.
            release();
            alloc_ = other.alloc_;
        }
    }

    if (other.vsize_ <= vcap_) {
        // Fast path: existing capacity is sufficient, no allocation needed.
        if constexpr (std::is_trivially_copyable_v<T> &&
                      std::is_same_v<Allocator, std::allocator<T>>) {
            if (other.vsize_ > 0) {
                std::memcpy(std::to_address(data_), std::to_address(other.data_),
                            other.vsize_ * sizeof(T));
            }
            for (std::size_t i = other.vsize_; i < vsize_; ++i) {
                std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
            }
        } else {
            std::size_t common = std::min(vsize_, other.vsize_);

            for (std::size_t i = 0; i < common; ++i) {
                data_[i] = other.data_[i];
            }

            if (other.vsize_ > vsize_) {
                std::size_t constructed = vsize_;
                try {
                    for (; constructed < other.vsize_; ++constructed) {
                        std::allocator_traits<Allocator>::construct(alloc_, data_ + constructed,
                                                                    other.data_[constructed]);
                    }
                } catch (...) {
                    for (std::size_t i = vsize_; i < constructed; ++i) {
                        std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
                    }
                    throw; // basic guarantee: vsize_ left at its pre-call value
                }
            } else {
                for (std::size_t i = other.vsize_; i < vsize_; ++i) {
                    std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
                }
            }
        }

        vsize_ = other.vsize_;
    } else {
        // Not enough capacity — fall back to copy-and-swap for the strong guarantee.
        // tmp's allocator must itself respect propagate_on_container_copy_assignment:
        // only adopt other's allocator (via select_on_container_copy_construction)
        // when propagation is permitted; otherwise keep using our own, so that
        // after the swap, *this still owns the allocator it started with.
        Vector tmp;
        tmp.alloc_ = kPropagate
                         ? std::allocator_traits<Allocator>::select_on_container_copy_construction(
                               other.alloc_)
                         : alloc_;
        tmp.copyBufferFrom(other);
        swap(tmp);
    }

    return *this;
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::Vector(Vector&& other) noexcept
    : alloc_(std::move(other.alloc_)), data_(other.data_), vsize_(other.vsize_), vcap_(other.vcap_),
      listenerStore_(other.listenerStore_) {
    other.data_ = nullptr;
    other.vsize_ = 0;
    other.vcap_ = 0;
    if constexpr (EnableEvents) {
        other.listenerStore_.listeners_ = nullptr;
        other.listenerStore_.lsize_ = 0;
        other.listenerStore_.lcap_ = 0;
    }
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>&
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::operator=(Vector&& other) noexcept {
    if (this != &other) {
        constexpr bool kPropagate =
            std::allocator_traits<Allocator>::propagate_on_container_move_assignment::value;
        constexpr bool kAlwaysEqual = std::allocator_traits<Allocator>::is_always_equal::value;

        const bool canSteal = kPropagate || kAlwaysEqual || (alloc_ == other.alloc_);

        release();
        if constexpr (EnableEvents) {
            if (listenerStore_.listeners_) {
                delete[] listenerStore_.listeners_;
                listenerStore_.listeners_ = nullptr;
            }
        }

        if (canSteal) {
            if constexpr (kPropagate) {
                alloc_ = std::move(other.alloc_);
            }

            data_ = other.data_;
            vsize_ = other.vsize_;
            vcap_ = other.vcap_;
            if constexpr (EnableEvents) {
                listenerStore_ = other.listenerStore_;
            }

            other.data_ = nullptr;
            other.vsize_ = 0;
            other.vcap_ = 0;
            if constexpr (EnableEvents) {
                other.listenerStore_.listeners_ = nullptr;
                other.listenerStore_.lsize_ = 0;
                other.listenerStore_.lcap_ = 0;
            }
        } else {
            // Unequal, non-propagating allocators: move elements individually
            // using our own allocator; leave `other` cleared via its own release().
            if (other.vsize_ > 0) {
                data_ = std::allocator_traits<Allocator>::allocate(alloc_, other.vsize_);
                vcap_ = other.vsize_;
                for (std::size_t i = 0; i < other.vsize_; ++i) {
                    std::allocator_traits<Allocator>::construct(alloc_, data_ + i,
                                                                std::move(other.data_[i]));
                }
                vsize_ = other.vsize_;
            }
            other.release();
        }
    }

    return *this;
}

// ============================================================
// Section 3 — Modifiers
// ============================================================

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
void Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::push_back(const T& value) {
    // Hot path only: no branch-heavy aliasing bookkeeping, no reallocate()
    // call site inline here. Small enough to reliably inline at call sites,
    // which the mixed hot/cold version wasn't.
    if (vsize_ == vcap_) [[unlikely]] {
        grow_and_push_back(value);
        return;
    }
    std::allocator_traits<Allocator>::construct(alloc_, data_ + vsize_, value);
    std::size_t old = vsize_;
    ++vsize_;
    notify({EventType::PUSHBACK, old, old, vsize_});
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
void Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::grow_and_push_back(const T& value) {
    // About to reallocate, which can invalidate `value` if it's a
    // reference into our own storage (e.g. v.push_back(v[i])).
    // Snapshot its offset now, while the old buffer is still valid.
    std::ptrdiff_t offset = -1;
    if (data_ && std::addressof(value) >= data_ && std::addressof(value) < data_ + vsize_) {
        offset = std::addressof(value) - data_;
    }

    reallocate(growCapacity());

    const T& src = (offset >= 0) ? data_[offset] : value;
    std::allocator_traits<Allocator>::construct(alloc_, data_ + vsize_, src);

    std::size_t old = vsize_;
    ++vsize_;
    notify({EventType::PUSHBACK, old, old, vsize_});
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
void Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::push_back(T&& value) {
    if (vsize_ == vcap_) [[unlikely]] {
        grow_and_push_back(std::move(value));
        return;
    }
    std::allocator_traits<Allocator>::construct(alloc_, data_ + vsize_, std::move(value));
    std::size_t old = vsize_;
    ++vsize_;
    notify({EventType::PUSHBACK, old, old, vsize_});
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
void Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::grow_and_push_back(T&& value) {
    // About to reallocate, which can invalidate `value` if it's a
    // reference into our own storage (e.g. v.push_back(std::move(v[i]))).
    // Snapshot its offset now, while the old buffer is still valid.
    std::ptrdiff_t offset = -1;
    if (data_ && std::addressof(value) >= data_ && std::addressof(value) < data_ + vsize_) {
        offset = std::addressof(value) - data_;
    }

    reallocate(growCapacity());

    if (offset >= 0) {
        std::allocator_traits<Allocator>::construct(alloc_, data_ + vsize_,
                                                    std::move(data_[offset]));
    } else {
        std::allocator_traits<Allocator>::construct(alloc_, data_ + vsize_, std::move(value));
    }

    std::size_t old = vsize_;
    ++vsize_;
    notify({EventType::PUSHBACK, old, old, vsize_});
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
template <typename... Args>
    requires std::constructible_from<T, Args...>
void Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::emplace_back(Args&&... args) {
    if (vsize_ == vcap_)
        reallocate(growCapacity());

    std::allocator_traits<Allocator>::construct(alloc_, data_ + vsize_,
                                                std::forward<Args>(args)...);

    std::size_t old = vsize_;
    ++vsize_;
    notify({EventType::EMPLACEBACK, old, old, vsize_});
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::iterator
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::insert(const_iterator pos,
                                                                 const T& value) {
    std::size_t index = pos - cbegin();
    const bool willReallocate = (vsize_ == vcap_);
    const bool willShift = (index != vsize_);

    if (!willReallocate && !willShift) {
        // Fast path: plain append with capacity to spare. Nothing below
        // touches or invalidates `value`, so no aliasing check is needed.
        std::allocator_traits<Allocator>::construct(alloc_, data_ + index, value);
    } else {
        // Either reallocation (invalidates the old buffer) or the shift
        // loop (can overwrite the slot `value` points to) is about to
        // happen — snapshot `value` first if it aliases our own storage.
        std::ptrdiff_t offset = -1;
        if (data_ && std::addressof(value) >= data_ && std::addressof(value) < data_ + vsize_) {
            offset = std::addressof(value) - data_;
        }

        if (willReallocate)
            reallocate(growCapacity());

        // Snapshot before shifting — the shift loop can overwrite `offset`
        // before we'd otherwise read it.
        std::optional<T> snapshot;
        if (offset >= 0)
            snapshot.emplace(data_[offset]);
        const T& src = snapshot ? *snapshot : value;

        if (index == vsize_) {
            std::allocator_traits<Allocator>::construct(alloc_, data_ + index, src);
        } else {
            std::allocator_traits<Allocator>::construct(alloc_, data_ + vsize_,
                                                        std::move(data_[vsize_ - 1]));
            for (std::size_t i = vsize_ - 1; i > index; --i)
                data_[i] = std::move(data_[i - 1]);
            data_[index] = src;
        }
    }

    std::size_t old = vsize_;
    ++vsize_;
    notify({EventType::INSERT, index, old, vsize_});
    return iterator(data_ + index);
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::iterator
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::insert(const_iterator pos, T&& value) {
    std::size_t index = pos - cbegin();
    const bool willReallocate = (vsize_ == vcap_);
    const bool willShift = (index != vsize_);

    if (!willReallocate && !willShift) {
        // Fast path: plain append with capacity to spare.
        std::allocator_traits<Allocator>::construct(alloc_, data_ + index, std::move(value));
    } else {
        std::ptrdiff_t offset = -1;
        if (data_ && std::addressof(value) >= data_ && std::addressof(value) < data_ + vsize_) {
            offset = std::addressof(value) - data_;
        }

        if (willReallocate)
            reallocate(growCapacity());

        std::optional<T> snapshot;
        if (offset >= 0)
            snapshot.emplace(std::move(data_[offset]));

        if (index == vsize_) {
            if (snapshot)
                std::allocator_traits<Allocator>::construct(alloc_, data_ + index,
                                                            std::move(*snapshot));
            else
                std::allocator_traits<Allocator>::construct(alloc_, data_ + index,
                                                            std::move(value));
        } else {
            std::allocator_traits<Allocator>::construct(alloc_, data_ + vsize_,
                                                        std::move(data_[vsize_ - 1]));
            for (std::size_t i = vsize_ - 1; i > index; --i)
                data_[i] = std::move(data_[i - 1]);

            if (snapshot)
                data_[index] = std::move(*snapshot);
            else
                data_[index] = std::move(value);
        }
    }

    std::size_t old = vsize_;
    ++vsize_;
    notify({EventType::INSERT, index, old, vsize_});
    return iterator(data_ + index);
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
template <std::input_iterator It>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::iterator
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::insert(const_iterator pos, It first,
                                                                 It last) {
    std::size_t index = pos - cbegin();
    std::size_t i = index;

    for (; first != last; ++first, ++i) {
        (void)insert(cbegin() + i, *first);
    }

    return iterator(data_ + index);
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
template <typename... Args>
    requires std::constructible_from<T, Args...>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::iterator
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::emplace(const_iterator pos,
                                                                  Args&&... args) {
    std::size_t index = pos - cbegin();

    if (vsize_ == vcap_)
        reallocate(growCapacity());

    if (index == vsize_) {
        std::allocator_traits<Allocator>::construct(alloc_, data_ + index,
                                                    std::forward<Args>(args)...);
    } else {
        T tmp(std::forward<Args>(args)...);

        std::allocator_traits<Allocator>::construct(alloc_, data_ + vsize_,
                                                    std::move(data_[vsize_ - 1]));
        for (std::size_t i = vsize_ - 1; i > index; --i)
            data_[i] = std::move(data_[i - 1]);

        data_[index] = std::move(tmp);
    }

    std::size_t old = vsize_;
    ++vsize_;
    notify({EventType::INSERT, index, old, vsize_});
    return iterator(data_ + index);
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
template <typename Predicate>
    requires std::predicate<Predicate, const T&>
std::size_t Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::remove_if(Predicate pred) {
    std::size_t old = vsize_;
    std::size_t dest = 0;

    for (std::size_t i = 0; i < vsize_; ++i) {
        if (!pred(data_[i])) {
            if (dest != i)
                data_[dest] = std::move(data_[i]);
            ++dest;
        }
        // Removed elements are intentionally left untouched here — the
        // cleanup loop below destroys every leftover slot in [dest, vsize_)
        // exactly once, whether it holds an original removed element or a
        // moved-from "kept" element. Destroying here too caused a
        // double-destroy for any removed index >= the final `dest`.
    }

    for (std::size_t i = dest; i < vsize_; ++i) {
        std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
    }

    vsize_ = dest;
    notify({EventType::REMOVE, dest, old, vsize_});
    return old - vsize_;
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
void Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::pop_back() {
    if (vsize_ == 0)
        return;

    std::size_t old = vsize_;
    std::allocator_traits<Allocator>::destroy(alloc_, data_ + vsize_ - 1);
    --vsize_;

    notify({EventType::POPBACK, vsize_, old, vsize_});
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::iterator
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::erase(const_iterator pos) {
    std::size_t index = pos - cbegin();

    for (std::size_t i = index; i + 1 < vsize_; ++i) {
        data_[i] = std::move(data_[i + 1]);
    }

    std::allocator_traits<Allocator>::destroy(alloc_, data_ + vsize_ - 1);

    std::size_t old = vsize_;
    --vsize_;

    notify({EventType::ERASE, index, old, vsize_});
    return iterator(data_ + index);
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::iterator
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::erase(const_iterator first,
                                                                const_iterator last) {
    std::size_t indexFirst = first - cbegin();
    std::size_t indexLast = last - cbegin();
    std::size_t count = indexLast - indexFirst;

    if (count == 0)
        return iterator(data_ + indexFirst);

    for (std::size_t i = indexFirst; i + count < vsize_; ++i) {
        data_[i] = std::move(data_[i + count]);
    }

    for (std::size_t i = vsize_ - count; i < vsize_; ++i) {
        std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
    }

    std::size_t old = vsize_;
    vsize_ -= count;

    notify({EventType::ERASE, indexFirst, old, vsize_});
    return iterator(data_ + indexFirst);
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
void Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::clear() noexcept {
    if (vsize_ == 0)
        return;

    for (std::size_t i = 0; i < vsize_; ++i) {
        std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
    }

    std::size_t old = vsize_;
    vsize_ = 0;

    notify({EventType::CLEAR, 0, old, 0});
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
void Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::reserve(std::size_t newCap) {
    if (newCap <= vcap_)
        return;

    std::size_t old = vcap_;
    reallocate(newCap);

    notify({EventType::RESERVE, 0, old, vcap_});
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
void Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::shrink_to_fit() {
    if (vsize_ == vcap_)
        return;

    std::size_t old = vcap_;
    reallocate(vsize_);

    notify({EventType::SHRINK, 0, old, vcap_});
}

// ============================================================
// Section 4 — Observer
// ============================================================

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
template <typename F>
    requires EnableEvents && Listener<F, Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::ListenerHandle
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::subscribe(F&& listeners) {
    if (listenerStore_.lsize_ == listenerStore_.lcap_) {
        std::size_t newCap = listenerStore_.lcap_ == 0 ? INITIAL_CAP : listenerStore_.lcap_ * 2;
        ListenerFn* newBuf = new ListenerFn[newCap];

        for (std::size_t i = 0; i < listenerStore_.lsize_; ++i) {
            newBuf[i] = std::move(listenerStore_.listeners_[i]);
        }

        delete[] listenerStore_.listeners_;
        listenerStore_.listeners_ = newBuf;
        listenerStore_.lcap_ = newCap;
    }

    listenerStore_.listeners_[listenerStore_.lsize_] = std::forward<F>(listeners);
    return listenerStore_.lsize_++;
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
void Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::unsubscribe(ListenerHandle handle)
    requires EnableEvents
{
    if (handle >= listenerStore_.lsize_)
        return;

    for (std::size_t i = handle; i < listenerStore_.lsize_ - 1; ++i) {
        listenerStore_.listeners_[i] = std::move(listenerStore_.listeners_[i + 1]);
    }

    listenerStore_.listeners_[listenerStore_.lsize_ - 1] = ListenerFn{};
    --listenerStore_.lsize_;
}

// ============================================================
// Section 5 — Comparison
// ============================================================//
// ============================================================

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
bool Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::operator==(const Vector& other) const
    noexcept(noexcept(std::declval<const T&>() == std::declval<const T&>())) {
    if (vsize_ != other.vsize_)
        return false;

    if (vsize_ == 0)
        return true;

    // Fast path: for trivially-copyable T with the default allocator, the
    // underlying storage is a flat byte buffer, so a single memcmp() over
    // the whole range is equivalent to comparing element-by-element but
    // lets the CPU compare many bytes per instruction instead of one T at
    // a time. std::equal() can't take this shortcut here because it only
    // recognizes libstdc++'s own vector iterator as "really a pointer";
    // our Iterator type is a true contiguous_iterator but isn't special-cased
    // by that internal check, so without this branch it falls back to a
    // scalar, element-by-element loop.
    if constexpr (std::is_trivially_copyable_v<T> && std::is_same_v<Allocator, std::allocator<T>>) {
        return std::memcmp(std::to_address(data_), std::to_address(other.data_),
                           vsize_ * sizeof(T)) == 0;
    } else {
        return std::equal(begin(), end(), other.begin());
    }
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
auto Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::operator<=>(const Vector& other)
    const noexcept(noexcept(std::declval<const T&>() <=> std::declval<const T&>())) {
    return std::lexicographical_compare_three_way(begin(), end(), other.begin(), other.end());
}

// ============================================================
// Section 6 — Span Access
// ============================================================

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
std::span<T> Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::as_span() noexcept {
    return std::span<T>(data_, vsize_);
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
std::span<const T>
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::as_span() const noexcept {
    return std::span<const T>(data_, vsize_);
}

// ============================================================
// Section 7 — Element Access
// ============================================================

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::pointer
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::data_ptr() noexcept {
    return data_;
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::const_pointer
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::data_ptr() const noexcept {
    return data_;
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::reference
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::at(std::size_t index) {
    if (index >= vsize_)
        throw std::out_of_range("Vector::at() index out of range");

    return data_[index];
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::const_reference
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::at(std::size_t index) const {
    if (index >= vsize_)
        throw std::out_of_range("Vector::at() index out of range");

    return data_[index];
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::reference
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::front() {
    return data_[0];
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::const_reference
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::front() const {
    return data_[0];
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::reference
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::back() {
    return data_[vsize_ - 1];
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::const_reference
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::back() const {
    return data_[vsize_ - 1];
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::reference
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::operator[](std::size_t index) noexcept {
    return data_[index];
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::const_reference
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::operator[](
    std::size_t index) const noexcept {
    return data_[index];
}

// ============================================================
// Section 8 — Lookup
// ============================================================

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
bool Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::contains(const T& value) const
    noexcept(noexcept(std::declval<const T&>() == std::declval<const T&>())) {
    if (vsize_ == 0)
        return false;

    // std::find() on a raw pointer range picks up libstdc++'s internal
    // fast paths (manual loop-unrolling for scalar types, memchr for
    // byte-sized types) that our own hand-written loop doesn't get for free.
    const T* first = std::to_address(data_);
    return std::find(first, first + vsize_, value) != first + vsize_;
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::iterator
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::find(const T& value) noexcept(
    noexcept(std::declval<const T&>() == std::declval<const T&>())) {
    T* first = std::to_address(data_);
    return iterator(std::find(first, first + vsize_, value));
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::const_iterator
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::find(const T& value) const
    noexcept(noexcept(std::declval<const T&>() == std::declval<const T&>())) {
    const T* first = std::to_address(data_);
    return const_iterator(std::find(first, first + vsize_, value));
}

// ============================================================
// Section 9 — Swap
// ============================================================

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
void Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::swap(Vector& other) noexcept {
    using std::swap;
    swap(alloc_, other.alloc_);
    swap(data_, other.data_);
    swap(vsize_, other.vsize_);
    swap(vcap_, other.vcap_);
    swap(listenerStore_, other.listenerStore_);
}

// ============================================================
// Section 10 — Capacity
// ============================================================

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
bool Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::empty() const noexcept {
    return vsize_ == 0;
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
std::size_t Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::size() const noexcept {
    return vsize_;
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
std::size_t Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::capacity() const noexcept {
    return vcap_;
}

// ============================================================
// Section 11 — Iterator Access
// ============================================================

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::iterator
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::begin() noexcept {
    return iterator(data_);
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::iterator
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::end() noexcept {
    return iterator(data_ + vsize_);
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::const_iterator
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::begin() const noexcept {
    return const_iterator(data_);
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::const_iterator
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::end() const noexcept {
    return const_iterator(data_ + vsize_);
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::const_iterator
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::cbegin() const noexcept {
    return const_iterator(data_);
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::const_iterator
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::cend() const noexcept {
    return const_iterator(data_ + vsize_);
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::reverse_iterator
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::rbegin() noexcept {
    return reverse_iterator(end());
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::reverse_iterator
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::rend() noexcept {
    return reverse_iterator(begin());
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::const_reverse_iterator
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::rbegin() const noexcept {
    return const_reverse_iterator(end());
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::const_reverse_iterator
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::rend() const noexcept {
    return const_reverse_iterator(begin());
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::const_reverse_iterator
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::crbegin() const noexcept {
    return const_reverse_iterator(end());
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
typename Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::const_reverse_iterator
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::crend() const noexcept {
    return const_reverse_iterator(begin());
}

// ============================================================
// Section 12 — Private Helpers
// ============================================================

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
void Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::release() noexcept {
    for (std::size_t i = 0; i < vsize_; ++i) {
        std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
    }

    if (data_) {
        std::allocator_traits<Allocator>::deallocate(alloc_, data_, vcap_);
    }

    data_ = nullptr;
    vsize_ = 0;
    vcap_ = 0;
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
void Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::copyBufferFrom(const Vector& other) {
    if (other.vcap_ == 0)
        return;

    data_ = std::allocator_traits<Allocator>::allocate(alloc_, other.vcap_);
    vcap_ = other.vcap_;

    if constexpr (std::is_trivially_copyable_v<T> && std::is_same_v<Allocator, std::allocator<T>>) {
        if (other.vsize_ > 0) {
            std::memcpy(std::to_address(data_), std::to_address(other.data_),
                        other.vsize_ * sizeof(T));
        }
    } else {
        std::size_t constructed = 0;
        try {
            for (; constructed < other.vsize_; ++constructed) {
                std::allocator_traits<Allocator>::construct(alloc_, data_ + constructed,
                                                            other.data_[constructed]);
            }
        } catch (...) {
            for (std::size_t i = 0; i < constructed; ++i)
                std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
            std::allocator_traits<Allocator>::deallocate(alloc_, data_, vcap_);
            data_ = nullptr;
            vcap_ = 0;
            throw;
        }
    }

    vsize_ = other.vsize_;
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
std::size_t
Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::growCapacity() const noexcept {
    if (vcap_ == 0)
        return INITIAL_CAP;

    constexpr std::size_t kMax = std::numeric_limits<std::size_t>::max();
    if (vcap_ > kMax / GrowthNum)
        return kMax;

    std::size_t grown = vcap_ * GrowthNum / GrowthDen;

    // Integer division can truncate back down to the current capacity for
    // small vcap_ values combined with a fractional growth ratio (e.g.
    // 1 * 3 / 2 == 1). Guarantee growth always strictly increases capacity
    // by at least one element, saturating at SIZE_MAX instead of overflowing.
    if (grown <= vcap_)
        grown = (vcap_ < kMax) ? vcap_ + 1 : kMax;

    return grown;
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
void Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::reallocate(std::size_t newCap) {
    pointer newData = std::allocator_traits<Allocator>::allocate(alloc_, newCap);

    if constexpr (std::is_trivially_copyable_v<T> && std::is_same_v<Allocator, std::allocator<T>>) {
        if (vsize_ > 0) {
            std::memcpy(std::to_address(newData), std::to_address(data_), vsize_ * sizeof(T));
        }
    } else {
        std::size_t constructed = 0;
        try {
            for (; constructed < vsize_; ++constructed) {
                std::allocator_traits<Allocator>::construct(
                    alloc_, newData + constructed, std::move_if_noexcept(data_[constructed]));
            }
        } catch (...) {
            for (std::size_t i = 0; i < constructed; ++i)
                std::allocator_traits<Allocator>::destroy(alloc_, newData + i);
            std::allocator_traits<Allocator>::deallocate(alloc_, newData, newCap);
            throw;
        }

        for (std::size_t i = 0; i < vsize_; ++i) {
            std::allocator_traits<Allocator>::destroy(alloc_, data_ + i);
        }
    }

    if (data_) {
        std::allocator_traits<Allocator>::deallocate(alloc_, data_, vcap_);
    }

    data_ = newData;
    vcap_ = newCap;
}

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
    requires std::destructible<T>
void Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>::notify(EventData data) {
    // `if constexpr` here is what makes EnableEvents = false a true zero-cost
    // path: unlike a runtime `if (lsize_ == 0) return;`, this branch is
    // discarded during compilation, not just at runtime. There's no leftover
    // load of lsize_, no branch, and no reason for the compiler to leave a
    // real function call behind — the whole call site collapses to nothing.
    if constexpr (EnableEvents) {
        for (std::size_t i = 0; i < listenerStore_.lsize_; ++i) {
            if (listenerStore_.listeners_[i])
                listenerStore_.listeners_[i](*this, data);
        }
    }
}

// ============================================================
// Section 13 — Non-member Functions
// ============================================================

template <typename T, typename Allocator, std::size_t GrowthNum, std::size_t GrowthDen,
          bool EnableEvents>
void swap(Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>& a,
          Vector<T, Allocator, GrowthNum, GrowthDen, EnableEvents>& b) noexcept {
    a.swap(b);
}

} // namespace VectorPro
