#pragma once
#include <bits/stdc++.h>
#include "Aho-Cora.h"

struct EHOG_NODE {
    int p; // index of parent
    int link = 0; // index of suffix link
    bool output = false; // is this node an exact match
    int aho_index; // index corresponding to aho-corasick tree
    int strIndex;
    std::vector<int>childs;
    
    inline EHOG_NODE(){}
    inline EHOG_NODE(int par) : p(par) {}
    EHOG_NODE(std::ifstream& in);
    inline bool is_leaf() {return output;}
    void file_output(std::ofstream& out);
    int memory_calculate();
};


struct EHOG{
    std::vector<EHOG_NODE> t; // the nodes in the automaton
    std::vector<int> leaves;
public:
    EHOG(AhoCorasick &ahotree);
    EHOG(std::ifstream& in);
    
    inline int get_link(int v) {return t[v].link;}
    void file_output(std::ofstream &out);
};
