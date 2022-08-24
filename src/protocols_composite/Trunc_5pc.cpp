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

#include "Trunc_5pc.h"

// priv_m is of the form 2^{M - m}, where the only thing we need to do is multiply x by priv_m and call Rss_trunc_Pr_1 with public_M
// priv_m_index corresponds to the fixed priv_m that we are truncating x by (will correspond to layer_id in neural network)
void Rss_truncPriv_mp(Lint **res, Lint **x, Lint **priv_m, Lint public_M, uint priv_m_index, uint size, uint ring_size, NodeNetwork *nodeNet) {

    // printf("public_M: %llu\n", public_M);
    // printf("priv_m_index: %llu\n", priv_m_index);

    Rss_Mult_fixed_b_mp(res, x, priv_m, priv_m_index, size, ring_size, nodeNet); // need to be changed such that we multiply a single priv_m valeu by all x's
    // need to add batch support here priv_m
    // priv_m will be of size priv_m_size, and the next batch would be priv_m_size*j (j \in 0,batch_size-1)

    Rss_truncPr_1_mp(res, res, public_M, size, ring_size, nodeNet);
}

void Rss_truncPriv_time_mp(Lint **res, Lint **x, Lint **priv_m, Lint public_M, uint priv_m_index, uint size, uint ring_size, NodeNetwork *nodeNet, unsigned long &timer) {

    // printf("public_M: %llu\n", public_M);
    // printf("priv_m_index: %llu\n", priv_m_index);

    Rss_Mult_fixed_b_mp(res, x, priv_m, priv_m_index, size, ring_size, nodeNet); // need to be changed such that we multiply a single priv_m valeu by all x's
    // need to add batch support here priv_m
    // priv_m will be of size priv_m_size, and the next batch would be priv_m_size*j (j \in 0,batch_size-1)

    Rss_truncPr_time_mp(res, res, public_M, size, ring_size, nodeNet, timer);
}

// REQUIREMENT: the MSB of x MUST BE ZERO
void Rss_truncPr_1_mp(Lint **res, Lint **x, Lint m, uint size, uint ring_size, NodeNetwork *nodeNet) {

    int pid = nodeNet->getID();
    uint numShares = nodeNet->getNumShares();
    Lint i; // used for loops

    Lint **edaBit_r = new Lint *[numShares];
    Lint **sum = new Lint *[numShares];
    Lint **r_m_prime = new Lint *[numShares];
    Lint **b = new Lint *[numShares];
    Lint **r_km1 = new Lint *[numShares];

    Lint *c = new Lint[size];
    memset(c, 0, sizeof(Lint) * size);
    Lint *c_prime = new Lint[size];
    memset(c_prime, 0, sizeof(Lint) * size);

    for (i = 0; i < numShares; i++) {
        edaBit_r[i] = new Lint[size];
        r_m_prime[i] = new Lint[size];
        r_km1[i] = new Lint[size];
        sum[i] = new Lint[size];

        b[i] = new Lint[size];
        memset(b[i], 0, sizeof(Lint) * size);
    }
    Lint *ai = new Lint[numShares];
    memset(ai, 0, sizeof(Lint) * numShares);

    // uint numParties = nodeNet->getNumParties();
    if (pid == 1) {
        ai[0] = 1;
    } else if (pid == 4) {
        ai[5] = 1; // party 4's share 6
    } else if (pid == 5) {
        ai[3] = 1; // parthy 5's share 4
    }

    // Lint ai[s] = 0;
    // Lint a2 = 0;
    // switch (pid) {
    // case 1:
    //     ai[s] = 1;
    //     a2 = 0;
    //     break;
    // case 2:
    //     ai[s] = 0;
    //     a2 = 0;
    //     break;
    // case 3:
    //     ai[s] = 0;
    //     a2 = 1;
    //     break;
    // }

    // // generating one edaBit
    // gettimeofday(&start, NULL); //start timer here
    Rss_edaBit_trunc_mp(edaBit_r, r_m_prime, r_km1, size, ring_size, m, nodeNet);
    // gettimeofday(&end, NULL); //stop timer here
    // op_timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    // printf("--> edaBit trunc time = %.6lf\n", (double)(op_timer * 1e-6));
    // // Rss_edaBit_trunc_test(edaBit_r, r_m_prime, r_km1, size, ring_size, m, map, nodeNet);

    // computing the sum of x and edabit_r
    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++) {
            sum[s][i] = (x[s][i] + edaBit_r[s][i]);
            // sum[1][i] = (x[1][i] + edaBit_r[1][i]);
        }
    }

    Rss_Open_mp(c, sum, size, ring_size, nodeNet);

    for (i = 0; i < size; i++) {

        // (c / 2^m) mod 2^(k-m-1)
        c_prime[i] = (c[i] >> Lint(m)) & nodeNet->SHIFT[ring_size - m - 1];
        for (size_t s = 0; s < numShares; s++) {

            b[s][i] = r_km1[s][i] + ((c[i] * ai[s]) >> Lint(ring_size - 1)) - 2 * ((c[i]) >> Lint(ring_size - 1)) * r_km1[s][i];
            // b[1][i] = r_km1[1][i] + ((c[i] * a2) >> Lint(ring_size - 1)) - 2 * ((c[i]) >> Lint(ring_size - 1)) * r_km1[1][i];

            r_m_prime[s][i] = r_m_prime[s][i] - (r_km1[s][i] << Lint(ring_size - 1 - m));
            // r_m_prime[1][i] = r_m_prime[1][i] - (r_km1[1][i] << Lint(ring_size - 1 - m));

            res[s][i] = (c_prime[i] * ai[s]) - r_m_prime[s][i] + (b[s][i] << Lint(ring_size - m - 1));
            // res[1][i] = (c_prime[i] * a2) - r_m_prime[1][i] + (b[1][i] << Lint(ring_size - m - 1));
        }
    }

    delete[] ai;
    delete[] c;
    delete[] c_prime;
    for (i = 0; i < numShares; i++) {
        delete[] edaBit_r[i];
        delete[] sum[i];
        delete[] b[i];
        delete[] r_m_prime[i];
        delete[] r_km1[i];
    }
    delete[] edaBit_r;
    delete[] b;
    delete[] sum;
    delete[] r_m_prime;
    delete[] r_km1;
}

