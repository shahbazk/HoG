#include <bits/stdc++.h>

#include "Aho-Cora.h"

struct HOG_SK {
    AhoCorasick trie;
    std::vector<bool> marked;
    std::vector<std::vector<int>> l; //list of string indexes that have the i'th node on their suffix path
    std::vector<std::stack<int>> s;
    std::vector<int> unmarked; // list of indices of stacks that have unmarked tops
    std::vector<bool> is_unmarked; // whether the top of this stack is unmarked
    
    HOG_SK();
    HOG_SK(const std::vector<std::string>& v);

    void add_string(const std::string& s);
    void construct();
    void dfs(int node);
};