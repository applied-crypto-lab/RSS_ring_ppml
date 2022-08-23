#include "bitLT_5pc.h"

// returns a binary sharing res, where res = a ?< b (we pack 2 shares in a single bit
// the function expects a and b to be arrays of binary values
// a is a single value
// b is two single values of packed shares
void Rss_BitLT_mp(Lint **res, Lint *a, Lint **b, uint ring_size, uint size, NodeNetwork *nodeNet) {
    uint numShares = nodeNet->getNumShares();
    // printf("Rss_BitLT_mp\n");

    int pid = nodeNet->getID();
    uint i, j; // used for loops

    Lint **b_prime = new Lint *[numShares];
    for (i = 0; i < numShares; i++) {
        b_prime[i] = new Lint[size];
    }



    Lint *ai = new Lint[numShares];
    memset(ai, 0, sizeof(Lint) * numShares);
        if (pid == 1) {
            ai[0] = -1;
        } else if (pid == 4) {
            ai[5] = -1; // party 4's share 6
        } else if (pid == 5) {
            ai[3] = -1; // parthy 5's share 4
        }

    for (j = 0; j < size; j++) {
        // step 1  -- flipping all the bits of bk
        // if switch value is zero, do nothing
        for (size_t s = 0; s < numShares; s++) {
            // printf("s: %llu\n", s);
            b_prime[s][j] = (b[s][j] ^ ai[s]);
        }
    }
    // printf("Rss_CarryOut_mp\n");

    Rss_CarryOut_mp(res, a, b_prime, ring_size, size, nodeNet);

    // flipping on share of output
    for (j = 0; j < size; ++j) {

        for (size_t s = 0; s < numShares; s++) {
            res[s][j] = GET_BIT(res[s][j], 0) ^ GET_BIT(ai[s], 0);
        }

        // res[1][j] = GET_BIT(res[1][j], 0) ^ GET_BIT(b2, 0);
    }

    for (i = 0; i < numShares; i++) {
        delete[] b_prime[i];
    }
    delete[] b_prime;
    delete[] ai;
}

void Rss_CarryOut_mp(Lint **res, Lint *a, Lint **b, uint ring_size, uint size, NodeNetwork *nodeNet) {

    uint i;
    int pid = nodeNet->getID();
    uint numShares = nodeNet->getNumShares();

    // d is shared
    // printf("inits\n");
    Lint **d = new Lint *[numShares * 2];
    for (i = 0; i < numShares * 2; i++) {
        d[i] = new Lint[size];
        memset(d[i], 0, sizeof(Lint) * size);
    }

    Lint *ai = new Lint[numShares];
    memset(ai, 0, sizeof(Lint) * numShares);
        if (pid == 1) {
            ai[0] = -1;
        } else if (pid == 4) {
            ai[5] = -1; // party 4's share 6
        } else if (pid == 5) {
            ai[3] = -1; // parthy 5's share 4
    }

    // printf("initial step\n");

    // initial step
    for (i = 0; i < size; i++) {
        // printf("i: %llu\n", i);

        for (size_t s = 0; s < numShares; s++) {
            // printf("s_0: %llu\n", s);
            d[s][i] = (a[i] & ai[s]) ^ b[s][i];
            // d[1][i] = (a[i] & a2) ^ b[1][i];
            // printf("s_1: %llu\n", s);

            d[numShares + s][i] = (a[i] & b[s][i]);
        }
        for (size_t s = 0; s < numShares; s++) {
            // printf("s: %llu\n", s);
            d[numShares + s][i] = SET_BIT(d[numShares + s][i], Lint(0), GET_BIT(d[numShares + s][i], Lint(0)) ^ (GET_BIT(d[s][i], Lint(0))));

            // d[3][i] = (a[i] & b[1][i]);
        }

        // shares of p
        // changed from a + b - 2ab
        // d[0][i] = (a[i] & a1) ^ b[0][i];
        // d[1][i] = (a[i] & a2) ^ b[1][i];

        // d[2][i] = (a[i] & b[0][i]);
        // d[3][i] = (a[i] & b[1][i]);

        // d[2][i] = SET_BIT(d[2][i], Lint(0), GET_BIT(d[2][i], Lint(0)) ^ (GET_BIT(d[0][i], Lint(0))));
        // d[3][i] = SET_BIT(d[3][i], Lint(0), GET_BIT(d[3][i], Lint(0)) ^ (GET_BIT(d[1][i], Lint(0))));
    }
    // printf("new_Rss_CarryOutAux_mp\n");

    // Rss_CarryOutAux(res, d, ring_size, size, map, nodeNet);
    new_Rss_CarryOutAux_mp(res, d, ring_size, size, nodeNet);
    // Rss_CarryOutAux(res, d, ring_size, size, map, nodeNet);

    for (i = 0; i < numShares * 2; i++) {
        delete[] d[i];
    }
    delete[] d;
    delete[] ai;

    // res is the actual return value of carry out, being [g] secret shared
}

