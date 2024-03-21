#include<bits/stdc++.h>
using namespace std;

#include "timer.h"

#ifdef SSP

#include "HOG-SSP.h"
typedef HOG_SSP HOG;
#include "EHOG.h"
typedef EHOG EHOG;
#elif SK

#include "HOG-SK.h"
typedef HOG_SK HOG;
#include "EHOG.h"
typedef EHOG EHOG;
#elif BCER

#include "HOG-BCER.h"
typedef HOG_BCER HOG;
// #include "EHOGx.h";
typedef EHOGx EHOG;
#elif EC

#include "HOG-EC.h"
typedef HOG_EC HOG;
// #include "EHOGx.h";
typedef EHOGx EHOG;
#else
#include "HOG-SSP.h"
typedef HOG_SSP HOG;
#include "EHOG.h";
typedef EHOG EHOG;
#endif

const int TRIALS = 1;
map<string, double> trial_results;

//TODO : write bigger validity test comparing dumps from both algos
void test_validity() {
    cout << "\nTesting validity of algorithm\n";
    vector<string> v = {"aabaa", "aadbd", "dbdaa"};
    HOG hog(v);
    assert(hog.marked == vector<bool>({0,1,0,1,0,0,1,0,0,1,0,0,1,0,1}));
    cout<<"All tests passed\n";
}

void test_with(const vector<string>& v, bool verbose = false) {
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

pair<double, double> get_mean_and_sd(vector<double> &a) {
    sort(a.begin(), a.end());
    double sum = 0, sq_sum = 0, cnt=0;
    for(int i=0;i<(int)a.size();i++) {
        sum += a[i];
        sq_sum += a[i]*a[i];
        cnt++;
    }
    double avg = sum/cnt, sd = sqrt(sq_sum/cnt - avg*avg);
    return {avg, sd};
}

void stress_test_with(function<vector<string>()> generator, bool verbose = false) {
    // cout<<"Number of trials: " << TRIALS << endl;
    map<string, vector<double>> all_results;
    for(int i=0;i<TRIALS;i++) {
        auto v = generator();
        test_with(v);
        for(auto data_pair:trial_results) {
            all_results[data_pair.first].push_back(data_pair.second);
        }
    }
    for(auto result:all_results) {
        if(verbose) cout << '\n' << result.first << ':';
        cout << fixed << setprecision(3) << get_mean_and_sd(result.second).first << ',';
    }
    if(verbose) cout << "\nmemory:";
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
}
void random_strings_stress_test(int n, int p, int seed) {
    assert(p>=n);
    // cout << "\nTesting on randomly generated strings...\n" << "N = " << n << ", P = " << p << '\n';
    cout << n << ',' << p << ',';cout.flush();
    srand(seed);

    auto generator = [&]() {
        vector<string> v(n);
        int j=0;
        for(int i=0;i<p;i++,j++) {
            if(j>=n) j-=n;
            v[j] += ('a'+rand()%alphabet);
        }
        return v;
    };

    stress_test_with(generator);
}

// TODO : change to use real data as complete string and randomly positioned reads
void random_string_reads_stress_test(int n, int p, double overlap, int seed) {
    assert(p>=n);
    assert(0.0<overlap);
    assert(overlap<1.0);
    int len = p/n;
    int total_len = p*(1.0-overlap) + len*overlap;
    // cout << "\nTesting on randomly generated reads on a randomly generated string...\n" << "N = " << n << ", P = " << p << ", o = " << overlap << '\n';
    cout << n << ',' << p << ',' << overlap << ',';
    srand(seed);
    string complete_string = "";
    for(int i=0;i<total_len;i++) complete_string += ('a' + rand()%alphabet);

    auto generator = [&]() {
        vector<string> v;
        // might miss a few ending characters
        for(double i=0;(int)i<=total_len-len;i+=(double)len*(1.0-overlap)) {
            v.push_back(complete_string.substr((int)i,len));
        }
        return v;
    };

    stress_test_with(generator);
}

void real_data_test(string fname) {
    string data_path = "data/";

    cout<<'\n'<<fname<<":\n";
#ifdef EHOG_CONSTRUCTION
    fstream fin;
    fin.open(data_path+fname, ios::in);
    if(!fin) {
        cout<<"couldn't open file: "<<fname<<endl;
        return;
    }
    long long n, total_length = 0;
    fin>>n;
    vector<string> v(n);
    for(int i=0;i<n;i++) {
        fin>>v[i];
        total_length += v[i].length();
    }
    // cout<<"Number of strings = "<<v.size()<<'\n'<<"Sum of lengths = "<<total_length<<'\n';
    // stress_test_with([&](){return v;});
        cout<<","<<v.size()<<","<<total_length;
        stress_test_with_ehog(v,fname);
#else
        stress_test_with_hog(fname);
#endif
}

int main(int argc, char **argv) {
    #ifdef SSP
        // cout<<"\nUsing algo by SSP...\n";
    #elif SK
        // cout<<"\nUsing algo by SK...\n";
    #elif BCER
        // cout<<"\nUsing algo by BCER...\n";
    #elif EC
        // cout<<"\nUsing algo by EC...\n";
    #else
        // cout<<"\nUsing algo by SSP...\n";
    #endif
    // int seed = chrono::system_clock::now().time_since_epoch().count();
    int n = stoi(argv[1]), p = stoi(argv[2]), seed = stoi(argv[3]);
    // double o = stod(argv[3]);
    // std::string d_name = argv[1];
    // test_validity();
    random_strings_stress_test(n, p, seed);
    // random_string_reads_stress_test(n, p, o, seed);
    // real_data_test(argv[1]);
    return 0;
}
