#include "HOG-SSP.h"
using namespace std;

HOG_SSP::HOG_SSP() {}

void HOG_SSP::construct() {
    const int root = 1, p = (int)t.size();
    marked.resize(p, false);

    vector<int> up(p), down(p), subTreeCnt(p, 0), oneChild(p);
    for(int i:leaves) subTreeCnt[t[i].p] = 1;
    for(int i=p-1;i>=root;i--) {
        if(subTreeCnt[i] == 1) down[i] = down[oneChild[i]];
        else down[i] = i;
        subTreeCnt[t[i].p]++;
        oneChild[t[i].p] = i;
    }
    up[root] = root;
    for(int i=root+1;i<p;i++) {
        if(subTreeCnt[t[i].p] == 1) up[i] = up[t[i].p];
        else up[i] = t[i].p;
    }

    vector<int> subTreeLeft(subTreeCnt), modified;
    marked[root] = true; //root is implicitly marked
    int u,v;
    for(int i:leaves) {
        marked[i] = true; //leaves are implicitly marked
        v = get_link(i); // iterate over proper suffixes of i, that are prefix (may not be proper) of some string
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
            v = get_link(v);
        }
        for(int x:modified) subTreeLeft[x] = subTreeCnt[x];
        modified.clear();
    }
}
void HOG_SSP::print_details(bool verbose){
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

void HOG_SSP::inp(std::ifstream& in){
    int treeSize, leavesSize;
    in>>treeSize;
    t.resize(treeSize);
    for(int i = 0;i<treeSize;i++)t[i].inp(in);
    in>>leavesSize;
    leaves.resize(leavesSize);
    for(int i = 0;i<leavesSize;i++)in>>leaves[i];
}
