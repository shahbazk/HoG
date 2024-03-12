#pragma once
#include <bits/stdc++.h>
#include "EHOG.h"

struct HOG_SP {
    std::vector<bool> marked;

    HOG_SP();
    HOG_SP(const std::vector<std::string>& v);

    void add_string(const std::string& s);
    void add_strings(const std::vector<std::string>& v);
    void construct();
    void print_details();

    private:
    std::vector<std::string> strings;
    EHOG trie;
};