#include <bits/stdc++.h>
using namespace std;
 
typedef long long ll;

#include "trace.h"
#include "Aho-Cora.h"

int main() {
    vector<string> v = {"a", "ab", "bc", "bca", "c", "caa"};
    AhoCorasick aut;
    for(string &s:v) {
        aut.add_string(s);
    }
    assert(aut.t.size() == 10);
    //verify more stuff like links and transitions
    cout<<"All tests passed"<<'\n';
    return 0;
}