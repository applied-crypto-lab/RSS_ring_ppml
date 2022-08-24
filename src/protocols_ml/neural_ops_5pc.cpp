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

#include "neural_ops_5pc.h"

// computes the 2d depthwise convolution on an input tensor x w/ weight matrix W

void MaxPool_mp(Lint **res, Lint **a, uint c, uint m, uint n, uint batch_size, uint ring_size, NodeNetwork *nodeNet, uint flag) {
    if (flag == 0) {
        old_MaxPool_mp(res, a, c, m, n, batch_size, ring_size, nodeNet);
    } else {
        eda_MaxPool_mp(res, a, c, m, n, batch_size, ring_size, nodeNet);
    }
}

void old_MaxPool_mp(Lint **res, Lint **a, uint c, uint m, uint n, uint batch_size, uint ring_size, NodeNetwork *nodeNet) {
    // takes in matrix of dimension m x n and pools according to the window
    // output matrix dimensions are calculated based off of inputs
    // we exclude the z direction in function since it is one in all cases
    uint i, j, k, l; // index; // used for loops
    // uint bytes = ((ring_size) + 7) >> 3;
    uint numShares = nodeNet->getNumShares();

    // uint size = (c*m*n)/2;
    uint size = (c * m * n) / 2;
    // printf("size : %u\n", size);

    // Lint *res2 = new Lint [100000];
    // memset(res2,0,sizeof(Lint)*100000);

    Lint **x0 = new Lint *[numShares];
    Lint **x1 = new Lint *[numShares];
    Lint **temp = new Lint *[numShares];
    for (i = 0; i < numShares; i++) {
        x0[i] = new Lint[size * batch_size];
        memset(x0[i], 0, sizeof(Lint) * (size * batch_size));
        x1[i] = new Lint[size * batch_size];
        memset(x1[i], 0, sizeof(Lint) * (size * batch_size));
        temp[i] = new Lint[size * batch_size];
        memset(temp[i], 0, sizeof(Lint) * (size * batch_size));
    }

    for (j = 0; j < batch_size; j++) {

        for (i = 0; i < size; i++) {
            for (size_t s = 0; s < numShares; s++) {

                x0[s][j * size + i] = a[s][j * size * 2 + 2 * i];
                // x0[1][j * size + i] = a[1][j * size * 2 + 2 * i];
                x1[s][j * size + i] = a[s][j * size * 2 + 2 * i + 1];
                // x1[1][j * size + i] = a[1][j * size * 2 + 2 * i + 1];
            }
        }
    }

    Rss_LT_mp(temp, x0, x1, size * batch_size, ring_size, nodeNet); // c is the problem

    for (i = 0; i < numShares; i++) {
        memset(x0[i], 0, sizeof(Lint) * (size * batch_size));
        memset(x1[i], 0, sizeof(Lint) * (size * batch_size));
        // memset(res[i],0,sizeof(Lint)*(size));
    }
    size = size / 2;
    // printf("size : %u\n", size);

    // Rss_Open(res2, temp, 16*12*12, map, ring_size, nodeNet);
    // for(int i =0; i< 30; i++) {
    //     printf("res[%i]  : %llu\n", i, res2[i]);
    // }
    for (l = 0; l < batch_size; l++) {
        k = 0;
        for (i = 0; i < c * m / 2; i++) {
            for (j = 0; j < n / 2; j++) {
                for (size_t s = 0; s < numShares; s++) {

                    // printf("j + i*n : %u\n", j + i*n);
                    // printf("j + m/2 + i*n : %u\n", j + m/2 +i*n);
                    x0[s][l * size + k] = temp[s][2 * l * size + j + i * n];
                    // x0[1][l * size + k] = temp[1][2 * l * size + j + i * n];
                    x1[s][l * size + k] = temp[s][2 * l * size + j + m / 2 + i * n];
                    // x1[1][l * size + k] = temp[1][2 * l * size + j + m / 2 + i * n];
                }

                k++;
            }
        }
    }

    Rss_LT_mp(res, x0, x1, size * batch_size, ring_size, nodeNet);

    // delete [] res2;
    // cleanup
    for (i = 0; i < numShares; i++) {
        delete[] x0[i];
        delete[] temp[i];
        delete[] x1[i];
    }
    delete[] x0;
    delete[] x1;
    delete[] temp;
}

