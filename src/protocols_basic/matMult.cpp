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

#include "matMult.h"
// extern "C"{
// #include "../aes_ni.h"
// }

void Rss_MatMult(Lint ***c, Lint ***a, Lint ***b, uint m, uint n, uint s, uint ring_size, NodeNetwork *nodeNet) {
    //  [m][n] size matrix a;   [n][s] size matrix b
    //  For party 1, a[0,1]=a_2,3; b[0,1]=b_2,3;  c[0,1] = c_2,3;
    //  For party 2, a[0,1]=a_3,1; b[0,1]=b_3,1;  c[0,1] = c_3,1;
    //  For party 3, a[0,1]=a_1,2; b[0,1]=b_1,2;  c[0,1] = c_1,2;

    // uint bytes = (nodeNet->RING[ring_size]+7)>>3;
    uint bytes = (ring_size + 7) >> 3;
    int i, j, k; // used for loops

    Lint **v_a = new Lint *[m];
    for (i = 0; i < m; i++) {
        v_a[i] = new Lint[s];
        memset(v_a[i], 0, sizeof(Lint) * s);
    }

    Lint *send_buf = new Lint[m * s];
    memset(send_buf, 0, sizeof(Lint) * m * s);
    Lint *recv_buf = new Lint[m * s];
    memset(recv_buf, 0, sizeof(Lint) * m * s);

    uint8_t *buffer = new uint8_t[bytes * m * s];
    nodeNet->prg_getrandom(1, bytes, m * s, buffer);

    for (i = 0; i < m; i++) {
        for (j = 0; j < s; j++) {
            for (k = 0; k < n; k++) {
                v_a[i][j] += a[0][i][k] * b[0][k][j] + a[0][i][k] * b[1][k][j] + a[1][i][k] * b[0][k][j];
            }
            memcpy(c[0][i] + j, buffer + (i * s + j) * bytes, bytes);
            // nodeNet->prg_getrandom(1, bytes, c[0][i]+j);
            v_a[i][j] = v_a[i][j] - c[0][i][j];
        }
    }

    // send v_a
    for (i = 0; i < m; i++) {
        memcpy(send_buf + i * s, v_a[i], sizeof(Lint) * s);
    }
    nodeNet->SendAndGetDataFromPeer(send_buf, recv_buf, m * s, ring_size);
    for (i = 0; i < m; i++) {
        memcpy(c[1][i], recv_buf + i * s, sizeof(Lint) * s);
    }

    nodeNet->prg_getrandom(0, bytes, m * s, buffer);
    for (i = 0; i < m; i++) {
        for (j = 0; j < s; j++) {
            c[1][i][j] = c[1][i][j] + c[0][i][j];

            // nodeNet->prg_getrandom(0, bytes, c[0][i]+j);
            memcpy(c[0][i] + j, buffer + (i * s + j) * bytes, bytes);
            c[0][i][j] = c[0][i][j] + v_a[i][j];
        }
    }
    // free
    delete[] send_buf;
    delete[] recv_buf;
    delete[] buffer;
    for (i = 0; i < m; i++) {
        delete[] v_a[i];
    }
    delete[] v_a;
}

