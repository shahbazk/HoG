#include<bits/stdc++.h>
using namespace std;

#include "timer.h"
#include "trace.h"

#ifdef SSP

#include "HOG-SSP.h"
typedef HOG_SSP HOG;

#else

#include "HOG-SK.h"
typedef HOG_SK HOG;

#endif

const int TRIALS = 10;

//TODO : write bigger validity test comparing dumps from both algos
void test_validity() {
    cout << "\nTesting validity of algorithm\n";
    vector<string> v = {"aabaa", "aadbd", "dbdaa"};
    HOG hog(v);
    assert(hog.marked == vector<bool>({0,1,0,1,0,0,1,0,0,1,0,0,1,0,1}));
    cout<<"All tests passed\n";
}

pair<double, double> test_with(const vector<string>& v, bool verbose = false) {
    HOG hog;
    if(verbose) {cout<<"Building Aho-Corasick automaton..."; cout.flush();}
    timer ahocora_t;
    hog.add_strings(v);
    double aho_time = ahocora_t.end();
    if(verbose) {cout<<"Elapsed time: " << aho_time; cout.flush();}
    
    if(verbose) {cout<<"Constructing HOG..."; cout.flush();}
    timer hog_t;
    hog.construct();
    double hog_time = hog_t.end();
    if(verbose) {cout<<"Elapsed time: " << hog_time; cout.flush();}

    if(verbose) {
        int cnt = 0;
        for(auto b:hog.marked) cnt+=b;
        cout << "Size of Aho-Corasick trie: " << hog.marked.size()-1 
        << ", Size of HOG: " << cnt
        << ", Compression factor: "<< (double)cnt/(hog.marked.size()-1);
        cout.flush();
    }
    return {aho_time, hog_time};
}

pair<double, double> get_mean_and_sd(vector<double> &a) {
    sort(a.begin(), a.end());
    double sum = 0, sq_sum = 0, cnt=0;
    for(int i=a.size()/10;i<(9*a.size()/10);i++) {
        sum += a[i];
        sq_sum += a[i]*a[i];
        cnt++;
    }
    double avg = sum/cnt, sd = sqrt(sq_sum/cnt - avg*avg);
    return {avg, sd};
}

void stress_test_with(function<vector<string>()> generator) {
    // cout<<"Number of trials: " << TRIALS << endl;
    vector<double> aho_times(TRIALS), hog_times(TRIALS), tot_times(TRIALS);
    for(int i=0;i<TRIALS;i++) {
        auto v = generator();
        auto results = test_with(v);
        aho_times[i] = results.first;
        hog_times[i] = results.second;
        tot_times[i] = aho_times[i] + hog_times[i];
    }
    auto aho_data = get_mean_and_sd(aho_times);
    auto hog_data = get_mean_and_sd(hog_times);
    auto tot_data = get_mean_and_sd(tot_times);
    // cout<<"Aho: "<<aho_data.first<<' '<<aho_data.second<<'\n';
    // cout<<"HOG: "<<hog_data.first<<' '<<hog_data.second<<'\n';
    cout<<fixed<<setprecision(6)<<tot_data.first<<','<<tot_data.second<<',';
    // cout<<"Time taken by Aho-Corasick algorithm: " << aho_times[TRIALS/2] << "s\n";
    // cout<<"Time taken by HOG algorithm: " << hog_times[TRIALS/2] << "s\n";
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

void real_data_test() {
    // cout<<"\nRunning on real datasets...\n";
    string data_path = "data/";
    // vector<string> filenames = {"clementina", "sinensis", "trifoliata", "elegans"};
    vector<string> filenames = {"trifoliata"};
    for(string fname:filenames) {
        cout<<'\n'<<fname<<":\n";
        fstream fin;
        fin.open(data_path+fname, ios::in);
        if(!fin) {
            cout<<"couldn't open file: "<<fname<<endl;
            continue;
        }
        long long n, total_length = 0;
        fin>>n;
        vector<string> v(n);
        for(int i=0;i<n;i++) {
            fin>>v[i];
            total_length += v[i].length();
        }
        cout<<"Number of strings = "<<v.size()<<'\n'<<"Sum of lengths = "<<total_length<<'\n';

        stress_test_with([&](){return v;});
    }
}

int main(int argc, char **argv) {
    #ifdef SSP
        // cout<<"\nUsing algo by SSP...\n";
    #else 
        // cout<<"\nUsing algo by SK...\n";
    #endif
    // int seed = chrono::system_clock::now().time_since_epoch().count();
    int n = pow(10, stod(argv[1])/10), p = pow(10, stod(argv[2])/10),seed = 42;
    // double o = stod(argv[3]);
    // test_validity();
    random_strings_stress_test(n, p, seed);
    // random_string_reads_stress_test(n, p, o, seed);
    // real_data_test();
    return 0;
}