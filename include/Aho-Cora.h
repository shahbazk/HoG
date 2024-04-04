#pragma once

#include <bits/stdc++.h>

const int alphabet = 4;

struct AhoNode {
    int p; // index of parent
    int link = 0; // index of suffix link
    char pch; // character of parent edge
    int strIndex = -1;
    int next[alphabet]= {0}; // direct transition from this node using a character

    inline AhoNode(int par, char ch) : p(par), pch(ch) {}
    AhoNode(std::ifstream &in);
    void file_output(std::ofstream &out);
    inline bool is_leaf() { 
        if(strIndex != -1)return true;
        else return false;
    }
};

struct AhoCorasick {
    std::vector<AhoNode> t; // the nodes in the automaton
    std::vector<int> leaves;

    AhoCorasick();
    AhoCorasick(const std::vector<std::string> &v);
    AhoCorasick(std::ifstream& in);
    void add_string(std::string const& s);
    int get_link(int v);
    void file_output(std::ofstream& out);
    long long memory_required();
};

