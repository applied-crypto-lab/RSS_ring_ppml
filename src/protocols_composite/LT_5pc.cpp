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

#include "LT_5pc.h"

// returns the higher value
void Rss_LT_mp(Lint **res, Lint **a, Lint **b, uint size, uint ring_size, NodeNetwork *nodeNet) {

    uint numShares = nodeNet->getNumShares();
    uint i; // used for loops
    Lint **c = new Lint *[numShares];
    Lint **diff = new Lint *[numShares];
    Lint **d = new Lint *[numShares];

    for (i = 0; i < numShares; i++) {
        c[i] = new Lint[size];
        diff[i] = new Lint[size];
        d[i] = new Lint[size];
    }

    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++)
            diff[s][i] = a[s][i] - b[s][i];
        // diff[1][i] = a[1][i] - b[1][i];
    }
    Rss_MSB_mp(c, diff, size, ring_size, nodeNet);

    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++)
            diff[s][i] = b[s][i] - a[s][i];
        // diff[1][i] = b[1][i] - a[1][i];
    }
    Rss_Mult_mp(d, c, diff, size, ring_size, nodeNet);

    // [d] = [c] * ([a] - [b]); [c] = MSB([a] - [b])
    // [c] * [b] + (1 - [c]) * [a] =  [c] * ([b] - [a]) + [a]
    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++)
            res[s][i] = d[s][i] + a[s][i];
        // res[1][i] = d[1][i] + a[1][i];
    }

    for (i = 0; i < numShares; i++) {
        delete[] c[i];
        delete[] diff[i];
        delete[] d[i];
    }

    delete[] c;
    delete[] d;
    delete[] diff;
}

void new_Rss_LT_mp(Lint **res, Lint **a, Lint **b, uint size, uint ring_size, NodeNetwork *nodeNet) {

    uint numShares = nodeNet->getNumShares();
    uint i; // used for loops
    Lint **c = new Lint *[numShares];
    Lint **diff = new Lint *[numShares];
    Lint **d = new Lint *[numShares];

    for (i = 0; i < numShares; i++) {
        c[i] = new Lint[size];
        diff[i] = new Lint[size];
        d[i] = new Lint[size];
    }

    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++)
            diff[s][i] = a[s][i] - b[s][i];
        // diff[1][i] = a[1][i] - b[1][i];
    }
    new_Rss_MSB_mp(c, diff, size, ring_size, nodeNet);

    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++)
            diff[s][i] = b[s][i] - a[s][i];
        // diff[1][i] = b[1][i] - a[1][i];
    }
    Rss_Mult_mp(d, c, diff, size, ring_size, nodeNet);

    // [d] = [c] * ([a] - [b]); [c] = MSB([a] - [b])
    // [c] * [b] + (1 - [c]) * [a] =  [c] * ([b] - [a]) + [a]
    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++)
            res[s][i] = d[s][i] + a[s][i];
        // res[1][i] = d[1][i] + a[1][i];
    }

    for (i = 0; i < numShares; i++) {
        delete[] c[i];
        delete[] diff[i];
        delete[] d[i];
    }

    delete[] c;
    delete[] d;
    delete[] diff;
}

// used when the "b" is the same for all input a's
// size depends on a only
void fixed_Rss_LT_mp(Lint **res, Lint **a, Lint **b, uint size, uint ring_size, uint layer, NodeNetwork *nodeNet) {

    // struct timeval start;
    // struct timeval end;

    uint numShares = nodeNet->getNumShares();
    uint i; // used for loops
    Lint **c = new Lint *[numShares];
    Lint **diff = new Lint *[numShares];
    Lint **d = new Lint *[numShares];

    for (i = 0; i < numShares; i++) {
        c[i] = new Lint[size];
        diff[i] = new Lint[size];
        d[i] = new Lint[size];
    }

    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++)
            diff[s][i] = a[s][i] - b[s][layer];
        // diff[1][i] = a[1][i] - b[1][layer];
    }

    new_Rss_MSB_mp(c, diff, size, ring_size, nodeNet);

    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++)
            diff[s][i] = b[s][layer] - a[s][i];
        // diff[1][i] = b[1][layer] - a[1][i];
    }
    Rss_Mult_mp(d, c, diff, size, ring_size, nodeNet);

    // [d] = [c] * ([a] - [b]); [c] = MSB([a] - [b])
    // [c] * [b] + (1 - [c]) * [a] =  [c] * ([b] - [a]) + [a]
    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++)
            res[s][i] = d[s][i] + a[s][i];
        // res[1][i] = d[1][i] + a[1][i];
    }

    for (i = 0; i < numShares; i++) {
        delete[] c[i];
        delete[] diff[i];
        delete[] d[i];
    }

    delete[] c;
    delete[] d;
    delete[] diff;
}

