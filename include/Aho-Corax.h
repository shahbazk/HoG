#pragma once

#include <bits/stdc++.h>

// -----------------------------------------------------------------------------
// Aho-Corax.h : query-oriented ("x") variant of the Aho-Corasick trie.
//
// Counterpart to the base Aho-Cora.h / AhoCorasick used by the construction
// benchmarks. Where the base builds a trie to be measured, this variant also
// computes, for every node, the data the query HOG (apsphog Hog, via
// HOG-SKx::make_hog) needs:
//
//   * length  : number of characters from the root to this node (its depth).
//   * l, r    : the [min, max] range of SORTED string indices whose string has
//               this node's label as a prefix (the leaves in this node's
//               subtree, numbered by sorted DFS order). apsphog uses these as
//               HogNode from()/to() for the report/count/top/one-to-all queries.
//   * str_index : for a leaf, the string's original insertion index.
//
// sorted_order() returns the strings ordered by a lexicographic DFS over the
// trie (visiting children next[0..alphabet-1] in order); entry k is the
// original index of the string ranked k-th.
//
// Character encoding matches the base trie: c = ch - 'a', so with alphabet = 4
// the usable characters are 'a','b','c','d'. Root is node index 1; node 0 is a
// sentinel/garbage node.
//
// As in the base header, only the trivial constructor and is_leaf() are inline;
// all substantive method bodies live in src/Aho-Corax.cpp.
// -----------------------------------------------------------------------------

const int alphabet = 4;

struct AhoNodex {
    int p;              // index of parent
    int link = 0;       // index of suffix link (lazily computed by get_link)
    char pch;           // character on the edge from parent
    int str_index = -1; // if a leaf: index into leaves / original string id
    int length = 0;     // characters from root to this node (root has length 0)
    int l = 0;          // min sorted string index in this node's subtree
    int r = 0;          // max sorted string index in this node's subtree
    int next[alphabet] = {0}; // child transitions

    inline AhoNodex(int par, char ch, int len) : p(par), pch(ch), length(len) {}

    inline bool is_leaf() const {
        return str_index != -1;
    }
};

struct AhoCoraxsick {
    std::vector<AhoNodex> t; // nodes; t[0] sentinel, t[1] root
    std::vector<int> leaves; // node indices terminating a string, insertion order

    AhoCoraxsick();

    void add_string(const std::string& s);
    int  get_link(int v);

    // Lexicographic DFS over the trie. Assigns each string a sorted rank and,
    // as a side effect, fills every node's [l, r] leaf-range in sorted-rank
    // space; returns sorted_order where entry k is the original id of the
    // string ranked k-th. Call after all strings are inserted, before make_hog().
    std::vector<int> sorted_order();

private:
    // Recursive DFS backing sorted_order(): appends leaf string-ids in sorted
    // order and sets t[node].l / t[node].r to the inclusive range of sorted
    // ranks covered by node's subtree.
    void finalize_ranges(int node, std::vector<int>& order);
};
