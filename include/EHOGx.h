#pragma once
#include <bits/stdc++.h>
#include "Aho-Corax.h"


struct EHOGx_NODE {
    int p; // index of parent
    int link = 0; // index of suffix link
    bool output = false; // is this node an exact match
    int aho_index; // index corresponding to aho-corasick tree
    std::vector<int>childs;
    std::vector<int>rl;
    int l, r;
    
    inline EHOGx_NODE(){}
    inline EHOGx_NODE(int par) : p(par) {}
    inline bool is_leaf() {return output;}
    // inline bool is_leaf() {return output;}
    void dump(std::ofstream& out){
        out<<p<<" "<<link<<" "<<output<<" "<<l<<" "<<r<<" ";
        out<<childs.size()<<" ";
        for(int a:childs)out<<a<<" ";
        out << rl.size()<<" ";
        for(int a:rl)out << a << " ";
        // out<<"\n";
    }
    void inp(std::ifstream& in){
        in>>p>>link>>output>>l>>r;
        int numChild;
        in>>numChild;
        childs.resize(numChild);
        for(int i = 0;i<numChild;i++){in>>childs[i];}
        in >> numChild;
        rl.resize(numChild);
        for(int i = 0;i<numChild;i++)in>>rl[i];
    }
    int memory_calculate(){
        return childs.size()*sizeof(int) + rl.size()*sizeof(int);
    }
};


struct EHOGx{
    AhoCoraxsick trie;
    std::vector<bool>marked;
    std::vector<EHOGx_NODE> t; // the nodes in the automaton
    std::vector<int>conversion; // ehog node equivalent to which aho corasik node;
    std::vector<int> leaves;

    EHOGx();

    void construct();
    void build_rl();
    void dfs(int v, int par);
    inline int get_link(int v) {return t[v].link;}
    void dump(std::ofstream& out);
    void add_string(const std::string& s);
    void add_strings(const std::vector<std::string>& v) ;
};
