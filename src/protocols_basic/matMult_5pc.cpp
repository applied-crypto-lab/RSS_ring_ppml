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

#include "matMult_5pc.h"

void Rss_MatMultArray_batch_mp(Lint **c, Lint **a, Lint **b, uint m, uint n, uint s, uint ring_size, uint batch_size, uint weight_flag_a, uint weight_flag_b, NodeNetwork *nodeNet) {
    // modified implementation that uses a 1d array representation of a matrix
    // a [m*n] = i*n+j (accessing the (i,j)th element)
    // [m][n] size matrix a;   [n][s] size matrix b
    // ---> [m][s] size matrix c;
    // For party 1, a[0,1]=a_2,3; b[0,1]=b_2,3;  c[0,1] = c_2,3;
    // For party 2, a[0,1]=a_3,1; b[0,1]=b_3,1;  c[0,1] = c_3,1;
    // For party 3, a[0,1]=a_1,2; b[0,1]=b_1,2;  c[0,1] = c_1,2;

    // if weight_flag == 0 --> the a matrix is the same throughout the computation
    uint total_size = batch_size * m * s;
    uint bytes = (ring_size + 7) >> 3;
    uint i, j, k, l; // used for loops
    // printf("bytes: %u\n", bytes);

    uint p_prime, T_index;
    uint numShares = nodeNet->getNumShares();
    uint numParties = nodeNet->getNumParties();
    uint threshold = nodeNet->getThreshold();
    int pid = nodeNet->getID();
    uint8_t prg_ctrs[6] = {2, 3, 3, 2, 3, 2};

    Lint *v = new Lint[total_size];
    memset(v, 0, sizeof(Lint) * (total_size));

    Lint **recv_buf = new Lint *[threshold];
    for (i = 0; i < threshold; i++) {
        recv_buf[i] = new Lint[total_size];
        memset(recv_buf[i], 0, sizeof(Lint) * total_size);
    }

    uint8_t **buffer = new uint8_t *[numShares];
    for (i = 0; i < numShares; i++) {
        buffer[i] = new uint8_t[prg_ctrs[i] * bytes * total_size];
        nodeNet->prg_getrandom(i, bytes, prg_ctrs[i] * total_size, buffer[i]);
        // memset(buffer[i], 0, sizeof(uint8_t) * total_size[numShares] * bytes * size); // FOR

        // sanitizing destination (just in case)
        // memset(c[i], 0, sizeof(Lint) * total_size);

        // memset(buffer[i], 0, sizeof(uint8_t) * prg_ctrs[numShares] * bytes * size);
    }
    Lint z = 0;
    uint trackers[6] = {0, 0, 0, 0, 0, 0};

    Lint **b_transpose = new Lint *[numShares];
    for (i = 0; i < numShares; i++) {
        b_transpose[i] = new Lint[batch_size * n * s];
        memset(b_transpose[i], 0, sizeof(Lint) * batch_size * n * s);
    }
    // transposing
    for (int ss = 0; ss < numShares; ss++) {
        for (l = 0; l < batch_size; l++) {
            for (i = 0; i < n; ++i) {
                for (j = 0; j < s; ++j) {
                    b_transpose[ss][weight_flag_b * l * n * s + j * n + i] = b[ss][weight_flag_b * l * n * s + i * s + j];
                }
            }
        }
    }
    // uint8_t *buffer = new uint8_t[bytes * total_size];

    // nodeNet->prg_getrandom(1, bytes, total_size, buffer);
    // memset(buffer, 0, bytes*batch_size*m*s); //USED FOR TESTING

    // a [m*n] = i*n+j (accessing the (i,j)th element)
    // [m][n] size matrix a;   [n][s] size matrix b
    // ---> [m][s] size matrix c;

    uint a_index_1, b_index_1, v_index;
    for (l = 0; l < batch_size; l++) {
        for (i = 0; i < m; i++) {
            for (j = 0; j < s; j++) {
                for (k = 0; k < n; k++) {
                    a_index_1 = weight_flag_a * l * m * n + i * n + k;
                    b_index_1 = weight_flag_b * l * n * s + j * n + k;
                    v_index = l * m * s + i * s + j;

                    v[v_index] += a[0][a_index_1] * (b_transpose[0][b_index_1] + b_transpose[1][b_index_1] + b_transpose[2][b_index_1] + b_transpose[3][b_index_1] + b_transpose[4][b_index_1] + b_transpose[5][b_index_1]) +
                                  a[1][a_index_1] * (b_transpose[0][b_index_1] + b_transpose[1][b_index_1] + b_transpose[2][b_index_1] + b_transpose[3][b_index_1] + b_transpose[4][b_index_1] + b_transpose[5][b_index_1]) +
                                  a[2][a_index_1] * (b_transpose[1][b_index_1] + b_transpose[3][b_index_1]) +
                                  a[3][a_index_1] * (b_transpose[0][b_index_1] + b_transpose[2][b_index_1]) +
                                  a[4][a_index_1] * (b_transpose[0][b_index_1] + b_transpose[1][b_index_1]) +
                                  a[5][a_index_1] * (b_transpose[0][b_index_1] + b_transpose[4][b_index_1]);
                }

                for (p_prime = 1; p_prime < numParties + 1; p_prime++) {
                    for (T_index = 0; T_index < numShares; T_index++) {
                        z = 0;

                        if ((p_prime != (pid)) and (!(p_prime_in_T(p_prime, nodeNet->T_map_5pc[T_index]))) and (!(chi_p_prime_in_T(p_prime, nodeNet->T_map_5pc[T_index], numParties)))) {
                            memcpy(&z, buffer[T_index] + (trackers[T_index]) * bytes, bytes);
                            c[T_index][v_index] += z;
                            trackers[T_index] += 1;
                        } else if ((p_prime == pid) and (!(chi_p_prime_in_T(pid, nodeNet->T_map_5pc[T_index], numParties)))) {
                            memcpy(&z, buffer[T_index] + (trackers[T_index]) * bytes, bytes);
                            c[T_index][v_index] += z;
                            v[v_index] = v[v_index] - z;
                            trackers[T_index] += 1;
                        }
                    }
                }
            }
        }
    }

    nodeNet->SendAndGetDataFromPeer_Mult(v, recv_buf, total_size, ring_size);

    for (l = 0; l < batch_size; l++) {
        for (i = 0; i < m; i++) {
            for (j = 0; j < s; j++) {
                v_index = l * m * s + i * s + j;
                c[3][v_index] = c[3][v_index] + recv_buf[1][v_index];
                c[5][v_index] = c[5][v_index] + recv_buf[0][v_index];

                c[0][v_index] = c[0][v_index] + v[v_index];
            }
        }
    }

    for (i = 0; i < threshold; i++) {
        delete[] recv_buf[i];
    }

    for (i = 0; i < numShares; i++) {
        delete[] buffer[i];
        delete[] b_transpose[i];
    }

    // free
    delete[] b_transpose;
    delete[] v;
    delete[] buffer;
    delete[] recv_buf;
}

