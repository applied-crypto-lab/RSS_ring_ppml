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

#include "edaBit.h"

void Rss_edaBit(Lint **r, Lint **b_2, uint size, uint ring_size, NodeNetwork *nodeNet) {

    // int pid = nodeNet->getID();
    uint numParties = nodeNet->getNumParties();
    // printf("numParties : %u\n",numParties);
    // struct timeval start;
    // struct timeval end;
    // unsigned long timer;

    uint i;
    // need to multiply by the number of parties in the computation
    uint new_size = numParties * size;

    Lint **r_bitwise = new Lint *[2];
    for (i = 0; i < 2; i++) {
        r_bitwise[i] = new Lint[new_size];
        memset(r_bitwise[i], 0, sizeof(Lint) * new_size);

        // ensuring destinations are sanitized
        memset(r[i], 0, sizeof(Lint) * size);
        memset(b_2[i], 0, sizeof(Lint) * size);
    }

    Rss_GenerateRandomShares(r, r_bitwise, ring_size, size, nodeNet);

    Rss_nBitAdd(b_2, r_bitwise, ring_size, size, nodeNet);

    for (i = 0; i < 2; i++) {
        delete[] r_bitwise[i];
    }
    delete[] r_bitwise;
}

void Rss_GenerateRandomShares(Lint **res, Lint **res_bitwise, uint ring_size, uint size, NodeNetwork *nodeNet) {
    // printf("start\n");
    int pid = nodeNet->getID();
    uint i, j;
    uint bytes = (ring_size + 7) >> 3;
    // printf("bytes : %u \n", bytes);
    uint p_index = pid - 1;
    uint numParties = nodeNet->getNumParties();
    // printf("numParties : %u \n", numParties);

    // Lint temp0, temp1;

    // used since we have effectively double the number of values
    // since we need to represent both arithmetic and binary shares
    uint new_size = 2 * size; // DO NOT CHANGE, IT IS IRRELEVANT

    // generate a single random value, which happens to already be the sum of random bits *2^j
    // [shares (0,1)][party (0,1,2)][new_size (2*size)]
    Lint ***r_values = new Lint **[2];
    for (i = 0; i < 2; i++) {
        r_values[i] = new Lint *[numParties];
        for (j = 0; j < numParties; j++) {
            r_values[i][j] = new Lint[new_size];
            memset(r_values[i][j], 0, sizeof(Lint) * new_size);
        }
    }

    int gamma[2];
    switch (pid) {
    case 1:
        gamma[0] = 1;
        gamma[1] = 2;
        break;
    case 2:
        gamma[0] = 2;
        gamma[1] = 0;
        break;
    case 3:
        gamma[0] = 0;
        gamma[1] = 1;
        break;
    }

    Lint *r_bits = new Lint[size];
    memset(r_bits, 0, sizeof(Lint) * size);

    uint8_t *buffer = new uint8_t[bytes * new_size];
    // memset(buffer, 0, sizeof(uint8_t) * bytes * new_size);

    // each party generating a unique random value
    // printf("prg1\n");
    nodeNet->prg_getrandom(bytes, size, buffer);
    // printf("mc1\n");
    memcpy(r_bits, buffer, size * bytes);

    // printf("prg2\n");
    // SEGFAULT HERE FOR VERY LARGE VALUES
    nodeNet->prg_getrandom(1, bytes, new_size, buffer);

    // store arithmetic and bitwise representation sequentially
    // calculating p_i's own individual shares

    memcpy(r_values[1][p_index], buffer, size * bytes);
    memcpy(r_values[1][p_index] + size, buffer + size * bytes, size * bytes);

    for (i = 0; i < size; i++) {

        r_values[0][p_index][1 * i] = r_bits[i] - r_values[1][p_index][1 * i];
        r_values[0][p_index][size + i] = r_bits[i] ^ r_values[1][p_index][size + i];


        // r_values[0][p_index][1 * i] = Lint(5) - r_values[1][p_index][1 * i];
        // r_values[0][p_index][size + i] = Lint(5) ^ r_values[1][p_index][size + i];



    }

    // need to generate more random shares so that binary and arithmetic representations are different
    nodeNet->prg_getrandom(0, bytes, new_size, buffer);
    memcpy(r_values[0][gamma[1]], buffer, size * bytes);
    memcpy(r_values[0][gamma[1]] + size, buffer + size * bytes, size * bytes);

    //  sending r_values[0][p_index], receiving r_values[1][gamma[0]],
    nodeNet->SendAndGetDataFromPeer(r_values[0][p_index], r_values[1][gamma[0]], new_size, ring_size);

    for (i = 0; i < numParties - 1; i++) {
        // for (i = 0; i < numParties; i++) {
        memcpy(res_bitwise[0] + i * size, r_values[0][i] + (size), size * sizeof(Lint));
        memcpy(res_bitwise[1] + i * size, r_values[1][i] + (size), size * sizeof(Lint));
    }

    for (i = 0; i < size; i++) {
        // this is so we only have two parties generating shares
        for (j = 0; j < numParties - 1; j++) {
            // adding all the parties arithmetic shares together
            res[0][i] += r_values[0][j][1 * i];
            res[1][i] += r_values[1][j][1 * i];
        }
    }

    for (i = 0; i < 2; i++) {
        for (j = 0; j < numParties; j++) {
            delete[] r_values[i][j];
        }
        delete[] r_values[i];
    }
    delete[] r_values;
    delete[] buffer;
    delete[] r_bits;
}