void OptimalBuffer_mp(Lint **buffer, Lint **d, uint size, uint r_size, NodeNetwork *nodeNet) {
    // prepares input u for multiplication
    // buffer and d are the same size (4 x size)
    // reduction to a single loop, making complexity O(size), rather than O(size * r_size)
    uint numShares = nodeNet->getNumShares();

    Lint i, even, odd;
    // doing this once
    even = nodeNet->EVEN[r_size];
    odd = nodeNet->ODD[r_size];

    for (i = 0; i < size; ++i) {
        for (size_t s = 0; s < numShares; s++) {
            buffer[s][i] = buffer[s][i] | (d[s][i] & even);
            buffer[s][i] = buffer[s][i] | ((d[s + numShares][i] & even) << Lint(1));

            buffer[s + numShares][i] = buffer[s + numShares][i] | ((d[s][i] & odd) >> Lint(1));
            buffer[s + numShares][i] = buffer[s + numShares][i] | (d[s][i] & odd);
        }

        // buffer[0][i] = buffer[0][i] | (d[0][i] & even);
        // buffer[1][i] = buffer[1][i] | (d[1][i] & even);
        // buffer[0][i] = buffer[0][i] | ((d[2][i] & even) << Lint(1));
        // buffer[1][i] = buffer[1][i] | ((d[3][i] & even) << Lint(1));

        // buffer[2][i] = buffer[2][i] | ((d[0][i] & odd) >> Lint(1));
        // buffer[3][i] = buffer[3][i] | ((d[1][i] & odd) >> Lint(1));
        // buffer[2][i] = buffer[2][i] | (d[0][i] & odd);
        // buffer[3][i] = buffer[3][i] | (d[1][i] & odd);

        // buffer[0][i] = buffer[0][i] & nodeNet->SHIFT[r_size];
        // buffer[1][i] = buffer[1][i] & nodeNet->SHIFT[r_size];
        // buffer[2][i] = buffer[2][i] & nodeNet->SHIFT[r_size];
        // buffer[3][i] = buffer[3][i] & nodeNet->SHIFT[r_size];
    }
}

