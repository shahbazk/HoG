#include<bits/stdc++.h>
using namespace std;

#include "timer.h"
#include "trace.h"

#include "HOG-SKx.h"
typedef HOG_SKx HOG;

const int TRIALS = 10000000;
#define SEED 346
void test_validity_queries(){
    cout << "\nTesting validity of queries\n";
    vector<string> v = {"aabaa", "dbdaa", "aadbd"};
    // vector<string> v = {"aab", "aabcd", "bcd"};
    HOG hog(v);
    trace(hog.marked);
    // assert(hog.marked == vector<bool>({0,1,0,1,0,0,1,0,0,1,0,0,1,0,1}));
    Hog hg = hog.make_hog();
    // trace(hg.top(0,3));
    trace(hg.suffix_prefix_length(0,2));
    trace(hg.suffix_prefix_length(2,0));
    trace(hg.suffix_prefix_length(0,1));
    trace(hg.suffix_prefix_length(1,0));
    trace(hg.suffix_prefix_length(2,1));
    trace(hg.suffix_prefix_length(1,2));
    trace(hg.one_to_all(0));
    trace(hg.one_to_all(1));
    trace(hg.one_to_all(2));
    trace(hg.top(0,3));
    trace(hg.top(1,3));
    trace(hg.top(2,3));
    trace(hg.report(0,1));
    trace(hg.report(1,1));
    trace(hg.report(2,1));
    trace(hg.count(0,1));
    trace(hg.count(1,1));
    trace(hg.count(2,1));
    assert(hg.suffix_prefix_length(0,2)==2);
    assert(hg.suffix_prefix_length(2,0)==0);
    assert(hg.suffix_prefix_length(0,1)==0);
    assert(hg.suffix_prefix_length(1,0)==2);
    assert(hg.suffix_prefix_length(2,1)==3);
    assert(hg.suffix_prefix_length(1,2)==2);
    assert((hg.one_to_all(0) == std::vector<int>{5, 0, 2}));
    assert((hg.one_to_all(1) == std::vector<int>{2, 5, 2}));
    assert((hg.one_to_all(2) == std::vector<int>{0, 3, 5}));
    trace(hg.top(0,3));
    trace(hg.top(1,3));
    trace(hg.top(2,3));
    assert((hg.report(0,1) == std::vector<int>{0,2}));
    assert((hg.report(1,1) == std::vector<int>{1, 0, 2}));
    assert((hg.report(2,1) == std::vector<int>{2, 1}));
    assert((hg.count(0,1) == 2));
    assert((hg.count(1,1) == 3));
    assert((hg.count(2,1) == 2));
    cout<<"All tests passed\n";
}

pair<double, double> get_mean_and_sd(vector<double> &a) {
    sort(a.begin(), a.end());
    double sum = 0, sq_sum = 0, cnt=0;
    for(int i=a.size()/10;i<(int)(9*a.size()/10);i++) {
        sum += a[i];
        sq_sum += a[i]*a[i];
        cnt++;
    }
    double avg = sum/cnt, sd = sqrt(sq_sum/cnt - avg*avg);
    return {avg, sd};
}

void stress_test_with(const vector<string>& v, int seed) {
    HOG hog;
    hog.add_strings(v);
    timer t1;
    hog.construct();
    std::cout << t1.end() << std::endl;

    timer t2;
    Hog hg = hog.make_hog();
    std::cout << t2.end() << "\n";
    std::cout << "Construction Finished" << std::endl;
    srand(seed);
    vector<pair<int,int>>queries(TRIALS);
    for(int i = 0;i<TRIALS;i++){
        int a = rand()%hg.sorted_order_conversion.size();
        int b = rand()%hg.sorted_order_conversion.size();
        if(a==b)i--;
        else{
            queries[i] = {a,b};
        }
    }
    timer q_time;
    for(int i = 0;i<TRIALS;i++){
        hg.suffix_prefix_length(queries[i].first, queries[i].second);
    }
    double tot_time = q_time.end();
    std::cout << "Time Taken for one-to-one " << TRIALS << "queries is " << tot_time << "\n";
}

