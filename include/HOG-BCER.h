#pragma once
#include <bits/stdc++.h>
#include "EHOG.h"
#include "HOG-BASE.h"

struct HOG_BCER : public HOG_BASE{
public:
    HOG_BCER(EHOG &ehog);
    HOG_BCER(AhoCorasick &ahocora);

private:
    std::vector<bool> mark_hog(int v, EHOG &ehog);
    vector<bool> mark_hog(int v, AhoCorasick &ahocora);
    void build_rl(EHOG &ehog);
    void build_rl(AhoCorasick &ahocora);
    std::vector<std::vector<int>>rl;
};
