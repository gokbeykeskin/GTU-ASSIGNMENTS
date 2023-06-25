#ifndef TSSet_CPP
#define TSSet_CPP

#include "TSSet.h"
#include <iostream>
#include <mutex>

using namespace std;

template <typename T>
ThreadSafeSet<T>::ThreadSafeSet(){}

template <typename T>
bool ThreadSafeSet<T>::insert(const T& item){
    std::unique_lock<mutex> lock(m);
    if(search(item))
        return false;
    
    elements.insert(item);
    return true;
}

template <typename T>
bool ThreadSafeSet<T>::insert(T&& item){
    return insert(item);
}

template <typename T>
bool ThreadSafeSet<T>::remove(const T item){
    std::unique_lock<mutex> lock(m);
    if(!search(item)) return false;
    elements.erase(elements.find(item));
    return true;
}

template <typename T>
void ThreadSafeSet<T>::clear(){
    std::unique_lock<mutex> lock(m);
    elements.clear();
}

template <typename T>
int ThreadSafeSet<T>::size(){
    return elements.size();
}

template <typename T>
void ThreadSafeSet<T>::iterate(std::function<void(T)> func){
    typename set<T>::iterator itr;
    for(itr = elements.begin();itr != elements.end();itr++){
        func(*itr);
    }
}

template <typename T>
bool ThreadSafeSet<T>::search(T item){
    return elements.find(item) != elements.end();
}

#endif