void Rss_MatMultArray(Lint **c, Lint **a, Lint **b, uint m, uint n, uint s, uint ring_size, NodeNetwork *nodeNet) {
    // modified implementation that uses a 1d array representation of a matrix
    // a [m*n] = i*n+j (accessing the (i,j)th element)
    // [m][n] size matrix a;   [n][s] size matrix b
    // For party 1, a[0,1]=a_2,3; b[0,1]=b_2,3;  c[0,1] = c_2,3;
    // For party 2, a[0,1]=a_3,1; b[0,1]=b_3,1;  c[0,1] = c_3,1;
    // For party 3, a[0,1]=a_1,2; b[0,1]=b_1,2;  c[0,1] = c_1,2;
    uint bytes = (ring_size + 7) >> 3;
    uint i, j, k; // used for loops
                  // printf("bytes: %u\n", bytes);
    // struct timeval start;
    // struct timeval end;
    // unsigned long timer;

    Lint *v = new Lint[m * s];
    memset(v, 0, sizeof(Lint) * (m * s));

    Lint *send_buf = new Lint[m * s];
    memset(send_buf, 0, sizeof(Lint) * m * s);
    Lint *recv_buf = new Lint[m * s];
    memset(recv_buf, 0, sizeof(Lint) * m * s);

    uint8_t *buffer = new uint8_t[bytes * m * s];
    nodeNet->prg_getrandom(1, bytes, m * s, buffer);
    // memset(buffer, 0, bytes*m*s); //USED FOR TESTING
    // gettimeofday(&start, NULL); //start timer here

    for (i = 0; i < m; i++) {
        for (j = 0; j < s; j++) {
            for (k = 0; k < n; k++) {
                v[i * s + j] += a[0][i * n + k] * b[0][k * s + j] + a[0][i * n + k] * b[1][k * s + j] + a[1][i * n + k] * b[0][k * s + j];
            }
            memcpy(c[0] + (i * s + j), buffer + (i * s + j) * bytes, bytes);
            v[i * s + j] = v[i * s + j] - c[0][i * s + j];
        }
    }
    // gettimeofday(&end, NULL); //stop timer here
    // timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    // printf("Runtime for MatMult dot prod 1 with data size %d = %.6lf s\n", m * s, (double)(timer * 1e-6));

    // send v_a
    for (i = 0; i < m; i++) {
        // memcpy(send_buf+i*s, v+i, sizeof(Lint)*s);
        memcpy(send_buf + i * s, v + i * s, sizeof(Lint) * s);
    }

    // gettimeofday(&start, NULL); //start timer here
    nodeNet->SendAndGetDataFromPeer(send_buf, recv_buf, m * s, ring_size);
    // gettimeofday(&end, NULL); //stop timer here
    // timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    // printf("Runtime for MatMult send with data size %d = %.6lf s\n", m * s, (double)(timer * 1e-6));

    for (i = 0; i < m; i++) {
        memcpy(c[1] + i * s, recv_buf + i * s, sizeof(Lint) * s); // PROBLEM HERE
    }

    nodeNet->prg_getrandom(0, bytes, m * s, buffer);
    // memset(buffer, 0, bytes*m*s); //USED FOR TESTING

    // gettimeofday(&start, NULL); //start timer here
    for (i = 0; i < m; i++) {
        for (j = 0; j < s; j++) {
            c[1][i * s + j] = c[1][i * s + j] + c[0][i * s + j];

            memcpy(c[0] + (i * s + j), buffer + (i * s + j) * bytes, bytes);
            c[0][i * s + j] = c[0][i * s + j] + v[i * s + j];
        }
    }
    // gettimeofday(&end, NULL); //stop timer here
    // timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    // printf("Runtime for MatMult dot prod 2 with data size %d = %.6lf s\n", m * s, (double)(timer * 1e-6));

    delete[] send_buf;
    delete[] recv_buf;
    delete[] buffer;
    delete[] v;
    // for(i = 0; i< m; i++){
    //     delete [] v_a[i];
    // }
    // delete [] v_a;
}

