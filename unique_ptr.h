#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

#include <stdexcept>

template <class T>
 class unique_ptr
 {
 private:
 T * _ptr;
 public:
 unique_ptr() // Initializes with null
 {
    _ptr = nullptr;
 }
 unique_ptr(int bytes) // Allocates memeory
 {
    _ptr = new T[bytes];
 }
 unique_ptr(const unique_ptr<T>&p) = delete; // Throws compiler error
 unique_ptr& operator=(const unique_ptr<T>&p) = delete; // Throws compiler error
 unique_ptr(const unique_ptr<T>&&p)
 {
    _ptr = std::move(p._ptr);
 }
 // This would make _ptr point to the memory p was pointing to
// p would start pointing to null
 unique_ptr& operator=(const unique_ptr<T>&&p)
 {
    if( this == &p ) 
        return *this;
    if (*this != p)
    {
        _ptr =std::move(p._ptr);
        return *this;
    }
 }
 // Deallocates whatever _ptr is pointing to
// This would make _ptr point to the memory p was pointing to
// _p would start pointing to null
 T* operator->() // returns _ptr
 {
    return _ptr;
 }
 T& operator*() // returns reference to what _ptr is pointing to
 {
    
    if (!_ptr)
        throw std::out_of_range("Illegal Memory access");
    return *_ptr;
 }
T& operator[](int index)
{
    if (!_ptr)
    throw std::out_of_range("No value assigned.");
    return _ptr[index];
} // returns reference to value at index
 // Also handles illegal memory access
 static unique_ptr<T> make_unique()
 {
    unique_ptr<T> temp;
    return temp;

 }
 bool operator==(const unique_ptr<T>& p)
 {
    return _ptr = p._ptr;
 }
 // returns an instance of unique pointer object
void reset() // deletes the memory and points _ptr to null
{
    delete[] _ptr;
    _ptr = nullptr;
}
~unique_ptr() // Deallocates memory
{
    if(_ptr != nullptr)
    {
        delete[] _ptr;
        _ptr = nullptr;
    }
}
 };
 #endif