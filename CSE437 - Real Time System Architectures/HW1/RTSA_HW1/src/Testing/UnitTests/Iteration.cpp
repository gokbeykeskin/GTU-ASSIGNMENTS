#include <iostream>
#include "../../TSSet.h"
using namespace std;

int main(){
    ThreadSafeSet<int> tsset;
    for(int i=0;i<10000;i++){
        tsset.insert(i);
    }

    tsset.iterate([](int x){
        cout<<"Number:"<<x<<": ";
        if(x%3==0) cout<<"Fizz";
        if(x%5==0) cout<<"Buzz";
        cout << endl;
    });


    return 0;
}
