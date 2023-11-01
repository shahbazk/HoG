#include<bits/stdc++.h>
using namespace std;

#include "../include/timer.h"

#ifdef SSP

#include "../include/HOG-SSP.h"
typedef HOG_SSP HOG;

#else

#include "../include/HOG-SK.h"
typedef HOG_SK HOG;

#endif

void test_validity() {
    cout << "\nTesting validity of algorithm\n";
    vector<string> v = {"aabaa", "aadbd", "dbdaa"};
    HOG hog(v);
    assert((hog.marked == vector<bool>({1,0,1,0,0,1,0,0,1,0,0,1,0,1})));
    cout<<"All tests passed\n";
}

void stress_test_with(vector<string>& v) {
    timer t;
    HOG stress_hog(v);
    t.end();

    int cnt = 0;
    for(auto b:stress_hog.marked) cnt+=b;
    cout << "Size of Aho-Corasick trie: " << stress_hog.marked.size() << ", Size of HOG: " << cnt
         << ", Compression factor: "<< (double)cnt/stress_hog.marked.size() << '\n';
}

void random_strings_stress_test(int n, int p, int seed) {
    assert(p>=n);
    int len = p/n;
    cout << "\nTesting on randomly generated strings...\n" 
        << "Number of strings = " << n << ", Sum of lengths of all strings = " << p << '\n'; 
    srand(seed);
    vector<string> v;
    for(int i=0;i<n;i++) {
        string s = "";
        for(int j=0;j<len;j++) {
            s += ('a'+rand()%alphabet);
        }
        v.push_back(s);
    }

    stress_test_with(v);
}

void random_string_reads_stress_test(int n, int p, int rep, int seed) {
    assert(p>=n);
    assert(n>=rep);
    assert(rep>1);
    int len = p/n;
    int total_len = p/rep + len - len/rep;
    double overlap = 1.0 - 1.0/rep;
    cout << "\nTesting on randomly generated reads on a randomly generated string...\n" 
    << "Number of strings = " << n << ", Sum of lengths of all strings = " << p 
    << ", Expected overlap = " << overlap << '\n';
    
    srand(seed);
    string complete_string = "";
    for(int i=0;i<total_len;i++) complete_string += ('a' + rand()%alphabet);
    vector<string> v;
    // might miss a few ending characters
    for(double i=0;(int)i<=total_len-len;i+=(double)len/rep) {
        v.push_back(complete_string.substr((int)i,len));
    }

    stress_test_with(v);
}

int main() {
    int seed = chrono::system_clock::now().time_since_epoch().count();
    #ifdef SSP
        cout<<"\nUsing algo by SSP...\n";
    #else 
        cout<<"\nUsing algo by SK...\n";
    #endif
    test_validity();
    random_strings_stress_test(1000, 1e6, seed);
    random_string_reads_stress_test(1000, 1e6, 20, seed);
    return 0;
}