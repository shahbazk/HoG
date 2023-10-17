#include<bits/stdc++.h>
using namespace std;

#define now chrono::system_clock::now()

// #define SSP
#ifdef SSP
#include "../include/HOG-SSP.h"
#endif

void test_validity() {
    cout << "Testing validity of algorithm\n";
    vector<string> v = {"aabaa", "aadbd", "dbdaa"};
    HOG hog(v);
    assert((hog.marked == vector<bool>({1,0,1,0,0,1,0,0,1,0,0,1,0,1})));
    cout<<"All tests passed\n\n";
}

void random_strings_stress_test(int n, int p, int seed) {
    assert(p>=n);
    int len = p/n;
    cout << "Testing on randomly generated strings...\n" << "Number of strings = " << n << ", Sum of lengths of all strings = " << p << '\n'; 
    srand(seed);
    vector<string> v;
    for(int i=0;i<n;i++) {
        string s = "";
        for(int j=0;j<len;j++) {
            s += ('a'+rand()%alphabet);
        }
        v.push_back(s);
    }
    auto start_time = now;
    HOG stress_hog(v);
    auto end_time = now;
    chrono::duration<double> elapsed_time = end_time - start_time;

    cout << "Elapsed time: " << elapsed_time.count() << "s\n";
    int cnt = 0;
    for(auto b:stress_hog.marked) cnt+=b;
    cout << "Compression factor: "<< (double)cnt/stress_hog.marked.size() << '\n';
}

void random_string_reads_stress_test(int n, int p, int rep, int seed) {
    assert(p>=n);
    assert(n>=rep);
    assert(rep>1);
    int len = p/n;
    int total_len = p/rep + len - len/rep;
    double overlap = 1.0 - 1.0/rep;
    cout << "Testing on randomly generated reads on a randomly generated string...\n" 
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
    auto start_time = now;
    HOG stress_hog(v);
    auto end_time = now;
    chrono::duration<double> elapsed_time = end_time - start_time;

    cout << "Elapsed time: " << elapsed_time.count() << "s\n";
    int cnt = 0;
    for(auto b:stress_hog.marked) cnt+=b;
    cout << "Compression factor: "<< (double)cnt/stress_hog.marked.size() << '\n';
}

int main() {
    test_validity();
    random_strings_stress_test(1000, 1e6, now.time_since_epoch().count());
    random_string_reads_stress_test(1000, 1e6, 20, now.time_since_epoch().count());
    return 0;
}