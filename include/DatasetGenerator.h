#pragma once

#include<bits/stdc++.h>

class DatasetGenerator {

    public:
    static std::vector<std::string> generate_real_data(std::string datasetName);
    static std::vector<std::string> generate_random_data(int k, int n, int seed);
    static std::vector<std::string> generate_random_read_data(int complete_len, int snapshot_len, int coverage, int seed);
    static void dump_data(const std::vector<std::string>& v, std::ofstream &fout);
    static std::vector<std::string> read_data(std::ifstream &fin);
};