// used when the "b" is the same for all input a's
// size depends on a only
// returns the SMALLER VALUE
void fixed_Rss_GT_mp(Lint **res, Lint **a, Lint **b, uint size, uint ring_size, uint layer, NodeNetwork *nodeNet) {

    // struct timeval start;
    // struct timeval end;
    uint numShares = nodeNet->getNumShares();

    uint i; // used for loops
    Lint **c = new Lint *[numShares];
    Lint **diff = new Lint *[numShares];
    Lint **d = new Lint *[numShares];

    for (i = 0; i < numShares; i++) {
        c[i] = new Lint[size];
        diff[i] = new Lint[size];
        d[i] = new Lint[size];
    }

    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++)
            diff[s][i] = a[s][i] - b[s][layer];
        // diff[1][i] = a[1][i] - b[1][layer];
    }

    new_Rss_MSB_mp(c, diff, size, ring_size, nodeNet);

    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++)
            diff[s][i] = a[s][i] - b[s][layer];
        // diff[1][i] = a[1][i] - b[1][layer];
    }
    Rss_Mult_mp(d, c, diff, size, ring_size, nodeNet);

    // [d] = [c] * ([a] - [b]); [c] = MSB([a] - [b])
    // [c] * [b] + (1 - [c]) * [a] =  [c] * ([b] - [a]) + [a]
    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++)
            res[s][i] = d[s][i] + b[s][layer];
        // res[1][i] = d[1][i] + b[1][layer];
    }

    for (i = 0; i < numShares; i++) {
        delete[] c[i];
        delete[] diff[i];
        delete[] d[i];
    }

    delete[] c;
    delete[] d;
    delete[] diff;
}

void fixed_Rss_LT_time_mp(Lint **res, Lint **a, Lint **b, uint size, uint ring_size, uint layer, NodeNetwork *nodeNet, unsigned long &timer) {

    uint numShares = nodeNet->getNumShares();
    uint i; // used for loops
    Lint **c = new Lint *[numShares];
    Lint **diff = new Lint *[numShares];
    Lint **d = new Lint *[numShares];

    for (i = 0; i < numShares; i++) {
        c[i] = new Lint[size];
        diff[i] = new Lint[size];
        d[i] = new Lint[size];
    }

    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++)
            diff[s][i] = a[s][i] - b[s][layer];
        // diff[1][i] = a[1][i] - b[1][layer];
    }

    new_Rss_MSB_time_mp(c, diff, size, ring_size, nodeNet, timer);

    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++)
            diff[s][i] = b[s][layer] - a[s][i];
        // diff[1][i] = b[1][layer] - a[1][i];
    }
    Rss_Mult_mp(d, c, diff, size, ring_size, nodeNet);

    // [d] = [c] * ([a] - [b]); [c] = MSB([a] - [b])
    // [c] * [b] + (1 - [c]) * [a] =  [c] * ([b] - [a]) + [a]
    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++)
            res[s][i] = d[s][i] + a[s][i];
        // res[1][i] = d[1][i] + a[1][i];
    }

    for (i = 0; i < numShares; i++) {
        delete[] c[i];
        delete[] diff[i];
        delete[] d[i];
    }

    delete[] c;
    delete[] d;
    delete[] diff;
}

