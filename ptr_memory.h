#ifndef PTR_MEMORY_H
#define PTR_MEMORY_H

#include <iostream>
#include <vector>
#include "shared_ptr.h"
#include "weak_ptr.h"

template <class T>
class shared_ptr;

template <class T>
class weak_ptr;


template <class T>
 class memory{
 private:
 int reference_count;
 // You can implement the below attributes as you like
 std::vector<shared_ptr<T>*> shared_memory;
 std::vector<weak_ptr<T>*> weak_memory;
 public:
    void assign(shared_ptr<T>& _shared);
    // Assign the shared_ptr to the memory block
    // Increment the count by 1
    void free(shared_ptr<T>& _shared);
    // Free the shared_ptr from the memory block
    // Decrement the count by 1
    void assign(weak_ptr<T>& _weak);
    // Assign the weak_ptr to the memory block
    void free(weak_ptr<T>& _weak);
    // Free the weak_ptr from the memory block
    void free();
    // Free all the shared/weak pointers and point them to nullptr
    int count();
 // Returns reference count
};
#include "ptr_memory.tpp"
 #endif