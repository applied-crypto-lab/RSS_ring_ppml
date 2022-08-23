#include "MSB_7pc.h"

void new_Rss_MSB_7pc(Lint **res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet) {
    int pid = nodeNet->getID();
    uint numShares = nodeNet->getNumShares();
    uint i; // used for loops

    // only need to generate a single random bit per private value
    Lint **b = new Lint *[numShares];
    Lint **sum = new Lint *[numShares];
    Lint **u_2 = new Lint *[numShares];
    Lint **edaBit_r = new Lint *[numShares];
    Lint **edaBit_b_2 = new Lint *[numShares];
    Lint **rprime = new Lint *[numShares];

    Lint *c = new Lint[size];
    Lint *e = new Lint[size];

    // used for testing correctness
    Lint *res_check = new Lint[size];
    Lint *r_2_open = new Lint[size];
    Lint *u_2_open = new Lint[size];

    for (i = 0; i < numShares; i++) {
        b[i] = new Lint[size];
        edaBit_r[i] = new Lint[size];
        edaBit_b_2[i] = new Lint[size];
        sum[i] = new Lint[size];
        u_2[i] = new Lint[size];

        rprime[i] = new Lint[size];
    }

    Lint *ai = new Lint[numShares];
    memset(ai, 0, sizeof(Lint) * numShares);
    if (pid == 1) {
        ai[0] = 1;
    } else if (pid == 5) {
        ai[19] = 1; // party 4's share 6
    } else if (pid == 6) {
        ai[16] = 1; // parthy 5's share 4
    } else if (pid == 7) {
        ai[10] = 1; // parthy 5's share 4
    } 

    Rss_RandBit_7pc(b, size, ring_size, nodeNet);
    Rss_edaBit_7pc(edaBit_r, edaBit_b_2, size, ring_size, nodeNet);

    // need to generate full edabit for final implementation
    // Rss_edaBit_7pc(edaBit_r, edaBit_b_2, size, ring_size, nodeNet);

    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++) {
            rprime[s][i] = edaBit_r[s][i] - (GET_BIT(edaBit_b_2[s][i], Lint(ring_size - 1)) << Lint(ring_size - 1));
            sum[s][i] = (a[s][i] + edaBit_r[s][i]);
        }
    }

    Rss_Open_7pc(c, sum, size, ring_size, nodeNet);

    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++)
            edaBit_b_2[s][i] = edaBit_b_2[s][i] & nodeNet->SHIFT[ring_size - 1];
        c[i] = c[i] & nodeNet->SHIFT[ring_size - 1];
    }

    Rss_BitLT_7pc(u_2, c, edaBit_b_2, ring_size, size, nodeNet);

    for (i = 0; i < size; ++i) {
        // cant do this because we modify edaBit_b_2 earlier
        // for (size_t s = 0; s < numShares; s++)
        for (size_t s = 0; s < numShares; s++)
            sum[s][i] = a[s][i] - c[i] * ai[s] + rprime[s][i] - (u_2[s][i] << Lint(ring_size - 1)) + (b[s][i] << Lint(ring_size - 1));
        // sum[1][i] = a[1][i] - c[i] * a2 + rprime[1][i] - (u_2[1][i] << Lint(ring_size - 1)) + (b[1][i] << Lint(ring_size - 1));
    }
    // opening sum
    Rss_Open_7pc(e, sum, size, ring_size, nodeNet);

    Lint e_bit;
    for (i = 0; i < size; ++i) {
        e_bit = GET_BIT(e[i], ring_size - 1); // getting the (k-1)th bit
        for (size_t s = 0; s < numShares; s++) {
            res[s][i] = e_bit * ai[s] + b[s][i] - (e_bit * b[s][i] << Lint(1));
        }
        // res[1][i] = e_bit * a2 + b[1][i] - (e_bit * b[1][i] << Lint(1));
    }

    // cleanup
    delete[] c;
    delete[] ai;
    delete[] e;
    delete[] res_check;
    delete[] r_2_open;
    delete[] u_2_open;

    for (i = 0; i < numShares; i++) {
        delete[] edaBit_r[i];
        delete[] edaBit_b_2[i];
        delete[] b[i];
        delete[] sum[i];
        delete[] u_2[i];
        delete[] rprime[i];
    }
    delete[] edaBit_r;
    delete[] edaBit_b_2;
    delete[] b;
    delete[] sum;
    delete[] u_2;
    delete[] rprime;
}


