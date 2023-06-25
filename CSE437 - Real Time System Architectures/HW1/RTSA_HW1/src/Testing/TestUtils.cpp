

#include <random>
#include "TestUtils.h"

using namespace std;

int getRand(int lower, int upper) {                                                                                                                                                            
    std::random_device rd;                                                                                                                                                                    
    std::mt19937_64 mt(rd());                                                                                                                                                                 
    std::uniform_int_distribution<int> dist(lower,upper);                                                                                                                                     
    return dist(mt);                                                                                                                                                                          
}               