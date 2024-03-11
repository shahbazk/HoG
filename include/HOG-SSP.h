#ifndef HOG_SSP_H
#define HOG_SSP_H

#include <bits/stdc++.h>

#include "EHOG.h"

struct HOG_SSP {
    std::vector<bool> marked;

    HOG_SSP();
    HOG_SSP(const std::vector<std::string>& v);

    void add_string(const std::string& s);
    void add_strings(const std::vector<std::string>& v);
    void construct();
    void print_details();
    
    private:
    EHOG trie;
};

#endif