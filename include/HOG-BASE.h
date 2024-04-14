#pragma once
#include <bits/stdc++.h>
#ifdef MEMORY_READING

#include "memprofile.h"

#endif

class HOG_BASE{
protected:
    HOG_BASE(){};
public:
    void print_details(bool verbose);
    std::vector<bool>marked;
};
