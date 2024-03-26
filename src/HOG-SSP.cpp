#include "HOG-SSP.h"
using namespace std;

HOG_SSP::HOG_SSP(EHOG &ehog) {
    const int root = 1, p = (int)ehog.t.size();
    marked.resize(p, false);

    vector<int> up(p), down(p), subTreeCnt(p, 0), oneChild(p);
    for(int i:ehog.leaves) subTreeCnt[ehog.t[i].p] = 1;
    for(int i=p-1;i>=root;i--) {
        if(subTreeCnt[i] == 1) down[i] = down[oneChild[i]];
        else down[i] = i;
        subTreeCnt[ehog.t[i].p]++;
        oneChild[ehog.t[i].p] = i;
    }
    up[root] = root;
    for(int i=root+1;i<p;i++) {
        if(subTreeCnt[ehog.t[i].p] == 1) up[i] = up[ehog.t[i].p];
        else up[i] = ehog.t[i].p;
    }

    vector<int> subTreeLeft(subTreeCnt), modified;
    marked[root] = true; //root is implicitly marked
    int u,v;
    for(int i:ehog.leaves) {
        marked[i] = true; //leaves are implicitly marked
        v = ehog.get_link(i); // iterate over proper suffixes of i, that are prefix (may not be proper) of some string
        while(v!=root) {
            if(subTreeLeft[down[v]] != 0) { //if the subtree of v (including v) contains a split node having subtrees left
                marked[v] = true;
                modified.push_back(down[v]);
                subTreeLeft[down[v]] = 0;
                u = v;
                do {
                    u = up[u];
                    subTreeLeft[u]--; //remove subtree of u containing v
                } while((u!=root) && (subTreeLeft[u]==0));
                modified.push_back(u); // add only last u to modified as all other up nodes are already in modified
            }
            v = ehog.get_link(v);
        }
        for(int x:modified) subTreeLeft[x] = subTreeCnt[x];
        modified.clear();
    }
}

HOG_SSP::HOG_SSP(AhoCorasick &ahocora) {
    const int root = 1, p = (int)ahocora.t.size();
    marked.resize(p, false);

    vector<int> up(p), down(p), subTreeCnt(p, 0), oneChild(p);
    for(int i:ahocora.leaves) subTreeCnt[ahocora.t[i].p] = 1;
    for(int i=p-1;i>=root;i--) {
        if(subTreeCnt[i] == 1) down[i] = down[oneChild[i]];
        else down[i] = i;
        subTreeCnt[ahocora.t[i].p]++;
        oneChild[ahocora.t[i].p] = i;
    }
    up[root] = root;
    for(int i=root+1;i<p;i++) {
        if(subTreeCnt[ahocora.t[i].p] == 1) up[i] = up[ahocora.t[i].p];
        else up[i] = ahocora.t[i].p;
    }

    vector<int> subTreeLeft(subTreeCnt), modified;
    marked[root] = true; //root is implicitly marked
    int u,v;
    for(int i:ahocora.leaves) {
        marked[i] = true; //leaves are implicitly marked
        v = ahocora.get_link(i); // iterate over proper suffixes of i, that are prefix (may not be proper) of some string
        while(v!=root) {
            if(subTreeLeft[down[v]] != 0) { //if the subtree of v (including v) contains a split node having subtrees left
                marked[v] = true;
                modified.push_back(down[v]);
                subTreeLeft[down[v]] = 0;
                u = v;
                do {
                    u = up[u];
                    subTreeLeft[u]--; //remove subtree of u containing v
                } while((u!=root) && (subTreeLeft[u]==0));
                modified.push_back(u); // add only last u to modified as all other up nodes are already in modified
            }
            v = ahocora.get_link(v);
        }
        for(int x:modified) subTreeLeft[x] = subTreeCnt[x];
        modified.clear();
    }
}
