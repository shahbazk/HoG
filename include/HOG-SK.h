#pragma once

#include <bits/stdc++.h>

#include "EHOG.h"

struct HOG_SK {
    std::vector<bool> marked;
    std::vector<EHOG_NODE> t;
    std::vector<int> leaves;

    HOG_SK();
    void construct();
    void print_details(bool verbose);
    void inp(std::ifstream& in);

    private:
    std::vector<std::vector<int>> l; //list of string indexes that have the i'th node on their suffix path
    std::vector<std::stack<int>> s;
    std::vector<int> unmarked; // list of indices of stacks that have unmarked tops
    std::vector<bool> is_unmarked; // whether the top of this stack is unmarked

    void dfs(int node);
    inline int get_link(int node) { return t[node].link; }
};

