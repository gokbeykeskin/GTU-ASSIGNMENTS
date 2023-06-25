#include <iostream>
#include "../../TSSet.h"
using namespace std;

int main(){

    ThreadSafeSet<double> tsset;
    cout<< "Inserting 5.3, Result:"<<tsset.insert(5.3)<<endl;
    cout<< "Inserting -13.7, Result:" << tsset.insert(-13.7)<<endl;
    cout<< "Inserting 5.3 again, Result:" << tsset.insert(5.3)<<endl;
    cout<< "Inserting 0, Result:"<< tsset.insert(0)<<endl;
    cout<< "Inserting -13.7 again, Result:" <<tsset.insert(-13.7)<<endl;

    cout<<"Iterating on Resulting Set:";
    tsset.iterate([](double x){cout<<x<<"|";});
    return 0;
}