void Rss_truncPr_time_mp(Lint **res, Lint **x, Lint m, uint size, uint ring_size, NodeNetwork *nodeNet, unsigned long &timer) {
    struct timeval start;
    struct timeval end;

    int pid = nodeNet->getID();
    uint numShares = nodeNet->getNumShares();
    Lint i; // used for loops

    Lint **edaBit_r = new Lint *[numShares];
    Lint **sum = new Lint *[numShares];
    Lint **r_m_prime = new Lint *[numShares];
    Lint **b = new Lint *[numShares];
    Lint **r_km1 = new Lint *[numShares];

    Lint *c = new Lint[size];
    memset(c, 0, sizeof(Lint) * size);
    Lint *c_prime = new Lint[size];
    memset(c_prime, 0, sizeof(Lint) * size);

    for (i = 0; i < numShares; i++) {
        edaBit_r[i] = new Lint[size];
        r_m_prime[i] = new Lint[size];
        r_km1[i] = new Lint[size];
        sum[i] = new Lint[size];

        b[i] = new Lint[size];
        memset(b[i], 0, sizeof(Lint) * size);
    }
    Lint *ai = new Lint[numShares];
    memset(ai, 0, sizeof(Lint) * numShares);
    if (pid == 1) {
        ai[0] = 1;
    } else if (pid == 4) {
        ai[5] = 1; // party 4's share 6
    } else if (pid == 5) {
        ai[3] = 1; // parthy 5's share 4
    }

    unsigned long op_timer = 0;

    // generating one edaBit
    gettimeofday(&start, NULL); // start timer here
    Rss_edaBit_trunc_mp(edaBit_r, r_m_prime, r_km1, size, ring_size, m, nodeNet);
    gettimeofday(&end, NULL); // stop timer here
    timer += 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    op_timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    // printf("--> edaBit trunc time = %.6lf\n", (double)(op_timer * 1e-6));
    // Rss_edaBit_trunc_test(edaBit_r, r_m_prime, r_km1, size, ring_size, m, map, nodeNet);

    // computing the sum of x and edabit_r
    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++)
            sum[s][i] = (x[s][i] + edaBit_r[s][i]);
        // sum[1][i] = (x[1][i] + edaBit_r[1][i]);
    }

    Rss_Open_mp(c, sum, size, ring_size, nodeNet);

    for (i = 0; i < size; i++) {

        // (c / 2^m) mod 2^(k-m-1)
        c_prime[i] = (c[i] >> Lint(m)) & nodeNet->SHIFT[ring_size - m - 1];
        for (size_t s = 0; s < numShares; s++) {

            b[s][i] = r_km1[s][i] + ((c[i] * ai[s]) >> Lint(ring_size - 1)) - 2 * ((c[i]) >> Lint(ring_size - 1)) * r_km1[s][i];
            // b[1][i] = r_km1[1][i] + ((c[i] * a2) >> Lint(ring_size - 1)) - 2 * ((c[i]) >> Lint(ring_size - 1)) * r_km1[1][i];

            r_m_prime[s][i] = r_m_prime[s][i] - (r_km1[s][i] << Lint(ring_size - 1 - m));
            // r_m_prime[1][i] = r_m_prime[1][i] - (r_km1[1][i] << Lint(ring_size - 1 - m));

            res[s][i] = (c_prime[i] * ai[s]) - r_m_prime[s][i] + (b[s][i] << Lint(ring_size - m - 1));
            // res[1][i] = (c_prime[i] * a2) - r_m_prime[1][i] + (b[1][i] << Lint(ring_size - m - 1));
        }
    }

    delete[] c;
    delete[] c_prime;
    for (i = 0; i < numShares; i++) {
        delete[] edaBit_r[i];
        delete[] sum[i];
        delete[] b[i];
        delete[] r_m_prime[i];
        delete[] r_km1[i];
    }
    delete[] ai;
    delete[] edaBit_r;
    delete[] b;
    delete[] sum;
    delete[] r_m_prime;
    delete[] r_km1;
}