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
    srand(k + n + seed);

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

vector<string> DatasetGenerator::generate_random_read_data(int complete_len,
                                                           int snapshot_len,
                                                           int coverage,
                                                           int seed) {
    assert(snapshot_len <= complete_len);
    int n = complete_len * coverage, k = n / snapshot_len;
    // cout << "\nTesting on randomly generated reads on a randomly generated
    // string...\n" << "k = " << k << ", n = " << n << ", o = " << overlap <<
    // '\n';
    cout << complete_len << ',' << snapshot_len << ',' << coverage << ',' << k << ',' << n << ',';
    cout.flush();
    srand(complete_len + snapshot_len + coverage + seed);
    string complete_string = "";
    for (int i = 0; i < complete_len; i++)
        complete_string += ('a' + rand() % alphabet);

    vector<string> v;
    // might miss a few ending characters
    for (int i = 0; i < k; i++) {
        v.push_back(complete_string.substr(
            rand() % (complete_len - snapshot_len + 1), snapshot_len));
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
