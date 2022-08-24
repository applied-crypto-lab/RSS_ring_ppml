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

#include "mpc_util.h"

void print_binary(Lint n, uint size){
    uint temp = size-1;
    int i = size-1;
    uint b;
    while(i !=-1) {
        b = GET_BIT(n, temp);
        printf("%u", b);
        temp--;
        i -= 1;
    }
    printf("\n");
}

double absMax(double *input, uint size){
    double max = abs(input[0]);
    for (size_t i = 1; i < size; i++) {
        if (abs(input[i]) > max) {
            max = abs(input[i]);
        }
    }
    return max;
}

double absMax(std::vector<double> input){
    double max = abs(input[0]);
    for (size_t i = 1; i < input.size(); i++) {
        if (abs(input[i]) > max) {
            max = abs(input[i]);
        }
    }
    return max;
}


double getScale(uint bound, double *input, uint size){
    double abs_max = absMax(input, size);
    return ((double) bound / abs_max);
}

double getScale(uint bound, std::vector<double> x){

    double abs_max = absMax(x);
    return ((double) bound / abs_max);
}

void q_applyScale(sLint *output, double *input, double scale, uint size){
    for (size_t i = 0; i < size ; i++) {
        // y.at(i) = scale * x.at(i);
        output[i] = input[i] * scale;
    }
}
