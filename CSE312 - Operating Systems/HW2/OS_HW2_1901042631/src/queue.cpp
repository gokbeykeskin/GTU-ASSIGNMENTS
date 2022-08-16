#include "queue.h"

queue::queue(){
    front = 0;
    rear = -1;
    capacity = 50;
    size_ = 0;
}

int queue::size(){
    return size_;
}

bool queue::isEmpty(){
    return size_ == 0;
}

int queue::peek(){
    return arr[front];
}

int queue::dequeue(){
    int temp = arr[front];
    front = (front + 1) % capacity;
    size_--;
    return temp;
}

void queue::enqueue(int item){
    rear = (rear + 1) % capacity;
    arr[rear] = item;
    size_++;
}