#include "../include/HOG-SSP.h"
using namespace std;

void HOG::add_string(string s) {
    trie.add_string(s);
}

void HOG::calculateSplitNodes(int node, int upHere) {
    up[node] = upHere;
    vector<int> children;
    for(int i=0;i<alphabet;i++) {
        if(trie.t[node].next[i]!=-1) {
            children.push_back(trie.t[node].next[i]);
        }
    }
    subTreeCnt[node] = children.size() + trie.t[node].output;
    if(children.size() == 1 && !trie.t[node].output) {
        calculateSplitNodes(children[0], upHere);
        down[node] = down[children[0]];
    } else {
        down[node] = node;
        for(int child:children) calculateSplitNodes(child, node);
    }
}

void HOG::construct() {
    up.resize(trie.t.size(), 0);
    down.resize(trie.t.size(), 0);
    subTreeCnt.resize(trie.t.size(), 0);
    marked.resize(trie.t.size(), false);
    int root = 0;
    calculateSplitNodes(root, root);

    vector<int> subTreeLeft = subTreeCnt;
    marked[root] = true;
    stack<int> modified;
    for(int i=1;i<trie.t.size();i++) {
        if(!trie.t[i].output) continue;
        int v = i;
        while(v!=root) {
            if(subTreeLeft[down[v]] != 0) {
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