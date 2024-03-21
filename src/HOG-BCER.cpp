#include "HOG-BCER.h"
using namespace std;

HOG_BCER::HOG_BCER() {}

void HOG_BCER::construct() {
    vector<bool>bHOG(t.size(), false);
    // build_rl();
    marked.resize(t.size());
    mark_hog(1);
}

vector<bool> HOG_BCER::mark_hog(int v){
    if(t[v].is_leaf()){
        vector<bool>C(leaves.size(), false);
        marked[v] = true;
        return C;
    }
    vector<bool> C(leaves.size(), true);
    for(int u:t[v].childs){
        vector<bool> temp = mark_hog(u);
        for(int i = 0;i<(int)C.size();i++){
            C[i] = C[i]&temp[i];
        }
    }
    for(int x:t[v].rl){
        if(C[x] == false)marked[v] = true;
        C[x] = true;
    }
    return C;
}

void HOG_BCER::print_details(bool verbose){
    int hsz = 0;
    for(bool a:marked)hsz+=a;
    if(verbose){
        std::cout << "EHOG Size: " << t.size() << "\n";
        std::cout << "HOG Size: " << hsz << "\n";
    }
    else{
        std::cout << "," << t.size() << "," << hsz;
    }
}
