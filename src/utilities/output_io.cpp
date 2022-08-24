/*   
   Multi-Party Replicated Secret Sharing over a Ring 
   ** Copyright (C) 2022 Alessandro Baccarini, Marina Blanton, and Chen Yuan
   ** Department of Computer Science and Engineering, University of Buffalo (SUNY)

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

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
