#include "EHOG.h"

using namespace std;

EHOG_NODE::EHOG_NODE(ifstream& in){
    cin >> p >> link >> aho_index >> strIndex;
    int childsSize;
    cin >> childsSize;
    childs.resize(childsSize);
    for(int i = 0;i<childsSize;i++)cin >> childs[i];
}

void EHOG_NODE::file_output(ofstream &out){
    cout << p << " " << link << " " << aho_index << " " << strIndex << " ";
    cout << childs.size() << " ";
    for(int i = 0;i<(int)childs.size();i++)cout << childs[i] << " ";
}

long long EHOG_NODE::memory_required(){
    return childs.capacity() * (sizeof(int));
}

EHOG::EHOG(AhoCorasick &ahotree){
    std::vector<bool>marked(ahotree.t.size());
    for(int v:ahotree.leaves){
        int temp = v;
        while(temp != 1){
            marked[temp] = true;
            temp = ahotree.get_link(temp);
        }
    }
    t.emplace_back(-1); // garbage node
    marked[1] = true;
    std::vector<int>conversion(ahotree.t.size());
    std::function<void(int,int)> dfs;
    dfs = [&marked, &ahotree, &conversion, this, &dfs](int v, int par){
        if(marked[v] == true){
            int eind = t.size();
            t[par].childs.push_back(eind);
            t.emplace_back(par);
            t[eind].aho_index = v;
            t[eind].strIndex = ahotree.t[v].strIndex;
            conversion[v] = eind;
            if(ahotree.t[v].is_leaf()){
                leaves.push_back(eind);
            }
            for(int i = 0;i<alphabet;i++){
                if(ahotree.t[v].next[i] != 0){
                    dfs(ahotree.t[v].next[i], eind);
                }
            }
        }
        else{
            for(int i = 0;i<alphabet;i++){
                if(ahotree.t[v].next[i] != 0){
                    dfs(ahotree.t[v].next[i], par);
                }
            }
        }
    };
    dfs(1,0);
    for(int i = 1;i<(int)t.size();i++) {
        t[i].link = conversion[ahotree.get_link(t[i].aho_index)];
    }
}

EHOG::EHOG(std::ifstream &in){
    int treeSize;
    cin >> treeSize;
    for(int i = 0;i<treeSize;i++)t.emplace_back(in);
    int leavesSize;
    cin >> leavesSize;
    leaves.resize(leavesSize);
    for(int i = 0;i<leavesSize;i++)cin >> leaves[i];
}

void EHOG::file_output(std::ofstream &out){
    cout << t.size() << " ";
    for(int i = 0;i<(int)t.size();i++)t[i].file_output(out);
    cout << leaves.size() << " ";
    for(int i = 0;i<(int)leaves.size();i++)cout << leaves[i];
}

long long EHOG::memory_required(){
    long long mem = (t.capacity())*(sizeof(t)) + leaves.capacity()*(sizeof(int));
    for(int i = 0;i<t.size();i++){
        mem += t[i].memory_required();
    }
    return mem;
}
