#pragma once

#include "Iterator.h"
#include "ReverseIterator.h"

#include <iostream>
#include <stdexcept>
#include <cstddef>
#include <utility>

enum class EventType {
    PushBack,
    PopBack,
    Insert,
    Remove,
    Erase,
    Clear,
    Shrink,
    Reserve
};

template<typename T>
class VectorPro {
    public:
    // Types
    using size_type = std::size_t;
    
    using Listener = void(*)(const VectorPro<T>&, EventType);
    
    using iterator = Iterator<T>;
    using const_iterator = Iterator<const T>;
    using reverse_iterator = ReverseIterator<T>;
    using const_reverse_iterator = ReverseIterator<const T>;
    
    private:
    // Data
    T* data = nullptr;
    size_type v_size = 0;
    size_type v_cap = 0;
    
    // Observer
    Listener* listeners = nullptr;
    size_type l_size = 0;
    size_type l_cap = 0;
    
    // Data Management
    void release();
    
    void reallocateData(size_type newCap);
    void reallocateListener(size_type newCap);
    
    size_type growData();
    size_type growListener();
    
    void notify(EventType type);
    void swap(VectorPro& a, VectorPro& b) noexcept;
    
    public:
    // Constructors & Destructor
    VectorPro() = default;
    ~VectorPro() noexcept;
    
    VectorPro(const VectorPro& other);
    VectorPro& operator=(const VectorPro& other);
    
    VectorPro(VectorPro&& other) noexcept;
    VectorPro& operator=(VectorPro&& other) noexcept;
    
    // Modifiers 
    void push_back(const T& value);
    void push_back(T&& value);
    
    template<typename... Args>
    void emplace_back(Args&&... args);
    
    void insert(size_type pos, T&& value);
    void insert(size_type pos, const T& value);
    
    template<typename Predicate>
    void remove_if(Predicate pred);
    
    void pop_back();
    void erase(size_type index);
    
    void clear();
    
    void reserve(size_type newCap);
    void shrink_to_fit();
    
    // Observer
    template<typename Func>
    void subscribe(Func listener);
    void unsubscribe(size_type index);
    
    // Capacity
    bool empty() const noexcept;
    size_type size() const noexcept;
    size_type capacity() const noexcept;
    
    // Element Access 
    T& operator[](size_type index) noexcept;
    const T& operator[](size_type index) const noexcept;
    
    T& at(size_type index);
    const T& at(size_type index) const;
    
    T& front();
    const T& front() const;
    
    T& back();
    const T& back() const;
    
    // Iterators
    iterator begin() noexcept;
    iterator end() noexcept;
    
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    
    reverse_iterator rbegin() noexcept;
    reverse_iterator rend() noexcept;
    
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator rend() const noexcept;
    
};

#include "VectorPro.tpp"