void Rss_edaBit(Lint **r, Lint **b_2, uint size, uint ring_size, uint bit_length, NodeNetwork *nodeNet) {

    // int pid = nodeNet->getID();
    uint numParties = nodeNet->getNumParties();
    // printf("numParties : %u\n",numParties);

    uint i;
    // need to multiply by the number of parties in the computation
    uint new_size = numParties * size;

    Lint **r_bitwise = new Lint *[2];
    for (i = 0; i < 2; i++) {
        r_bitwise[i] = new Lint[new_size];
        memset(r_bitwise[i], 0, sizeof(Lint) * new_size);

        // ensuring destinations are sanitized
        memset(r[i], 0, sizeof(Lint) * size);
        memset(b_2[i], 0, sizeof(Lint) * size);
    }

    Rss_GenerateRandomShares(r, r_bitwise, ring_size, bit_length, size, nodeNet);

    Rss_nBitAdd(b_2, r_bitwise, ring_size, size, nodeNet);

    for (i = 0; i < 2; i++) {
        delete[] r_bitwise[i];
    }
    delete[] r_bitwise;
}

void Rss_GenerateRandomShares(Lint **res, Lint **res_bitwise, uint ring_size, uint bit_length, uint size, NodeNetwork *nodeNet) {

    int pid = nodeNet->getID();
    uint i, j;
    uint bytes = (ring_size + 7) >> 3;
    // printf("bytes : %u \n", bytes);
    uint p_index = pid - 1;
    uint numParties = nodeNet->getNumParties();
    // Lint temp0, temp1;

    // used since we have effectively double the number of values
    // since we need to represent both arithmetic and binary shares
    uint new_size = 2 * size;

    // generate a single random value, which happens to already be the sum of random bits *2^j
    // [shares (0,1)][party (0,1,2)][new_size (2*size)]
    Lint ***r_values = new Lint **[2];
    for (i = 0; i < 2; i++) {
        r_values[i] = new Lint *[numParties];
        for (j = 0; j < numParties; j++) {
            r_values[i][j] = new Lint[new_size];
            memset(r_values[i][j], 0, sizeof(Lint) * new_size);
        }
    }

    int gamma[2];
    switch (pid) {
    case 1:
        gamma[0] = 1;
        gamma[1] = 2;
        break;
    case 2:
        gamma[0] = 2;
        gamma[1] = 0;
        break;
    case 3:
        gamma[0] = 0;
        gamma[1] = 1;
        break;
    }

    Lint *r_bits = new Lint[size];
    memset(r_bits, 0, sizeof(Lint) * size);

    uint8_t *buffer = new uint8_t[bytes * new_size];
    // each party generating a unique random value
    nodeNet->prg_getrandom(bytes, size, buffer);
    for (i = 0; i < size; i++) {
        memcpy(r_bits + i, buffer + i * bytes, bytes);
        // is this what we need to do to ensure we have a shorter value?
        // or do we need to do something at the end of the computation

        // r_bits[i] = -1; // USED FOR TESTING
        r_bits[i] = r_bits[i] & nodeNet->SHIFT[bit_length];
    }

    nodeNet->prg_getrandom(1, bytes, new_size, buffer);

    // store arithmetic and bitwise representation sequentially
    // calculating p_i's own individual shares
    for (i = 0; i < size; i++) {
        memcpy(r_values[1][p_index] + 2 * i, buffer + (2 * i) * bytes, bytes);
        memcpy(r_values[1][p_index] + 2 * i + 1, buffer + (2 * i + 1) * bytes, bytes);
        // r_values[0][p_index][2 * i] = r_bits[i] ;
        r_values[0][p_index][2 * i] = r_bits[i] - r_values[1][p_index][2 * i];
        r_values[0][p_index][2 * i + 1] = r_bits[i] ^ r_values[1][p_index][2 * i + 1];

    }

    // need to generate more random shares so that binary and arithmetic representations are different
    nodeNet->prg_getrandom(0, bytes, new_size, buffer);
    for (i = 0; i < size; i++) {
        memcpy(r_values[0][gamma[1]] + (2 * i), buffer + (2 * i) * bytes, bytes);
        memcpy(r_values[0][gamma[1]] + (2 * i + 1), buffer + (2 * i + 1) * bytes, bytes);
    }

    //  sending r_values[0][p_index], receiving r_values[1][gamma[0]],
    nodeNet->SendAndGetDataFromPeer(r_values[0][p_index], r_values[1][gamma[0]], new_size, ring_size);

    for (i = 0; i < size; i++) {
        for (j = 0; j < numParties; j++) {
            // adding all the parties arithmetic shares together
            // memcpy(res[0] + (3 * i + j), r_values[0][j] + (2 * i), sizeof(Lint));
            // memcpy(res[1] + (3 * i + j), r_values[1][j] + (2 * i), sizeof(Lint));
            res[0][i] += r_values[0][j][2 * i];
            res[1][i] += r_values[1][j][2 * i];

            memcpy(res_bitwise[0] + (numParties * i + j), r_values[0][j] + (2 * i + 1), sizeof(Lint));
            memcpy(res_bitwise[1] + (numParties * i + j), r_values[1][j] + (2 * i + 1), sizeof(Lint));
        }
    }

    for (i = 0; i < 2; i++) {
        for (j = 0; j < numParties; j++) {
            delete[] r_values[i][j];
        }
        delete[] r_values[i];
    }
    delete[] r_values;
    delete[] buffer;
    delete[] r_bits;
}