void eda_MaxPool_mp(Lint **res, Lint **a, uint c, uint m, uint n, uint batch_size, uint ring_size, NodeNetwork *nodeNet) {
    // takes in matrix of dimension m x n and pools according to the window
    // output matrix dimensions are calculated based off of inputs
    // we exclude the z direction in function since it is one in all cases
    uint i, j, k, l; //index; // used for loops
    // uint bytes = ((ring_size) + 7) >> 3;
    uint numShares = nodeNet->getNumShares();

    // uint size = (c*m*n)/2;
    uint size = (c * m * n) / 2;
    // printf("size : %u\n", size);

    // Lint *res2 = new Lint [100000];
    // memset(res2,0,sizeof(Lint)*100000);

    Lint **x0 = new Lint *[numShares];
    Lint **x1 = new Lint *[numShares];
    Lint **temp = new Lint *[numShares];
    for (i = 0; i < numShares; i++) {
        x0[i] = new Lint[size * batch_size];
        memset(x0[i], 0, sizeof(Lint) * (size * batch_size));
        x1[i] = new Lint[size * batch_size];
        memset(x1[i], 0, sizeof(Lint) * (size * batch_size));
        temp[i] = new Lint[size * batch_size];
        memset(temp[i], 0, sizeof(Lint) * (size * batch_size));
    }

    for (j = 0; j < batch_size; j++) {

        for (i = 0; i < size; i++) {
            for (size_t s = 0; s < numShares; s++) {
                x0[s][j * size + i] = a[s][j * size * 2 + 2 * i];
                x1[s][j * size + i] = a[s][j * size * 2 + 2 * i + 1];
            }
            // x0[0][j * size + i] = a[0][j * size * 2 + 2 * i];
            // x0[1][j * size + i] = a[1][j * size * 2 + 2 * i];
            // x1[0][j * size + i] = a[0][j * size * 2 + 2 * i + 1];
            // x1[1][j * size + i] = a[1][j * size * 2 + 2 * i + 1];
        }
    }

    new_Rss_LT_mp(temp, x0, x1, size * batch_size, ring_size, nodeNet); // c is the problem

    for (i = 0; i < numShares; i++) {
        memset(x0[i], 0, sizeof(Lint) * (size * batch_size));
        memset(x1[i], 0, sizeof(Lint) * (size * batch_size));
        // memset(res[i],0,sizeof(Lint)*(size));
    }
    size = size / 2;
    // printf("size : %u\n", size);

    // Rss_Open(res2, temp, 16*12*12, map, ring_size, nodeNet);
    // for(int i =0; i< 30; i++) {
    //     printf("res[%i]  : %llu\n", i, res2[i]);
    // }
    for (l = 0; l < batch_size; l++) {
        k = 0;
        for (i = 0; i < c * m / 2; i++) {
            for (j = 0; j < n / 2; j++) {

                for (size_t s = 0; s < numShares; s++) {
                    // printf("j + i*n : %u\n", j + i*n);
                    // printf("j + m/2 + i*n : %u\n", j + m/2 +i*n);
                    x0[s][l * size + k] = temp[s][2 * l * size + j + i * n];
                    // x0[1][l * size + k] = temp[1][2 * l * size + j + i * n];
                    x1[s][l * size + k] = temp[s][2 * l * size + j + m / 2 + i * n];
                    // x1[1][l * size + k] = temp[1][2 * l * size + j + m / 2 + i * n];
                }
                k++;
            }
        }
    }

    new_Rss_LT_mp(res, x0, x1, size * batch_size, ring_size, nodeNet);

    // delete [] res2;
    // cleanup
    for (i = 0; i < numShares; i++) {
        delete[] x0[i];
        delete[] temp[i];
        delete[] x1[i];
    }
    delete[] x0;
    delete[] x1;
    delete[] temp;
}

void ReLU_mp(Lint **res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet, uint flag) {
    if (flag == 0) {
        old_ReLU_mp(res, a, size, ring_size, nodeNet);
    } else {
        eda_ReLU_mp(res, a, size, ring_size, nodeNet);
    }
}

// does not need to be changed for batch optimization
void old_ReLU_mp(Lint **res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet) {
    // takes in matrix of dimension m x n and calculates the ReLU for each element
    // output dimensions same as input
    uint i; // j, k, index; // used for loops
    // uint bytes = ((ring_size) + 7) >> 3;
    uint numShares = nodeNet->getNumShares();

    Lint **zero = new Lint *[numShares];
    for (i = 0; i < numShares; i++) {
        zero[i] = new Lint[size];
        memset(zero[i], 0, sizeof(Lint) * (size));
    }
    Rss_LT_mp(res, a, zero, size, ring_size, nodeNet);
    // cleanup
    for (i = 0; i < numShares; i++) {
        delete[] zero[i];
    }
    delete[] zero;
}

