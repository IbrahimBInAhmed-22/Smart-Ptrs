#include <iostream>
#include <vector>
#include <exception>
#include <stdexcept>

//using namespace std;


template <class T>
class shared_ptr;


template <class T>
class weak_ptr;


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
    if(*this == p) 
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
    return *_ptr;
 }
T& operator[](int index)
{
    if (_ptr+index == nullptr)
    throw std::out_of_range("Illegal memory access");
    return _ptr[index];
} // returns reference to value at index
 // Also handles illegal memory access
 static unique_ptr<T> make_unique()
 {
    unique_ptr<T> temp;
    return temp;

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






template <class T>
 class memory{
 private:
 int reference_count;
 // You can implement the below attributes as you like
 std::vector<shared_ptr<T>*> shared_memory;
 std::vector<weak_ptr<T>*> weak_memory;
 public:
 void assign(shared_ptr<T>& _shared)
 {
    shared_memory.push_back(&_shared);
    reference_count++;
 }
 // Assign the shared_ptr to the memory block
 // Increment the count by 1
 void free(shared_ptr<T>& _shared)
{

        for(int i=0;i<shared_memory.size();i++)
        {
            if (shared_memory[i] == (&_shared) )
            {
                shared_memory.erase(shared_memory.begin() + i);
                reference_count--;
                return;
            }
        }
    }

 // Free the shared_ptr from the memory block
 // Decrement the count by 1
 void assign(weak_ptr<T>& _weak)
 {
    weak_memory.push_back(&_weak);
 }
 // Assign the weak_ptr to the memory block
 void free(weak_ptr<T>& _weak)
 {
        for (int i=0;i<weak_memory.size();i++)
        {
            if (weak_memory[i] == _weak)
            {
                weak_memory.erase(weak_memory.begin() + i);
                return;
            }
        }
 }
 // Free the weak_ptr from the memory block
 void free()
 {
    shared_memory.clear();
    weak_memory.clear();
    reference_count = 0; 
 }
 // Free all the shared/weak pointers and point them to nullptr
 int count()
 {
    return reference_count;
 }
 // Returns reference count
};
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
                delete block;
                block = nullptr;
                delete[]_ptr;
                _ptr = nullptr;
            }

        }
    }
    _ptr = p._ptr;
    block = p.block;
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
    return *_ptr;
 } // returns reference to what _ptr is pointing to
T& operator[](int index)
{
    if (_ptr[index] != nullptr)
        return (_ptr+index);
    //throw out_of_range("Illegal memory access");

} // returns reference to value at index
 // Also handles illegal memory access
 bool operator==(const shared_ptr<T>&p)
 {
    return _ptr == p._ptr && block == p.block;
 } // Compares two shared pointers
 static shared_ptr& make_shared()
 {
    return *new shared_ptr<T>();
 }
 // returns an instance of shared pointer object
void reset()
{
    if (block != nullptr)
    {
        block->free(*this);
        if (block->count() == 0)
        {
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
    block->assign(*this);
 }
 // Copies contents of p into our object(this)
// Assign our object to memory block
 weak_ptr& operator=(const shared_ptr<T>&p)
 {
    if (this != &p)
    {
        block->free(*this);
    }
    _ptr = p._ptr;
    block = p.block;
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
    return *_ptr;
 } // returns reference to what _ptr is pointing to
T& operator[](int index)
{
    if (_ptr[index] == nullptr)
        throw std::out_of_range("Illegal Memory access");
    return *_ptr[index];
} // returns reference to value at index
 // Also handles illegal memory access
 bool operator==(const weak_ptr<T>&p)
 {
    return _ptr == p._ptr && block == p.block;
 } // Compares two weak pointers
 };

int main()
{
    unique_ptr<int> unq_ptr = std::move(unique_ptr<int>::make_unique()); // Allocates memory for 10 integers
    std::cout<< "Debug point:";
    // unq_ptr[0] = 5;
    // std::cout<<"Unique Pointer: " << unq_ptr[0] << std::endl;
//      //std::cout<<"Between line 2 and 3";
//     shared_ptr<int> project = shared_ptr<int>::make_shared();
//      //std::cout<<"Between line 2 and 3";
//     shared_ptr<int> project2 = project;
//     //std::cout<<"Between line 2 and 3";
//     weak_ptr<int> project3 = project;
//    // std::cout<<" Entering project.count()";
//     std::cout<<" Reference Count: " << project.count() << std::endl;

    return 0;
}