void Rss_dw_matrixmul_mp(Lint **res, Lint **x, Lint **W, uint width, uint height, uint channels, uint filter, uint ring_size, uint batch_size, uint stride, NodeNetwork *nodeNet) {

    uint total_size = batch_size * channels * (width / stride) * (height / stride);
    // uint in_dim = batch_size * channels * (width) * (height);
    uint padded_height = height + 2;

    uint W_index, x_index;
    uint dp_index = 0;

    uint bytes = (ring_size + 7) >> 3;
    uint i; // j, k, l; // used for loops
    // printf("bytes: %u\n", bytes);

    uint p_prime, T_index;
    uint numShares = nodeNet->getNumShares();
    uint numParties = nodeNet->getNumParties();
    uint threshold = nodeNet->getThreshold();
    int pid = nodeNet->getID();
    uint8_t prg_ctrs[6] = {2, 3, 3, 2, 3, 2};

    Lint *v = new Lint[total_size];
    memset(v, 0, sizeof(Lint) * (total_size));

    Lint **recv_buf = new Lint *[threshold];
    for (i = 0; i < threshold; i++) {
        recv_buf[i] = new Lint[total_size];
        memset(recv_buf[i], 0, sizeof(Lint) * total_size);
    }

    uint8_t **buffer = new uint8_t *[numShares];
    for (i = 0; i < numShares; i++) {
        buffer[i] = new uint8_t[prg_ctrs[i] * bytes * total_size];
        nodeNet->prg_getrandom(i, bytes, prg_ctrs[i] * total_size, buffer[i]);
        // memset(buffer[i], 0, sizeof(uint8_t) * total_size[numShares] * bytes * size); // FOR

        // sanitizing destination (just in case)
        // memset(c[i], 0, sizeof(Lint) * total_size);

        // memset(buffer[i], 0, sizeof(uint8_t) * prg_ctrs[numShares] * bytes * size);
    }
    Lint z = 0;
    uint trackers[6] = {0, 0, 0, 0, 0, 0};

    // uint8_t *buffer = new uint8_t[bytes * total_size];

    // nodeNet->prg_getrandom(1, bytes, total_size, buffer);
    // memset(buffer, 0, bytes*batch_size*m*s); //USED FOR TESTING

    // a [m*n] = i*n+j (accessing the (i,j)th element)
    // [m][n] size matrix a;   [n][s] size matrix b
    // ---> [m][s] size matrix c;

    for (uint b = 0; b < batch_size; b++) {
        for (uint i = (stride - 1); i < width; i += stride) {
            for (uint j = (stride - 1); j < height; j += stride) {
                for (uint c = 0; c < channels; c++) {
                    for (uint fi = 0; fi < filter; fi++) {
                        for (uint fj = 0; fj < filter; fj++) {

                            W_index = fi * filter * channels + fj * channels + c;

                            x_index = (i + fi) * (padded_height)*channels + (j + fj) * channels + c + (total_size / batch_size) * b; // (out_dim/batch_size) * b used for multiple input images

                            v[dp_index] += x[0][x_index] * (W[0][W_index] + W[1][W_index] + W[2][W_index] + W[3][W_index] + W[4][W_index] + W[5][W_index]) +

                                           x[1][x_index] * (W[0][W_index] + W[1][W_index] + W[2][W_index] + W[3][W_index] + W[4][W_index] + W[5][W_index]) +
                                           x[2][x_index] * (W[1][W_index] + W[3][W_index]) +
                                           x[3][x_index] * (W[0][W_index] + W[2][W_index]) +
                                           x[4][x_index] * (W[0][W_index] + W[1][W_index]) +
                                           x[5][x_index] * (W[0][W_index] + W[4][W_index]);
                        }
                    }

                    for (p_prime = 1; p_prime < numParties + 1; p_prime++) {
                        for (T_index = 0; T_index < numShares; T_index++) {
                            z = 0;

                            if ((p_prime != (pid)) and (!(p_prime_in_T(p_prime, nodeNet->T_map_5pc[T_index]))) and (!(chi_p_prime_in_T(p_prime, nodeNet->T_map_5pc[T_index], numParties)))) {
                                memcpy(&z, buffer[T_index] + (trackers[T_index]) * bytes, bytes);
                                res[T_index][dp_index] += z;
                                trackers[T_index] += 1;
                            } else if ((p_prime == pid) and (!(chi_p_prime_in_T(pid, nodeNet->T_map_5pc[T_index], numParties)))) {
                                memcpy(&z, buffer[T_index] + (trackers[T_index]) * bytes, bytes);
                                res[T_index][dp_index] += z;
                                v[dp_index] = v[dp_index] - z;
                                trackers[T_index] += 1;
                            }
                        }
                    }
                    dp_index++;
                    // memcpy(res[0] + (dp_index), buffer + (dp_index)*bytes, bytes);
                    // v[dp_index] = v[dp_index] - res[0][dp_index];
                }
            }
        }
    }
    // nodeNet->SendAndGetDataFromPeer_Mult(v, recv_buf, output_dim, ring_size);

    nodeNet->SendAndGetDataFromPeer_Mult(v, recv_buf, total_size, ring_size);

    for (uint i = 0; i < total_size; i++) {
        res[3][i] = res[3][i] + recv_buf[1][i];
        res[5][i] = res[5][i] + recv_buf[0][i];
        res[0][i] = res[0][i] + v[i];
    }

    for (i = 0; i < threshold; i++) {
        delete[] recv_buf[i];
    }

    for (i = 0; i < numShares; i++) {
        delete[] buffer[i];
    }

    // free
    delete[] v;
    delete[] buffer;
    delete[] recv_buf;
}

