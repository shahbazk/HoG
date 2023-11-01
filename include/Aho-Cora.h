#ifndef AHO_CORA_H
#define AHO_CORA_H

#include <bits/stdc++.h>

const int alphabet = 4;

struct AhoNode {
    std::vector<int> next; // direct transition from this node using a character
    bool output = false; // is this node an exact match
    int p = -1; // index of parent
    char pch; // character of parent edge
    int link = -1; // index of suffix link
    std::vector<int> go; // transition from this node

    AhoNode(int p, char ch);
};

struct AhoCorasick {
    std::vector<AhoNode> t; // the nodes in the automaton

    AhoCorasick();

    void add_string(std::string const& s);

    int go(int v, char ch);

    int get_link(int v);
};


#endif
 