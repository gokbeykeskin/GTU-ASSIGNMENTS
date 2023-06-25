#ifndef TSSetH_H
#define TSSetH_H

#include <set>
#include <functional>
#include <mutex>

template <typename T>
class ThreadSafeSet{
public:
    ThreadSafeSet();
    bool insert(const T& item);
    bool insert(T&& item);
    bool search(T item);
    bool remove(T item);
    void clear();
    int size();
    void iterate(std::function<void(T)>);

private:
    std::set<T> elements;
    std::mutex m;
};

#include "TSSet.cpp"

#endif //TSSET_H