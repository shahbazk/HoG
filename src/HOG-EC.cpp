#include "HOG-EC.h"
#include "trace.h"

using namespace std;

HOG_EC::HOG_EC() {}

void HOG_EC::construct(){
    int n = leaves.size();
    marked.resize(t.size());
    segtree S(n);
    for(int u:leaves){
        marked[u] = true;
        std::vector<pair<int,int>>oper;
        while(u!=1){
            u = get_link(u);
            if(S.segtree_min(0, 0, n-1,t[u].l, t[u].r) == 0){
                marked[u] = true;
            }
            S.segtree_update(0,0,n-1, t[u].l, t[u].r,1);
            oper.push_back({t[u].l, t[u].r});
        }
        for(auto p:oper){
            S.segtree_update(0,0,n-1,p.first,p.second,-1);
        }
    }   
}
void HOG_EC::print_details(bool verbose){
    int hsz = 0;
    for(bool a:marked)hsz+=a;
    if(verbose){
        std::cout << "EHOG Size: " << t.size() << "\n";
        std::cout << "HOG Size: " << hsz << "\n";
    }
    else{
        std::cout << "," << t.size() << "," << hsz;
    }
}