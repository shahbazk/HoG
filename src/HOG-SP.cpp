#include "HOG-SP.h"
using namespace std;

HOG_SP::HOG_SP(AhoCorasick &ahocora, const std::vector<std::string>& input_strings) {
    const int root = 1, p = (int)ahocora.t.size();
    const int n = input_strings.size();
    marked.resize(p, false);

    vector<vector<int>> pnode(n); // index of j'th prefix of i'th string in ACtrie
    vector<vector<int>> border(n);
    vector<int> up(p, root);

    // calculate pnode array
    for (int i = 0; i < n; i++) {
        pnode[i].resize(input_strings[i].length()+1);
        int curr = root;
        pnode[i][0] = root;
        for (int j = 0; j < (int)input_strings[i].length(); j++) {
            curr = ahocora.t[curr].next[input_strings[i][j]-'a'];
            pnode[i][j+1] = curr;
        }
    }

    // calculate border array and up
    for(int i=0;i<n;i++) {
        border[i].resize(input_strings[i].length()+1, 0);
        int k=0;
        for(int j=1;j<(int)input_strings[i].length();j++) {
            while(k>0 && input_strings[i][k] != input_strings[i][j]) k = border[i][k];
            if(input_strings[i][k] == input_strings[i][j]) k++;
            border[i][j+1] = k;

            // calculate up array
            up[pnode[i][j+1]] = pnode[i][border[i][j+1]];
        }
    }

    // main algorithm
    vector<int> R(p, 0), I(p);
    for(int leaf: ahocora.leaves) R[ahocora.t[leaf].p]++;
    for(int i=p-1;i>0;i--) {
        R[ahocora.t[i].p] += R[i];
    }

    marked[root] = true;
    vector<vector<int>> Child(p);
    for (int u : ahocora.leaves) {
        marked[u] = true;
        while(u != root) {
            I[u] = R[u];
            for (int v : Child[u]) {
                I[u] -= R[v];
            }
            if(I[u] > 0) marked[u] = true;
            Child[u].clear();
            Child[up[u]].push_back(u);
            u = ahocora.get_link(u);
        }
    }
}

HOG_SP::HOG_SP(EHOG &ehog, AhoCorasick &ahocora, const std::vector<std::string>& input_strings) {
    const int root = 1, ehog_size = (int)ehog.t.size();
    const int n = input_strings.size();
    const int aho_size = ahocora.t.size();
    marked.resize(ehog_size, false);

    vector<vector<int>> pnode(n); // index of j'th prefix of i'th string in ACtrie
    vector<vector<int>> border(n);
    vector<int> aho_up(aho_size, root);

    // calculate pnode array
    for (int i = 0; i < n; i++) {
        pnode[i].resize(input_strings[i].length()+1);
        int curr = root;
        pnode[i][0] = root;
        for (int j = 0; j < (int)input_strings[i].length(); j++) {
            curr = ahocora.t[curr].next[input_strings[i][j]-'a'];
            pnode[i][j+1] = curr;
        }
    }

    // calculate border array and up
    for(int i=0;i<n;i++) {
        border[i].resize(input_strings[i].length()+1, 0);
        int k=0;
        for(int j=1;j<(int)input_strings[i].length();j++) {
            while(k>0 && input_strings[i][k] != input_strings[i][j]) k = border[i][k];
            if(input_strings[i][k] == input_strings[i][j]) k++;
            border[i][j+1] = k;

            // calculate up array
            aho_up[pnode[i][j+1]] = pnode[i][border[i][j+1]];
        }
    }

    // calculate ehog_up using aho_up
    vector<int> aho_to_ehog_index(aho_size);
    for(int i=root;i<ehog_size;i++) {
        aho_to_ehog_index[ehog.t[i].aho_index] = i;
    }

    vector<int> ehog_up(ehog_size);
    for(int i=root;i<ehog_size;i++) {
        ehog_up[i] = aho_to_ehog_index[aho_up[ehog.t[i].aho_index]];
    }

    // main algorithm
    vector<int> R(ehog_size, 0), I(ehog_size);
    for(int leaf: ehog.leaves) R[ehog.t[leaf].p]++;
    for(int i=ehog_size-1;i>0;i--) {
        R[ehog.t[i].p] += R[i];
    }

    marked[root] = true;
    vector<vector<int>> Child(ehog_size);
    for (int u : ehog.leaves) {
        marked[u] = true;
        while(u != root) {
            I[u] = R[u];
            for (int v : Child[u]) {
                I[u] -= R[v];
            }
            if(I[u] > 0) marked[u] = true;
            Child[u].clear();
            Child[ehog_up[u]].push_back(u);
            u = ehog.get_link(u);
        }
    }
}
