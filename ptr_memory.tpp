#include"ptr_memory.h"
#include<iostream>
#include<vector>
#include"shared_ptr.h"
#include"weak_ptr.h"

using namespace std;
template <class T>
int memory<T>::count()
{
    return reference_count;
}

template <class T>
void memory<T>::free()
{
    for(auto p:weak_memory)
    {
        p->reset();
    }
    weak_memory.clear();
    reference_count = 0; 
}

template <class T>
void memory<T>::free(weak_ptr<T>& _weak)
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

template <class T>
void memory<T>::assign(weak_ptr<T>& _weak)
{
weak_memory.push_back(&_weak);
}

template <class T>
void memory<T>::free(shared_ptr<T>& _shared)
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

template <class T>
 void memory<T>::assign(shared_ptr<T>& _shared)
 {
    

    shared_memory.push_back(&_shared);
    reference_count++;
    
 }