#include "HOG-SKx.h"
#include "trace.h"
using namespace std;

HOG_SKx::HOG_SKx() {}

HOG_SKx::HOG_SKx(const vector<string>& v) {
    add_strings(v);
    construct();
}

void HOG_SKx::add_string(const std::string& s) {
    trie.add_string(s);
}

void HOG_SKx::add_strings(const vector<string>& v) {
    int p = 0;
    for(auto &s:v) p += s.length();
    trie.leaves.reserve(v.size());
    trie.t.reserve(p);
    for(auto &s:v) add_string(s);
}

void HOG_SKx::construct() {
    //construct l
    order = trie.sorted_order();
    sorted_order_conversion.resize(order.size());
    for(int i = 0;i<(int)order.size();i++){
        sorted_order_conversion[order[i]] = i;
    }
    int root = 1;
    l.resize(trie.t.size()); //initialise l with empty lists
    for(int i=0;i<(int)trie.leaves.size();i++) {
        int curr = trie.get_link(trie.leaves[i]);
        while(curr != root) { // add to the list of each node on suffix path, except the leaf itself
            l[curr].push_back(i);
            curr = trie.get_link(curr);
        }
    }
    marked.resize(trie.t.size(), false); // inH
    marked[root] = true; //root is in HOG
    s.resize(trie.leaves.size());
    is_unmarked.resize(trie.leaves.size(), false);
    dfs(root);
}

void HOG_SKx::dfs(int node) {
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

Hog HOG_SKx::make_hog(){
    Hog hog_tree;
    hog_tree.sorted_order_conversion = sorted_order_conversion;
    hog_tree._strToTreeIndex.resize(sorted_order_conversion.size());
    hog_tree.sorted_order = order;
    vector<int>suf_links(marked.size(),-1);
    vector<int>aho_to_hog(marked.size());
    for(int i = 1;i<(int)marked.size();i++){
        if(marked[i] == true){
            int suf_node = trie.get_link(i);
            vector<int>path;
            while(true){
                if(marked[suf_node]){
                    break;
                }
                if(suf_links[suf_node] != -1){
                    suf_node = suf_links[suf_node];
                    break;
                }
                else{
                    path.push_back(suf_node);
                    suf_node = trie.get_link(suf_node);
                }
                
            }
            for(int u:path)suf_links[u] = suf_node;
            aho_to_hog[i] = hog_tree.add_node(aho_to_hog[trie.t[i].p], suf_node,trie.t[i].length,trie.t[i].l,trie.t[i].r);
            if(trie.t[i].is_leaf()){
                hog_tree._strToTreeIndex[sorted_order_conversion[trie.t[i].str_index]] = aho_to_hog[i]; 
            }
        }
    }
    for(int i = 1;i<(int)hog_tree.t.size();i++){
        hog_tree.t[i].suf_set(aho_to_hog[hog_tree.t[i].suf_link()]);
    }
    return hog_tree;
}