// does not need to be changed for batch optimization
void eda_ReLU_mp(Lint **res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet) {
    // takes in matrix of dimension m x n and calculates the ReLU for each element
    // output dimensions same as input
    uint i; // used for loops
    uint numShares = nodeNet->getNumShares();

    Lint **zero = new Lint *[numShares];
    for (i = 0; i < numShares; i++) {
        zero[i] = new Lint[size];
        memset(zero[i], 0, sizeof(Lint) * (size));
    }
    new_Rss_LT_mp(res, a, zero, size, ring_size, nodeNet);
    // cleanup
    for (i = 0; i < numShares; i++) {
        delete[] zero[i];
    }
    delete[] zero;
}

// upper will be ALL of the alpha*6 values for the network
// we compare all the x's to upper[layer]
void ReLU_6_alpha_timer_mp(Lint **res, Lint **x, Lint **upper, uint size, uint ring_size, uint layer, NodeNetwork *nodeNet, unsigned long &timer) {
    // uint i;

    // Lint **zero = new Lint *[2];
    // for (i = 0; i < numShares; i++) {
    //     zero[i] = new Lint[1];
    //     memset(zero[i], 0, sizeof(Lint) * (1));
    // }
    // must be done sequentially
    // first compare with zero
    // printf("num edaBits : %u\n", 2*size);
    // fixed_Rss_LT_time(res, x, zero, size, ring_size, 0, map, nodeNet, timer);
    // // then compare with 6*alpha
    // fixed_Rss_GT_time(res, res, upper, size, ring_size, layer, nodeNet, timer);

    fixed_Rss_GT_LT_time_mp(res, res, upper, size, ring_size, layer, nodeNet, timer);

    // // cleanup
    // for (i = 0; i < numShares; i++) {
    //     delete[] zero[i];
    // }
    // delete[] zero;
}

void ReLU_6_alpha_mp(Lint **res, Lint **x, Lint **upper, uint size, uint ring_size, uint layer, NodeNetwork *nodeNet) {
    uint i;
    uint numShares = nodeNet->getNumShares();

    Lint **zero = new Lint *[numShares];
    for (i = 0; i < numShares; i++) {
        zero[i] = new Lint[1];
        memset(zero[i], 0, sizeof(Lint) * (1));
    }
    // must be done sequentially
    // first compare with zero
    fixed_Rss_LT_mp(res, x, zero, size, ring_size, 0, nodeNet);
    // then compare with 6*alpha
    fixed_Rss_GT_mp(res, res, upper, size, ring_size, layer, nodeNet);

    // cleanup
    for (i = 0; i < numShares; i++) {
        delete[] zero[i];
    }
    delete[] zero;
}

// does NOT compute the average, just sums the results (no division)
void Rss_avg_pool_trunc_mp(Lint **res, Lint **x, uint in_channels, uint *n, uint kernel, uint stride, uint batch_size, uint ring_size, NodeNetwork *nodeNet, unsigned long &timer) {
    uint input_dim = in_channels * (*n) * (*n);
    uint out_dim = in_channels * batch_size;

    Lint t_prime = log2((*n) * (*n));
    uint numShares = nodeNet->getNumShares();

    uint i, j, k, l;

    // destination must be sanitized
    for (i = 0; i < numShares; i++) {
        memset(res[i], 0, sizeof(Lint) * out_dim);
    }

    uint out_index = 0;
    for (l = 0; l < batch_size; l++) {
        for (i = 0; i < in_channels; i++) {
            for (j = 0; j < (*n); j++) {
                for (k = 0; k < (*n); k++) {
                    for (size_t s = 0; s < numShares; s++)
                        res[s][out_index] += x[s][k * in_channels + j * (*n) * in_channels + i + l * input_dim];
                    // res[1][out_index] += x[1][k * in_channels + j * (*n) * in_channels + i + l * input_dim];
                }
            }
            out_index++;
        }
    }

    Rss_truncPr_time_mp(res, res, t_prime, out_dim, ring_size, nodeNet, timer);

    // updating size of n
    *n = (*n / *n);
}