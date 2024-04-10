#include<bits/stdc++.h>

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

using namespace std;

class DatasetGenerator {

    public:

    static vector<string> generate_real_data(string datasetName){
        string data_path = "data/";
        fstream fin;
        fin.open(data_path+datasetName, ios::in);
        if(!fin) {
            cout<<"couldn't open file: "<<datasetName<<endl;
            return {};
        }
        long long k, total_length = 0;
        fin>>k;
        vector<string> v(k);
        for(int i=0;i<k;i++) {
            fin>>v[i];
            total_length += v[i].length();
        }

        /* cout<<"Dataset Name = " << datasetName << "\n";
        cout<<"Number of strings = "<<v.size()<<'\n'<<"Sum of lengths = "<<total_length<<'\n'; */

        cout<<","<<datasetName<<","<<v.size()<<","<<total_length;

        return v;
    }

    static vector<string> generate_random_data(int k, int n, int seed) {
        assert(n>=k);
        // cout << "\nTesting on randomly generated strings...\n" << "N = " << n << ", P = " << p << '\n';
        srand(seed);

        cout << k << ',' << n << ','; cout.flush();
        vector<string> v(k);
        int j=0;
        for(int i=0;i<n;i++,j++) {
            if(j>=k) j-=k;
            v[j] += ('a'+rand()%alphabet);
        }
        return v;
    }

    static vector<string> generate_random_read_data(int k, int n, int overlap, int seed) {
        assert(n>=k);
        assert(0.0<overlap);
        assert(overlap<1.0);
        int len = n/k;
        int total_len = n*(1.0-overlap) + len*overlap;
        // cout << "\nTesting on randomly generated reads on a randomly generated string...\n" << "N = " << n << ", P = " << p << ", o = " << overlap << '\n';
        cout << k << ',' << n << ',' << overlap << ','; cout.flush();
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

    static void dump_data(const vector<string>& v, ofstream &fout) {
        fout << v.size() << '\n';
        for(auto &s : v) {
            fout << s << '\n';
        }
    }

    static vector<string> read_data(ifstream &fin) {
        long long k;
        fin >> k;
        vector<string> v(k);
        for(int i=0; i<k; i++) {
            fin >> v[i];
        }
        return v;
    }
};

class TestRunner {

    public:
    static double aho_construct_and_print(const vector<string> &v, const string output_file_name){
        timer time_aho;
        AhoCorasick aho(v);
        double time_aho_build = time_aho.end();
        cout << aho.t.size() << ',' << time_aho_build << ',';
        ofstream fout;
        fout.open("./dump/aho/"+output_file_name);
        aho.file_output(fout);
        return time_aho_build;
    }

    static double ehog_construct_and_print(AhoCorasick &aho, const string output_file_name){
        timer time_ehog;
        EHOG ehog(aho);
        double time_ehog_build = time_ehog.end();
        cout << ehog.t.size() << ',' << time_ehog_build << ',';
        ofstream fout;
        fout.open("./dump/ehog/" + output_file_name);
        ehog.file_output(fout);
        return time_ehog_build;
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

    // string dataset_name = argv[1];
    // string output_file_name = dataset_name;

    int k = stoi(argv[1]), n = stoi(argv[2]), seed = stoi(argv[3]);
    string output_file_name = "random/k_" + to_string(k) + "_n_" + to_string(n) + "_seed_" + to_string(seed);

    // int k = stoi(argv[1]), n = stoi(argv[2]), seed = stoi(argv[3]);
    // double o = stod(argv[3]);



    #ifdef CONSTRUCT_AHO

    // create dump directories
    filesystem::create_directory("dump");
    for(string dump_type: {"data", "aho", "ehog"}) {
        for(string test_type: {"real", "random"}) {
            filesystem::create_directories("dump/" + dump_type + "/" + test_type);
        }
    }

    // vector<string> dataset = DatasetGenerator::generate_real_data(dataset_name);
    vector<string> dataset = DatasetGenerator::generate_random_data(k, n, seed);
    // vector<string> dataset = DatasetGenerator::generate_random_read_data(k, n, o, seed);

    ofstream fout;
    fout.open("./dump/data/" + output_file_name, ios::out);
    DatasetGenerator::dump_data(dataset, fout);

    TestRunner::aho_construct_and_print(dataset, output_file_name);

    #elif CONSTRUCT_EHOG

    string data_path = "./dump/aho/" + output_file_name;
    ifstream fin;
    fin.open(data_path, ios::in);
    if(!fin) {
        cout<<"couldn't open file: "<< data_path <<endl;
        return 0;
    }
    AhoCorasick ahocora(fin);
    TestRunner::ehog_construct_and_print(ahocora, output_file_name);

    #elif SP

    string data_path = "./dump/data/" + output_file_name;
    ifstream fin;
    fin.open(data_path, ios::in);
    if(!fin) {
        cout<<"couldn't open file: "<< data_path <<endl;
        return 0;
    }
    auto dataset = DatasetGenerator::read_data(fin);
    fin.close();

    data_path = "./dump/aho/" + output_file_name;
    fin.open(data_path, ios::in);
    if(!fin) {
        cout<<"couldn't open file: "<< data_path <<endl;
        return 0;
    }
    AhoCorasick ahocora(fin);
    fin.close();

    #ifdef VIA_EHOG

    data_path = "./dump/ehog/" + output_file_name;
    fin.open(data_path, ios::in);
    if(!fin) {
        cout<<"couldn't open file: "<< data_path <<endl;
        return 0;
    }
    EHOG ehog(fin);
    fin.close();

    timer hog_timer;
    HOG hog(ehog, ahocora, dataset);
    cout << hog_timer.end() << ',';

    #else

    timer hog_timer;
    HOG hog(ahocora, dataset);
    cout << hog_timer.end() << ',';

    #endif // VIA_EHOG

    #else

    #ifdef VIA_EHOG

    string data_path = "./dump/ehog/" + output_file_name;
    ifstream fin;
    fin.open(data_path, ios::in);
    if(!fin) {
        cout<<"couldn't open file: "<< data_path <<endl;
        return 0;
    }
    EHOG ehog(fin);
    timer hog_timer;
    HOG hog(ehog);
    cout << hog_timer.end() << ',';

    #else

    string data_path = "./dump/aho/" + output_file_name;
    ifstream fin;
    fin.open(data_path, ios::in);
    if(!fin) {
        cout<<"couldn't open file: "<< data_path <<endl;
        return 0;
    }
    AhoCorasick ahocora(fin);
    timer hog_timer;
    HOG hog(ahocora);
    cout << hog_timer.end() << ',';

    #endif // VIA_EHOG

    #endif

    return 0;
}
