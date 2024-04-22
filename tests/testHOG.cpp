#include<bits/stdc++.h>

#include "timer.h"
#include "DatasetGenerator.h"

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

void safe_open(ifstream& fin, string filename) {
    fin.open(filename, ios::in);
    if(!fin) {
        cerr<<"couldn't open file: "<< filename <<endl;
        exit(-1);
    }
}

void safe_open(ofstream& fout, string filename) {
    fout.open(filename, ios::out);
    if(!fout) {
        cerr<<"couldn't open file: "<< filename <<endl;
        exit(-1);
    }
}

class TestRunner {

    public:
    static double aho_construct_and_print(const vector<string> &v, const string output_file_name){
        timer time_aho;
        AhoCorasick aho(v);
        double time_aho_build = time_aho.end();
        cout << aho.t.size() << ',' << time_aho_build << ',';
        ofstream fout;
        safe_open(fout, "./dump/aho/"+output_file_name);
        aho.file_output(fout);
        return time_aho_build;
    }

    static double ehog_construct_and_print(AhoCorasick &aho, const string output_file_name){
        timer time_ehog;
        EHOG ehog(aho);
        double time_ehog_build = time_ehog.end();
        cout << ehog.t.size() << ',' << time_ehog_build << ',';
        ofstream fout;
        safe_open(fout, "./dump/ehog/" + output_file_name);
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

    // int k = stoi(argv[1]), n = stoi(argv[2]), seed = stoi(argv[3]);
    // string output_file_name = "random/" + to_string(seed);

    int complete_len = stoi(argv[1]), snapshot_len = stoi(argv[2]), coverage = stoi(argv[3]), seed = stoi(argv[4]);
    string output_file_name = "read_random/" + to_string(seed);



    #ifdef CONSTRUCT_AHO

    // create dump directories
    filesystem::create_directory("dump");
    for(string dump_type: {"data", "aho", "ehog"}) {
        for(string test_type: {"random", "read_random"}) {
            filesystem::create_directories("dump/" + dump_type + '/' + test_type);
        }
    }

    // vector<string> dataset = DatasetGenerator::generate_real_data(dataset_name);
    // vector<string> dataset = DatasetGenerator::generate_random_data(k, n, seed);
    vector<string> dataset = DatasetGenerator::generate_random_read_data(complete_len, snapshot_len, coverage, seed);

    ofstream fout;
    safe_open(fout, "./dump/data/" + output_file_name);
    DatasetGenerator::dump_data(dataset, fout);

    TestRunner::aho_construct_and_print(dataset, output_file_name);

    #elif CONSTRUCT_EHOG

    ifstream fin;
    safe_open(fin, "./dump/aho/" + output_file_name);
    AhoCorasick ahocora(fin);
    fin.close();
    TestRunner::ehog_construct_and_print(ahocora, output_file_name);

    #elif SP

    ifstream fin;
    safe_open(fin, "./dump/data/" + output_file_name);
    auto dataset = DatasetGenerator::read_data(fin);
    fin.close();

    safe_open(fin, "./dump/aho/" + output_file_name);
    AhoCorasick ahocora(fin);
    fin.close();

    #ifdef VIA_EHOG

    safe_open(fin, "./dump/ehog/" + output_file_name);
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

    ifstream fin;
    safe_open(fin, "./dump/ehog/" + output_file_name);
    EHOG ehog(fin);

    timer hog_timer;
    HOG hog(ehog);
    cout << hog_timer.end() << ',';

    #else

    ifstream fin;
    safe_open(fin, "./dump/aho/" + output_file_name);
    AhoCorasick ahocora(fin);

    timer hog_timer;
    HOG hog(ahocora);
    cout << hog_timer.end() << ',';

    #endif // VIA_EHOG

    #endif

    return 0;
}

