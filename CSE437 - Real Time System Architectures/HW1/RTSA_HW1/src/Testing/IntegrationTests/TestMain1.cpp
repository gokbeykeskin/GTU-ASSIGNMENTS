/*
One reader, One writer thread with random number generation (between 0-300000).
All threads terminates when the number (369) is found.
*/

#include "../../TSSet.h"
#include "../TestUtils.h"
#include <thread>
#include <iostream>
using namespace std;

ThreadSafeSet<int> tsset;




void writer();
void reader(int);
bool found=false;
int main(int argc, char** argv){

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::thread writerThread(writer);
    std::thread readerThread(reader,369);


    if(writerThread.joinable()) writerThread.join();
    if(readerThread.joinable()) readerThread.join();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    
    return 0;
}

void writer(){
    for(;;){
        int addVal = getRand(0,300000);
        tsset.insert(addVal);
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