// void Rss_edaBit_trunc(Lint **r, Lint **r_prime, Lint **b_2, uint size, uint ring_size, uint m, NodeNetwork *nodeNet) {
void Rss_edaBit_trunc(Lint **r, Lint **r_prime, Lint **r_km1, uint size, uint ring_size, uint m, NodeNetwork *nodeNet) {

    // int pid = nodeNet->getID();
    uint numParties = nodeNet->getNumParties();

    uint i;

    Lint **r_bitwise = new Lint *[2];
    Lint **carry = new Lint *[2];
    Lint **b_2 = new Lint *[2];
    uint new_size = numParties * size;
    uint b2a_size = 3 * size;

    for (i = 0; i < 2; i++) {
        r_bitwise[i] = new Lint[new_size];
        memset(r_bitwise[i], 0, sizeof(Lint) * new_size);
        // carry will hold both kth and m-1th bits, in succession
        carry[i] = new Lint[b2a_size];
        memset(carry[i], 0, sizeof(Lint) * b2a_size);

        b_2[i] = new Lint[size];
        memset(b_2[i], 0, sizeof(Lint) * size);

        // ensuring destinations are sanitized
        memset(r[i], 0, sizeof(Lint) * size);
        memset(r_prime[i], 0, sizeof(Lint) * size);
        memset(r_km1[i], 0, sizeof(Lint) * size);
    }

    Rss_GenerateRandomShares_trunc(r, r_prime, r_bitwise, ring_size, m, size, nodeNet);

    Rss_nBitAdd_trunc(b_2, carry, r_bitwise, ring_size, m, size, nodeNet);

    Rss_b2a(carry, carry, ring_size, b2a_size, nodeNet);

    memcpy(r_km1[0], carry[0] + 2 * (size), size * sizeof(Lint));
    memcpy(r_km1[1], carry[1] + 2 * (size), size * sizeof(Lint));

    // adding m-1 and subtracting k carries
    for (size_t i = 0; i < size; i++) {

        r_prime[0][i] = r_prime[0][i] + carry[0][i] - ((carry[0][size + i]) << Lint(ring_size - m));
        r_prime[1][i] = r_prime[1][i] + carry[1][i] - ((carry[1][size + i]) << Lint(ring_size - m));
    }

    for (i = 0; i < 2; i++) {
        delete[] r_bitwise[i];
        delete[] carry[i];
        delete[] b_2[i];
    }
    delete[] r_bitwise;
    delete[] carry;
    delete[] b_2;
}

