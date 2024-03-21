#pragma once
#include <bits/stdc++.h>
#include "EHOGx.h"

struct HOG_BCER{
public:

    std::vector<bool>marked;
    std::vector<EHOGx_NODE> t;
    std::vector<int> leaves;
    HOG_BCER();
    HOG_BCER(const std::vector<std::string>& v);

    // void add_string(const std::string &s);
    // void add_strings(const std::vector<std::string>& v);
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
    std::vector<bool> mark_hog(int v);
    inline int get_link(int node) { return t[node].link; }
};
