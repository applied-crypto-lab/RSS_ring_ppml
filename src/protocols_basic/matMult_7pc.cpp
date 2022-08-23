#include "matMult_7pc.h"

void Rss_MatMultArray_batch_7pc(Lint **c, Lint **a, Lint **b, uint m, uint n, uint s, uint ring_size, uint batch_size, uint weight_flag_a, uint weight_flag_b, NodeNetwork *nodeNet) {

    // struct timeval start;
    // struct timeval end;
    // unsigned long timer = 0;

    // if weight_flag == 0 --> the a matrix is the same throughout the computation
    uint total_size = batch_size * m * s;
    // printf("total_size = %u\n", total_size);
    // printf("m = %u\n", m);
    // printf("n = %u\n", n);
    // printf("s = %u\n", s);
    uint bytes = (ring_size + 7) >> 3;
    uint i, j, k, l; // used for loops
    // printf("bytes: %u\n", bytes);

    uint p_prime, T_index;
    uint numShares = nodeNet->getNumShares();
    uint numParties = nodeNet->getNumParties();
    uint threshold = nodeNet->getThreshold();

    int pid = nodeNet->getID();
    uint8_t prg_ctrs[20] = {3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 3, 4, 4, 3};

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
    }
    Lint z = 0;
    uint trackers[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    // printf("pre local\n");
    uint a_index_1, b_index_1, v_index;

    // gettimeofday(&start, NULL); // start timer here

    Lint **b_transpose = new Lint *[numShares];
    for (i = 0; i < numShares; i++) {
        b_transpose[i] = new Lint[batch_size * n * s];
        memset(b_transpose[i], 0, sizeof(Lint) * batch_size * n * s);
    }
    //transposing
    for (int ss = 0; ss < numShares; ss++) {
        for (i = 0; i < n; ++i) {
            for (j = 0; j < s; ++j) {
                b_transpose[ss][i + j * s] = b[ss][i * n + j];
            }
        }
    }

    Lint temp0 = 0, temp1 = 0;
    for (l = 0; l < batch_size; l++) {

        for (i = 0; i < m; i++) {
            for (j = 0; j < s; j++) {
                for (k = 0; k < n; k++) {
                    a_index_1 = weight_flag_a * l * m * n + i * m + k;
                    b_index_1 = weight_flag_b * l * n * s + j * s + k;
                    v_index = l * m * s + i * s + j;
                    temp0 = (b_transpose[0][b_index_1] + b_transpose[1][b_index_1] + b_transpose[2][b_index_1] + b_transpose[3][b_index_1] + b_transpose[4][b_index_1] + b_transpose[5][b_index_1] + b_transpose[6][b_index_1] + b_transpose[7][b_index_1] + b_transpose[8][b_index_1] + b_transpose[9][b_index_1] + b_transpose[10][b_index_1] + b_transpose[11][b_index_1] + b_transpose[12][b_index_1] + b_transpose[13][b_index_1] + b_transpose[14][b_index_1] + b_transpose[15][b_index_1] + b_transpose[16][b_index_1] + b_transpose[17][b_index_1] + b_transpose[18][b_index_1] + b_transpose[19][b_index_1]);
                    temp1 = (b_transpose[0][b_index_1] + b_transpose[1][b_index_1] + b_transpose[2][b_index_1] + b_transpose[3][b_index_1]);

                    v[v_index] = temp0 * (a[0][a_index_1] + a[1][a_index_1] + a[2][a_index_1] + a[3][a_index_1] + a[5][a_index_1]) +
                                 a[4][a_index_1] * (temp1 + b_transpose[10][b_index_1] + b_transpose[11][b_index_1] + b_transpose[12][b_index_1] + b_transpose[13][b_index_1] + b_transpose[15][b_index_1]) +
                                 a[6][a_index_1] * (b_transpose[2][b_index_1] + b_transpose[5][b_index_1] + b_transpose[7][b_index_1] + b_transpose[9][b_index_1] + b_transpose[11][b_index_1] + b_transpose[13][b_index_1]) +
                                 a[7][a_index_1] * (b_transpose[0][b_index_1] + b_transpose[4][b_index_1] + b_transpose[5][b_index_1] + b_transpose[6][b_index_1] + b_transpose[10][b_index_1] + b_transpose[11][b_index_1] + b_transpose[12][b_index_1]) +
                                 a[8][a_index_1] * (b_transpose[0][b_index_1] + b_transpose[4][b_index_1] + b_transpose[5][b_index_1] + b_transpose[10][b_index_1] + b_transpose[11][b_index_1] + b_transpose[16][b_index_1]) +
                                 a[9][a_index_1] * (b_transpose[1][b_index_1] + b_transpose[4][b_index_1] + b_transpose[7][b_index_1] + b_transpose[8][b_index_1] + b_transpose[10][b_index_1] + b_transpose[13][b_index_1]) +
                                 a[10][a_index_1] * (temp1 + b_transpose[4][b_index_1] + b_transpose[5][b_index_1] + b_transpose[9][b_index_1]) +
                                 a[11][a_index_1] * (b_transpose[0][b_index_1] + b_transpose[1][b_index_1] + b_transpose[4][b_index_1] + b_transpose[6][b_index_1] + b_transpose[7][b_index_1] + b_transpose[8][b_index_1]) +
                                 a[12][a_index_1] * (b_transpose[0][b_index_1] + b_transpose[1][b_index_1] + b_transpose[2][b_index_1] + b_transpose[4][b_index_1] + b_transpose[5][b_index_1] + b_transpose[7][b_index_1]) +
                                 a[13][a_index_1] * (b_transpose[0][b_index_1] + b_transpose[4][b_index_1] + b_transpose[5][b_index_1] + b_transpose[6][b_index_1]) +
                                 a[14][a_index_1] * (b_transpose[2][b_index_1] + b_transpose[4][b_index_1] + b_transpose[5][b_index_1]) +
                                 a[15][a_index_1] * (b_transpose[1][b_index_1] + b_transpose[4][b_index_1]) +
                                 a[16][a_index_1] * (temp1 + b_transpose[15][b_index_1]) +
                                 a[17][a_index_1] * (b_transpose[0][b_index_1] + b_transpose[1][b_index_1] + b_transpose[2][b_index_1]) +
                                 a[18][a_index_1] * (b_transpose[1][b_index_1] + b_transpose[8][b_index_1]) +
                                 a[19][a_index_1] * (b_transpose[0][b_index_1] + b_transpose[5][b_index_1] + b_transpose[6][b_index_1]);

                    // v[v_index] =

                    //     a[0][a_index_1] * (temp0) +
                    //     a[1][a_index_1] * (temp0) +
                    //     a[2][a_index_1] * (temp0) +
                    //     a[3][a_index_1] * (temp0) +
                    //     a[4][a_index_1] * (temp1 + b[10][b_index_1] + b[11][b_index_1] + b[12][b_index_1] + b[13][b_index_1] + b[15][b_index_1]) +
                    //     a[5][a_index_1] * (temp0) +
                    //     a[6][a_index_1] * (b[2][b_index_1] + b[5][b_index_1] + b[7][b_index_1] + b[9][b_index_1] + b[11][b_index_1] + b[13][b_index_1]) +
                    //     a[7][a_index_1] * (b[0][b_index_1] + b[4][b_index_1] + b[5][b_index_1] + b[6][b_index_1] + b[10][b_index_1] + b[11][b_index_1] + b[12][b_index_1]) +
                    //     a[8][a_index_1] * (b[0][b_index_1] + b[4][b_index_1] + b[5][b_index_1] + b[10][b_index_1] + b[11][b_index_1] + b[16][b_index_1]) +
                    //     a[9][a_index_1] * (b[1][b_index_1] + b[4][b_index_1] + b[7][b_index_1] + b[8][b_index_1] + b[10][b_index_1] + b[13][b_index_1]) +
                    //     a[10][a_index_1] * (temp1 + b[4][b_index_1] + b[5][b_index_1] + b[9][b_index_1]) +
                    //     a[11][a_index_1] * (b[0][b_index_1] + b[1][b_index_1] + b[4][b_index_1] + b[6][b_index_1] + b[7][b_index_1] + b[8][b_index_1]) +
                    //     a[12][a_index_1] * (b[0][b_index_1] + b[1][b_index_1] + b[2][b_index_1] + b[4][b_index_1] + b[5][b_index_1] + b[7][b_index_1]) +
                    //     a[13][a_index_1] * (b[0][b_index_1] + b[4][b_index_1] + b[5][b_index_1] + b[6][b_index_1]) +
                    //     a[14][a_index_1] * (b[2][b_index_1] + b[4][b_index_1] + b[5][b_index_1]) +
                    //     a[15][a_index_1] * (b[1][b_index_1] + b[4][b_index_1]) +
                    //     a[16][a_index_1] * (temp1 + b[15][b_index_1]) +
                    //     a[17][a_index_1] * (b[0][b_index_1] + b[1][b_index_1] + b[2][b_index_1]) +
                    //     a[18][a_index_1] * (b[1][b_index_1] + b[8][b_index_1]) +
                    //     a[19][a_index_1] * (b[0][b_index_1] + b[5][b_index_1] + b[6][b_index_1]);
                }
            }
        }
    }
    // gettimeofday(&end, NULL); // stop timer here

    // timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

    // printf("[s1] [%.3lf ms]\n", (double)(timer * 0.001));

    // gettimeofday(&start, NULL); // start timer here

    for (i = 0; i < total_size; i++) {

        for (p_prime = 1; p_prime < numParties + 1; p_prime++) {
            for (T_index = 0; T_index < numShares; T_index++) {
                z = 0;
                // printf("before if else\n");
                //  if ((p_prime != (pid)) and (!(p_prime_in_T_7(p_prime, nodeNet->T_map_7pc[T_index]))) and (!(chi_p_prime_in_T_7(p_prime, nodeNet->T_map_7pc[T_index], numParties)))) {
                if ((p_prime != (pid)) and (!(p_prime_in_T_7(p_prime, nodeNet->T_map_7pc[T_index]))) and (!(chi_p_prime_in_T_7(p_prime, nodeNet->T_map_7pc[T_index], numParties)))) {
                    memcpy(&z, buffer[T_index] + (trackers[T_index]) * bytes, bytes);
                    // printf("T_index = %u, v_index = %u \n", T_index, v_index);
                    c[T_index][i] += z;
                    trackers[T_index] += 1;
                } else if (
                    (p_prime == pid) and (!(chi_p_prime_in_T_7(pid, nodeNet->T_map_7pc[T_index], numParties)))) {
                    // printf("elif\n");
                    memcpy(&z, buffer[T_index] + (trackers[T_index]) * bytes, bytes);
                    c[T_index][i] += z;
                    v[i] = v[i] - z;
                    trackers[T_index] += 1;
                }
            }
        }
    }

    // gettimeofday(&end, NULL); // stop timer here

    // timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

    // printf("[s2] [%.3lf ms]\n", (double)(timer * 0.001));
    // // printf("pre send\n");
    // gettimeofday(&start, NULL); // start timer here

    nodeNet->SendAndGetDataFromPeer_Mult(v, recv_buf, total_size, ring_size);

    // gettimeofday(&end, NULL); // stop timer here

    // timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

    // printf("[s3] [%.3lf ms]\n", (double)(timer * 0.001));
    // // printf("post send\n");
    // gettimeofday(&start, NULL); // start timer here

    for (l = 0; l < batch_size; l++) {
        for (i = 0; i < m; i++) {
            for (j = 0; j < s; j++) {
                v_index = l * m * s + i * s + j;
                // printf("v_index = %u\n", v_index);

                c[19][v_index] = c[19][v_index] + recv_buf[0][v_index];
                c[16][v_index] = c[16][v_index] + recv_buf[1][v_index];
                c[10][v_index] = c[10][v_index] + recv_buf[2][v_index];

                c[0][v_index] = c[0][v_index] + v[v_index];
            }
        }
    }
    // gettimeofday(&end, NULL); // stop timer here

    // timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

    // printf("[s4] [%.3lf ms]\n", (double)(timer * 0.001));

    for (i = 0; i < threshold; i++) {
        delete[] recv_buf[i];
    }
    for (i = 0; i < numShares; i++) {
        delete[] buffer[i];
        delete[] b_transpose[i];
    }

    // free
    delete[] v;
    delete[] b_transpose;
    delete[] buffer;
    delete[] recv_buf;
}
