#include "HOG-SK.h"
#include "trace.h"
#include "trace.h"

using namespace std;

HOG_SK::HOG_SK(EHOG &ehog){
    //construct l
    int root = 1;
    l.resize(ehog.t.size());
    for(int i=0;i<(int)ehog.leaves.size();i++) {
        int curr = ehog.get_link(ehog.leaves[i]);
        while(curr != root) { // add to the list of each node on suffix path, except the leaf itself
            l[curr].push_back(i);
            curr = ehog.get_link(curr);
        }
    }
    marked.resize(ehog.t.size(), false); // inH
    marked[root] = true; //root is in HOG
    s.resize(ehog.leaves.size());
    is_unmarked.resize(ehog.leaves.size(), false);
    std::function<void(int)> dfs;
    dfs = [&ehog, this, &dfs](int node){
        if(ehog.t[node].is_leaf()) {
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
        for(int child : ehog.t[node].childs){
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
    };
    dfs(root);
}

HOG_SK::HOG_SK(AhoCorasick &ahocora){
    int root = 1;
    l.resize(ahocora.t.size());
    for(int i=0;i<(int)ahocora.leaves.size();i++) {
        int curr = ahocora.get_link(ahocora.leaves[i]);
        while(curr != root) { // add to the list of each node on suffix path, except the leaf itself
            l[curr].push_back(i);
            curr = ahocora.get_link(curr);
        }
    }
    marked.resize(ahocora.t.size(), false); // inH
    marked[root] = true; //root is in HOG
    s.resize(ahocora.leaves.size());
    is_unmarked.resize(ahocora.leaves.size(), false);
    std::function<void(int)> dfs;
    dfs = [&ahocora, this, &dfs](int node){
        if(ahocora.t[node].is_leaf()) {
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

        for(int i = 0;i<alphabet;i++){
            if(ahocora.t[node].next[i] != 0){
                dfs(ahocora.t[node].next[i]);
            }
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
    };
    dfs(root);
}