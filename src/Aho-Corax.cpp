#include "Aho-Corax.h"

using namespace std;

AhoCoraxsick::AhoCoraxsick() {
    t.emplace_back(-1, -1, 0); // garbage node
    t.emplace_back(0, '$', 0); // root node
    t[1].link = 1;
}

// Insert one string, extending the trie. A repeated string keeps its first
// leaf id (mirrors the base add_string semantics).
void AhoCoraxsick::add_string(const string& s) {
    int v = 1;
    for (char ch : s) {
        int c = ch - 'a';
        if (t[v].next[c] == 0) {
            t[v].next[c] = (int)t.size();
            t.emplace_back(v, ch, t[v].length + 1);
        }
        v = t[v].next[c];
    }
    if (t[v].str_index == -1) {
        leaves.push_back(v);
        t[v].str_index = (int)leaves.size() - 1;
    }
}

// Lazily resolve the Aho-Corasick suffix link of node v (same recurrence as
// the base trie).
int AhoCoraxsick::get_link(int v) {
    if (t[v].link == 0) {
        if (t[v].p == 1) {            // child of root
            t[v].link = 1;
        } else {
            int x = get_link(t[v].p), c = t[v].pch - 'a';
            while (t[x].next[c] == 0 && x != 1) {
                x = get_link(x);
            }
            t[v].link = (t[x].next[c] == 0 ? 1 : t[x].next[c]);
        }
    }
    return t[v].link;
}

std::vector<int> AhoCoraxsick::sorted_order() {
    vector<int> order;               // original string id in sorted rank order
    order.reserve(leaves.size());
    finalize_ranges(1, order);
    return order;
}

// Post-order DFS that (a) appends leaf string-ids in sorted order and (b) sets
// t[node].l / t[node].r to the inclusive range of sorted ranks covered by
// node's subtree. A string may be both a leaf and a proper prefix of others,
// so a leaf node contributes its own rank before descending into children.
void AhoCoraxsick::finalize_ranges(int node, vector<int>& order) {
    int start = (int)order.size();   // first sorted rank in this subtree

    if (t[node].is_leaf()) {
        order.push_back(t[node].str_index);
    }

    for (int c = 0; c < alphabet; c++) {
        int child = t[node].next[c];
        if (child != 0) {
            finalize_ranges(child, order);
        }
    }

    int end = (int)order.size() - 1; // last sorted rank in this subtree
    t[node].l = start;
    t[node].r = end;
}
