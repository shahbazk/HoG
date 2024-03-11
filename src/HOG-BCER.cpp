#include "HOG-BCER.h"
#include "trace.h"
using namespace std;

HOG_BCER::HOG_BCER() {}

HOG_BCER::HOG_BCER(const vector<string>& v) {
    add_strings(v);
    construct();
}

void HOG_BCER::add_string(const std::string& s) {
    _etrie.trie.add_string(s);
}

void HOG_BCER::add_strings(const vector<string>& v) {
    int p = 0;
    for(auto &s:v) p += s.length();
    _etrie.trie.leaves.reserve(v.size());
    _etrie.trie.t.reserve(p);
    for(auto &s:v) add_string(s);
    _etrie.construct();
}

void HOG_BCER::construct() {
    vector<bool>bHOG(_etrie.t.size(), false);
    _etrie.build_rl();
    marked.resize(_etrie.t.size());
    mark_hog(1);
    // marked.resize(_etrie.trie.t.size());
    // for(int i = 0;i<marked_ehog.size();i++){
    //     marked[_etrie.t[i].aho_index] = marked_ehog[i];
    // }
}

vector<bool> HOG_BCER::mark_hog(int v){
    if(_etrie.t[v].is_leaf()){
        vector<bool>C(_etrie.leaves.size(), false);
        marked[v] = true;
        return C;
    }
    vector<bool> C(_etrie.leaves.size(), true);
    for(int u:_etrie.t[v].childs){
        vector<bool> temp = mark_hog(u);
        for(int i = 0;i<C.size();i++){
            C[i] = C[i]&temp[i];
        }
    }
    for(int x:_etrie.t[v].rl){
        if(C[x] == false)marked[v] = true;
        C[x] = true;
    }
    return C;
}