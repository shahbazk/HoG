#include <bits/stdc++.h>

#include "../include/Aho-Cora.h"

struct HOG_SSP {
    std::vector<int> up, down, subTreeCnt;
    std::vector<bool> marked;
    AhoCorasick trie;

    HOG_SSP();
    HOG_SSP(const std::vector<std::string>& v);

    void add_string(const std::string& s);
    void calculateSplitNodes(int node, int upHere);
    void construct();
};