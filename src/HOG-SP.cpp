#include "HOG-SP.h"
using namespace std;

HOG_SP::HOG_SP() {}

HOG_SP::HOG_SP(const vector<string>& v) {
    add_strings(v);
    construct();
}

void HOG_SP::add_string(const string& s) {
    input_trie.add_string(s);
}

void HOG_SP::add_strings(const vector<string>& v) {
    input_strings = v;
    int p = 0;
    for(auto &s:v) p += s.length();
    input_trie.leaves.reserve(v.size());
    input_trie.t.reserve(p);
    for(auto &s:v) add_string(s);
}

void HOG_SP::construct() {
    const int root = 1, p = (int)input_trie.t.size();
    const int n = input_strings.size();
    marked.resize(p, false);

    vector<vector<int>> pnode(n); // index of j'th prefix of i'th string in ACtrie
    vector<vector<int>> border(n);
    vector<int> up(p);

    // calculate pnode array ??
    for (int i = 0; i < n; i++) {
        pnode[i].resize(input_strings[i].length());
        int curr = root;
        for (int j = 0; j < input_strings[i].length(); j++) {
            curr = input_trie.t[curr].next[input_strings[i][j]-'a'];
            pnode[i][j] = curr;
        }
    }

    // calculate border array
    for(int i=0;i<n;i++) {
        border[i].resize(input_strings[i].length(), 0);
        int k=0;
        for(int j=1;j<input_strings[i].length();j++) {
            while(k>0 && input_strings[i][k] != input_strings[i][j]) k = border[i][k-1];
            if(input_strings[i][k] == input_strings[i][j]) k++;
            border[i][j] = k;

            // calculate up array
            up[pnode[i][j]] = pnode[i][border[i][j]];
        }
    }

    vector<int> R(p, 0), I(p);
    for(int leaf: input_trie.leaves) R[input_trie.t[leaf].p]++;
    for(int i=p-1;i>0;i--) {
        R[input_trie.t[i].p] += R[i];
    }

    marked[root] = true;
    vector<vector<int>> Child(p);
    for (int u : input_trie.leaves) {
        marked[u] = true;
        while(u != root) {
            I[u] = R[u];
            for (int v : Child[u]) {
                I[u] -= R[v];
            }
            if(I[u] > 0) marked[u] = true;
            Child[u].clear();
            Child[up[u]].push_back(u);
            u = input_trie.get_link(u);
        }
    }
}

void HOG_SP::print_details(){
    std::cout << "Aho-Corasick Size: " << input_trie.t.size() << "\n";
    int hsz = 0;
    for(bool a:marked)hsz+=a;
    std::cout << "HOG Size: " << hsz << "\n";
}