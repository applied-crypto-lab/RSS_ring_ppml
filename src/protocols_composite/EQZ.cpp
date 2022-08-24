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

#include "EQZ.h"

void Rss_EQZ(Lint **res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet) {

    int pid = nodeNet->getID();
    uint numShares = nodeNet->getNumShares();
    uint i; // used for loops

    Lint **sum = new Lint *[numShares];
    Lint **edaBit_r = new Lint *[numShares];
    Lint **edaBit_b_2 = new Lint *[numShares];
    Lint **rprime = new Lint *[numShares];
    for (i = 0; i < numShares; i++) {
        edaBit_r[i] = new Lint[size];
        edaBit_b_2[i] = new Lint[size];
        sum[i] = new Lint[size];
        rprime[i] = new Lint[size];
    }
    Lint *res_check = new Lint[size];

    Lint *c = new Lint[size];

    Lint *ai = new Lint[numShares];
    memset(ai, 0, sizeof(Lint) * numShares);
    if (pid == 1) {
        ai[0] = -1; // party 1's share 1
    } else if (pid == 3) {
        ai[1] = -1; // party 3's share 2
    }

    Rss_edaBit(edaBit_r, edaBit_b_2, size, ring_size, nodeNet);

    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++) {
            rprime[s][i] = edaBit_r[s][i] - (GET_BIT(edaBit_b_2[s][i], Lint(ring_size - 1)) << Lint(ring_size));
            sum[s][i] = (a[s][i] + rprime[s][i]);
        }
    }
    Rss_Open(c, sum, size, ring_size, nodeNet);
    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++) {
            rprime[s][i] = (c[i] & ai[s]) ^ edaBit_b_2[s][i];
        }
    }

    // Rss_Open_Bitwise(res_check, rprime, size, ring_size, nodeNet);
    // for (uint i = 0; i < size; i++) {
    //     printf("(no mask ) res_check[%u]: %llu\n", i, res_check[i]);
    // }

    // for (i = 0; i < size; i++) {
    //     for (size_t s = 0; s < numShares; s++) {
    //         rprime[s][i] = rprime[s][i] & nodeNet->SHIFT[ring_size];
    //     }
    // }
    // Rss_Open_Bitwise(res_check, rprime, size, ring_size, nodeNet);

    // for (uint i = 0; i < size; i++) {
    //     printf("(  mask  ) res_check[%u]: %llu\n", i, res_check[i]);
    // }

    Rss_k_OR_L(res, rprime, size, ring_size, nodeNet);

    delete[] c;
    delete[] ai;
    delete[] res_check;

    for (i = 0; i < numShares; i++) {
        delete[] edaBit_r[i];
        delete[] edaBit_b_2[i];
        delete[] sum[i];
        delete[] rprime[i];
    }
    delete[] edaBit_r;
    delete[] edaBit_b_2;
    delete[] sum;
    delete[] rprime;
}

void Rss_k_OR_L(Lint **res, Lint **r, uint size, uint ring_size, NodeNetwork *nodeNet) {
    uint numShares = nodeNet->getNumShares();

    uint i, j, s, originial_num, num, r_size, n_uints;
    for (i = 0; i < size; i++) {

        // removing any leading bits that could interfere with future masking
        for (s = 0; s < numShares; s++) {
            r[s][i] = r[s][i] & nodeNet->SHIFT[ring_size];
        }
    }

    r_size = ring_size >> 1; // dividing by two because we only need half as many uuint8_t's in the first (and subsequent rounds)
    n_uints = ((ring_size + 7) >> 3);

    originial_num = ((r_size + 7) >> 3);
    printf("originial_num: %u\n", originial_num);
    printf("n_uints: %u\n", n_uints);
    printf("ring_size/2: %u\n", ring_size >> 1);
    printf("r_size: %u\n", r_size);
    printf("sizeof(uint8_t) = %lu\n", sizeof(uint8_t));

    uint8_t **a = new uint8_t *[numShares];
    uint8_t **b = new uint8_t *[numShares];
    uint8_t **u = new uint8_t *[numShares];

    for (i = 0; i < numShares; i++) {
        a[i] = new uint8_t[originial_num];
        memset(a[i], 0, sizeof(uint8_t) * originial_num);
        b[i] = new uint8_t[originial_num];
        memset(b[i], 0, sizeof(uint8_t) * originial_num);
        u[i] = new uint8_t[originial_num];
        memset(u[i], 0, sizeof(uint8_t) * originial_num);
    }

    Lint msk = (Lint(1) << r_size) - Lint(1);
    printf("msk: %u\n", msk);
    printf("msk: %u\n", (msk << r_size));
    print_binary(msk, 8*sizeof(Lint));
    print_binary((msk << r_size), 8*sizeof(Lint));

    Lint temp1, temp2;

    // while (r_size > 1) {
        for (i = 0; i < size; ++i) {
            for (s = 0; s < numShares; s++) {
                temp1 = r[s][i] & msk;
                temp2 = r[s][i] & (msk << r_size);
                
                
                memcpy(a[s] + i * n_uints, &temp1, n_uints);
                memcpy(b[s] + i * n_uints, &temp2, n_uints);
            }
        }

        // need to move the unused bit forward
        // if ((r_size & 1)) {
        //     for (i = 0; i < size; ++i) {
        //         for (s = 0; s < numShares; s++) {

        //         }
        //     }
        //     new_r_size += 1;
        // }
        // r_size = r_size >> 1; // updating

    // }

    for (i = 0; i < numShares; i++) {

        delete[] a[i];
        delete[] b[i];
        delete[] u[i];
    }

    delete[] a;
    delete[] b;
    delete[] u;
}
