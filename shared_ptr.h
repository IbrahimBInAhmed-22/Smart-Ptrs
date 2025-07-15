#ifndef SHARED_PTR_H
#define SHARED_PTR_H
#include<iostream>
#include <vector>
#include "ptr_memory.h"
#include "weak_ptr.h"

template <class T>
class memory;
template <class T>
class weak_ptr;


template <class T>
 class shared_ptr{
 private:
 T* _ptr;
 memory<T>* block;
 public:
 shared_ptr();//Initializes null pointer
 shared_ptr(int bytes); // Allocates memory and block
 shared_ptr(const shared_ptr<T>&p);
 // Copies contents of p into our object(this)
 // Assign our object to memory block
 shared_ptr& operator=(const shared_ptr<T>&p);
 int count(); 
    // Free our object from it's memory block
    // If the count becomes 0, free and delete that memory block
    // Copies contents of p into our object(this)
    // Assign our object to memory block of p
 T* operator->(); // returns _ptr
 T& operator*(); // returns reference to what _ptr is pointing to
 T& operator[](int index);// returns reference to value at index
    // Also handles illegal memory access
 bool operator==(const shared_ptr<T>&p); // Compares two shared pointers
 static shared_ptr<T> make_shared();
    // returns an instance of shared pointer object
 void reset(); // Free the memory block
 ~shared_ptr();
    // Free the object from it's memory block
    // If the count becomes 0, free and delete that memory block

 friend class weak_ptr<T>;

    };
#include "shared_ptr.tpp"
 #endif