#include <bits/stdc++.h>

#include "../include/trace.h"
#include "../include/Aho-Cora.h"

struct HOG {
    std::vector<int> up, down, subTreeCnt;
    std::vector<bool> marked;
    AhoCorasick trie;

    HOG();
    HOG(const std::vector<std::string>& v);

    void add_string(const std::string& s);
    void calculateSplitNodes(int node, int upHere);
    void construct();
};