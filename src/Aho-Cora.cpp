#include "Aho-Cora.h"

using namespace std;

AhoNode::AhoNode(ifstream&in){
    in >> p  >> link >> pch >> strIndex;
    for(int i = 0;i<alphabet;i++)in >> next[i];
}

void AhoNode::file_output(ofstream& out){
    out << p << " " << link << " " << pch << " " << strIndex << " ";
    for(int i = 0;i<alphabet;i++){
        out << next[i]<< " ";
    } 
}

AhoCorasick::AhoCorasick() {
    t.emplace_back(-1, -1); //garbage node
    t.emplace_back(0, '$'); //root node
    t[1].link = 1;
}

AhoCorasick::AhoCorasick(const std::vector<std::string> &v){
    t.emplace_back(-1, -1); //garbage node
    t.emplace_back(0, '$'); //root node
    t[1].link = 1;

    int p = 0;
    for(auto &s:v) p += s.length();
    leaves.reserve(v.size());
    t.reserve(p);
    for(auto &s:v) add_string(s);

}

AhoCorasick::AhoCorasick(std::ifstream &in){
    int treeSize;
    in >> treeSize;
    for(int i = 0;i<treeSize;i++)t.emplace_back(in);
    int leavesSize;
    in >> leavesSize;
    leaves.resize(leavesSize);
    for(int i = 0;i<leavesSize;i++)in >> leaves[i];
}

void AhoCorasick::file_output(std::ofstream &out){
    out << t.size() << " ";
    for(int i = 0;i<(int)t.size();i++)t[i].file_output(out);
    out << leaves.size() << " ";
    for(int i = 0;i<(int)leaves.size();i++)out << leaves[i] << " ";
}

void AhoCorasick::add_string(string const& s) {
    int v = 1;
    for (char ch : s) {
        int c = ch - 'a';
        if (t[v].next[c] == 0) {
            t[v].next[c] = t.size();
            t.emplace_back(v, ch);
        }
        v = t[v].next[c];
    }
    if(t[v].strIndex == -1) {
        leaves.push_back(v);
        t[v].strIndex = leaves.size()-1;
    }
}

int AhoCorasick::get_link(int v) {
    #ifdef DDEBUG
    assert(v>0);
    #endif
    if (t[v].link == 0) {
        if (t[v].p == 1) // if child of root
            t[v].link = 1;
        else {
            int x = get_link(t[v].p), c = t[v].pch-'a';
            while(t[x].next[c] == 0 && x != 1) {
                x = get_link(x);
            }
            t[v].link = (t[x].next[c] == 0 ? 1 : t[x].next[c]);
        }
    }
    return t[v].link;
}

long long AhoCorasick::memory_required(){
    return (t.capacity())*(sizeof(t)) + leaves.capacity()*(sizeof(int));
}