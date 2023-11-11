#include "HOG-SSP.h"

using namespace std;

HOG_SSP::HOG_SSP() {}

HOG_SSP::HOG_SSP(const vector<string>& v) {
    add_strings(v);
    construct();
}

void HOG_SSP::add_string(const string& s) {
    trie.add_string(s);
}

void HOG_SSP::add_strings(const vector<string>& v) {
    int p = 0;
    for(auto &s:v) p += s.length();
    trie.leaves.reserve(v.size());
    trie.t.reserve(p);
    for(auto &s:v) add_string(s);
}

void HOG_SSP::construct() {
    const int root = 1, p = (int)trie.t.size();
    marked.resize(p, false);

    vector<int> up(p), down(p), subTreeCnt(p, 0), oneChild(p);
    for(int i:trie.leaves) subTreeCnt[trie.t[i].p] = 1;
    for(int i=p-1;i>=root;i--) {
        if(subTreeCnt[i] == 1) down[i] = down[oneChild[i]];
        else down[i] = i;
        subTreeCnt[trie.t[i].p]++;
        oneChild[trie.t[i].p] = i;
    }
    up[root] = root;
    for(int i=root+1;i<p;i++) {
        if(subTreeCnt[trie.t[i].p] == 1) up[i] = up[trie.t[i].p];
        else up[i] = trie.t[i].p;
    }

    vector<int> subTreeLeft(subTreeCnt);
    marked[root] = true; //root is implicitly marked
    vector<int> modified;
    for(int i:trie.leaves) {
        marked[i] = true; //leaves are implicitly marked
        int v = trie.get_link(i); // iterate over proper suffixes of i, that are prefix (may not be proper) of some string
        while(v!=root) {
            if(subTreeLeft[down[v]] != 0) { //if the subtree of v (including v) contains a split node having subtrees left
                marked[v] = true;
                subTreeLeft[down[v]] = 0;
                modified.push_back(down[v]);
                int u = up[v];
                while(true) {
                    subTreeLeft[u]--; //remove subtree of u containing v
                    modified.push_back(u);
                    if((u==root) || (subTreeLeft[u] != 0)) break; // break if u is root or subtree of u has not been completely removed
                    u = up[u];
                }
            }
            v = trie.get_link(v);
        }
        for(int x:modified) subTreeLeft[x] = subTreeCnt[x];
        modified.clear();
    }
}