void Rss_MatMultArray_batch(Lint **c, Lint **a, Lint **b, uint m, uint n, uint s, uint ring_size, uint batch_size, uint weight_flag_a, uint weight_flag_b, NodeNetwork *nodeNet) {
    // modified implementation that uses a 1d array representation of a matrix
    // a [m*n] = i*n+j (accessing the (i,j)th element)
    // [m][n] size matrix a;   [n][s] size matrix b
    // ---> [m][s] size matrix c;
    // For party 1, a[0,1]=a_2,3; b[0,1]=b_2,3;  c[0,1] = c_2,3;
    // For party 2, a[0,1]=a_3,1; b[0,1]=b_3,1;  c[0,1] = c_3,1;
    // For party 3, a[0,1]=a_1,2; b[0,1]=b_1,2;  c[0,1] = c_1,2;

    // if weight_flag == 0 --> the a matrix is the same throughout the computation
    uint numShares = nodeNet->getNumShares();

    uint bytes = (ring_size + 7) >> 3;
    uint i, j, k, l; // used for loops
    // printf("bytes: %u\n", bytes);

    Lint *v = new Lint[batch_size * m * s];
    memset(v, 0, sizeof(Lint) * (batch_size * m * s));

    Lint *send_buf = new Lint[batch_size * m * s];
    memset(send_buf, 0, sizeof(Lint) * batch_size * m * s);
    Lint *recv_buf = new Lint[batch_size * m * s];
    memset(recv_buf, 0, sizeof(Lint) * batch_size * m * s);

    uint8_t *buffer = new uint8_t[bytes * batch_size * m * s];
    nodeNet->prg_getrandom(1, bytes, batch_size * m * s, buffer);
    // memset(buffer, 0, bytes*batch_size*m*s); //USED FOR TESTING

    Lint **b_transpose = new Lint *[numShares];
    for (i = 0; i < numShares; i++) {
        b_transpose[i] = new Lint[batch_size * n * s];
        memset(b_transpose[i], 0, sizeof(Lint) * batch_size * n * s);
    }
    for (int ss = 0; ss < numShares; ss++) {
        for (l = 0; l < batch_size; l++) {
            for (i = 0; i < n; ++i) {
                for (j = 0; j < s; ++j) {
                    b_transpose[ss][weight_flag_b * l * n * s + j * n + i] = b[ss][weight_flag_b * l * n * s + i * s + j];
                }
            }
        }
    }

    uint a_index, b_index, v_index;
    for (l = 0; l < batch_size; l++) {

        for (i = 0; i < m; i++) {
            for (j = 0; j < s; j++) {
                v_index = l * m * s + i * s + j;
                for (k = 0; k < n; k++) {
                    a_index = weight_flag_a * l * m * n + i * n + k;
                    b_index = weight_flag_b * l * n * s + j * n + k;
                    // printf("l, i, j, k      : %u, %u, %u, %u\n",  l,i,j,k);
                    // printf("a_index, b_index: %u, %u\n",  a_index, b_index);

                    v[v_index] +=
                        a[0][a_index] * b_transpose[0][b_index] + a[0][a_index] * b_transpose[1][b_index] + a[1][a_index] * b_transpose[0][b_index];
                }
                memcpy(c[0] + (v_index), buffer + (v_index)*bytes, bytes);
                v[v_index] = v[v_index] - c[0][v_index];
            }
        }
    }

    // send v_a
    for (l = 0; l < batch_size; l++) {

        for (i = 0; i < m; i++) {
            // memcpy(send_buf+i*s, v+i, sizeof(Lint)*s);
            memcpy(send_buf + i * s + l * m * s, v + i * s + l * m * s, sizeof(Lint) * s);
        }
    }
    nodeNet->SendAndGetDataFromPeer(send_buf, recv_buf, batch_size * m * s, ring_size);

    for (l = 0; l < batch_size; l++) {
        for (i = 0; i < m; i++) {
            memcpy(c[1] + i * s + l * m * s, recv_buf + i * s + l * m * s, sizeof(Lint) * s); // PROBLEM HERE
        }
    }

    nodeNet->prg_getrandom(0, bytes, batch_size * m * s, buffer);
    // memset(buffer, 0, bytes*batch_size*m*s); //USED FOR TESTING

    for (l = 0; l < batch_size; l++) {
        for (i = 0; i < m; i++) {
            for (j = 0; j < s; j++) {
                c[1][l * m * s + i * s + j] = c[1][l * m * s + i * s + j] + c[0][l * m * s + i * s + j];

                memcpy(c[0] + (l * m * s + i * s + j), buffer + (l * m * s + i * s + j) * bytes, bytes);
                c[0][l * m * s + i * s + j] = c[0][l * m * s + i * s + j] + v[l * m * s + i * s + j];
            }
        }
    }

    delete[] send_buf;
    delete[] recv_buf;
    delete[] buffer;
    delete[] v;
    for (i = 0; i < numShares; i++) {
        delete[] b_transpose[i];
    }
    delete[] b_transpose;
}

