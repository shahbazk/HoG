#include <bits/stdc++.h>

#include "../include/trace.h"
#include "../include/Aho-Cora.h"

struct HOG {
    std::vector<int> up, down, subTreeCnt;
    std::vector<bool> marked;
    AhoCorasick trie;

    void add_string(std::string s);

    void calculateSplitNodes(int node, int upHere);

    void construct();
};