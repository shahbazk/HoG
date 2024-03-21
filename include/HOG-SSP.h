#pragma once

#include <bits/stdc++.h>

#include "EHOG.h"

struct HOG_SSP {
    std::vector<bool> marked;
    std::vector<EHOG_NODE> t;
    std::vector<int> leaves;

    HOG_SSP();
    void construct();
    void print_details(bool verbose);
    void inp(std::ifstream& in){
        int treeSize, leavesSize;
        in>>treeSize;
        t.resize(treeSize);
        for(int i = 0;i<treeSize;i++)t[i].inp(in);
        in>>leavesSize;
        leaves.resize(leavesSize);
        for(int i = 0;i<leavesSize;i++)in>>leaves[i];
    }

    private:
      inline int get_link(int node) { return t[node].link; }
};