void Rss_pw_matrixmul_mp(Lint **res, Lint **x, Lint **W, uint width, uint height, uint in_channels, uint kernel, uint out_channels, uint ring_size, uint batch_size, NodeNetwork *nodeNet) {

    uint total_size = batch_size * out_channels * width * height;
    uint in_dim = batch_size * in_channels * width * height;
    uint bytes = (ring_size + 7) >> 3;

    uint W_index, x_index;
    uint dp_index = 0;
    int crow = kernel * kernel * in_channels;
    int ccol = height * width;
    int srow = out_channels;

    uint i; // j, k, l; // used for loops
    // printf("bytes: %u\n", bytes);

    uint p_prime, T_index;
    uint numShares = nodeNet->getNumShares();
    uint numParties = nodeNet->getNumParties();
    uint threshold = nodeNet->getThreshold();
    int pid = nodeNet->getID();
    uint8_t prg_ctrs[6] = {2, 3, 3, 2, 3, 2};

    Lint *v = new Lint[total_size];
    memset(v, 0, sizeof(Lint) * (total_size));

    Lint **recv_buf = new Lint *[threshold];
    for (i = 0; i < threshold; i++) {
        recv_buf[i] = new Lint[total_size];
        memset(recv_buf[i], 0, sizeof(Lint) * total_size);
    }

    uint8_t **buffer = new uint8_t *[numShares];
    for (i = 0; i < numShares; i++) {
        buffer[i] = new uint8_t[prg_ctrs[i] * bytes * total_size];
        nodeNet->prg_getrandom(i, bytes, prg_ctrs[i] * total_size, buffer[i]);
        // memset(buffer[i], 0, sizeof(uint8_t) * total_size[numShares] * bytes * size); // FOR

        // sanitizing destination (just in case)
        // memset(c[i], 0, sizeof(Lint) * total_size);

        // memset(buffer[i], 0, sizeof(uint8_t) * prg_ctrs[numShares] * bytes * size);
    }
    Lint z = 0;
    uint trackers[6] = {0, 0, 0, 0, 0, 0};

    // uint8_t *buffer = new uint8_t[bytes * total_size];

    // nodeNet->prg_getrandom(1, bytes, total_size, buffer);
    // memset(buffer, 0, bytes*batch_size*m*s); //USED FOR TESTING

    // a [m*n] = i*n+j (accessing the (i,j)th element)
    // [m][n] size matrix a;   [n][s] size matrix b
    // ---> [m][s] size matrix c;
    for (uint b = 0; b < batch_size; b++) {
        // ordered this way to match TF matmult
        for (int i = 0; i < ccol; i++) {
            for (int j = 0; j < srow; j++) {
                // sLint dot_product = 0;
                for (int k = 0; k < crow; k++) {
                    W_index = j + k * srow;
                    // (out_dim/batch_size) * b used for multiple input images
                    x_index = k + i * crow + (in_dim / batch_size) * b;
                    v[dp_index] += x[0][x_index] * (W[0][W_index] + W[1][W_index] + W[2][W_index] + W[3][W_index] + W[4][W_index] + W[5][W_index]) +

                                   x[1][x_index] * (W[0][W_index] + W[1][W_index] + W[2][W_index] + W[3][W_index] + W[4][W_index] + W[5][W_index]) +
                                   x[2][x_index] * (W[1][W_index] + W[3][W_index]) +
                                   x[3][x_index] * (W[0][W_index] + W[2][W_index]) +
                                   x[4][x_index] * (W[0][W_index] + W[1][W_index]) +
                                   x[5][x_index] * (W[0][W_index] + W[4][W_index]);
                }
                for (p_prime = 1; p_prime < numParties + 1; p_prime++) {
                    for (T_index = 0; T_index < numShares; T_index++) {
                        z = 0;

                        if ((p_prime != (pid)) and (!(p_prime_in_T(p_prime, nodeNet->T_map_5pc[T_index]))) and (!(chi_p_prime_in_T(p_prime, nodeNet->T_map_5pc[T_index], numParties)))) {
                            memcpy(&z, buffer[T_index] + (trackers[T_index]) * bytes, bytes);
                            res[T_index][dp_index] += z;
                            trackers[T_index] += 1;
                        } else if ((p_prime == pid) and (!(chi_p_prime_in_T(pid, nodeNet->T_map_5pc[T_index], numParties)))) {
                            memcpy(&z, buffer[T_index] + (trackers[T_index]) * bytes, bytes);
                            res[T_index][dp_index] += z;
                            v[dp_index] = v[dp_index] - z;
                            trackers[T_index] += 1;
                        }
                    }
                }
                dp_index++;
            }
        }
    }

    nodeNet->SendAndGetDataFromPeer_Mult(v, recv_buf, total_size, ring_size);

    for (uint i = 0; i < total_size; i++) {
        res[3][i] = res[3][i] + recv_buf[1][i];
        res[5][i] = res[5][i] + recv_buf[0][i];
        res[0][i] = res[0][i] + v[i];
    }

    for (i = 0; i < threshold; i++) {
        delete[] recv_buf[i];
    }

    for (i = 0; i < numShares; i++) {
        delete[] buffer[i];
    }

    // free
    delete[] v;
    delete[] buffer;
    delete[] recv_buf;
}
