#include "EHOGx.h"
#include "trace.h"
using namespace std;

EHOGx::EHOGx() {}

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
    for(int i=1;i<t.size();i++) {
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