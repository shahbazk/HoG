#ifndef HOG_SK_H
#define HOG_SK_H

#include <bits/stdc++.h>

#include "Aho-Corax.h"
#include "../apsphog/include/Hog.h"

struct HOG_SKx {
    std::vector<bool> marked;
    std::vector<int>sorted_order_conversion;
    
    HOG_SKx();
    HOG_SKx(const std::vector<std::string>& v);

    void add_string(const std::string& s);
    void add_strings(const std::vector<std::string>& v);
    void construct();
    Hog make_hog();

    private:
    AhoCoraxsick trie;
    std::vector<std::vector<int>> l; //list of string indexes that have the i'th node on their suffix path
    std::vector<std::stack<int>> s;
    std::vector<int> unmarked; // list of indices of stacks that have unmarked tops
    std::vector<bool> is_unmarked; // whether the top of this stack is unmarked
    std::vector<int>order;
    void dfs(int node);
    
};

#endif