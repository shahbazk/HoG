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
    void inp(std::ifstream& in);

    private:
    inline int get_link(int node) { return t[node].link; }
};

