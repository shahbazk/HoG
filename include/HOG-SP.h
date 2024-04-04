#pragma once
#include "Aho-Cora.h"
#include "EHOG.h"
#include "HOG-BASE.h"
#include <bits/stdc++.h>

struct HOG_SP : public HOG_BASE {

    HOG_SP(AhoCorasick &ahocora, const std::vector<std::string>& input_strings);
    HOG_SP(EHOG &ehog, AhoCorasick &ahocora, const std::vector<std::string>& input_strings);

  private:
};