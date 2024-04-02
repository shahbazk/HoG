#include "HOG-EC.h"
#include "trace.h"

using namespace std;

HOG_EC::HOG_EC(EHOG &ehog) {
    l.resize(ehog.t.size(), 1e9);
    r.resize(ehog.t.size(), -1);

    int leaves_visited = 0;
    function<void(int)> dfs;
    dfs = [&leaves_visited, &ehog, &dfs, this](int v){
        if(ehog.t[v].is_leaf()){
            l[v] = r[v] = leaves_visited;
            leaves_visited++;
        }
        for(int child:ehog.t[v].childs){
            dfs(child);
        }
        for(int child:ehog.t[v].childs){
            l[v] = min(l[v], l[child]);
        }
        for(int child:ehog.t[v].childs){
            r[v] = max(r[v], r[child]);
        }
    };

    dfs(1);
    int n = ehog.leaves.size();
    marked.resize(ehog.t.size());
    segtree S(n);
    marked[1] = 1;
    for(int u:ehog.leaves){
        marked[u] = true;
        std::vector<pair<int,int>>oper;
        while(u!=1){
            u = ehog.get_link(u);
            if(S.segtree_min(0, 0, n-1,l[u], r[u]) == 0){
                marked[u] = true;
            }
            S.segtree_update(0,0,n-1, l[u], r[u],1);
            oper.push_back({l[u], r[u]});
        }
        for(auto p:oper){
            S.segtree_update(0,0,n-1,p.first,p.second,-1);
        }
    } 
}

HOG_EC::HOG_EC(AhoCorasick &ahocora) {
    l.resize(ahocora.t.size(), 1e9);
    r.resize(ahocora.t.size(), -1);

    int leaves_visited = 0;
    function<void(int)> dfs;
    dfs = [&leaves_visited, &ahocora, &dfs, this](int v){
        if(ahocora.t[v].is_leaf()){
            l[v] = r[v] = leaves_visited;
            leaves_visited++;
        }
        for(int i = 0;i<alphabet;i++){
            if(ahocora.t[v].next[i] != 0)
                dfs(ahocora.t[v].next[i]);
        }
        for(int i = 0;i<alphabet;i++){
            if(ahocora.t[v].next[i] != 0){
                l[v] = min(l[v], l[ahocora.t[v].next[i]]);
                r[v] = max(r[v], r[ahocora.t[v].next[i]]);
            }
        }
    };

    dfs(1);
    int n = ahocora.leaves.size();
    marked.resize(ahocora.t.size());
    segtree S(n);
    marked[1] = 1;
    for(int u:ahocora.leaves){
        marked[u] = true;
        std::vector<pair<int,int>>oper;
        while(u!=1){
            u = ahocora.get_link(u);
            if(S.segtree_min(0, 0, n-1,l[u], r[u]) == 0){
                marked[u] = true;
            }
            S.segtree_update(0,0,n-1, l[u], r[u],1);
            oper.push_back({l[u], r[u]});
        }
        for(auto p:oper){
            S.segtree_update(0,0,n-1,p.first,p.second,-1);
        }
    } 
}