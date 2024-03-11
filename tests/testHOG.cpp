#include<bits/stdc++.h>
using namespace std;

#include "timer.h"
#include "trace.h"

#ifdef SSP

#include "HOG-SSP.h"
typedef HOG_SSP HOG;

#elif SK

#include "HOG-SK.h"
typedef HOG_SK HOG;

#elif BCER

#include "HOG-BCER.h"
typedef HOG_BCER HOG;


#elif EC

#include "HOG-EC.h"
typedef HOG_EC HOG;

#else
#include "HOG-SK.h"
typedef HOG_SK HOG;
#endif

const int TRIALS = 10;
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

template<typename T>
pair<T, T> get_mean_and_sd(vector<T> &a) {
    sort(a.begin(), a.end());
    T sum = 0, sq_sum = 0, cnt=0;
    for(int i=a.size()/10;i<(9*a.size()/10);i++) {
        sum += a[i];
        sq_sum += a[i]*a[i];
        cnt++;
    }
    T avg = sum/cnt, sd = sqrt(sq_sum/cnt - avg*avg);
    return {avg, sd};
}

void stress_test_with(function<vector<string>()> generator, bool verbose = false) {
    // cout<<"Number of trials: " << TRIALS << endl;
    map<string, vector<double>> all_results;
    for(int i=0;i<TRIALS;i++) {
        auto v = generator();
        trial_results["data_memory"] = sizeof(vector<string>) + sizeof(string)*v.capacity();
        for(auto &s:v) {
            trial_results["data_memory"] += sizeof(char)*s.capacity();
        }
        trial_results["data_memory"] /= 1000;
        test_with(v);
        trial_results["aho_memory"] /= 1000;
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

// TODO : change to use real data as complete string and randomly positioned reads
void random_strings_stress_test(int n, int p, int seed) {
    assert(p>=n);
    // cout << "\nTesting on randomly generated strings...\n" << "N = " << n << ", P = " << p << '\n';
    cout << n << ',' << p << ',';
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


void real_data_test_on(string fname) {
    cout<<'\n'<<fname<<":\n";
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
    cout<<"Number of strings : "<<v.size()<<'\n'<<"Sum of lengths : "<<total_length<<'\n';

    stress_test_with([&](){return v;}, true);
}
void real_data_test(int file_index = -1) {
    // cout<<"\nRunning on real datasets...\n";
    if(file_index == -1) {
        for(string fname:filenames) {
            real_data_test_on(fname);
        }
    } else {
        real_data_test_on(filenames[file_index]);
    }
}

int main(int argc, char **argv) {
    #ifdef SSP
        // cout<<"\nUsing algo by SSP...\n";
    #elif SK
        // cout<<"\nUsing algo by SK...\n";
    #elif BCER
        // cout<<"\nUsing algo by BCER...\n";
    #else
        // cout<<"\nUsing algo by EC...\n";
    #endif
    // int seed = chrono::system_clock::now().time_since_epoch().count();
    // int n = pow(10, stod(argv[1])/10), p = pow(10, stod(argv[2])/10),seed = 42;
    int file_index = stoi(argv[1]);
    // double o = stod(argv[3]);
    // test_validity();
    // random_strings_stress_test(n, p, seed);
    // random_string_reads_stress_test(n, p, o, seed);
    real_data_test(file_index);
    return 0;
}