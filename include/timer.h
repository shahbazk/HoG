#pragma once

#include<bits/stdc++.h>
#ifdef MEMORY_READING

#include "memprofile.h"

#endif

struct timer {
    timer();
    ~timer();
    double end();
    
    private:
    std::chrono::_V2::system_clock::time_point start_time;
    bool completed;
};

