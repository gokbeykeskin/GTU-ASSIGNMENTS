#include <iostream>
#include "../../TSSet.h"
using namespace std;

int main(){
    ThreadSafeSet<int> tsset;

    cout<<"Trying to remove 0 from empty set, Result:"<<tsset.remove(0)<<endl;
    
    for(int i=0;i<10;i++) tsset.insert(i);

    cout<<"Initial Set:";
    tsset.iterate([](int x){
        cout<<x<<",";
    });
    cout << endl;

    cout<<"Trying to remove existing number 5, Result:" << tsset.remove(5)<<endl;
    cout<<"Trying to remove 5 again, Result:" << tsset.remove(5)<<endl;
    cout<<"Trying to remove existing number 8, Result:" << tsset.remove(8)<<endl;

    cout<<"Resulting Set:";
    tsset.iterate([](int x){
        cout<<x<<",";
    });
    cout << endl;
    cout <<"Clearing the set..."<<endl;
    tsset.clear();
    cout<<"Resulting Set:";
    tsset.iterate([](int x){
        cout<<x<<",";
    });
    cout<<endl;



}