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
    vector<int> parents, links;
    string pchs;
    vector<bool> outputs;
    for(int i=1;i<(int)aut.t.size();i++) {
        auto node = aut.t[i];
        parents.push_back(node.p);
        links.push_back(aut.get_link(i));
        pchs.push_back(node.pch);
        outputs.push_back(node.output);
    }
    assert(aut.t.size() == 10);
    assert(parents == vector<int>({0,1,2,1,4,5,1,7,8}));
    assert(links == vector<int>({1,1,4,1,7,8,1,2,2}));
    assert(pchs == "$abbcacaa");
    assert(outputs == vector<bool>({0,1,1,0,1,1,1,0,1}));
    cout<<"All tests passed"<<'\n';
    return 0;
}