void Rss_MSB_7pc(Lint **res, Lint **a, uint size, uint ring_size, NodeNetwork *nodeNet) {

    int pid = nodeNet->getID();
    uint i, j, k, index; // used for loops
    // struct timeval start;
    // struct timeval end;
    // unsigned long timer;
    uint numShares = nodeNet->getNumShares();
    uint n_rand_bits = size * (ring_size + 1);

    Lint **r_shares = new Lint *[numShares];
    for (i = 0; i < numShares; i++) {
        r_shares[i] = new Lint[n_rand_bits];
    }

    Lint **b = new Lint *[numShares];
    Lint **r = new Lint *[numShares];
    Lint **sum = new Lint *[numShares];
    Lint **rprime = new Lint *[numShares];
    Lint **r_2 = new Lint *[numShares];
    Lint **u_2 = new Lint *[numShares];

    Lint *c = new Lint[size];
    memset(c, 0, sizeof(Lint) * size);
    Lint *e = new Lint[size];
    memset(e, 0, sizeof(Lint) * size);

    // Lint *res_check = new Lint[size];
    // Lint *r_2_open = new Lint[size];
    // Lint *u_2_open = new Lint[size];

    for (i = 0; i < numShares; i++) {
        b[i] = new Lint[size];
        r[i] = new Lint[size];
        sum[i] = new Lint[size];
        rprime[i] = new Lint[size];
        memset(rprime[i], 0, sizeof(Lint) * size);
        r_2[i] = new Lint[size];
        memset(r_2[i], 0, sizeof(Lint) * size);
        u_2[i] = new Lint[size];
        memset(u_2[i], 0, sizeof(Lint) * size);
    }

    Lint *ai = new Lint[numShares];
    memset(ai, 0, sizeof(Lint) * numShares);
    if (pid == 1) {
        ai[0] = 1;
    } else if (pid == 5) {
        ai[19] = 1; // party 4's share 6
    } else if (pid == 6) {
        ai[16] = 1; // parthy 5's share 4
    } else if (pid == 7) {
        ai[10] = 1; // parthy 5's share 4
    } 
   
    // printf("Rss_RandBit_7pc start\n");
   
    Rss_RandBit_7pc(r_shares, n_rand_bits, ring_size, nodeNet);
    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++)
            b[s][i] = r_shares[s][size * ring_size + i];
        // b[1][i] = r_shares[1][size * ring_size + i];
    }
    // printf("Rss_RandBit_7pc end\n");

    for (j = 0; j < size; j++) {
        for (k = 0; k < ring_size - 1; k++) {
            // this is for step 3
            index = j * ring_size + k;
            for (size_t s = 0; s < numShares; s++)
                rprime[s][j] = rprime[s][j] + (r_shares[s][index] << Lint(k));
            // rprime[1][j] = rprime[1][j] + (r_shares[1][index] << Lint(k));
        }
        index = j * ring_size + k;
        for (size_t s = 0; s < numShares; s++)
            r[s][j] = (rprime[s][j] + ((r_shares[s][index]) << Lint(k)));
        // r[1][j] = (rprime[1][j] + ((r_shares[1][index]) << Lint(k)));
    }



    for (j = 0; j < size; j++) {
        for (k = 0; k < ring_size - 1; k++) {
            index = j * ring_size + k;

            for (size_t s = 0; s < numShares; s++)
                r_2[s][j] = Lint(SET_BIT(r_2[s][j], Lint(k), GET_BIT(r_shares[s][index], Lint(0))));
            // r_2[1][j] = Lint(SET_BIT(r_2[1][j], Lint(k), GET_BIT(r_shares[1][index], Lint(0))));
        }
    }
    // offline component ends
    // step 2
    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++)
            sum[s][i] = (a[s][i] + r[s][i]); // & nodeNet->SHIFT[1] ;
        // sum[1][i] = (a[1][i] + r[1][i]); // & nodeNet->SHIFT[1] ;
    }

    // printf("Rss_Open_7pc\n");
    Rss_Open_7pc(c, sum, size, ring_size, nodeNet);
    // printf("Rss_Open_7pc_end\n");

    // step 3 -- getting the k-1 lowest bits of c[i]
    for (i = 0; i < size; i++) {
        c[i] = c[i] & nodeNet->SHIFT[ring_size - 1];
    }
    // printf("Rss_BitLT_7pc\n");

    Rss_BitLT_7pc(u_2, c, r_2, ring_size, size, nodeNet);
 
    for (i = 0; i < size; ++i) {
        // cant do this because we modify edaBit_b_2 earlier
        // for (size_t s = 0; s < numShares; s++)
        for (size_t s = 0; s < numShares; s++)
            sum[s][i] = a[s][i] - c[i] * ai[s] + rprime[s][i] - (u_2[s][i] << Lint(ring_size - 1)) + (b[s][i] << Lint(ring_size - 1));
        // sum[1][i] = a[1][i] - c[i] * a2 + rprime[1][i] - (u_2[1][i] << Lint(ring_size - 1)) + (b[1][i] << Lint(ring_size - 1));
    }
    // opening sum
    // printf("Rss_Open_7pc\n");

    Rss_Open_7pc(e, sum, size, ring_size, nodeNet);
    // printf("Rss_Open_7pc_end\n");

    Lint e_bit;
    for (i = 0; i < size; ++i) {
        e_bit = GET_BIT(e[i], ring_size - 1); // getting the (k-1)th bit
        for (size_t s = 0; s < numShares; s++)
            res[s][i] = e_bit * ai[s] + b[s][i] - (e_bit * b[s][i] << Lint(1));
        // res[1][i] = e_bit * a2 + b[1][i] - (e_bit * b[1][i] << Lint(1));
    }

    // cleanup
    delete[] ai;
    delete[] c;
    delete[] e;
    // delete[] res_check;
    // delete[] r_2_open;
    // delete[] u_2_open;

    for (i = 0; i < numShares; i++) {
        delete[] r_shares[i];
        delete[] b[i];
        delete[] r[i];
        delete[] sum[i];
        delete[] rprime[i];
        delete[] r_2[i];
        delete[] u_2[i];
    }
    delete[] r_shares;
    delete[] b;
    delete[] r;
    delete[] sum;
    delete[] rprime;
    delete[] r_2;
    delete[] u_2;
}