void Rss_dw_matrixmul(Lint **res, Lint **x, Lint **W, uint width, uint height, uint channels, uint filter, uint ring_size, uint batch_size, uint stride, NodeNetwork *nodeNet) {

    uint output_dim = batch_size * channels * (width / stride) * (height / stride);
    // uint in_dim = batch_size * channels * (width) * (height);
    uint bytes = (ring_size + 7) >> 3;

    Lint *v = new Lint[output_dim];
    memset(v, 0, sizeof(Lint) * (output_dim));

    Lint *send_buf = new Lint[output_dim];
    memset(send_buf, 0, sizeof(Lint) * output_dim);
    Lint *recv_buf = new Lint[output_dim];
    memset(recv_buf, 0, sizeof(Lint) * output_dim);

    uint8_t *buffer = new uint8_t[bytes * output_dim];
    nodeNet->prg_getrandom(1, bytes, output_dim, buffer);
    // memset(buffer, 0, bytes*batch_size*m*s); //USED FOR TESTING
    uint padded_height = height + 2;

    uint W_index, x_index;
    uint dp_index = 0;

    for (uint b = 0; b < batch_size; b++) {

        for (uint i = (stride - 1); i < width; i += stride) {
            for (uint j = (stride - 1); j < height; j += stride) {

                // calculating the actual dot prod
                for (uint fi = 0; fi < filter; fi++) {
                    for (uint fj = 0; fj < filter; fj++) {
                        
                        for (uint c = 0; c < channels; c++) {
                            W_index = fi * filter * channels + fj * channels + c;

                            x_index = (i + fi) * (padded_height)*channels + (j + fj) * channels + c + (output_dim / batch_size) * b; // (out_dim/batch_size) * b used for multiple input images

                            v[dp_index] += x[0][x_index] * W[0][W_index] + x[0][x_index] * W[1][W_index] + x[1][x_index] * W[0][W_index];
                        }
                    }
                    memcpy(res[0] + (dp_index), buffer + (dp_index)*bytes, bytes);
                    v[dp_index] = v[dp_index] - res[0][dp_index];

                    dp_index++;
                }
            }
        }
    }

    // sending v_a
    memcpy(send_buf, v, sizeof(Lint) * output_dim);
    nodeNet->SendAndGetDataFromPeer(send_buf, recv_buf, output_dim, ring_size);
    memcpy(res[1], recv_buf, sizeof(Lint) * output_dim);

    nodeNet->prg_getrandom(0, bytes, output_dim, buffer);

    for (uint i = 0; i < output_dim; i++) {
        res[1][i] = res[1][i] + res[0][i];
        memcpy(res[0] + (i), buffer + (i)*bytes, bytes);
        res[0][i] = res[0][i] + v[i];
    }

    delete[] send_buf;
    delete[] recv_buf;
    delete[] buffer;
    delete[] v;
}

void Rss_pw_matrixmul(Lint **res, Lint **x, Lint **W, uint width, uint height, uint in_channels, uint kernel, uint out_channels, uint ring_size, uint batch_size, NodeNetwork *nodeNet) {

    uint output_dim = batch_size * out_channels * width * height;
    uint in_dim = batch_size * in_channels * width * height;
    uint bytes = (ring_size + 7) >> 3;

    Lint *v = new Lint[output_dim];
    memset(v, 0, sizeof(Lint) * (output_dim));

    Lint *send_buf = new Lint[output_dim];
    memset(send_buf, 0, sizeof(Lint) * output_dim);
    Lint *recv_buf = new Lint[output_dim];
    memset(recv_buf, 0, sizeof(Lint) * output_dim);

    uint8_t *buffer = new uint8_t[bytes * output_dim];
    nodeNet->prg_getrandom(1, bytes, output_dim, buffer);
    // memset(buffer, 0, bytes*batch_size*m*s); //USED FOR TESTING
    // uint padded_height = height + 2;

    uint W_index, x_index;
    uint dp_index = 0;

    int crow = kernel * kernel * in_channels;
    int ccol = height * width;
    int srow = out_channels;

    for (uint b = 0; b < batch_size; b++) {
        // ordered this way to match TF matmult
        for (int i = 0; i < ccol; i++) {
            for (int j = 0; j < srow; j++) {
                for (int k = 0; k < crow; k++) {
                    W_index = j + k * srow;
                    // (out_dim/batch_size) * b used for multiple input images
                    x_index = k + i * crow + (in_dim / batch_size) * b;
                    v[dp_index] += x[0][x_index] * W[0][W_index] + x[0][x_index] * W[1][W_index] + x[1][x_index] * W[0][W_index];
                }

                memcpy(res[0] + (dp_index), buffer + (dp_index)*bytes, bytes);
                v[dp_index] = v[dp_index] - res[0][dp_index];
                dp_index++;
            }
        }
    }
    // sending v_a
    memcpy(send_buf, v, sizeof(Lint) * output_dim);
    nodeNet->SendAndGetDataFromPeer(send_buf, recv_buf, output_dim, ring_size);
    memcpy(res[1], recv_buf, sizeof(Lint) * output_dim);
    nodeNet->prg_getrandom(0, bytes, output_dim, buffer);

    for (uint i = 0; i < output_dim; i++) {
        res[1][i] = res[1][i] + res[0][i];
        memcpy(res[0] + (i), buffer + (i)*bytes, bytes);
        res[0][i] = res[0][i] + v[i];
    }

    delete[] send_buf;
    delete[] recv_buf;
    delete[] buffer;
    delete[] v;
}
