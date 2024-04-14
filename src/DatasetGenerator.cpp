#include "DatasetGenerator.h"
#include "Aho-Cora.h"
using namespace std;

vector<string> DatasetGenerator::generate_real_data(string datasetName) {
    string data_path = "data/";
    fstream fin;
    fin.open(data_path + datasetName, ios::in);
    if (!fin) {
        cout << "couldn't open file: " << datasetName << endl;
        return {};
    }
    long long k, total_length = 0;
    fin >> k;
    vector<string> v(k);
    for (int i = 0; i < k; i++) {
        fin >> v[i];
        total_length += v[i].length();
    }

    /* cout<<"Dataset Name = " << datasetName << "\n";
    cout<<"Number of strings = "<<v.size()<<'\n'<<"Sum of lengths =
    "<<total_length<<'\n'; */

    cout << "," << datasetName << "," << v.size() << "," << total_length;

    return v;
}

vector<string> DatasetGenerator::generate_random_data(int k, int n, int seed) {
    assert(n >= k);
    // cout << "\nTesting on randomly generated strings...\n" << "k = " << k <<
    // ", n = " << n << '\n';
    srand(seed);

    cout << k << ',' << n << ',';
    cout.flush();
    vector<string> v(k);
    int j = 0;
    for (int i = 0; i < n; i++, j++) {
        if (j >= k)
            j -= k;
        v[j] += ('a' + rand() % alphabet);
    }
    return v;
}

vector<string> DatasetGenerator::generate_random_read_data(int k, int n,
                                                           int overlap,
                                                           int seed) {
    assert(n >= k);
    assert(0.0 < overlap);
    assert(overlap < 1.0);
    int len = n / k;
    int total_len = n * (1.0 - overlap) + len * overlap;
    // cout << "\nTesting on randomly generated reads on a randomly generated
    // string...\n" << "k = " << k << ", n = " << n << ", o = " << overlap <<
    // '\n';
    cout << k << ',' << n << ',' << overlap << ',';
    cout.flush();
    srand(seed);
    string complete_string = "";
    for (int i = 0; i < total_len; i++)
        complete_string += ('a' + rand() % alphabet);

    vector<string> v;
    // might miss a few ending characters
    for (double i = 0; (int)i <= total_len - len;
         i += (double)len * (1.0 - overlap)) {
        v.push_back(complete_string.substr((int)i, len));
    }
    return v;
}

void DatasetGenerator::dump_data(const vector<string>& v, ofstream& fout) {
    fout << v.size() << '\n';
    for (auto& s : v) {
        fout << s << '\n';
    }
}

vector<string> DatasetGenerator::read_data(ifstream& fin) {
    long long k;
    fin >> k;
    vector<string> v(k);
    for (int i = 0; i < k; i++) {
        fin >> v[i];
    }
    return v;
}
