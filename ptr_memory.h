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
            if (weak_memory[i] == &_weak)
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
 #endif