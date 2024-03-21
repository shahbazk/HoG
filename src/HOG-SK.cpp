#include "HOG-SK.h"
#include "trace.h"
#include "trace.h"

using namespace std;

HOG_SK::HOG_SK() {}

void HOG_SK::construct() {
    //construct l
    int root = 1;
    l.resize(t.size()); //initialise l with empty lists
    for(int i=0;i<(int)leaves.size();i++) {
        int curr = get_link(leaves[i]);
        while(curr != root) { // add to the list of each node on suffix path, except the leaf itself
            l[curr].push_back(i);
            curr = get_link(curr);
        }
    }
    marked.resize(t.size(), false); // inH
    marked[root] = true; //root is in HOG
    s.resize(leaves.size());
    is_unmarked.resize(leaves.size(), false);
    dfs(root);
}

void HOG_SK::dfs(int node) {
    if(t[node].is_leaf()) {
        marked[node] = true; // leaves are implicitly in HOG
        for(int x:unmarked) { // iterate over all stacks with unmarked tops
            if(is_unmarked[x]) {
                marked[s[x].top()] = true;
                is_unmarked[x] = false;
            }
        }
        unmarked.clear();
    }

    // node visited for the first time
    for(int x:l[node]) {
        s[x].push(node);
        if(!is_unmarked[x]) {
            is_unmarked[x] = true;
            unmarked.push_back(x);
        }
    }
    for(int child : t[node].childs){
        dfs(child);
    }
    // node visited for the last time
    for(int x:l[node]) {
        s[x].pop();
        if((!s[x].empty()) && (!marked[s[x].top()])) {
            if(!is_unmarked[x]) {
                is_unmarked[x] = true;
                unmarked.push_back(x);
            }
        } else {
            is_unmarked[x] = false;
        }
    }
}
void HOG_SK::print_details(bool verbose){
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