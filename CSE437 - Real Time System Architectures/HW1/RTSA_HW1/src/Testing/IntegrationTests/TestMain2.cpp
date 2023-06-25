/*
One reader, One writer thread with sequential number generation (0 to 300000).
Terminates when the number (256178) is found.
*/

#include "../../TSSet.h"
#include <thread>
#include <random>
#include <iostream>

using namespace std;

ThreadSafeSet<int> tsset;



void writer();
void reader(int);
bool found=false;

int main(int argc, char** argv){

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    std::thread writerThread(writer);
    std::thread readerThread(reader,256178);


    if(writerThread.joinable()) writerThread.join();
    if(readerThread.joinable()) readerThread.join();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    
    return 0;
}

void writer(){
    for(int i=0;i<300000;i++){
        tsset.insert(i);
        if(found) return;
    }
}
void reader(int searchNum){
    for(;;){
        found = tsset.search(searchNum);
        //cout <<"Searching "<<searchNum<<":"<< found <<endl;
        if(found) return;
    }
}
        