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

#include "edaBit_7pc.h"

void Rss_edaBit_7pc(Lint **r, Lint **b_2, uint size, uint ring_size, NodeNetwork *nodeNet) {

    // int pid = nodeNet->getID();
    uint numShares = nodeNet->getNumShares();
    uint threshold = nodeNet->getThreshold();
    // struct timeval start;
    // struct timeval end;
    // unsigned long timer;

    uint i;
    // this is the number of shares we need to add (t+1)
    uint new_size = (threshold + 1) * size;
    // printf("new_size: %llu\n", new_size);

    Lint **r_bitwise = new Lint *[numShares];
    for (i = 0; i < numShares; i++) {
        r_bitwise[i] = new Lint[new_size];
        memset(r_bitwise[i], 0, sizeof(Lint) * new_size);

        // ensuring destinations are sanitized
        memset(r[i], 0, sizeof(Lint) * size);
        memset(b_2[i], 0, sizeof(Lint) * size);
    }

    Rss_GenerateRandomShares_7pc(r, r_bitwise, ring_size, size, nodeNet);

    Rss_nBitAdd_7pc(b_2, r_bitwise, ring_size, size, nodeNet);

    for (i = 0; i < numShares; i++) {
        delete[] r_bitwise[i];
    }
    delete[] r_bitwise;
}

