#include "HOG-SSP.h"
#include "trace.h"
using namespace std;

HOG_SSP::HOG_SSP() {}

HOG_SSP::HOG_SSP(const vector<string>& v) {
    add_strings(v);
    construct();
}

void HOG_SSP::add_string(const string& s) {
    trie.trie.add_string(s);
}

void HOG_SSP::add_strings(const vector<string>& v) {
    int p = 0;
    for(auto &s:v) p += s.length();
    trie.trie.leaves.reserve(v.size());
    trie.trie.t.reserve(p);
    for(auto &s:v) add_string(s);
    trie.construct();
}

void HOG_SSP::construct() {
    const int root = 1, p = (int)trie.t.size();
    marked.resize(p, false);

    vector<int> up(p), down(p), subTreeCnt(p, 0), oneChild(p);
    for(int i:trie.leaves) subTreeCnt[trie.t[i].p] = 1;
    for(int i=p-1;i>=root;i--) {
        if(subTreeCnt[i] == 1) down[i] = down[oneChild[i]];
        else down[i] = i;
        subTreeCnt[trie.t[i].p]++;
        oneChild[trie.t[i].p] = i;
    }
    up[root] = root;
    for(int i=root+1;i<p;i++) {
        if(subTreeCnt[trie.t[i].p] == 1) up[i] = up[trie.t[i].p];
        else up[i] = trie.t[i].p;
    }

    vector<int> subTreeLeft(subTreeCnt), modified;
    marked[root] = true; //root is implicitly marked
    int u,v;
    for(int i:trie.leaves) {
        marked[i] = true; //leaves are implicitly marked
        v = trie.get_link(i); // iterate over proper suffixes of i, that are prefix (may not be proper) of some string
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
            v = trie.get_link(v);
        }
        for(int x:modified) subTreeLeft[x] = subTreeCnt[x];
        modified.clear();
    }
}
void HOG_SSP::print_details(){
    std::cout << "Aho-Corasick Size: " << trie.trie.t.size() << "\n";
    std::cout << "EHOG Size: " << trie.t.size() << "\n";
    int hsz = 0;
    for(bool a:marked)hsz+=a;
    std::cout << "HOG Size: " << hsz << "\n";
}