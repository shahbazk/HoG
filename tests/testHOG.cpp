#include<bits/stdc++.h>
using namespace std;

#include "timer.h"

#ifdef SSP

#include "HOG-SSP.h"
typedef HOG_SSP HOG;

#elif SK

#include "HOG-SK.h"
typedef HOG_SK HOG;

#elif SP

#include "HOG-SP.h"
typedef HOG_SP HOG;

#elif BCER

#include "HOG-BCER.h"
typedef HOG_BCER HOG;

#elif EC

#include "HOG-EC.h"
typedef HOG_EC HOG;

#else

#include "HOG-SSP.h"
typedef HOG_SSP HOG;

#endif

map<string, double> trial_results;

// TODO : write bigger validity test comparing dumps from both algos
void test_validity() {
    cout << "\nTesting validity of algorithm\n";
    vector<string> v = {"aabaa", "aadbd", "dbdaa"};
    AhoCorasick ahocora(v);
    EHOG ehog(ahocora);

    #ifdef SP

    HOG hog1(ahocora, v);
    HOG hog2(ehog, ahocora, v);

    #else

    HOG hog1(ahocora);
    HOG hog2(ehog);

    #endif

    assert(hog1.marked == vector<bool>({0,1,0,1,0,0,1,0,0,1,0,0,1,0,1}));
    assert(hog2.marked == vector<bool>({0,1,0,1,1,1,0,1,1}));
    cout<<"All tests passed\n";
}

/* void test_with(const vector<string>& v, bool verbose = false) {
    HOG hog;
    if(verbose) {cout<<"Building Aho-Corasick automaton..." << endl;}
    timer ehog_t;
    hog.add_strings(v);
    double ehog_time = ehog_t.end();
    trial_results["ehog_time"] = ehog_time;
    if(verbose) {cout<<"Elapsed time: " << ehog_time << endl;}
    
    if(verbose) {cout<<"Constructing HOG..." << endl;}
    timer hog_t;
    hog.construct();
    double hog_time = hog_t.end();
    trial_results["hog_time"] = hog_time;
    if(verbose) {cout<<"Elapsed time: " << hog_time << endl;}
    
    trial_results["tot_time"] = ehog_time + hog_time;

    int cnt = 0;
    for(auto b:hog.marked) cnt+=b;
    trial_results["ehog_size"] = hog.marked.size()-1;
    trial_results["hog_size"] = cnt;

    if(verbose) {
        cout << "Size of EHOG: " << hog.marked.size()-1 
        << ", Size of HOG: " << cnt
        << ", Compression factor: "<< (double)cnt/(hog.marked.size()-1)
        << endl;
    }
}


void stress_test_with_ehog(const vector<string>& v, std::string filename) {
    vector<double> ehog_times(TRIALS), hog_times(TRIALS), tot_times(TRIALS);
    ofstream fout;
#ifdef SSP
    fout.open("./ehog_dump/"+filename+"_ehog_object", ios::out);
#elif SK
    fout.open("./ehog_dump/"+filename+"_ehog_object", ios::out);
#elif EC
    fout.open("./ehog_dump/"+filename+"_ehogx_object", ios::out);
#else
    fout.open("./ehog_dump/"+filename+"_ehogx_object", ios::out);
#endif
    
    if(!fout) {
        cout<<"couldn't open file: "<<filename<<endl;
        return;
    }
    for(int i=0;i<TRIALS;i++) {
        EHOG ehog;
        timer ehog_t;
        ehog.add_strings(v);
        ehog_times[i] = ehog_t.end();
        ehog.dump(fout);
        // cout << ehog.t.size() << "\n";
        // cout << ehog.leaves.size() << "\n";
    }
    auto ehog_data = get_mean_and_sd(ehog_times);
    cout<<fixed<<setprecision(6);
    // cout<<"EHOG: "<<ehog_data.first<<' '<<ehog_data.second<<std::endl;
    cout<<","<<ehog_data.first<<','<<ehog_data.second<<std::endl;
}
// #else
void stress_test_with_hog(std::string filename) {
    vector<double> hog_times(TRIALS);
    ifstream fin;
#ifdef SSP
    fin.open("./ehog_dump/"+filename+"_ehog_object", ios::in);
#elif SK
    fin.open("./ehog_dump/"+filename+"_ehog_object", ios::in);
#elif EC
    fin.open("./ehog_dump/"+filename+"_ehogx_object", ios::in);
#else
    fin.open("./ehog_dump/"+filename+"_ehogx_object", ios::in);
#endif
    
    if(!fin) {
        cout<<"couldn't open file: "<<filename<<endl;
        return;
    }
    // cout << "here" << std::endl;
    long long memehog;
    for(int i=0;i<TRIALS;i++) {
        HOG hog;
        hog.inp(fin);
        memehog = sizeof(EHOG_NODE)*(hog.t.size()) + sizeof(int)*(hog.leaves.size());
        for(int i = 0;i<(int)hog.t.size();i++){
            memehog+=hog.t[i].memory_calculate();
        }
        timer hog_t;
        hog.construct();
        hog_times[i] = hog_t.end();
        if(i == TRIALS-1)hog.print_details(false);
    }
    auto hog_data = get_mean_and_sd(hog_times);
    cout<<fixed<<setprecision(6);
    // cout<<"ehog memory: "<<memehog<<"\n";
    // cout<<"hog: "<<hog_data.first<<' '<<hog_data.second<<std::endl;
    cout<<","<<memehog<<","<<hog_data.first<<','<<hog_data.second<<std::endl;
} */


