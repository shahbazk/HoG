#pragma once
#include <bits/stdc++.h>
#include "EHOG.h"
#include "HOG-BASE.h"

struct HOG_BCER : public HOG_BASE{
public:
    HOG_BCER(EHOG &ehog);
private:
    std::vector<bool> mark_hog(int v, EHOG &ehog);
    void build_rl(EHOG &ehog);
    std::vector<std::vector<int>>rl;
};
