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
    t[v].output = true;
}

int AhoCorasick::get_link(int v) {
    #ifdef DDEBUG
    assert(v>0);
    #endif
    if (t[v].link == 0) {
        if (t[v].p == 1)
            t[v].link = 1;
        else
            t[v].link = go(get_link(t[v].p), t[v].pch);
    }
    return t[v].link;
}

int AhoCorasick::go(int v, char ch) {
    #ifdef DDEBUG
    assert(v>0);
    assert((ch>='a') && (ch-'a'<alphabet));
    #endif
    int c = ch - 'a';
    if (t[v].go[c] == 0) {
        if (t[v].next[c] != 0)
            t[v].go[c] = t[v].next[c];
        else
            t[v].go[c] = (v == 1 ? 1 : go(get_link(v), ch));
    }
    return t[v].go[c];
}