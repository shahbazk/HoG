#pragma once
#include <bits/stdc++.h>
#include "SSPDefine.h"
class HogNode{
    int _parent;
    int _link; // suffix link
    int _from; //minimum index which has prefix upto this node
    int _to; // maximum index which has prefix upto this node
    int _length = 0; // upto this node
    std::vector<int> _child;
    // std::string _s;
public:
    HogNode(int parent, int link, int length, int from, int to) : 
        _parent(parent), _link(link),_from(from), _to(to), _length(length) {

    }
    pb_always_inline void suf_set(int link){_link = link;}
    pb_always_inline int from(){ return _from; }
    pb_always_inline int to(){ return _to; }
    pb_always_inline int suf_link(){ return _link; }
    pb_always_inline int length() { return _length; }
};

