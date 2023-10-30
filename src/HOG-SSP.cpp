#include "../include/HOG-SSP.h"
using namespace std;

HOG_SSP::HOG_SSP() {}

HOG_SSP::HOG_SSP(const vector<string>& v) {
    for(const string &s:v) add_string(s);
    construct();
}

void HOG_SSP::add_string(const string& s) {
    trie.add_string(s);
}

void HOG_SSP::calculateSplitNodes(int node, int upHere) {
    up[node] = upHere;
    vector<int> children;
    for(int i=0;i<alphabet;i++) {
        if(trie.t[node].next[i]!=-1) {
            children.push_back(trie.t[node].next[i]);
            if(trie.t[trie.t[node].next[i]].output) subTreeCnt[node]++;
        }
    }
    subTreeCnt[node] += children.size();
    if(subTreeCnt[node] == 1) {
        calculateSplitNodes(children[0], upHere);
        down[node] = down[children[0]];
    } else {
        down[node] = node;
        for(int child:children) calculateSplitNodes(child, node);
    }
}

//need to fix issue: overlaps should be proper suffix and proper prefix

void HOG_SSP::construct() {
    up.resize(trie.t.size(), 0);
    down.resize(trie.t.size(), 0);
    subTreeCnt.resize(trie.t.size(), 0);
    marked.resize(trie.t.size(), false);
    int root = 0;
    calculateSplitNodes(root, root);

    vector<int> subTreeLeft = subTreeCnt;
    marked[root] = true; //root is implicitly marked
    stack<int> modified;
    for(int i=1;i<(int)trie.t.size();i++) {
        if(!trie.t[i].output) continue;
        marked[i] = true; //leaves are implicitly marked
        int v = trie.get_link(i); // iterate over proper suffixes of i, that are prefix (may not be proper) of some string
        while(v!=root) {
            if(subTreeLeft[down[v]] != 0) { //if the subtree of v (including v) contains a split node having subtrees left
                marked[v] = true;
                subTreeLeft[down[v]] = 0;
                modified.push(down[v]);
                int u = up[v];
                while(true) {
                    subTreeLeft[u]--;
                    modified.push(u);
                    if((u==root) || (subTreeLeft[u] != 0)) break;
                    u = up[u];
                }
            }
            v = trie.get_link(v);
        }
        while(!modified.empty()) {
            subTreeLeft[modified.top()] = subTreeCnt[modified.top()];
            modified.pop();
        }
    }
}