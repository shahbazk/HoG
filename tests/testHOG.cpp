#include<bits/stdc++.h>
using namespace std;

#ifdef SSP
#include "../include/HOG-SSP.h"
#endif

int main() {
    vector<string> s = {"aabaa", "aadbd", "dbdaa"};
    HOG hog;

    for(string &str:s) hog.add_string(str);
    hog.construct();
    assert((hog.marked == vector<bool>({1,0,1,0,0,1,0,0,1,0,0,1,0,1})));
    cout<<"All tests passed\n";
    return 0;
}