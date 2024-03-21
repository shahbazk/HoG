#pragma once
#include "HogNode.h"
class Hog{
private:
    
    
public:
    std::vector<HogNode> t;
    std::vector<int> _strToTreeIndex;
    std::vector<int>sorted_order_conversion;
    std::vector<int>sorted_order;
    int suffix_prefix_length(int i, int j);
    std::vector<int> one_to_all(int i);
    std::vector<int> top(int i, int k);
    std::vector<int> report(int i, int l);
    int count(int i, int l);

    Hog(){
        t.emplace_back(-1,-1,-1, -1, -1);
    }
    int add_node(int parent, int link, int length, int from, int to){
        int node_index = t.size();
        t.emplace_back(parent, link, length, from , to);
        return node_index;
    }
};