class DatasetGenerator {

    vector<string> generate_real_data(string datasetName){
        string data_path = "data/";
        fstream fin;
        fin.open(data_path+datasetName, ios::in);
        if(!fin) {
            cout<<"couldn't open file: "<<datasetName<<endl;
            return {};
        }
        long long n, total_length = 0;
        fin>>n;
        vector<string> v(n);
        for(int i=0;i<n;i++) {
            fin>>v[i];
            total_length += v[i].length();
        }

        /* cout<<"Dataset Name = " << datasetName << "\n";
        cout<<"Number of strings = "<<v.size()<<'\n'<<"Sum of lengths = "<<total_length<<'\n'; */

        cout<<","<<datasetName<<","<<v.size()<<","<<total_length;

        return v;
    }

    vector<string> generate_random_data(int n, int k, int seed) {
        assert(k>=n);
        // cout << "\nTesting on randomly generated strings...\n" << "N = " << n << ", K = " << k << '\n';
        srand(seed);

        cout << n << ',' << k << ',';cout.flush();
        vector<string> v(n);
        int j=0;
        for(int i=0;i<k;i++,j++) {
            if(j>=n) j-=n;
            v[j] += ('a'+rand()%alphabet);
        }
        return v;
    }

    vector<string> generate_random_read_data(int n, int k, int overlap, int seed) {
        assert(k>=n);
        assert(0.0<overlap);
        assert(overlap<1.0);
        int len = k/n;
        int total_len = k*(1.0-overlap) + len*overlap;
        // cout << "\nTesting on randomly generated reads on a randomly generated string...\n" << "N = " << n << ", P = " << p << ", o = " << overlap << '\n';
        cout << n << ',' << k << ',' << overlap << ',';
        srand(seed);
        string complete_string = "";
        for(int i=0;i<total_len;i++) complete_string += ('a' + rand()%alphabet);

        vector<string> v;
        // might miss a few ending characters
        for(double i=0;(int)i<=total_len-len;i+=(double)len*(1.0-overlap)) {
            v.push_back(complete_string.substr((int)i,len));
        }
        return v;
    }
};

int main(int argc, char **argv) {
    #ifdef SSP
        // cout<<"\nUsing algo by SSP...\n";
    #elif SK
        // cout<<"\nUsing algo by SK...\n";
    #elif SP
        // cout<<"\nUsing algo by SP...\n";
    #elif BCER
        // cout<<"\nUsing algo by BCER...\n";
    #elif EC
        // cout<<"\nUsing algo by EC...\n";
    #else
        // cout<<"\nUsing algo by SSP...\n";
    #endif
    // int seed = chrono::system_clock::now().time_since_epoch().count();
    // int n = stoi(argv[1]), p = stoi(argv[2]), seed = stoi(argv[3]);
    // double o = stod(argv[3]);
    // std::string d_name = argv[1];
    test_validity();
    // random_strings_stress_test(n, p, seed);
    // random_string_reads_stress_test(n, p, o, seed);
    // real_data_test(argv[1]);
    return 0;
}
