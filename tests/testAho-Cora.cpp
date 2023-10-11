#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;

#include "../include/trace.h"
#include "../include/Aho-Cora.h"

int main() {
    vector<string> s = {"a", "ab", "bc", "bca", "c", "caa"};
    AhoCorasick aut;
    for(int i=0;i<s.size();i++) {
        aut.add_string(s[i]);
    }
    assert(aut.t.size() == 9);
    //verify more stuff like links and transitions
    cout<<"All tests passed";
    return 0;
}