#include <iostream>
#include "../../TSSet.h"
using namespace std;

int main(){
    ThreadSafeSet<int> tsset;
    cout << "Searching 5 on empty set, Result:" << tsset.search(5) << endl;
    cout << "Adding numbers 0 to 10 to set..." << endl;
    for(int i=0;i<10;i++) tsset.insert(i);
    cout << "Searching 5 on new set, Result:" << tsset.search(5) << endl;
}