#include "HOG-SP.h"
using namespace std;

HOG_SP::HOG_SP() {}

HOG_SP::HOG_SP(const vector<string>& v) {
    add_strings(v);
    construct();
}

void HOG_SP::add_string(const string& s) {
    trie.trie.add_string(s);
}

void HOG_SP::add_strings(const vector<string>& v) {
    input_strings = v;
    int p = 0;
    for(auto &s:v) p += s.length();
    trie.trie.leaves.reserve(v.size());
    trie.trie.t.reserve(p);
    for(auto &s:v) add_string(s);
    trie.construct();
}

void HOG_SP::construct() {
    const int root = 1, p = (int)trie.t.size();
    const int n = input_strings.size();
    marked.resize(p, false);

    // calculate pnode array ??

    // calculate border array
    vector<vector<int>> border(n), pnode(n);
    vector<int> up(p);
    for(int i=0;i<n;i++) {
        border[i].resize(input_strings[i].size(), 0);
        int k=0;
        for(int j=1;j<input_strings[i].size();j++) {
            while(k>0 && input_strings[i][k] != input_strings[i][j]) k = border[i][k-1];
            if(input_strings[i][k] == input_strings[i][j]) k++;
            border[i][j] = k;

            // calculate up array
            up[pnode[i][j]] = pnode[i][border[i][j]];
        }
    }

    vector<int> R(p, 0), I(p);
    for(int leaf: trie.leaves) R[trie.t[leaf].p]++;
    for(int i=p-1;i>0;i--) {
        R[trie.t[i].p] += R[i];
    }

    marked[root] = true;
    vector<vector<int>> Child(p);
    for (int u : trie.leaves) {
        marked[u] = true;
        while(u != root) {
            I[u] = R[u];
            for (int v : Child[u]) {
                I[u] -= R[v];
            }
            if(I[u] > 0) marked[u] = true;
            Child[u].clear();
            Child[up[u]].push_back(u);
            u = trie.get_link(u);
        }
    }
}

void HOG_SP::print_details(){
    std::cout << "Aho-Corasick Size: " << trie.trie.t.size() << "\n";
    std::cout << "EHOG Size: " << trie.t.size() << "\n";
    int hsz = 0;
    for(bool a:marked)hsz+=a;
    std::cout << "HOG Size: " << hsz << "\n";
}