void Rss_GenerateRandomShares_7pc(Lint **res, Lint **res_bitwise, uint ring_size, uint size, NodeNetwork *nodeNet) {
    // printf("start\n");
    int pid = nodeNet->getID();
    uint i, j;
    uint bytes = (ring_size + 7) >> 3;
    uint new_size = 2 * size; // DO NOT CHANGE, IT IS IRRELEVANT for n>3
    // printf("bytes : %u \n", bytes);
    uint p_index = pid - 1;
    uint numParties = nodeNet->getNumParties();
    uint numShares = nodeNet->getNumShares();
    uint threshold = nodeNet->getThreshold();
    // printf("threshold : %u \n", threshold);
    // printf("numParties : %u \n", numParties);

    Lint **recvbuf = new Lint *[threshold];
    for (i = 0; i < threshold; i++) {
        recvbuf[i] = new Lint[new_size];
        memset(recvbuf[i], 0, sizeof(Lint) * new_size);
    }

    // used since we have effectively double the number of values
    // since we need to represent both arithmetic and binary shares
    // printf("new_size : %u \n", new_size);

    // generate a single random value, which happens to already be the sum of random bits *2^j
    // [shares (0,1)][party (0,1,2)][new_size (2*size)]
    Lint ***r_values = new Lint **[numShares];
    for (i = 0; i < numShares; i++) {
        r_values[i] = new Lint *[(threshold + 1)];
        for (j = 0; j < (threshold + 1); j++) {
            r_values[i][j] = new Lint[new_size];
            memset(r_values[i][j], 0, sizeof(Lint) * new_size); // NECESSARY FOR n>3
        }
    }

    Lint *r_bits = new Lint[size];
    memset(r_bits, 0, sizeof(Lint) * size);

    uint8_t *r_buffer = new uint8_t[bytes * size];

    uint8_t **buffer = new uint8_t *[numShares];
    for (uint s = 0; s < numShares; s++) {
        buffer[s] = new uint8_t[(threshold + 1) * bytes * new_size]; // we may not use all of these random bytes, but
        nodeNet->prg_getrandom(s, bytes, (threshold + 1) * new_size, buffer[s]);
    }

    int *index_map = new int[threshold + 1];

    uint reset_value;
    switch (pid) {
    case 1:
        reset_value = 0;
        index_map[0] = -1;
        index_map[1] = 10;
        index_map[2] = 16;
        index_map[3] = 19;
        break;
    case 2:
        reset_value = 0;
        index_map[0] = -1;
        index_map[1] = -1;
        index_map[2] = 10;
        index_map[3] = 16;
        break;
    case 3:
        reset_value = 0;
        index_map[0] = -1;
        index_map[1] = -1;
        index_map[2] = -1;
        index_map[3] = 10;
        break;
    case 4:
        reset_value = 0;
        index_map[0] = -1;
        index_map[1] = -1;
        index_map[2] = -1;
        index_map[3] = -1;
        break;
    case 5:
        reset_value = 2;
        index_map[0] = 19;
        index_map[1] = -1;
        index_map[2] = -1;
        index_map[3] = -1;
        break;
    case 6:
        reset_value = 1;
        index_map[0] = 16;
        index_map[1] = 19;
        index_map[2] = -1;
        index_map[3] = -1;
        break;
    case 7:
        reset_value = 0;
        index_map[0] = 10;
        index_map[1] = 16;
        index_map[2] = 19;
        index_map[3] = -1;
        break;
    }

    bool prg_bools[6][20] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
        {0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
        {1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0},
        {1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0},
        {1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0}};

    if (pid < threshold + 2) {
        // p1, p2, p3, p4, choosing random values

        nodeNet->prg_getrandom(bytes, size, r_buffer);
        // memcpy(r_bits, r_buffer, size * bytes);

        for (i = 0; i < size; i++) {
            memcpy(r_bits + i, r_buffer + i * bytes, bytes);

            // printf("hi1\n");
            for (size_t s = 1; s < numShares; s++) {
                // memcpy(r_values[s][p_index], buffer[s], new_size * bytes);
                memcpy(r_values[s][p_index] + 2 * i, buffer[s] + (2 * i) * bytes, bytes);
                memcpy(r_values[s][p_index] + 2 * i + 1, buffer[s] + (2 * i + 1) * bytes, bytes);
            }

            // r_values[0][p_index][2 * i] = Lint(5 + i);
            // r_values[0][p_index][2 * i + 1] = Lint(5 + i);
            r_values[0][p_index][2 * i] = r_bits[i];
            r_values[0][p_index][2 * i + 1] = r_bits[i];

            for (size_t s = 1; s < numShares; s++) {
                r_values[0][p_index][2 * i] -= r_values[s][p_index][2 * i];
                r_values[0][p_index][2 * i + 1] ^= r_values[s][p_index][2 * i + 1];
            }
        }
    }

    for (size_t i = 0; i < size; i++) {

        int index = ((pid - 2) % (numParties - 1) + (numParties - 1)) % (numParties - 1);
        for (size_t t = 0; t < threshold + 1; t++) {
            if (p_index != t) {
                // int index = (((pid - 2 - t) % ((numParties - 1))) + ((numParties - 1)) )  % ((numParties - 1) );

                // printf("%i : bool_index = %i\n", t, index);
                // loop through num_shares
                // if we're supposed to generate, then memcpy from buffer[j]
                for (size_t s = 0; s < numShares; s++) {
                    if (prg_bools[index][s]) {
                        // printf("copying: %llu\n", copying);
                        memcpy(r_values[s][t] + (2 * i), buffer[s] + (2 * i) * bytes, bytes);
                        memcpy(r_values[s][t] + (2 * i + 1), buffer[s] + (2 * i + 1) * bytes, bytes);
                    }
                }
                // printf("subtracting\(numParties - 1)");
                index = ((index - 1) % (numParties - 1) + (numParties - 1)) % (numParties - 1);
            }
        }
    }

    nodeNet->SendAndGetDataFromPeer_Eda(r_values[0][p_index], recvbuf, new_size, ring_size);

    // extracting from buffer
    for (size_t i = 0; i < size; i++) {
        j = reset_value;
        for (size_t t = 0; t < threshold + 1; t++) {

            if (index_map[t] > 0) {
                // printf("%u : putting %llu from recvbuff[%i] in %u\n", t, recvbuf[j][2 * i], j, index_map[t]);
                // printf("%u : putting %llu from recvbuff[%i] in %u\n", t, recvbuf[j][2 * i + 1], j, index_map[t]);
                memcpy(r_values[index_map[t]][t] + 2 * i, recvbuf[j] + 2 * i, sizeof(Lint));
                memcpy(r_values[index_map[t]][t] + 2 * i + 1, recvbuf[j] + 2 * i + 1, sizeof(Lint));
                j++;
            }
        }
    }

    for (size_t i = 0; i < size; i++) {
        for (uint p = 0; p < threshold + 1; p++) {
            for (size_t s = 0; s < numShares; s++) {
                res_bitwise[s][p * (size) + i] = r_values[s][p][2 * i + 1];
            }
        }
    }

    for (i = 0; i < size; i++) {
        // this is so we only have t+1 parties generating shares
        for (j = 0; j < threshold + 1; j++) {
            // adding all the parties arithmetic shares together
            for (size_t s = 0; s < numShares; s++) {
                res[s][i] += r_values[s][j][2 * i];
                // res[1][i] += r_values[1][j][1 * i];
            }
        }
    }

    delete[] index_map;
    for (i = 0; i < threshold; i++) {
        delete[] recvbuf[i];
    }

    delete[] recvbuf;

    for (i = 0; i < numShares; i++) {
        for (j = 0; j < (threshold + 1); j++) {
            delete[] r_values[i][j];
        }
        delete[] r_values[i];
    }
    delete[] r_values;
    for (i = 0; i < numShares; i++) {
        delete[] buffer[i];
    }
    delete[] buffer;
    delete[] r_buffer;
    delete[] r_bits;
}