// a non-recursive solution to minimize memory consumption
void new_Rss_CarryOutAux_mp(Lint **res, Lint **d, uint ring_size, uint size, NodeNetwork *nodeNet) {
    uint numShares = nodeNet->getNumShares();
    uint i, j, originial_num, num, n_uints, r_size, new_r_size, t_index;
    // uint8_t utemp1, utemp2;
    Lint mask2, mask1m8, mask2m8;
    r_size = ring_size;
    originial_num = ((r_size + 7) >> 3) * size;

    Lint **buffer = new Lint *[numShares * 2];
    for (i = 0; i < numShares * 2; i++) {
        buffer[i] = new Lint[size];
        memset(buffer[i], 0, sizeof(Lint) * size);
    }

    uint8_t **a = new uint8_t *[numShares];
    uint8_t **b = new uint8_t *[numShares];
    uint8_t **u = new uint8_t *[numShares];

    //  need to do memsets every iteration
    for (i = 0; i < numShares; i++) {
        // memsets are actually needed here since are ORing
        // tried w/o memsets - still working?
        a[i] = new uint8_t[originial_num];
        memset(a[i], 0, sizeof(uint8_t) * originial_num);
        b[i] = new uint8_t[originial_num];
        memset(b[i], 0, sizeof(uint8_t) * originial_num);
        u[i] = new uint8_t[originial_num];
        // memset(u[i], 0, sizeof(uint8_t) * originial_num);
    }

    while (r_size > 1) {
        num = ((r_size + 7) >> 3) * size;
        n_uints = ((r_size + 7) >> 3);
        // rounding down here (by default)
        new_r_size = r_size >> 1; // (r_size / 2)

        // preparing the buffer
        // CarryBuffer(buffer, d, size, new_r_size);
        // OptimalBuffer(buffer2, d, size, r_size, nodeNet);
        OptimalBuffer_mp(buffer, d, size, r_size, nodeNet);

        // Splitting the buffer into bytes
        for (i = 0; i < size; ++i) {
            for (size_t s = 0; s < numShares; s++) {
                memcpy(a[s] + i * n_uints, buffer[s] + i, n_uints);
                memcpy(b[s] + i * n_uints, buffer[numShares + s] + i, n_uints);
            }
        }

        Rss_Mult_Byte_mp(u, a, b, num, nodeNet);

        // clearing the buffer
        for (int i = 0; i < numShares * 2; i++) {
            memset(buffer[i], 0, sizeof(Lint) * size);
        }

        // printf("adding g2j+1\n");
        for (i = 0; i < size; ++i) {
            for (j = 0; j < new_r_size; ++j) {
                // loop constants
                t_index = (j >> 2) + (i * n_uints);
                mask2 = (2 * j + 1);
                mask2m8 = (2 * j + 1) & 7;
                mask1m8 = (2 * j) & 7; // "&7" = %8, used for leftover bits

                for (size_t s = 0; s < numShares; s++) {

                    buffer[s][i] = SET_BIT(buffer[s][i], j, GET_BIT(u[s][t_index], mask1m8));
                    // buffer[s][i] = SET_BIT(buffer[s][i], j, GET_BIT(u[s][t_index], mask1m8));

                    buffer[numShares + s][i] = SET_BIT(buffer[numShares + s][i], j, (GET_BIT(u[s][t_index], mask2m8) ^ GET_BIT(d[numShares + s][i], mask2)));
                    // buffer[numShares + s][i] = SET_BIT(buffer[numShares + s][i], j, (GET_BIT(u[s][t_index], mask2m8) ^ GET_BIT(d[numShares + s][i], mask2)));
                }
            }
        }

        // checking if we have an unused (p,g) pair
        // if yes, move it to new_r_size's position
        // and update the new_r_size by 1
        if ((r_size & 1)) {
            // printf("moving unused\n");
            for (i = 0; i < size; ++i) {
                for (j = 0; j < 2 * numShares; ++j) {
                    // getting the unused p (or g) from d and
                    // moving it to new_r_size + 1
                    buffer[j][i] = SET_BIT(buffer[j][i], Lint(new_r_size), GET_BIT(d[j][i], Lint(r_size - 1)));
                }
            }
            new_r_size += 1;
        }
        // copying buffer back to d for next round
        for (i = 0; i < size; i++) {
            for (j = 0; j < 2 * numShares; j++) {
                memcpy(d[j] + i, buffer[j] + i, sizeof(Lint));
            }
        }

        // updating r_size for next round
        r_size = new_r_size;

        // sanitizing at end of round
        for (i = 0; i < 2 * numShares; i++) {
            memset(buffer[i], 0, sizeof(Lint) * size);
        }
        // for (i = 0; i < 2; i++) {
        //     memset(a[i], 0, sizeof(uint8_t) * originial_num);
        //     memset(b[i], 0, sizeof(uint8_t) * originial_num);
        //     memset(u[i], 0, sizeof(uint8_t) * originial_num);
        // }
    }

    // base case of recursive implementation
    for (i = 0; i < size; ++i) {
        res[0][i] = SET_BIT(res[0][i], 0, GET_BIT(d[2][i], 0));
        res[1][i] = SET_BIT(res[1][i], 0, GET_BIT(d[3][i], 0));
    }

    for (i = 0; i < numShares; i++) {

        delete[] a[i];
        delete[] b[i];
        delete[] u[i];
    }

    delete[] a;
    delete[] b;
    delete[] u;

    for (i = 0; i < numShares * 2; i++) {
        delete[] buffer[i];
    }
    delete[] buffer;
}
