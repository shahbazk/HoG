#include "EHOGx.h"
using namespace std;

EHOGx::EHOGx() {}

void EHOGx::add_string(const std::string& s) {
    trie.add_string(s);
}

void EHOGx::add_strings(const vector<string>& v) {
    int p = 0;
    for(auto &s:v) p += s.length();
    trie.leaves.reserve(v.size());
    trie.t.reserve(p);
    for(auto &s:v) add_string(s);
    construct();
    build_rl();
}

void EHOGx::construct(){
    trie.sorted_order();
    marked.resize(trie.t.size());
    for(int v: trie.leaves){
        int temp = v;
        while(temp!=1){
            marked[temp] = true;
            temp = trie.get_link(temp);
        }
    }
    t.emplace_back(-1); // garbage node   
    marked[1] = true;
    conversion.resize(trie.t.size());
    dfs(1, 0);
    for(int i = 1;i<(int)t.size();i++){
        t[i].link = conversion[trie.get_link(t[i].aho_index)];
    }
}

void EHOGx::build_rl(){
    for(int v:leaves){
        int temp = v;
        int str_index = trie.t[t[v].aho_index].str_index;
        while(temp!=1){
            t[temp].rl.push_back(str_index);   
            temp = get_link(temp); 
        }
        t[1].rl.push_back(str_index); 
    }
}

void EHOGx::dfs(int v, int par){
    if(marked[v] == true){
        int eind = t.size();
        t[par].childs.push_back(eind);
        t.emplace_back(par);
        t[eind].aho_index = v;
        t[eind].l = trie.t[v].l;
        t[eind].r = trie.t[v].r;
        conversion[v] = eind;
        if(trie.t[v].is_leaf()){
            t[eind].output = true;
            leaves.push_back(eind);
        }
        for(int i = 0;i<alphabet;i++){
            if(trie.t[v].next[i] != 0){
                dfs(trie.t[v].next[i], eind);
            }
        }
    }
    else{
        for(int i = 0;i<alphabet;i++){
            if(trie.t[v].next[i] != 0){
                dfs(trie.t[v].next[i], par);
            }
        }
    }
}

void EHOGx::dump(ofstream& out){
    out<<t.size()<<" ";
    for(EHOGx_NODE &a:t){
        a.dump(out);
    }
    out<<leaves.size()<<" ";
    for(int a:leaves)out<<a<<" ";
}
