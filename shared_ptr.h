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
 shared_ptr():_ptr(nullptr),block(nullptr)
 {
    
 }//Initializes null pointer
 shared_ptr(int bytes){
 _ptr = new T[bytes];
 block = new memory<T>();
 block->assign(*this);
 } // Allocates memory and block
 shared_ptr(const shared_ptr<T>&p)
 {
 _ptr = p._ptr;
 block = p.block;
 if(block)
 block->assign(*this);
 }
 // Copies contents of p into our object(this)
 // Assign our object to memory block
 shared_ptr& operator=(const shared_ptr<T>&p)
 {
        if (this != &p)
        {
           
            if (block != nullptr){
                block->free(*this);
                if (block->count() == 0){
                    block->free();
                    delete block;
                    block = nullptr;
                    delete[]_ptr;
                    _ptr =  nullptr;
                }

            }
            
        }

        _ptr = p._ptr;
        block = p.block;
        if(block != nullptr && this != &p)
        block->assign(*this);
        
        return *this;
 }
 int count()
    {
        if (block == nullptr)
            return 0;
        return block->count();
    }
    // Free our object from it's memory block
    // If the count becomes 0, free and delete that memory block
    // Copies contents of p into our object(this)
    // Assign our object to memory block of p
 T* operator->()
    {
        return _ptr;
    } // returns _ptr
 T& operator*()
    {
        if(_ptr)
        return *_ptr;
    } // returns reference to what _ptr is pointing to
 T& operator[](int index)
    {
        
        if (_ptr[index] == nullptr)
            throw std::out_of_range("Illegal Memory access");
        return *(_ptr+index);
        //throw out_of_range("Illegal memory access");

    } // returns reference to value at index
    // Also handles illegal memory access
 bool operator==(const shared_ptr<T>&p)
    {
        return _ptr == p._ptr && block == p.block;
    } // Compares two shared pointers
 static shared_ptr<T> make_shared()
    {
        shared_ptr<T> a(1);
        return a;
        
    }
    // returns an instance of shared pointer object
 void reset()
    {
        if (block != nullptr)
        {
            block->free(*this);
            if (block->count() == 0)
            {
                block->free();
                delete block;
                block = nullptr;
                delete[] _ptr;
                _ptr = nullptr;
            }
        }
    } // Free the memory block
 ~shared_ptr()
    {
        if (block != nullptr)
        {
            block->free(*this);
            if (block->count() == 0)
            {
                delete block;
                block = nullptr;
                delete[]_ptr;
                _ptr = nullptr;

            }
            _ptr = nullptr;
        }
    }
    // Free the object from it's memory block
    // If the count becomes 0, free and delete that memory block

 friend class weak_ptr<T>;
    };
 #endif