#pragma once
#include <bits/stdc++.h>

const int alphabet = 4;

// only for sorted strings
struct AhoxNode {
    int p; // index of parent
    int link = 0; // index of suffix link
    char pch; // character of parent edge
    bool output = false; // is this node an exact match
    int next[alphabet]= {0}; // direct transition from this node using a character
    int l = 1e9, r = -1e9;
    int str_index;
    int length = 0;
    inline AhoxNode(int par, char ch) : p(par), pch(ch) {}
    inline bool is_leaf() {return output;}
};

struct AhoCoraxsick {
    std::vector<AhoxNode> t; // the nodes in the automaton
    std::vector<int> leaves;
    int str_count;
    AhoCoraxsick();

    void add_string(std::string const& s);
    int get_link(int v);

    std::vector<int> sorted_order();
    void dfs(int v, std::vector<int>&leaves_in_order, int &leaves_visited);
};