void Rss_edaBit_trunc_test(Lint **r, Lint **r_prime, Lint **r_km1, uint size, uint ring_size, uint m, NodeNetwork *nodeNet) {

    // int pid = nodeNet->getID();
    uint numParties = nodeNet->getNumParties();

    uint i;

    Lint **r_bitwise = new Lint *[2];
    Lint **carry = new Lint *[2];
    Lint **b_2 = new Lint *[2];
    uint new_size = numParties * size;
    uint b2a_size = 3 * size;

    for (i = 0; i < 2; i++) {
        r_bitwise[i] = new Lint[new_size];
        memset(r_bitwise[i], 0, sizeof(Lint) * new_size);
        // carry will hold both kth and m-1th bits, in succession
        carry[i] = new Lint[b2a_size];
        memset(carry[i], 0, sizeof(Lint) * b2a_size);

        b_2[i] = new Lint[size];
        memset(b_2[i], 0, sizeof(Lint) * size);

        // ensuring destinations are sanitized
        memset(r[i], 0, sizeof(Lint) * size);
        memset(r_prime[i], 0, sizeof(Lint) * size);
        memset(r_km1[i], 0, sizeof(Lint) * size);
    }

    Rss_GenerateRandomShares_trunc(r, r_prime, r_bitwise, ring_size, m, size, nodeNet);

    Rss_nBitAdd_trunc(b_2, carry, r_bitwise, ring_size, m, size, nodeNet);

    Rss_b2a(carry, carry, ring_size, b2a_size, nodeNet);

    memcpy(r_km1[0], carry[0] + 2 * (size), size * sizeof(Lint));
    memcpy(r_km1[1], carry[1] + 2 * (size), size * sizeof(Lint));

    // adding m-1 and subtracting k carries
    for (size_t i = 0; i < size; i++) {

        // r_prime[0][i] = r_prime[0][i] + carry[0][i] - ((carry[0][size + i]) << Lint(ring_size - m));
        // r_prime[1][i] = r_prime[1][i] + carry[1][i] - ((carry[1][size + i]) << Lint(ring_size - m));

        r_prime[0][i] = r_prime[0][i] + carry[0][i];
        r_prime[1][i] = r_prime[1][i] + carry[1][i];
    }

    for (i = 0; i < 2; i++) {
        delete[] r_bitwise[i];
        delete[] carry[i];
        delete[] b_2[i];
    }
    delete[] r_bitwise;
    delete[] carry;
    delete[] b_2;
}

