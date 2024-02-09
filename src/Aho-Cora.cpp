#include "Aho-Cora.h"

using namespace std;

AhoCorasick::AhoCorasick() {
    t.emplace_back(-1, -1); //garbage node
    t.emplace_back(0, '$'); //root node
    t[1].link = 1;
}

void AhoCorasick::add_string(string const& s) {
    int v = 1;
    for (char ch : s) {
        int c = ch - 'a';
        if (t[v].next[c] == 0) {
            t[v].next[c] = t.size();
            t.emplace_back(v, ch);
        }
        v = t[v].next[c];
    }
    if(!t[v].output) {
        t[v].output = true;
        leaves.push_back(v);
    }
}

int AhoCorasick::get_link(int v) {
    #ifdef DDEBUG
    assert(v>0);
    #endif
    if (t[v].link == 0) {
        if (t[v].p == 1) // if child of root
            t[v].link = 1;
        else {
            int x = get_link(t[v].p), c = t[v].pch-'a';
            while(t[x].next[c] == 0 && x != 1) {
                x = get_link(x);
            }
            t[v].link = (t[x].next[c] == 0 ? 1 : t[x].next[c]);
        }
    }
    return t[v].link;
}