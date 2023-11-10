#ifndef HOG_SSP_H
#define HOG_SSP_H

#include <bits/stdc++.h>

#include "Aho-Cora.h"

struct HOG_SSP {
    std::vector<bool> marked;

    HOG_SSP();
    HOG_SSP(const std::vector<std::string>& v);

    void add_string(const std::string& s);
    void add_strings(const std::vector<std::string>& v);
    void construct();

    private:
    AhoCorasick trie;
    // can convert subTreeCnt to char array to save 3*p bytes, but int array is probably faster on modern machines
    std::vector<int> up, down, subTreeCnt;
    
    void calculateSplitNodes(int node, int upHere);
};

#endif