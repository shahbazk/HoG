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
    trie.t.reserve(p);
    for(auto &s:v) add_string(s);
}

void HOG_SSP::calculateSplitNodes(int node, int upHere) {
    up[node] = upHere;
    vector<int> children;
    for(int child:trie.t[node].next) {
        if(child != 0) {
            children.push_back(child);
            subTreeCnt[node]++;
            if(trie.t[child].is_leaf()) subTreeCnt[node]++; //treat leaves as being an extra subtree
        }
    }
    if(subTreeCnt[node] == 1) { // node has exactly one child and that child is not a leaf
        calculateSplitNodes(children[0], upHere);
        down[node] = down[children[0]];
    } else {
        down[node] = node;
        for(int child:children) calculateSplitNodes(child, node);
    }
}

void HOG_SSP::construct() {
    up.resize(trie.t.size(), 0);
    down.resize(trie.t.size(), 0);
    subTreeCnt.resize(trie.t.size(), 0);
    marked.resize(trie.t.size(), false);
    int root = 1;
    calculateSplitNodes(root, root);

    vector<int> subTreeLeft = subTreeCnt;
    marked[root] = true; //root is implicitly marked
    vector<int> modified;
    for(int i=root+1;i<(int)trie.t.size();i++) {
        if(!trie.t[i].is_leaf()) continue;
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