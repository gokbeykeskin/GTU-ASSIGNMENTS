/*
Multiple writer (10), Multiple reader(20) threads.
Writers adds random numbers (0-300000) to set.
10 reader threads searches for the numbers 0 to 10
10 reader thread iterates through the set and prints them over and over again.
All threads terminates when all 10 numbers are found.
*/

#include "../../TSSet.h"
#include "../TestUtils.h"
#include <thread>
#include <iostream>

using namespace std;

ThreadSafeSet<int> tsset;

void writer();
void reader(int);
void reader2();
int foundNumAmt=0;
thread_local bool found=false;
int main(int argc, char** argv){


    std::thread writerThreads[10];
    std::thread readerThreads[10];
    std::thread reader2Threads[10];

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    for(int i=0;i<10;i++){
        writerThreads[i] = std::thread(writer);
        readerThreads[i] = std::thread(reader,i);
        reader2Threads[i] = std::thread(reader2);
    }


    for(int i=0;i<10;i++){
        if(writerThreads[i].joinable()) writerThreads[i].join();
        if(readerThreads[i].joinable()) readerThreads[i].join();
        if(reader2Threads[i].joinable()) reader2Threads[i].join();

    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;

    return 0;
}

void writer(){
    for(;;){
        tsset.insert(getRand(0,300000));
        if(foundNumAmt==10) return;
    }
}
void reader(int searchNum){
    for(;;){
        found = tsset.search(searchNum);
        if(found) foundNumAmt++;
        cout <<"Searching "<<searchNum<<":"<< found <<endl;
        if(found) return;
    }
}

void reader2(){
    tsset.iterate([](int num){cout<<"Set contains:"<<num<<endl;});
    if(foundNumAmt==10) return;
}          