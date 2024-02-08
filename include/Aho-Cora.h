#ifndef AHO_CORA_H
#define AHO_CORA_H

#include <bits/stdc++.h>

const int alphabet = 4;

struct AhoNode {
    int p; // index of parent
    int link = 0; // index of suffix link
    char pch; // character of parent edge
    bool output = false; // is this node an exact match
    int next[alphabet]= {0}; // direct transition from this node using a character

    inline AhoNode(int par, char ch) : p(par), pch(ch) {}
    inline bool is_leaf() {return output;}
};

struct AhoCorasick {
    std::vector<AhoNode> t; // the nodes in the automaton
    std::vector<int> leaves;

    AhoCorasick();

    void add_string(std::string const& s);
    int get_link(int v);
};


#endif
 