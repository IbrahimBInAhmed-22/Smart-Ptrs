#include<iostream>
#include <vector>
#include "ptr_memory.h"
#include "weak_ptr.h"

template <class T>
 shared_ptr<T>::shared_ptr():_ptr(nullptr),block(nullptr)
 {
    
 }

 template<class T>
shared_ptr<T>::shared_ptr(int bytes){
 _ptr = new T[bytes];
 block = new memory<T>();
 block->assign(*this);
 } 

template<class T>
shared_ptr<T>::shared_ptr(const shared_ptr<T>&p)
 {
 _ptr = p._ptr;
 block = p.block;
 if(block)
 block->assign(*this);
 }


template<class T>
shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr<T>&p)
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

template<class T>
 int shared_ptr<T>::count()
    {
        if (block == nullptr)
            return 0;
        return block->count();
    }

template<class T>
 T* shared_ptr<T>::operator->()
    {
        return _ptr;
    }

template<class T>

 T& shared_ptr<T>::operator*()
    {
        if(_ptr)
        return *_ptr;
         throw std::runtime_error("Illegal Memory access");
    } 
    
template<class T>

 T& shared_ptr<T>::operator[](int index)
{
    
    if (_ptr[index] == nullptr)
        throw std::out_of_range("Illegal Memory access");
    return *(_ptr+index);
    //throw out_of_range("Illegal memory access");

} 

template<class T>
 bool shared_ptr<T>::operator==(const shared_ptr<T>&p)
    {
        return _ptr == p._ptr && block == p.block;
    } 

template<class T>
shared_ptr<T> shared_ptr<T>::make_shared()
{
    shared_ptr<T> a(1);
    return a;
    
}

template<class T>

 void shared_ptr<T>::reset()
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
    }

    template<class T>
     shared_ptr<T>::~shared_ptr()
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
