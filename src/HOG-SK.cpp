#include "HOG-SK.h"
#include "timer.h"

using namespace std;

HOG_SK::HOG_SK() {}

HOG_SK::HOG_SK(const vector<string>& v) {
    cout<<"Building Aho-Corasick automaton..."; cout.flush();
    timer ahocora_t;
    int p = 0;
    for(auto &s:v) p += s.length();
    trie.t.reserve(p);
    for(auto &s:v) add_string(s);
    ahocora_t.end();
    
    cout<<"Constructing HOG..."; cout.flush();
    timer hog_t;
    construct();
    hog_t.end();
}

void HOG_SK::add_string(const std::string& s) {
    trie.add_string(s);
}

void HOG_SK::construct() {
    //construct l
    int leaf_index = 0; //number of leaves found
    int root = 1;
    l.resize(trie.t.size()); //initialise l with empty lists
    for(int i=root+1;i<(int)trie.t.size();i++) {
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
        unmarked.clear();
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
        if(trie.t[node].next[i] != 0) {
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