#pragma once
#include <bits/stdc++.h>

using namespace std;

class HOG_BASE{
protected:
    HOG_BASE(){};
public:
    void print_details(bool verbose);
    std::vector<bool>marked;
};
