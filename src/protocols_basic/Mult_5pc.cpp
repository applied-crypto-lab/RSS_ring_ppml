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

#include "Mult_5pc.h"

void Rss_Mult_Bitwise_mp(Lint **c, Lint **a, Lint **b, uint size, uint ring_size, NodeNetwork *nodeNet) {

    uint bytes = (ring_size + 7) >> 3;
    uint i, p_prime, T_index;
    uint numShares = nodeNet->getNumShares();
    uint numParties = nodeNet->getNumParties();
    uint threshold = nodeNet->getThreshold();
    int pid = nodeNet->getID();

    Lint *v = new Lint[size];
    memset(v, 0, sizeof(Lint) * size);
    uint8_t prg_ctrs[6] = {2, 3, 3, 2, 3, 2};
    // uint8_t prg_ctrs[6] = {1, 1, 1, 1, 1, 1}; // FOR TESTING

    Lint **recv_buf = new Lint *[threshold];
    for (i = 0; i < threshold; i++) {
        recv_buf[i] = new Lint[size];
        memset(recv_buf[i], 0, sizeof(Lint) * size);
    }
    // printf("prg start\n");

    uint8_t **buffer = new uint8_t *[numShares];
    for (i = 0; i < numShares; i++) {
        buffer[i] = new uint8_t[prg_ctrs[i] * bytes * size];
        nodeNet->prg_getrandom(i, bytes, prg_ctrs[i] * size, buffer[i]);
        memset(c[i], 0, sizeof(Lint) * size);
    }
    Lint z = 0;
    uint trackers[6] = {0, 0, 0, 0, 0, 0};
    // printf("-- calculating v\n");
    // uint tracker;
    for (i = 0; i < size; i++) {
        v[i] = (a[0][i] & (b[0][i] ^ b[1][i] ^ b[2][i] ^ b[3][i] ^ b[4][i] ^ b[5][i])) ^ (a[1][i] & (b[0][i] ^ b[1][i] ^ b[2][i] ^ b[3][i] ^ b[4][i] ^ b[5][i])) ^ (a[2][i] & (b[1][i] ^ b[3][i])) ^ (a[3][i] & (b[0][i] ^ b[2][i])) ^ (a[4][i] & (b[0][i] ^ b[1][i])) ^ (a[5][i] & (b[0][i] ^ b[4][i]));
        // printf("\n------------v[i]: %llu\t", v[i]  & nodeNet->SHIFT[ring_size] );

        // printf("finished calculating v\n");
        for (p_prime = 1; p_prime < numParties + 1; p_prime++) {
            // printf("\n");
            for (T_index = 0; T_index < numShares; T_index++) {
                // tracker = 0;
                z = Lint(0);

                if ((p_prime != (pid)) and (!(p_prime_in_T(p_prime, nodeNet->T_map_5pc[T_index]))) and (!(chi_p_prime_in_T(p_prime, nodeNet->T_map_5pc[T_index], numParties)))) {
                    memcpy(&z, buffer[T_index] + (trackers[T_index]) * bytes, bytes);
                    c[T_index][i] = c[T_index][i] ^ z;
                    trackers[T_index] += 1;
                } else if ((p_prime == pid) and (!(chi_p_prime_in_T(pid, nodeNet->T_map_5pc[T_index], numParties)))) {
                    memcpy(&z, buffer[T_index] + (trackers[T_index]) * bytes, bytes);
                    c[T_index][i] = c[T_index][i] ^ z;
                    v[i] = v[i] ^ z;
                    trackers[T_index] += 1;
                }
            }
        }
    }
    // printf("-- sending v\n");

    // communication
    nodeNet->SendAndGetDataFromPeer_Mult(v, recv_buf, size, ring_size);
    // nodeNet->prg_getrandom(0, bytes, size, buffer);
    for (i = 0; i < size; i++) {
        c[3][i] = c[3][i] ^ recv_buf[1][i];
        c[5][i] = c[5][i] ^ recv_buf[0][i];
        c[0][i] = c[0][i] ^ v[i];
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

void Rss_Mult_mp(Lint **c, Lint **a, Lint **b, uint size, uint ring_size, NodeNetwork *nodeNet) {

    uint bytes = (ring_size + 7) >> 3;
    uint i, p_prime, T_index;
    uint numShares = nodeNet->getNumShares();
    uint numParties = nodeNet->getNumParties();
    uint threshold = nodeNet->getThreshold();
    int pid = nodeNet->getID();

    Lint *v = new Lint[size];
    uint8_t prg_ctrs[6] = {2, 3, 3, 2, 3, 2};

    Lint **recv_buf = new Lint *[threshold];
    for (i = 0; i < threshold; i++) {
        recv_buf[i] = new Lint[size];
        memset(recv_buf[i], 0, sizeof(Lint) * size);
    }

    uint8_t **buffer = new uint8_t *[numShares];
    for (i = 0; i < numShares; i++) {
        buffer[i] = new uint8_t[prg_ctrs[i] * bytes * size];
        nodeNet->prg_getrandom(i, bytes, prg_ctrs[i] * size, buffer[i]);

        // sanitizing destination (just in case)
        memset(c[i], 0, sizeof(Lint) * size);
    }
    Lint z = Lint(0);
    uint tracker;
    for (i = 0; i < size; i++) {
        v[i] = a[0][i] * (b[0][i] + b[1][i] + b[2][i] + b[3][i] + b[4][i] + b[5][i]) +
               a[1][i] * (b[0][i] + b[1][i] + b[2][i] + b[3][i] + b[4][i] + b[5][i]) +
               a[2][i] * (b[1][i] + b[3][i]) +
               a[3][i] * (b[0][i] + b[2][i]) +
               a[4][i] * (b[0][i] + b[1][i]) +
               a[5][i] * (b[0][i] + b[4][i]);
        // printf("finished calculating v\n");
        for (p_prime = 1; p_prime < numParties + 1; p_prime++) {
            // printf("\n");
            for (T_index = 0; T_index < numShares; T_index++) {
                tracker = 0;
                if ((p_prime != (pid)) and (!(p_prime_in_T(p_prime, nodeNet->T_map_5pc[T_index]))) and (!(chi_p_prime_in_T(p_prime, nodeNet->T_map_5pc[T_index], numParties)))) {
                    memcpy(&z, buffer[T_index] + (i * prg_ctrs[T_index] + tracker) * bytes, bytes);
                    c[T_index][i] += z;
                    tracker++;
                } else if (
                    (p_prime == pid) and (!(chi_p_prime_in_T(pid, nodeNet->T_map_5pc[T_index], numParties)))) {
                    memcpy(&z, buffer[T_index] + (i * prg_ctrs[T_index] + tracker) * bytes, bytes);
                    c[T_index][i] += z;
                    v[i] -= z;

                    tracker++;
                }
            }
        }
    }
    // printf("sending now\n");

    // communication
    nodeNet->SendAndGetDataFromPeer_Mult(v, recv_buf, size, ring_size);
    // nodeNet->prg_getrandom(0, bytes, size, buffer);
    for (i = 0; i < size; i++) {
        c[3][i] = c[3][i] + recv_buf[1][i];
        c[5][i] = c[5][i] + recv_buf[0][i];

        c[0][i] = c[0][i] + v[i];
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

void Rss_Mult_fixed_b_mp(Lint **c, Lint **a, Lint **b, uint b_index, uint size, uint ring_size, NodeNetwork *nodeNet) {

    uint bytes = (ring_size + 7) >> 3;
    uint i, p_prime, T_index;
    uint numShares = nodeNet->getNumShares();
    uint numParties = nodeNet->getNumParties();
    uint threshold = nodeNet->getThreshold();
    int pid = nodeNet->getID();

    Lint *v = new Lint[size];
    uint8_t prg_ctrs[6] = {2, 3, 3, 2, 3, 2};

    Lint **recv_buf = new Lint *[threshold];
    for (i = 0; i < threshold; i++) {
        recv_buf[i] = new Lint[size];
        memset(recv_buf[i], 0, sizeof(Lint) * size);
    }
    // printf("prg start\n");

    uint8_t **buffer = new uint8_t *[numShares];
    for (i = 0; i < numShares; i++) {
        buffer[i] = new uint8_t[prg_ctrs[i] * bytes * size];
        nodeNet->prg_getrandom(i, bytes, prg_ctrs[i] * size, buffer[i]);

        // sanitizing destination (just in case)
        memset(c[i], 0, sizeof(Lint) * size);
    }
    Lint z = 0;
    uint tracker;
    for (i = 0; i < size; i++) {
        v[i] = a[0][i] * (b[0][b_index] + b[1][b_index] + b[2][b_index] + b[3][b_index] + b[4][b_index] + b[5][b_index]) +
               a[1][i] * (b[0][b_index] + b[1][b_index] + b[2][b_index] + b[3][b_index] + b[4][b_index] + b[5][b_index]) +
               a[2][i] * (b[1][b_index] + b[3][b_index]) +
               a[3][i] * (b[0][b_index] + b[2][b_index]) +
               a[4][i] * (b[0][b_index] + b[1][b_index]) +
               a[5][i] * (b[0][b_index] + b[4][b_index]);
        // printf("finished calculating v\n");

        for (p_prime = 1; p_prime < numParties + 1; p_prime++) {
            // printf("\n");
            for (T_index = 0; T_index < numShares; T_index++) {
                tracker = 0;
                if ((p_prime != (pid)) and (!(p_prime_in_T(p_prime, nodeNet->T_map_5pc[T_index]))) and (!(chi_p_prime_in_T(p_prime, nodeNet->T_map_5pc[T_index], numParties)))) {
                    memcpy(&z, buffer[T_index] + (i * prg_ctrs[T_index] + tracker) * bytes, bytes);
                    c[T_index][i] += z;
                    tracker++;
                } else if (
                    (p_prime == pid) and (!(chi_p_prime_in_T(pid, nodeNet->T_map_5pc[T_index], numParties)))) {
                    memcpy(&z, buffer[T_index] + (i * prg_ctrs[T_index] + tracker) * bytes, bytes);
                    c[T_index][i] += z;
                    v[i] -= z;
                    tracker++;
                }
            }
        }
    }
    // printf("sending now\n");

    // communication
    nodeNet->SendAndGetDataFromPeer_Mult(v, recv_buf, size, ring_size);
    // nodeNet->prg_getrandom(0, bytes, size, buffer);
    for (i = 0; i < size; i++) {
        c[3][i] = c[3][i] + recv_buf[1][i];
        c[5][i] = c[5][i] + recv_buf[0][i];

        c[0][i] = c[0][i] + v[i];
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

void Rss_Mult_Byte_mp(uint8_t **c, uint8_t **a, uint8_t **b, uint size, NodeNetwork *nodeNet) {

    uint i = 0, p_prime, T_index;
    uint numShares = nodeNet->getNumShares();
    uint numParties = nodeNet->getNumParties();
    uint threshold = nodeNet->getThreshold();
    int pid = nodeNet->getID();

    // printf("size: %u\n", size);

    uint8_t *v = new uint8_t[size];
    memset(v, 0, sizeof(uint8_t) * size);
    uint8_t prg_ctrs[6] = {2, 3, 3, 2, 3, 2};
    // uint8_t prg_ctrs[6] = {1, 1, 1, 1, 1, 1}; // FOR TESTING

    uint8_t **recv_buf = new uint8_t *[threshold];
    for (i = 0; i < threshold; i++) {
        recv_buf[i] = new uint8_t[size];
        memset(recv_buf[i], 0, sizeof(uint8_t) * size);
    }
    // printf("prg start\n");

    uint8_t **buffer = new uint8_t *[numShares];
    for (i = 0; i < numShares; i++) {
        buffer[i] = new uint8_t[prg_ctrs[i] * size];
        nodeNet->prg_getrandom(i, 1, prg_ctrs[i] * size, buffer[i]);
        memset(c[i], 0, sizeof(uint8_t) * size);
    }
    // uint tracker;
    uint trackers[6] = {0, 0, 0, 0, 0, 0};

    for (i = 0; i < size; i++) {

        v[i] = (a[0][i] & (b[0][i] ^ b[1][i] ^ b[2][i] ^ b[3][i] ^ b[4][i] ^ b[5][i])) ^
               (a[1][i] & (b[0][i] ^ b[1][i] ^ b[2][i] ^ b[3][i] ^ b[4][i] ^ b[5][i])) ^
               (a[2][i] & (b[1][i] ^ b[3][i])) ^
               (a[3][i] & (b[0][i] ^ b[2][i])) ^
               (a[4][i] & (b[0][i] ^ b[1][i])) ^
               (a[5][i] & (b[0][i] ^ b[4][i]));

        for (p_prime = 1; p_prime < numParties + 1; p_prime++) {
            for (T_index = 0; T_index < numShares; T_index++) {
                if ((p_prime != (pid)) and (!(p_prime_in_T(p_prime, nodeNet->T_map_5pc[T_index]))) and (!(chi_p_prime_in_T(p_prime, nodeNet->T_map_5pc[T_index], numParties)))) {
                    c[T_index][i] = c[T_index][i] ^ buffer[T_index][trackers[T_index]];
                    trackers[T_index] += 1;
                } else if ((p_prime == pid) and (!(chi_p_prime_in_T(pid, nodeNet->T_map_5pc[T_index], numParties)))) {
                    c[T_index][i] = c[T_index][i] ^ buffer[T_index][trackers[T_index]];
                    v[i] = v[i] ^ buffer[T_index][trackers[T_index]];
                    trackers[T_index] += 1;
                }
            }
        }
    }
    nodeNet->SendAndGetDataFromPeer_bit_Mult(v, recv_buf, size);
    for (i = 0; i < size; i++) {
        c[3][i] = c[3][i] ^ recv_buf[1][i];
        c[5][i] = c[5][i] ^ recv_buf[0][i];

        c[0][i] = c[0][i] ^ v[i];
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

void Rss_MultPub_mp(Lint *c, Lint **a, Lint **b, uint size, uint ring_size, NodeNetwork *nodeNet) {

    uint bytes = (ring_size + 7) >> 3;
    uint i, j;
    uint numShares = nodeNet->getNumShares();
    uint numParties = nodeNet->getNumParties();
    // uint threshold = nodeNet->getThreshold();
    int pid = nodeNet->getID();

    Lint **send_buf = new Lint *[numParties];
    Lint **recv_buf = new Lint *[numParties];
    for (i = 0; i < numParties; i++) {
        send_buf[i] = new Lint[size];
        memset(send_buf[i], 0, sizeof(Lint) * size);
        recv_buf[i] = new Lint[size];
        memset(recv_buf[i], 0, sizeof(Lint) * size);
    }
    Lint *v = new Lint[size];

    // move to outside function
    Lint *ops;
    uint *prg_ctrs;
    int num_ops = 0;
    switch (pid) {
    case 1:
        num_ops = 6;
        prg_ctrs = new uint[numShares]{1, 1, 1, 1, 1, 1};
        // ops = new Lint[num_ops]{(1), (1), (1), (1), (1), (1)};
        ops = new Lint[num_ops];
        ops[0] = Lint(1);
        ops[1] = Lint(1);
        ops[2] = Lint(1);
        ops[3] = Lint(1);
        ops[4] = Lint(1);
        ops[5] = Lint(1);
        break;
    case 2:
        num_ops = 6;
        prg_ctrs = new uint[numShares]{1, 1, 1, 1, 1, 1};
        // ops = new Lint[num_ops]{(1), (1), (1), (1), (1), (1)};
        ops = new Lint[num_ops];
        ops[0] = Lint(1);
        ops[1] = Lint(1);
        ops[2] = Lint(1);
        ops[3] = Lint(1);
        ops[4] = Lint(1);
        ops[5] = Lint(1);
        break;
    case 3:
        num_ops = 7;
        prg_ctrs = new uint[numShares]{2, 1, 1, 1, 1, 1};
        // ops = new Lint[num_ops]{(-1), (-1), (1), (1), (1), (1), (1)};
        ops = new Lint[num_ops];
        ops[0] = Lint(-1);
        ops[1] = Lint(-1);
        ops[2] = Lint(1);
        ops[3] = Lint(1);
        ops[4] = Lint(1);
        ops[5] = Lint(1);
        ops[6] = Lint(1);
        break;
    case 4:
        num_ops = 9;
        prg_ctrs = new uint[numShares]{2, 2, 2, 1, 1, 1};
        // ops = new Lint[num_ops]{(-1), (-1), (-1), (-1), (-1), (-1), (1), (1), (1)};
        ops = new Lint[num_ops];
        ops[0] = Lint(-1);
        ops[1] = Lint(-1);
        ops[2] = Lint(-1);
        ops[3] = Lint(-1);
        ops[4] = Lint(-1);
        ops[5] = Lint(-1);
        ops[6] = Lint(1);
        ops[7] = Lint(1);
        ops[8] = Lint(1);

        break;
    case 5:
        num_ops = 12;
        prg_ctrs = new uint[numShares]{2, 2, 2, 2, 2, 2};
        ops = new Lint[num_ops];
        // ops = new Lint[num_ops]{(-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1), (-1)};
        ops[0] = Lint(-1);
        ops[1] = Lint(-1);
        ops[2] = Lint(-1);
        ops[3] = Lint(-1);
        ops[4] = Lint(-1);
        ops[5] = Lint(-1);
        ops[6] = Lint(-1);
        ops[7] = Lint(-1);
        ops[8] = Lint(-1);
        ops[9] = Lint(-1);
        ops[10] = Lint(-1);
        ops[11] = Lint(-1);
        break;
    }

    uint8_t **buffer = new uint8_t *[numShares];
    Lint **v_a = new Lint *[numShares];
    for (i = 0; i < numShares; i++) {
        buffer[i] = new uint8_t[bytes * size];
        v_a[i] = new Lint[size];
        nodeNet->prg_getrandom(i, bytes, size, buffer[i]);
        memcpy(v_a[i], buffer[i], bytes * size);
    }

    // Lint z = 0;
    uint sign_index = 0;

    for (size_t i = 0; i < size; i++) {
        c[i] = a[0][i] * (b[0][i] + b[1][i] + b[2][i] + b[3][i] + b[4][i] + b[5][i]) +
               a[1][i] * (b[0][i] + b[1][i] + b[2][i] + b[3][i] + b[4][i] + b[5][i]) +
               a[2][i] * (b[1][i] + b[3][i]) +
               a[3][i] * (b[0][i] + b[2][i]) +
               a[4][i] * (b[0][i] + b[1][i]) +
               a[5][i] * (b[0][i] + b[4][i]);

        sign_index = 0;
        for (size_t j = 0; j < numShares; j++) {
            for (size_t v_index = 0; v_index < prg_ctrs[j]; v_index++) {
                c[i] = c[i] + ops[sign_index] * v_a[j][i];
                sign_index++;
            }
            // printf("sign_index: %llu\n", sign_index);
        }
    }

    // move data into buf
    for (i = 1; i <= numParties; i++) {
        if (i == pid) {
            continue;
        }
        memcpy(send_buf[i - 1], c, sizeof(Lint) * size);
    }

    nodeNet->multicastToPeers(send_buf, recv_buf, size, ring_size);

    for (i = 0; i < size; i++) {
        for (j = 0; j < numParties; j++) {
            c[i] = c[i] + recv_buf[j][i]; // we can just add up all received messages, including the one from itself (which is zero from earlier)
        }
        c[i] = c[i] & nodeNet->SHIFT[ring_size];
    }

    for (i = 0; i < numParties; i++) {
        delete[] send_buf[i];
        delete[] recv_buf[i];
    }
    delete[] send_buf;
    delete[] recv_buf;

    for (i = 0; i < numShares; i++) {
        delete[] buffer[i];
        delete[] v_a[i];
    }
    delete[] v_a;

    // free
    delete[] v;
    delete[] buffer;
    delete[] ops;
    delete[] prg_ctrs;
    // delete[] recv_buf
}

// we pass T[ind] to this function
// since the order of T may be ordered differerntly, we need to
// we negate the output when we call this function


void test_prg(uint size, uint ring_size, NodeNetwork *nodeNet) {

    uint bytes = (ring_size + 7) >> 3;
    // printf("bytes %u\n", bytes);
    // int i;
    uint numShares = nodeNet->getNumShares();
    // uint numParties = nodeNet->getNumParties();
    // uint threshold = nodeNet->getThreshold();
    // int pid = nodeNet->getID();

    uint8_t **buffer = new uint8_t *[numShares];
    for (size_t i = 0; i < numShares; i++) {
        buffer[i] = new uint8_t[bytes * size];
        nodeNet->prg_getrandom(i, bytes, size, buffer[i]);
        // memset(buffer[i], 0, sizeof(uint8_t) * prg_ctrs[numShares] * bytes * size);
    }
    // Lint z = 0;

    Lint *prg_test = new Lint[numShares];
    memset(prg_test, 0, sizeof(Lint) * numShares);
    // testing prg keys
    for (size_t j = 0; j < size; j++) {

        for (size_t i = 0; i < numShares; i++) {
            memcpy(prg_test + i, buffer[i] + j * bytes, bytes);
            // printf("prg_test[%u] -- %llu\n", i, prg_test[i]);
        }
    }

    for (size_t i = 0; i < numShares; i++) {
        delete[] buffer[i];
    }

    delete[] buffer;
    delete[] prg_test;
}
