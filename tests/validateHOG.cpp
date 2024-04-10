#include<bits/stdc++.h>

#ifdef SSP

#include "HOG-SSP.h"
typedef HOG_SSP HOG;

#elif SK

#include "HOG-SK.h"
typedef HOG_SK HOG;

#elif SP

#include "HOG-SP.h"
typedef HOG_SP HOG;

#elif BCER

#include "HOG-BCER.h"
typedef HOG_BCER HOG;

#elif EC

#include "HOG-EC.h"
typedef HOG_EC HOG;

#else

#include "HOG-SSP.h"
typedef HOG_SSP HOG;

#endif

using namespace std;

// TODO : write bigger validity test comparing dumps from both algos
void test_validity() {
    cout << "\nTesting validity of algorithm\n";
    vector<string> v = {"aabaa", "aadbd", "dbdaa"};
    AhoCorasick ahocora(v);
    EHOG ehog(ahocora);

    #ifdef SP

    HOG hog1(ahocora, v);
    HOG hog2(ehog, ahocora, v);

    #else

    HOG hog1(ahocora);
    HOG hog2(ehog);

    #endif

    assert(hog1.marked == vector<bool>({0,1,0,1,0,0,1,0,0,1,0,0,1,0,1}));
    assert(hog2.marked == vector<bool>({0,1,0,1,1,1,0,1,1}));
    cout<<"All tests passed\n";
}


int main() {
    test_validity();
    return 0;
}
