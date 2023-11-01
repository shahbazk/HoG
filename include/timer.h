#include<bits/stdc++.h>

struct timer {
    timer();
    ~timer();
    void end();
    
    private:
    std::chrono::_V2::system_clock::time_point start_time;
    bool completed;
};