// used when the "b" is the same for all input a's
// size depends on a only
// returns the SMALLER VALUE
void fixed_Rss_GT_time_mp(Lint **res, Lint **a, Lint **b, uint size, uint ring_size, uint layer, NodeNetwork *nodeNet, unsigned long &timer) {

    uint numShares = nodeNet->getNumShares();
    uint i; // used for loops
    Lint **c = new Lint *[numShares];
    Lint **diff = new Lint *[numShares];
    Lint **d = new Lint *[numShares];

    for (i = 0; i < numShares; i++) {
        c[i] = new Lint[size];
        diff[i] = new Lint[size];
        d[i] = new Lint[size];
    }

    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++)
            diff[s][i] = a[s][i] - b[s][layer];
        // diff[1][i] = a[1][i] - b[1][layer];
    }

    new_Rss_MSB_time_mp(c, diff, size, ring_size, nodeNet, timer);

    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++)
            diff[s][i] = a[s][i] - b[s][layer];
        // diff[1][i] = a[1][i] - b[1][layer];
    }
    Rss_Mult_mp(d, c, diff, size, ring_size, nodeNet);

    // [d] = [c] * ([a] - [b]); [c] = MSB([a] - [b])
    // [c] * [b] + (1 - [c]) * [a] =  [c] * ([b] - [a]) + [a]
    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++)
            res[s][i] = d[s][i] + b[s][layer];
        // res[1][i] = d[1][i] + b[1][layer];
    }

    for (i = 0; i < numShares; i++) {
        delete[] c[i];
        delete[] diff[i];
        delete[] d[i];
    }

    delete[] c;
    delete[] d;
    delete[] diff;
}

// used when the "b" is the same for all input a's
// size depends on a only
// returns the SMALLER VALUE
void fixed_Rss_GT_LT_time_mp(Lint **res, Lint **a, Lint **b, uint size, uint ring_size, uint layer, NodeNetwork *nodeNet, unsigned long &timer) {

    uint numShares = nodeNet->getNumShares();
    uint i; // used for loops
    Lint **c = new Lint *[numShares];
    Lint **diff = new Lint *[numShares];
    Lint **d = new Lint *[numShares];

    for (i = 0; i < numShares; i++) {
        c[i] = new Lint[2 * size];
        diff[i] = new Lint[2 * size];
        d[i] = new Lint[size];
    }

    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++) {
            diff[s][i] = a[s][i] - b[s][layer];
            diff[s][i + size] = a[s][i];
        }
        // diff[1][i] = a[1][i] - b[1][layer];
        // diff[1][i + size] = a[1][i];
    }
    struct timeval start;
    struct timeval end;
    unsigned long op_timer = 0;

    gettimeofday(&start, NULL); // start timer here
    new_Rss_MSB_time_mp(c, diff, 2 * size, ring_size, nodeNet, timer);
    gettimeofday(&end, NULL); // stop timer here
    op_timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    // printf("--> MSB time = %.6lf\n", (double)(op_timer * 1e-6));

    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++)
            diff[s][i] = a[s][i] - b[s][layer];
        // diff[1][i] = a[1][i] - b[1][layer];
    }
    Rss_Mult_mp(d, c, diff, size, ring_size, nodeNet);

    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++)
            res[s][i] = d[s][i] + b[s][layer];
        // res[1][i] = d[1][i] + b[1][layer];
    }

    // moving the upper half down to the lower half
    for (size_t s = 0; s < numShares; s++)
        memcpy(c[s], c[s] + (size), size * sizeof(Lint));
    // memcpy(c[1], c[1] + (size), size * sizeof(Lint));

    Rss_Mult_mp(d, c, res, size, ring_size, nodeNet);

    for (i = 0; i < size; i++) {
        for (size_t s = 0; s < numShares; s++)
            res[s][i] = d[s][i] + res[s][i];
        // res[1][i] = d[1][i] + res[1][i];
    }

    for (i = 0; i < numShares; i++) {
        delete[] c[i];
        delete[] diff[i];
        delete[] d[i];
    }

    delete[] c;
    delete[] d;
    delete[] diff;
}
