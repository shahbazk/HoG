#pragma once
#include <bits/stdc++.h>

using namespace std;

class HOG_BASE{
protected:
    HOG_BASE(){};
    std::vector<bool>marked;
public:
    void print_details(bool verbose);
};
