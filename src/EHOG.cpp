#include "EHOG.h"
using namespace std;

void EHOG_NODE::dump(std::ofstream& out){
    out<<p<<" "<<link<<" "<<output<<" ";
    out<<childs.size()<<" ";
    for(int a:childs)out<<a<<" ";
    // out<<"\n";
}

void EHOG_NODE::inp(std::ifstream& in){
    in>>p>>link>>output;
    int numChild;
    in>>numChild;
    childs.resize(numChild);
    for(int i = 0;i<numChild;i++){in>>childs[i];}
}

int EHOG_NODE::memory_calculate(){
    return childs.size()*sizeof(int);
}

EHOG::EHOG() {}

void EHOG::add_string(const std::string& s) {
    trie.add_string(s);
}

void EHOG::add_strings(const vector<string>& v) {
    int p = 0;
    for(auto &s:v) p += s.length();
    trie.leaves.reserve(v.size());
    trie.t.reserve(p);
    for(auto &s:v) add_string(s);
    construct();
}

void EHOG::construct(){
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
    for(int i = 1;i<(int)t.size();i++) {
        t[i].link = conversion[trie.get_link(t[i].aho_index)];
    }
}

void EHOG::dfs(int v, int par){
    if(marked[v] == true){
        int eind = t.size();
        t[par].childs.push_back(eind);
        t.emplace_back(par);
        t[eind].aho_index = v;
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
void EHOG::dump(ofstream& out){
    out<<t.size()<<" ";
    for(EHOG_NODE &a:t){
        a.dump(out);
    }
    out<<leaves.size()<<" ";
    for(int a:leaves)out<<a<<" ";
}