void Rss_GenerateRandomShares_trunc(Lint **res, Lint **res_prime, Lint **res_bitwise, uint ring_size, uint m, uint size, NodeNetwork *nodeNet) {

    int pid = nodeNet->getID();
    uint i, j;
    uint bytes = (ring_size + 7) >> 3;
    // printf("bytes : %u \n", bytes);
    uint p_index = pid - 1;
    uint numParties = nodeNet->getNumParties();
    // printf("numParties : %u \n", numParties);

    // Lint temp0, temp1;

    // used since we have effectively double the number of values
    // since we need to represent both arithmetic and binary shares
    uint new_size = 3 * size;

    // generate a single random value, which happens to already be the sum of random bits *2^j
    // [shares (0,1)][party (0,1,2)][new_size (2*size)]
    Lint ***r_values = new Lint **[2];
    for (i = 0; i < 2; i++) {
        r_values[i] = new Lint *[numParties];
        for (j = 0; j < numParties; j++) {
            r_values[i][j] = new Lint[new_size];
            memset(r_values[i][j], 0, sizeof(Lint) * new_size);
        }
    }

    int gamma[2];
    switch (pid) {
    case 1:
        gamma[0] = 1;
        gamma[1] = 2;
        break;
    case 2:
        gamma[0] = 2;
        gamma[1] = 0;
        break;
    case 3:
        gamma[0] = 0;
        gamma[1] = 1;
        break;
    }

    Lint *r_bits = new Lint[size];
    memset(r_bits, 0, sizeof(Lint) * size);
    Lint *r_prime = new Lint[size];
    memset(r_prime, 0, sizeof(Lint) * size);

    uint8_t *buffer = new uint8_t[bytes * new_size];
    // each party generating a unique random value
    nodeNet->prg_getrandom(bytes, size, buffer);

    memcpy(r_bits, buffer, size * bytes);

    for (i = 0; i < size; i++) {
        r_bits[i] = r_bits[i] & nodeNet->SHIFT[ring_size];
        r_prime[i] = (r_bits[i] >> Lint(m));
    }

    nodeNet->prg_getrandom(1, bytes, new_size, buffer);

    // store arithmetic and bitwise representation sequentially
    // calculating p_i's own individual shares

    memcpy(r_values[1][p_index], buffer, size * bytes);
    memcpy(r_values[1][p_index] + size, buffer + size * bytes, size * bytes);
    memcpy(r_values[1][p_index] + 2 * size, buffer + 2 * size * bytes, size * bytes);

    for (i = 0; i < size; i++) {
        r_values[0][p_index][1 * i] = r_bits[i] - r_values[1][p_index][1 * i];
        r_values[0][p_index][size + i] = r_bits[i] ^ r_values[1][p_index][size + i];
        r_values[0][p_index][2 * size + i] = r_prime[i] - r_values[1][p_index][2 * size + i];
        // r_values[0][p_index][2*size + i] = r_bits[i] - r_values[1][p_index][2*size + i];
    }

    // need to generate more random shares so that binary and arithmetic representations are different
    nodeNet->prg_getrandom(0, bytes, new_size, buffer);
    memcpy(r_values[0][gamma[1]], buffer, size * bytes);
    memcpy(r_values[0][gamma[1]] + size, buffer + size * bytes, size * bytes);
    memcpy(r_values[0][gamma[1]] + 2 * size, buffer + 2 * size * bytes, size * bytes);

    //  sending r_values[0][p_index], receiving r_values[1][gamma[0]],
    nodeNet->SendAndGetDataFromPeer(r_values[0][p_index], r_values[1][gamma[0]], new_size, ring_size);

    for (i = 0; i < numParties - 1; i++) {
        memcpy(res_bitwise[0] + i * size, r_values[0][i] + (size), size * sizeof(Lint));
        memcpy(res_bitwise[1] + i * size, r_values[1][i] + (size), size * sizeof(Lint));
    }

    for (i = 0; i < size; i++) {
        // this is so we only have two parties generating shares
        for (j = 0; j < numParties - 1; j++) {

            // adding all the parties arithmetic shares together
            res[0][i] += r_values[0][j][1 * i];
            res[1][i] += r_values[1][j][1 * i];

            res_prime[0][i] += r_values[0][j][2 * size + i];
            res_prime[1][i] += r_values[1][j][2 * size + i];
        }
    }

    for (i = 0; i < 2; i++) {
        for (j = 0; j < numParties; j++) {
            delete[] r_values[i][j];
        }
        delete[] r_values[i];
    }
    delete[] r_values;
    delete[] buffer;
    delete[] r_bits;
    delete[] r_prime;
}
