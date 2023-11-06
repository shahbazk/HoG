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
    int go[alphabet] = {0}; // best possible transition from this node

    AhoNode(int p, char ch);
    bool is_leaf();
};

struct AhoCorasick {
    std::vector<AhoNode> t; // the nodes in the automaton

    AhoCorasick();

    void add_string(std::string const& s);
    int go(int v, char ch);
    int get_link(int v);
};


#endif
 