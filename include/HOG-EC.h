#pragma once
#include <bits/stdc++.h>
#include "EHOG.h"
#include "HOG-BASE.h"

struct segtree{
    std::vector<int>nim, add;
    segtree(int N){
        nim.resize(4*N+5,0);
        add.resize(4*N+5,0);
    }
    int segtree_min(int v, int l, int r, int ql, int qr){
        if(qr<ql)return 1e9;
        if(r<l)return 1e9;
        if(ql<=l && r<=qr){
            return nim[v];
        }
        else if(ql > r || l > qr){
            return 1e9;
        }
        nim[2*v+1]+=add[v];
        add[2*v+1]+=add[v];
        nim[2*v+2]+=add[v];
        add[2*v+2]+=add[v];
        add[v] = 0;
        return std::min(segtree_min(2*v+1, l, (l+r)/2, ql, std::min(qr,(l+r)/2)), segtree_min(2*v+2,(l+r)/2 + 1, r, std::max((l+r)/2 + 1, ql), qr));
    }
    void segtree_update(int v, int l, int r, int ql, int qr, int val){
        if(qr<ql)return;
        if(r<l)return;
        if(ql<=l && r<=qr){
            nim[v]+=val;
            add[v]+=val;
            return;
        }
        else if(ql > r || l > qr){
            return;
        }
        nim[2*v+1]+=add[v];
        add[2*v+1]+=add[v];
        nim[2*v+2]+=add[v];
        add[2*v+2]+=add[v];
        add[v] = 0;
        segtree_update(2*v+1, l, (l+r)/2, ql, std::min(qr,(l+r)/2), val);
        segtree_update(2*v+2, (l+r)/2 + 1, r, std::max((l+r)/2 + 1, ql), qr, val);
        nim[v] = std::min(nim[2*v+1], nim[2*v+2]);
        return;
    }
};

struct HOG_EC : public HOG_BASE{
public:
    HOG_EC(EHOG &ehog);
private:
    std::vector<int>l, r;
};