void stress_test_with_all(const vector<string>& v, int seed) {
    HOG hog;
    hog.add_strings(v);
    timer t1;
    hog.construct();
    std::cout << t1.end() << std::endl;

    timer t2;
    Hog hg = hog.make_hog();
    std::cout << t2.end() << "\n";
    std::cout << "Construction Finished" << std::endl;
    srand(seed);
    std::vector<int>queries(TRIALS / 1000);
    for(int i = 0;i<TRIALS/1000;i++){
        queries[i] = rand()%hg.sorted_order_conversion.size();
    }
    timer q_time;
    for(int i = 0;i<TRIALS/1000;i++){
        hg.one_to_all(queries[i]);
    }
    double tot_time = q_time.end();
    std::cout << "Time Taken for one-to-all " << TRIALS/1000 << "queries is " << tot_time << "\n";
}
void stress_test_with_top(const vector<string>& v, int seed) {
    HOG hog;
    hog.add_strings(v);
    timer t1;
    hog.construct();
    std::cout << t1.end() << std::endl;

    timer t2;
    Hog hg = hog.make_hog();
    std::cout << t2.end() << "\n";
    std::cout << "Construction Finished" << std::endl;
    srand(seed);
    vector<pair<int,int>>queries(TRIALS/1000);
    for(int i = 0;i<TRIALS/1000;i++){
        int a = rand()%hg.sorted_order_conversion.size();
        int b = rand()%hg.sorted_order_conversion.size() + 1;
        queries[i] = {a,b};
    }
    timer q_time;
    for(int i = 0;i<TRIALS/1000;i++){
        hg.top(queries[i].first, queries[i].second);
    }
    double tot_time = q_time.end();
    std::cout << "Time Taken for top " << TRIALS/1000 << "queries is " << tot_time << "\n";
}
void stress_test_with_report(const vector<string>& v, int seed) {
    HOG hog;
    hog.add_strings(v);
    timer t1;
    hog.construct();
    std::cout << t1.end() << std::endl;

    timer t2;
    Hog hg = hog.make_hog();
    std::cout << t2.end() << "\n";
    std::cout << "Construction Finished" << std::endl;
    srand(seed);
    vector<pair<int,int>>queries(TRIALS/1000);
    for(int i = 0;i<TRIALS/1000;i++){
        int a = rand()%hg.sorted_order_conversion.size();
        int b = rand()%10 + 1;
        queries[i] = {a,b};
    }
    timer q_time;
    for(int i = 0;i<TRIALS/1000;i++){
        hg.report(queries[i].first, queries[i].second);
    }
    double tot_time = q_time.end();
    std::cout << "Time Taken for report " << TRIALS/1000 << "queries is " << tot_time << "\n";
}
void stress_test_with_count(const vector<string>& v, int seed) {
    HOG hog;
    hog.add_strings(v);
    timer t1;
    hog.construct();
    std::cout << t1.end() << std::endl;

    timer t2;
    Hog hg = hog.make_hog();
    std::cout << t2.end() << "\n";
    std::cout << "Construction Finished" << std::endl;
    srand(seed);
    vector<pair<int,int>>queries(TRIALS/1000);
    for(int i = 0;i<TRIALS/1000;i++){
        int a = rand()%hg.sorted_order_conversion.size();
        int b = rand()%10 + 1;
        queries[i] = {a,b};
    }
    timer q_time;
    for(int i = 0;i<TRIALS/1000;i++){
        hg.count(queries[i].first, queries[i].second);
    }
    double tot_time = q_time.end();
    std::cout << "Time Taken for count " << TRIALS/1000 << "queries is " << tot_time << "\n";
}
void real_data_test() {
    string data_path = "../data/parsed_data/";
    // vector<string> filenames = {"clementina", "sinensis", "trifoliata", "elegans"};
    // vector<string> filenames = {"trifoliata"};
    vector<string> filenames = { "A_hydrophila_HiSeq",
                                "B_cereus_HiSeq",
                                "B_cereus_MiSeq",
                                "B_fragilis_HiSeq" ,
                                "M_abscessus_HiSeq" ,
                                "M_abscessus_MiSeq",
                                "R_sphaeroides_HiSeq" ,
                                "R_sphaeroides_MiSeq" ,
                                "S_aureus_HiSeq",
                                "V_cholerae_HiSeq" ,
                                "V_cholerae_MiSeq",
                                "X_axonopodis_HiSeq"}; 
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
        stress_test_with(v, SEED);
        stress_test_with_all(v, SEED);
        stress_test_with_top(v,SEED);
        stress_test_with_count(v, SEED);
        stress_test_with_report(v, SEED);
    }
}

int main(){
    test_validity_queries();
    real_data_test();
}