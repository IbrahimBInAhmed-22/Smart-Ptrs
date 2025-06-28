#ifndef WEAK_PTR_H
#define WEAK_PTR_H


#include<iostream>
#include<vector>
#include "ptr_memory.h"
#include "shared_ptr.h"

template <class T>
class memory;

template <class T>
class shared_ptr;


template <class T>
 class weak_ptr{
 private:
 T* _ptr;
 memory<T>* block;
 public:
 weak_ptr(const shared_ptr<T>&p)
 {
    _ptr = p._ptr;
    block = p.block;
    if(block)
    block->assign(*this);
 }
 // Copies contents of p into our object(this)
// Assign our object to memory block
 weak_ptr& operator=(const shared_ptr<T>&p)
 {
    _ptr = p._ptr;
    block = p.block;
    if(block)
    block->assign(*this);
    return *this;
 }
 // Copies contents of p into our object(this)
// Assign our object to memory block
 T* operator->()
 {
    return _ptr;
 } // returns _ptr
 T& operator*()
 {
    
    if(!_ptr || !block || block->count() == 0)
    throw std::out_of_range("Illegal Memory access");
    return *_ptr;
 } // returns reference to what _ptr is pointing to
T& operator[](int index)
{
    
    if (!_ptr)
        throw std::out_of_range("Illegal Memory access");
    return _ptr[index];
} // returns reference to value at index
 // Also handles illegal memory access
 bool operator==(const weak_ptr<T>&p)
 {
    return _ptr == p._ptr && block == p.block;
 } // Compares two weak pointers
 }; 
 #endif