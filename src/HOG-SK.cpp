#include "HOG-SK.h"

using namespace std;

HOG_SK::HOG_SK() {}

HOG_SK::HOG_SK(const vector<string>& v) {
    for(auto &s:v) add_string(s);
    construct();
}

void HOG_SK::add_string(const std::string& s) {
    trie.add_string(s);
}

void HOG_SK::construct() {
    //construct l
    int leaf_index = 0; //number of leaves found
    int root = 0;
    l.resize(trie.t.size()); //initialise l with empty lists
    for(int i=1;i<(int)trie.t.size();i++) {
        if(trie.t[i].is_leaf()) { // if i is a leaf
            int curr = trie.get_link(i);
            while(curr != root) { // add to the list of each node on suffix path, except the leaf itself
                l[curr].push_back(leaf_index);
                curr = trie.get_link(curr);
            }
            leaf_index++; // increment number of leaves found
        }
    }
    marked.resize(trie.t.size(), false); // inH
    marked[root] = true; //root is in HOG
    s.resize(leaf_index);
    is_unmarked.resize(leaf_index, false);
    dfs(root);
}

void HOG_SK::dfs(int node) {
    if(trie.t[node].is_leaf()) {
        marked[node] = true; // leaves are implicitly in HOG
        for(int x:unmarked) { // iterate over all stacks with unmarked tops
            if(is_unmarked[x]) {
                marked[s[x].top()] = true;
                is_unmarked[x] = false;
            }
        }
    }

    // node visited for the first time
    for(int x:l[node]) {
        s[x].push(node);
        if(!is_unmarked[x]) {
            is_unmarked[x] = true;
            unmarked.push_back(x);
        }
    }
    for(int i=0;i<alphabet;i++) {
        if(trie.t[node].next[i] != -1) {
            dfs(trie.t[node].next[i]);
        }
    }

    // node visited for the last time
    for(int x:l[node]) {
        s[x].pop();
        if((!s[x].empty()) && (!marked[s[x].top()])) {
            if(!is_unmarked[x]) {
                is_unmarked[x] = true;
                unmarked.push_back(x);
            }
        } else {
            is_unmarked[x] = false;
        }
    }
}