#include "../include/output_io.h"

void write_benchmark(string fname, vector<string> exp_names, vector<vector<double>> exp_times, vector<vector<double>> exp_ratios, uint ring_size) {

    ofstream thefile;
    thefile.open(fname);

    for (size_t i = 0; i < exp_names.size() - 1; i++) {
        thefile << exp_names.at(i) + "_" +to_string(ring_size) + " & ";
        for (size_t j = 0; j < exp_times.at(i).size(); j++) {
            thefile << exp_times.at(i).at(j) << " & ";
        }
        thefile << "\\\\ \n";
        thefile << exp_names.at(i) + "_" +to_string(ring_size) + "_ratio & ";

        for (size_t j = 0; j < exp_ratios.at(i).size(); j++) {
            thefile << exp_ratios.at(i).at(j) << " & ";
        }
        thefile << "\\\\ \n";
    }

    thefile << exp_names.at(exp_names.size() - 1) + "_" +to_string(ring_size) +  " & ";
    for (size_t j = 0; j < exp_times.at(exp_names.size() - 1).size(); j++) {
        thefile << exp_times.at(exp_names.size() - 1).at(j) << " & ";
    }
    thefile << "\\\\ \n";
    thefile << exp_names.at(exp_names.size() - 1) + "_" +to_string(ring_size) +  "_ratio & ";

    for (size_t j = 0; j < exp_ratios.at(exp_names.size() - 1).size(); j++) {
        thefile << exp_ratios.at(exp_names.size() - 1).at(j) << " & ";
    }
    thefile << "\\\\ \n";

    thefile.close();
}
