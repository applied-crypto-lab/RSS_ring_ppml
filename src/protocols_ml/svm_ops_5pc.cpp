#include "svm_ops_5pc.h"

// non-index preserving implementation, just returns the max value
void maximum_mp(Lint **res, Lint **a, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet) {
    uint numShares = nodeNet->getNumShares();
    if (size > 1) {
        uint i, j;
        uint new_size = size >> 1;           // rounding down
        uint new_size_ceil = (size + 1) / 2; // rounding up
        // printf("\nnew_size : %u\n", new_size);
        // printf("new_size_ceil : %u\n", new_size_ceil);
        // printf("size : %u\n", size);

        Lint **a0 = new Lint *[numShares];
        Lint **a1 = new Lint *[numShares];
        Lint **ap = new Lint *[numShares];
        for (i = 0; i < numShares; i++) {
            a0[i] = new Lint[size * batch_size];
            // memset(a0[i],0,sizeof(Lint)*size*batch_size);
            a1[i] = new Lint[size * batch_size];
            // memset(a1[i],0,sizeof(Lint)*size*batch_size);
            ap[i] = new Lint[size * batch_size];
            // memset(ap[i],0,sizeof(Lint)*size*batch_size);
        }

        Lint *res_check = new Lint[size * batch_size];
        // memset(res_check,0,sizeof(Lint)*size*batch_size);

        for (j = 0; j < batch_size; j++) {
            for (i = 0; i < new_size; i++) {

                // copying the first half of a into a0
                memcpy(a0[0] + i + j * new_size_ceil, a[0] + i + j * size, sizeof(Lint));
                memcpy(a0[1] + i + j * new_size_ceil, a[1] + i + j * size, sizeof(Lint));

                // copying the last half of a into a1
                memcpy(a1[0] + i + j * new_size_ceil, a[0] + new_size + i + j * size, sizeof(Lint));
                memcpy(a1[1] + i + j * new_size_ceil, a[1] + new_size + i + j * size, sizeof(Lint));
            }
        }

        Rss_LT_mp(ap, a0, a1, new_size_ceil * batch_size, ring_size, nodeNet);

        // checking for unused shares, putting them in a1
        if ((size & 1)) {
            // printf("HI\n");
            for (i = 0; i < batch_size; i++) {
                // printf("i = %u\n", i);
                // printf("i*new_size_ceil + new_size = %u\n", i*new_size_ceil + new_size);
                // printf("i*size + size-1 = %u\n", i*size + size-1);
                ap[0][i * new_size_ceil + new_size] = a[0][i * size + size - 1];
                ap[1][i * new_size_ceil + new_size] = a[1][i * size + size - 1];
            }
            new_size += 1;
        }

        maximum_mp(res, ap, ring_size, new_size, batch_size, nodeNet);

        for (i = 0; i < numShares; i++) {
            delete[] a0[i];
            delete[] a1[i];
            delete[] ap[i];
        }
        delete[] a0;
        delete[] a1;
        delete[] ap;
        delete[] res_check;

    } else {
        // fill the maximums in at the ith position where i=[0,batch_size]
        for (int i = 0; i < batch_size; i++) {
            res[0][i] = a[0][i];
            res[1][i] = a[1][i];
        }
    }
}

// non-index preserving implementation, just returns the max value
void eda_maximum_mp(Lint **res, Lint **a, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet) {
    uint numShares = nodeNet->getNumShares();
    if (size > 1) {
        uint i, j;
        uint new_size = size >> 1;           // rounding down
        uint new_size_ceil = (size + 1) / 2; // rounding up
        // printf("\nnew_size : %u\n", new_size);
        // printf("new_size_ceil : %u\n", new_size_ceil);
        // printf("size : %u\n", size);

        Lint **a0 = new Lint *[numShares];
        Lint **a1 = new Lint *[numShares];
        Lint **ap = new Lint *[numShares];
        for (i = 0; i < numShares; i++) {
            a0[i] = new Lint[size * batch_size];
            // memset(a0[i], 0, sizeof(Lint) * size * batch_size);
            a1[i] = new Lint[size * batch_size];
            // memset(a1[i], 0, sizeof(Lint) * size * batch_size);
            ap[i] = new Lint[size * batch_size];
            // memset(ap[i], 0, sizeof(Lint) * size * batch_size);
        }

        Lint *res_check = new Lint[size * batch_size];
        // memset(res_check, 0, sizeof(Lint) * size * batch_size);

        for (j = 0; j < batch_size; j++) {
            for (i = 0; i < new_size; i++) {

                // copying the first half of a into a0
                memcpy(a0[0] + i + j * new_size_ceil, a[0] + i + j * size, sizeof(Lint));
                memcpy(a0[1] + i + j * new_size_ceil, a[1] + i + j * size, sizeof(Lint));

                // copying the last half of a into a1
                memcpy(a1[0] + i + j * new_size_ceil, a[0] + new_size + i + j * size, sizeof(Lint));
                memcpy(a1[1] + i + j * new_size_ceil, a[1] + new_size + i + j * size, sizeof(Lint));
            }
        }

        new_Rss_LT_mp(ap, a0, a1, new_size_ceil * batch_size, ring_size, nodeNet);

        // checking for unused shares, putting them in a1
        if ((size & 1)) {
            // printf("HI\n");
            for (i = 0; i < batch_size; i++) {
                // printf("i = %u\n", i);
                // printf("i*new_size_ceil + new_size = %u\n", i*new_size_ceil + new_size);
                // printf("i*size + size-1 = %u\n", i*size + size-1);
                ap[0][i * new_size_ceil + new_size] = a[0][i * size + size - 1];
                ap[1][i * new_size_ceil + new_size] = a[1][i * size + size - 1];
            }
            new_size += 1;
        }

        eda_maximum_mp(res, ap, ring_size, new_size, batch_size, nodeNet);

        for (i = 0; i < numShares; i++) {
            delete[] a0[i];
            delete[] a1[i];
            delete[] ap[i];
        }
        delete[] a0;
        delete[] a1;
        delete[] ap;
        delete[] res_check;

    } else {
        // fill the maximums in at the ith position where i=[0,batch_size]
        for (int i = 0; i < batch_size; i++) {
            res[0][i] = a[0][i];
            res[1][i] = a[1][i];
        }
    }
}

// returns the index of the maximum a value (res_index), as well as the value itself (res)
void arg_max_mp(Lint **res, Lint **res_index, Lint **a, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet) {
    uint numShares = nodeNet->getNumShares();

    if (size > 1) {

        uint i, j;
        uint new_size = size >> 1;           // rounding down
        // uint new_size_original = size >> 1;  // rounding down
        uint new_size_ceil = (size + 1) / 2; // rounding up
        int pid = nodeNet->getID();
        // printf("size : %u\n", size);
        // printf("new_size : %u\n", new_size);

        uint total = size * batch_size;
        // uint total_new = new_size * batch_size;
        uint total_new_ceil = new_size_ceil * batch_size;

        Lint **a0 = new Lint *[numShares];
        Lint **a1 = new Lint *[numShares];
        Lint **ap = new Lint *[numShares];
        Lint **c = new Lint *[numShares];
        Lint **diff = new Lint *[numShares];
        Lint **d0 = new Lint *[numShares];
        Lint **index = new Lint *[numShares];

        Lint *res_check = new Lint[total];
        // memset(res_check,0,sizeof(Lint)*total);

        for (i = 0; i < numShares; i++) {
            a0[i] = new Lint[total];
            // memset(a0[i],0,sizeof(Lint)*total);
            a1[i] = new Lint[total];
            // memset(a1[i],0,sizeof(Lint)*total);
            ap[i] = new Lint[total];
            // memset(ap[i],0,sizeof(Lint)*total);
            c[i] = new Lint[total];
            // memset(c[i],0,sizeof(Lint)*total);
            diff[i] = new Lint[total];
            // memset(diff[i],0,sizeof(Lint)*total);
            d0[i] = new Lint[total];
            // memset(d0[i],0,sizeof(Lint)*total);
            index[i] = new Lint[total];
            // memset(index[i],0,sizeof(Lint)*total);
        }

        for (j = 0; j < batch_size; j++) {
            for (i = 0; i < new_size; i++) {

                // copying the first half of a into a0
                memcpy(a0[0] + i + j * new_size_ceil, a[0] + i + j * size, sizeof(Lint));
                memcpy(a0[1] + i + j * new_size_ceil, a[1] + i + j * size, sizeof(Lint));

                // copying the last half of a into a1
                memcpy(a1[0] + i + j * new_size_ceil, a[0] + new_size + i + j * size, sizeof(Lint));
                memcpy(a1[1] + i + j * new_size_ceil, a[1] + new_size + i + j * size, sizeof(Lint));
            }
        }

        // checking for unused shares, putting them in a1
        // if((size & 1)) {
        // 	for (i = 0; i < batch_size; i++) {
        //      a1[0][i*new_size_ceil + new_size] = a[0][i*size + size-1];
        //      a1[1][i*new_size_ceil + new_size] = a[1][i*size + size-1];
        // 	}
        //     new_size += 1;
        // }

        Lint b1 = 0;
        Lint b2 = 0;
        switch (pid) {
        case 1:
            b1 = 1;
            b2 = 0;
            break;
        case 2:
            b1 = 0;
            b2 = 0;
            break;
        case 3:
            b1 = 0;
            b2 = 1;
            break;
        }

        for (i = 0; i < total_new_ceil; i++) {
            diff[0][i] = a0[0][i] - a1[0][i];
            diff[1][i] = a0[1][i] - a1[1][i];
        }

        Rss_MSB_mp(c, diff, total_new_ceil, ring_size, nodeNet);

        for (i = 0; i < total_new_ceil; i++) {
            diff[0][i] = a1[0][i] - a0[0][i];
            diff[1][i] = a1[1][i] - a0[1][i];
        }

        Rss_Mult_mp(d0, c, diff, total_new_ceil, ring_size, nodeNet);

        for (i = 0; i < total_new_ceil; i++) {
            ap[0][i] = d0[0][i] + a0[0][i];
            ap[1][i] = d0[1][i] + a0[1][i];
        }

        for (j = 0; j < batch_size; j++) {
            // printf("j : %u\n", j);
            for (i = 0; i < new_size; i++) {
                // printf("i : %u\n", i);
                // printf("j*new_size + i : %u\n", j*new_size_ceil + i);
                index[0][j * new_size_ceil + i] = c[0][j * new_size_ceil + i] * (new_size) + i * b1;
                index[1][j * new_size_ceil + i] = c[1][j * new_size_ceil + i] * (new_size) + i * b2;
            }
            // printf("hi\n");
        }

        if ((size & 1)) {
            for (i = 0; i < batch_size; i++) {
                ap[0][i * new_size_ceil + new_size] = a[0][i * size + size - 1];
                ap[1][i * new_size_ceil + new_size] = a[1][i * size + size - 1];

                index[0][i * new_size_ceil + new_size] = b1 * (size - 1);
                index[1][i * new_size_ceil + new_size] = b2 * (size - 1);
            }
            new_size += 1;
        }

        arg_max_helper_mp(res, res_index, ap, index, ring_size, new_size, batch_size, nodeNet);

        for (i = 0; i < numShares; i++) {
            delete[] a0[i];
            delete[] a1[i];
            delete[] ap[i];
            delete[] c[i];
            delete[] diff[i];
            delete[] d0[i];
            delete[] index[i];
        }

        delete[] a0;
        delete[] a1;
        delete[] ap;
        delete[] c;
        delete[] diff;
        delete[] d0;
        delete[] index;
        delete[] res_check;

    } else {

        for (int i = 0; i < batch_size; i++) {
            res[0][i] = a[0][i];
            res[1][i] = a[1][i];
            res_index[0][i] = 0;
            res_index[1][i] = 0;
        }
    }
}

void arg_max_helper_mp(Lint **res, Lint **res_index, Lint **a, Lint **a_index, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet) {
    uint numShares = nodeNet->getNumShares();

    if (size > 1) {

        Lint i, j;
        uint new_size = size >> 1;           // rounding down
        uint new_size_ceil = (size + 1) / 2; // rounding up
        // int pid = nodeNet->getID();

        uint total = size * batch_size;
        // uint total_new = new_size * batch_size;
        uint total_new_ceil = new_size_ceil * batch_size;

        // printf("\nnew_size : %u\n", new_size);
        // printf("new_size_ceil : %u\n", new_size_ceil);
        // printf("size : %u\n", size);

        Lint **a0 = new Lint *[numShares];
        Lint **a1 = new Lint *[numShares];
        Lint **ap = new Lint *[numShares];
        Lint **a0_index = new Lint *[numShares];
        Lint **a1_index = new Lint *[numShares];
        Lint **c = new Lint *[numShares];
        Lint **diff = new Lint *[numShares];
        Lint **d0 = new Lint *[numShares];
        Lint **index = new Lint *[numShares];

        for (i = 0; i < numShares; i++) {
            a0[i] = new Lint[total];
            a0_index[i] = new Lint[total];
            a1[i] = new Lint[total];
            a1_index[i] = new Lint[total];
            ap[i] = new Lint[total];
            c[i] = new Lint[total];
            diff[i] = new Lint[total];
            d0[i] = new Lint[total];
            index[i] = new Lint[total];
        }

        Lint *res_check = new Lint[total];
        memset(res_check, 0, sizeof(Lint) * total);

        for (j = 0; j < batch_size; j++) {
            for (i = 0; i < new_size; i++) {

                // copying the first half of a into a0
                memcpy(a0[0] + i + j * new_size_ceil, a[0] + i + j * size, sizeof(Lint));
                memcpy(a0[1] + i + j * new_size_ceil, a[1] + i + j * size, sizeof(Lint));
                memcpy(a0_index[0] + i + j * new_size_ceil, a_index[0] + i + j * size, sizeof(Lint));
                memcpy(a0_index[1] + i + j * new_size_ceil, a_index[1] + i + j * size, sizeof(Lint));

                // copying the last half of a into a1
                memcpy(a1[0] + i + j * new_size_ceil, a[0] + new_size + i + j * size, sizeof(Lint));
                memcpy(a1[1] + i + j * new_size_ceil, a[1] + new_size + i + j * size, sizeof(Lint));
                memcpy(a1_index[0] + i + j * new_size_ceil, a_index[0] + new_size + i + j * size, sizeof(Lint));
                memcpy(a1_index[1] + i + j * new_size_ceil, a_index[1] + new_size + i + j * size, sizeof(Lint));
            }
        }

        // Rss_Open(res_check, a0, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("a0[%i] : %llu\n", i, res_check[i]);
        // }
        // printf("\n");

        // Rss_Open(res_check, a1, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("a1[%i] : %llu\n", i, res_check[i]);
        // }

        for (i = 0; i < total_new_ceil; i++) {
            diff[0][i] = a0[0][i] - a1[0][i];
            diff[1][i] = a0[1][i] - a1[1][i];
        }

        Rss_MSB_mp(c, diff, total_new_ceil, ring_size, nodeNet);

        // Rss_Open(res_check, c, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("c[%i] : %llu\n", i, res_check[i]);
        // }

        for (i = 0; i < total_new_ceil; i++) {
            diff[0][i] = a1[0][i] - a0[0][i];
            diff[1][i] = a1[1][i] - a0[1][i];
        }

        Rss_Mult_mp(d0, c, diff, total_new_ceil, ring_size, nodeNet);

        for (i = 0; i < total_new_ceil; i++) {
            ap[0][i] = d0[0][i] + a0[0][i];
            ap[1][i] = d0[1][i] + a0[1][i];

            diff[0][i] = a1_index[0][i] - a0_index[0][i];
            diff[1][i] = a1_index[1][i] - a0_index[1][i];
        }

        Rss_Mult_mp(d0, c, diff, total_new_ceil, ring_size, nodeNet);

        for (i = 0; i < total_new_ceil; i++) {
            index[0][i] = d0[0][i] + a0_index[0][i];
            index[1][i] = d0[1][i] + a0_index[1][i];
        }

        //       printf("\n");
        // Rss_Open(res_check, ap, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("ap[%i] : %llu\n", i, res_check[i]);
        // }

        if ((size & 1)) {

            for (i = 0; i < batch_size; i++) {
                ap[0][i * new_size_ceil + new_size] = a[0][i * size + size - 1];
                ap[1][i * new_size_ceil + new_size] = a[1][i * size + size - 1];

                index[0][i * new_size_ceil + new_size] = a_index[0][i * size + size - 1];
                index[1][i * new_size_ceil + new_size] = a_index[1][i * size + size - 1];
            }
            new_size += 1;
        }
        //       printf("\n");
        // Rss_Open(res_check, ap, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("ap'[%i] : %llu\n", i, res_check[i]);
        // }

        //       printf("\n");
        // Rss_Open(res_check, index, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("index'[%i] : %llu\n", i, res_check[i]);
        // }

        arg_max_helper_mp(res, res_index, ap, index, ring_size, new_size, batch_size, nodeNet);

        for (i = 0; i < numShares; i++) {
            delete[] a0[i];
            delete[] a0_index[i];
            delete[] a1[i];
            delete[] a1_index[i];
            delete[] ap[i];

            delete[] c[i];
            delete[] diff[i];
            delete[] d0[i];
            delete[] index[i];
        }

        delete[] a0;
        delete[] a1;
        delete[] ap;
        delete[] a0_index;
        delete[] a1_index;
        delete[] c;
        delete[] diff;
        delete[] d0;
        delete[] index;

        delete[] res_check;

    }

    else {

        for (int i = 0; i < batch_size; i++) {
            res[0][i] = a[0][i];
            res[1][i] = a[1][i];
            res_index[0][i] = a_index[0][i];
            res_index[1][i] = a_index[1][i];
        }
    }
}

// returns the index of the maximum a value (res_index), as well as the value itself (res)
void eda_arg_max_mp(Lint **res, Lint **res_index, Lint **a, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet) {
    uint numShares = nodeNet->getNumShares();

    if (size > 1) {

        uint i, j;
        uint new_size = size >> 1;           // rounding down
        // uint new_size_original = size >> 1;  // rounding down
        uint new_size_ceil = (size + 1) / 2; // rounding up
        int pid = nodeNet->getID();
        // printf("size : %u\n", size);
        // printf("new_size : %u\n", new_size);

        uint total = size * batch_size;
        // uint total_new = new_size * batch_size;
        uint total_new_ceil = new_size_ceil * batch_size;

        Lint **a0 = new Lint *[numShares];
        Lint **a1 = new Lint *[numShares];
        Lint **ap = new Lint *[numShares];
        Lint **c = new Lint *[numShares];
        Lint **diff = new Lint *[numShares];
        Lint **d0 = new Lint *[numShares];
        Lint **index = new Lint *[numShares];

        Lint *res_check = new Lint[total];
        // memset(res_check, 0, sizeof(Lint) * total);

        for (i = 0; i < numShares; i++) {
            a0[i] = new Lint[total];
            a1[i] = new Lint[total];
            ap[i] = new Lint[total];
            c[i] = new Lint[total];
            diff[i] = new Lint[total];
            d0[i] = new Lint[total];
            index[i] = new Lint[total];
        }

        for (j = 0; j < batch_size; j++) {
            for (i = 0; i < new_size; i++) {

                for (size_t s = 0; s < numShares; s++) {
                    memcpy(a0[s] + i + j * new_size_ceil, a[s] + i + j * size, sizeof(Lint));
                    memcpy(a1[s] + i + j * new_size_ceil, a[s] + new_size + i + j * size, sizeof(Lint));
                }

                // copying the first half of a into a0
                // memcpy(a0[0] + i + j * new_size_ceil, a[0] + i + j * size, sizeof(Lint));
                // memcpy(a0[1] + i + j * new_size_ceil, a[1] + i + j * size, sizeof(Lint));

                // copying the last half of a into a1
                // memcpy(a1[0] + i + j * new_size_ceil, a[0] + new_size + i + j * size, sizeof(Lint));
                // memcpy(a1[1] + i + j * new_size_ceil, a[1] + new_size + i + j * size, sizeof(Lint));
            }
        }

        // checking for unused shares, putting them in a1
        // if((size & 1)) {
        // 	for (i = 0; i < batch_size; i++) {
        //      a1[0][i*new_size_ceil + new_size] = a[0][i*size + size-1];
        //      a1[1][i*new_size_ceil + new_size] = a[1][i*size + size-1];
        // 	}
        //     new_size += 1;
        // }
        Lint *bi = new Lint[numShares];
        memset(bi, 0, sizeof(Lint) * numShares);
        // if (pid == 1) {
        //     bi[0] = 1;
        // } else if (pid == numShares) {
        //     bi[numShares - 1] = 1;
        // }

        uint numParties = nodeNet->getNumParties();
        if (numParties == 3) {
            if (pid == 1) {
                bi[0] = 1; // party 1's share 1
            } else if (pid == 3) {
                bi[1] = 1; // party 3's share 2
            }
        } else if (numParties == 5) {
            if (pid == 1) {
                bi[0] = 1;
            } else if (pid == 4) {
                bi[5] = 1; // party 4's share 6
            } else if (pid == 5) {
                bi[3] = 1; // parthy 5's share 4
            }
        }

        // Lint b1 = 0;
        // Lint b2 = 0;
        // switch (pid) {
        // case 1:
        //     b1 = 1;
        //     b2 = 0;
        //     break;
        // case 2:
        //     b1 = 0;
        //     b2 = 0;
        //     break;
        // case 3:
        //     b1 = 0;
        //     b2 = 1;
        //     break;
        // }

        // Rss_Open(res_check, a0, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("a0[%i] : %llu\n", i, res_check[i]);
        // }
        // printf("\n");

        // Rss_Open(res_check, a1, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("a1[%i] : %llu\n", i, res_check[i]);
        // }

        for (i = 0; i < total_new_ceil; i++) {
            for (size_t s = 0; s < numShares; s++)
                diff[s][i] = a0[s][i] - a1[s][i];
            // diff[1][i] = a0[1][i] - a1[1][i];
        }

        new_Rss_MSB_mp(c, diff, total_new_ceil, ring_size, nodeNet);

        // Rss_Open(res_check, diff, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("res_check[%i] : %llu\n", i, res_check[i]);
        // }

        for (i = 0; i < total_new_ceil; i++) {
            for (size_t s = 0; s < numShares; s++)
                diff[s][i] = a1[s][i] - a0[s][i];
            // diff[1][i] = a1[1][i] - a0[1][i];
        }

        Rss_Mult_mp(d0, c, diff, total_new_ceil, ring_size, nodeNet);

        for (i = 0; i < total_new_ceil; i++) {
            for (size_t s = 0; s < numShares; s++)
                ap[s][i] = d0[s][i] + a0[s][i];
            // ap[1][i] = d0[1][i] + a0[1][i];
        }

        // Rss_Open(res_check, c, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("c[%i] : %llu\n", i, res_check[i]);
        // }
        //    // new_size -=1;

        for (j = 0; j < batch_size; j++) {
            // printf("j : %u\n", j);
            for (i = 0; i < new_size; i++) {
                // printf("i : %u\n", i);
                // printf("j*new_size + i : %u\n", j*new_size_ceil + i);
                for (size_t s = 0; s < numShares; s++)
                    index[s][j * new_size_ceil + i] = c[s][j * new_size_ceil + i] * (new_size) + i * bi[s];
                // index[1][j * new_size_ceil + i] = c[1][j * new_size_ceil + i] * (new_size) + i * b2;
            }
            // printf("hi\n");
        }

        // Rss_Open(res_check, index, total_new_ceil, ring_size, nodeNet);

        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("res_check[%i] : %llu\n", i, res_check[i]);
        // }

        //       printf("\n");
        // Rss_Open(res_check, index, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("index'[%i] : %llu\n", i, res_check[i]);
        // }
        if ((size & 1)) {
            for (i = 0; i < batch_size; i++) {
                for (size_t s = 0; s < numShares; s++) {
                    ap[s][i * new_size_ceil + new_size] = a[s][i * size + size - 1];
                    // ap[1][i * new_size_ceil + new_size] = a[1][i * size + size - 1];

                    index[s][i * new_size_ceil + new_size] = bi[s] * (size - 1);
                    // index[1][i * new_size_ceil + new_size] = b2 * (size - 1);
                }
            }
            new_size += 1;
        }

        //       printf("\n");
        // Rss_Open(res_check, index, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("index'[%i] : %llu\n", i, res_check[i]);
        // }

        eda_arg_max_helper_mp(res, res_index, ap, index, ring_size, new_size, batch_size, nodeNet);

        for (i = 0; i < numShares; i++) {
            delete[] a0[i];
            delete[] a1[i];
            delete[] ap[i];
            delete[] c[i];
            delete[] diff[i];
            delete[] d0[i];
            delete[] index[i];
        }

        delete[] a0;
        delete[] a1;
        delete[] ap;
        delete[] c;
        delete[] diff;
        delete[] d0;
        delete[] index;
        delete[] res_check;
        delete[] bi;

    } else {

        for (int i = 0; i < batch_size; i++) {
            for (size_t s = 0; s < numShares; s++) {
                res[s][i] = a[s][i];
                // res[1][i] = a[1][i];
                res_index[s][i] = 0;
                // res_index[1][i] = 0;
            }
        }
    }
}

void eda_arg_max_helper_mp(Lint **res, Lint **res_index, Lint **a, Lint **a_index, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet) {
    uint numShares = nodeNet->getNumShares();

    if (size > 1) {

        Lint i, j;
        uint new_size = size >> 1;           // rounding down
        uint new_size_ceil = (size + 1) / 2; // rounding up
        // int pid = nodeNet->getID();

        uint total = size * batch_size;
        // uint total_new = new_size * batch_size;
        uint total_new_ceil = new_size_ceil * batch_size;

        // printf("\nnew_size : %u\n", new_size);
        // printf("new_size_ceil : %u\n", new_size_ceil);
        // printf("size : %u\n", size);

        Lint **a0 = new Lint *[numShares];
        Lint **a1 = new Lint *[numShares];
        Lint **ap = new Lint *[numShares];
        Lint **a0_index = new Lint *[numShares];
        Lint **a1_index = new Lint *[numShares];
        Lint **c = new Lint *[numShares];
        Lint **diff = new Lint *[numShares];
        Lint **d0 = new Lint *[numShares];
        Lint **index = new Lint *[numShares];

        for (i = 0; i < numShares; i++) {
            a0[i] = new Lint[total];
            a0_index[i] = new Lint[total];
            a1[i] = new Lint[total];
            a1_index[i] = new Lint[total];
            ap[i] = new Lint[total];
            c[i] = new Lint[total];
            diff[i] = new Lint[total];
            d0[i] = new Lint[total];
            index[i] = new Lint[total];
        }

        Lint *res_check = new Lint[total];
        // memset(res_check, 0, sizeof(Lint) * total);

        for (j = 0; j < batch_size; j++) {
            for (i = 0; i < new_size; i++) {
                for (size_t s = 0; s < numShares; s++) {
                    memcpy(a0[s] + i + j * new_size_ceil, a[s] + i + j * size, sizeof(Lint));
                    memcpy(a0_index[s] + i + j * new_size_ceil, a_index[s] + i + j * size, sizeof(Lint));
                    memcpy(a1[s] + i + j * new_size_ceil, a[s] + new_size + i + j * size, sizeof(Lint));
                    memcpy(a1_index[s] + i + j * new_size_ceil, a_index[s] + new_size + i + j * size, sizeof(Lint));
                }
                // copying the first half of a into a0
                // memcpy(a0[0] + i + j * new_size_ceil, a[0] + i + j * size, sizeof(Lint));
                // memcpy(a0[1] + i + j * new_size_ceil, a[1] + i + j * size, sizeof(Lint));
                // memcpy(a0_index[0] + i + j * new_size_ceil, a_index[0] + i + j * size, sizeof(Lint));
                // memcpy(a0_index[1] + i + j * new_size_ceil, a_index[1] + i + j * size, sizeof(Lint));

                // // copying the last half of a into a1
                // memcpy(a1[0] + i + j * new_size_ceil, a[0] + new_size + i + j * size, sizeof(Lint));
                // memcpy(a1[1] + i + j * new_size_ceil, a[1] + new_size + i + j * size, sizeof(Lint));
                // memcpy(a1_index[0] + i + j * new_size_ceil, a_index[0] + new_size + i + j * size, sizeof(Lint));
                // memcpy(a1_index[1] + i + j * new_size_ceil, a_index[1] + new_size + i + j * size, sizeof(Lint));
            }
        }

        // Rss_Open(res_check, a0, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("a0[%i] : %llu\n", i, res_check[i]);
        // }
        // printf("\n");

        // Rss_Open(res_check, a1, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("a1[%i] : %llu\n", i, res_check[i]);
        // }

        for (i = 0; i < total_new_ceil; i++) {
            for (size_t s = 0; s < numShares; s++)
                diff[s][i] = a0[s][i] - a1[s][i];
            // diff[1][i] = a0[1][i] - a1[1][i];
        }

        new_Rss_MSB_mp(c, diff, total_new_ceil, ring_size, nodeNet);

        // Rss_Open(res_check, c, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("c[%i] : %llu\n", i, res_check[i]);
        // }

        for (i = 0; i < total_new_ceil; i++) {
            for (size_t s = 0; s < numShares; s++)
                diff[s][i] = a1[s][i] - a0[s][i];
            // diff[1][i] = a1[1][i] - a0[1][i];
        }

        Rss_Mult_mp(d0, c, diff, total_new_ceil, ring_size, nodeNet);

        for (i = 0; i < total_new_ceil; i++) {
            for (size_t s = 0; s < numShares; s++) {

                ap[s][i] = d0[s][i] + a0[s][i];
                // ap[1][i] = d0[1][i] + a0[1][i];

                diff[s][i] = a1_index[s][i] - a0_index[s][i];
                // diff[1][i] = a1_index[1][i] - a0_index[1][i];
            }
        }

        Rss_Mult_mp(d0, c, diff, total_new_ceil, ring_size, nodeNet);

        for (i = 0; i < total_new_ceil; i++) {
            for (size_t s = 0; s < numShares; s++)
                index[s][i] = d0[s][i] + a0_index[s][i];
            // index[1][i] = d0[1][i] + a0_index[1][i];
        }

        //       printf("\n");
        // Rss_Open(res_check, ap, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("ap[%i] : %llu\n", i, res_check[i]);
        // }

        if ((size & 1)) {

            for (i = 0; i < batch_size; i++) {
                for (size_t s = 0; s < numShares; s++) {
                    ap[s][i * new_size_ceil + new_size] = a[s][i * size + size - 1];
                    // ap[1][i * new_size_ceil + new_size] = a[1][i * size + size - 1];

                    index[s][i * new_size_ceil + new_size] = a_index[s][i * size + size - 1];
                    // index[1][i * new_size_ceil + new_size] = a_index[1][i * size + size - 1];
                }
            }
            new_size += 1;
        }
        //       printf("\n");
        // Rss_Open(res_check, ap, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("ap'[%i] : %llu\n", i, res_check[i]);
        // }

        //       printf("\n");
        // Rss_Open(res_check, index, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("index'[%i] : %llu\n", i, res_check[i]);
        // }

        eda_arg_max_helper_mp(res, res_index, ap, index, ring_size, new_size, batch_size, nodeNet);

        for (i = 0; i < numShares; i++) {
            delete[] a0[i];
            delete[] a0_index[i];
            delete[] a1[i];
            delete[] a1_index[i];
            delete[] ap[i];

            delete[] c[i];
            delete[] diff[i];
            delete[] d0[i];
            delete[] index[i];
        }

        delete[] a0;
        delete[] a1;
        delete[] ap;
        delete[] a0_index;
        delete[] a1_index;
        delete[] c;
        delete[] diff;
        delete[] d0;
        delete[] index;

        delete[] res_check;

    }

    else {

        for (int i = 0; i < batch_size; i++) {
            for (size_t s = 0; s < numShares; s++) {
                res[s][i] = a[s][i];
                // res[1][i] = a[1][i];
                res_index[s][i] = a_index[s][i];
                // res_index[1][i] = a_index[1][i];
            }
        }
    }
}

// returns the index of the maximum a value (res_index), as well as the value itself (res)
void eda_arg_max_time_mp(Lint **res, Lint **res_index, Lint **a, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet, unsigned long &timer) {
    uint numShares = nodeNet->getNumShares();

    if (size > 1) {

        uint i, j;
        uint new_size = size >> 1;           // rounding down
        // uint new_size_original = size >> 1;  // rounding down
        uint new_size_ceil = (size + 1) / 2; // rounding up
        int pid = nodeNet->getID();
        // printf("size : %u\n", size);
        // printf("new_size : %u\n", new_size);

        uint total = size * batch_size;
        // uint total_new = new_size * batch_size;
        uint total_new_ceil = new_size_ceil * batch_size;

        Lint **a0 = new Lint *[numShares];
        Lint **a1 = new Lint *[numShares];
        Lint **ap = new Lint *[numShares];
        Lint **c = new Lint *[numShares];
        Lint **diff = new Lint *[numShares];
        Lint **d0 = new Lint *[numShares];
        Lint **index = new Lint *[numShares];

        Lint *res_check = new Lint[total];

        for (i = 0; i < numShares; i++) {
            a0[i] = new Lint[total];
            a1[i] = new Lint[total];
            ap[i] = new Lint[total];
            c[i] = new Lint[total];
            diff[i] = new Lint[total];
            d0[i] = new Lint[total];
            index[i] = new Lint[total];
        }

        for (j = 0; j < batch_size; j++) {
            for (i = 0; i < new_size; i++) {
                for (size_t s = 0; s < numShares; s++) {

                    // copying the first half of a into a0
                    memcpy(a0[s] + i + j * new_size_ceil, a[s] + i + j * size, sizeof(Lint));
                    // memcpy(a0[1] + i + j * new_size_ceil, a[1] + i + j * size, sizeof(Lint));

                    // copying the last half of a into a1
                    memcpy(a1[s] + i + j * new_size_ceil, a[s] + new_size + i + j * size, sizeof(Lint));
                    // memcpy(a1[1] + i + j * new_size_ceil, a[1] + new_size + i + j * size, sizeof(Lint));
                }
            }
        }

        // checking for unused shares, putting them in a1
        // if((size & 1)) {
        // 	for (i = 0; i < batch_size; i++) {
        //      a1[0][i*new_size_ceil + new_size] = a[0][i*size + size-1];
        //      a1[1][i*new_size_ceil + new_size] = a[1][i*size + size-1];
        // 	}
        //     new_size += 1;
        // }
        Lint *bi = new Lint[numShares];
        memset(bi, 0, sizeof(Lint) * numShares);
        // if (pid == 1) {
        //     bi[0] = 1;
        // } else if (pid == numShares) {
        //     bi[numShares - 1] = 1;
        // }
        uint numParties = nodeNet->getNumParties();
        if (numParties == 3) {
            if (pid == 1) {
                bi[0] = 1; // party 1's share 1
            } else if (pid == 3) {
                bi[1] = 1; // party 3's share 2
            }
        } else if (numParties == 5) {
            if (pid == 1) {
                bi[0] = 1;
            } else if (pid == 4) {
                bi[5] = 1; // party 4's share 6
            } else if (pid == 5) {
                bi[3] = 1; // parthy 5's share 4
            }
        }

        // Lint b1 = 0;
        // Lint b2 = 0;
        // switch (pid) {
        // case 1:
        //     b1 = 1;
        //     b2 = 0;
        //     break;
        // case 2:
        //     b1 = 0;
        //     b2 = 0;
        //     break;
        // case 3:
        //     b1 = 0;
        //     b2 = 1;
        //     break;
        // }

        // Rss_Open(res_check, a0, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("a0[%i] : %llu\n", i, res_check[i]);
        // }
        // printf("\n");

        // Rss_Open(res_check, a1, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("a1[%i] : %llu\n", i, res_check[i]);
        // }

        for (i = 0; i < total_new_ceil; i++) {
            for (size_t s = 0; s < numShares; s++)
                diff[s][i] = a0[s][i] - a1[s][i];
            // diff[1][i] = a0[1][i] - a1[1][i];
        }

        new_Rss_MSB_time_mp(c, diff, total_new_ceil, ring_size, nodeNet, timer);

        // Rss_Open(res_check, diff, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("res_check[%i] : %llu\n", i, res_check[i]);
        // }

        for (i = 0; i < total_new_ceil; i++) {
            for (size_t s = 0; s < numShares; s++)
                diff[s][i] = a1[s][i] - a0[s][i];
            // diff[1][i] = a1[1][i] - a0[1][i];
        }

        Rss_Mult_mp(d0, c, diff, total_new_ceil, ring_size, nodeNet);

        for (i = 0; i < total_new_ceil; i++) {
            for (size_t s = 0; s < numShares; s++)
                ap[s][i] = d0[s][i] + a0[s][i];
            // ap[1][i] = d0[1][i] + a0[1][i];
        }

        // Rss_Open(res_check, c, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("c[%i] : %llu\n", i, res_check[i]);
        // }
        //    // new_size -=1;

        for (j = 0; j < batch_size; j++) {
            // printf("j : %u\n", j);
            for (i = 0; i < new_size; i++) {
                // printf("i : %u\n", i);
                // printf("j*new_size + i : %u\n", j*new_size_ceil + i);
                for (size_t s = 0; s < numShares; s++)
                    index[s][j * new_size_ceil + i] = c[s][j * new_size_ceil + i] * (new_size) + i * bi[s];
                // index[1][j * new_size_ceil + i] = c[1][j * new_size_ceil + i] * (new_size) + i * b2;
            }
            // printf("hi\n");
        }

        // Rss_Open(res_check, index, total_new_ceil, ring_size, nodeNet);

        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("res_check[%i] : %llu\n", i, res_check[i]);
        // }

        //       printf("\n");
        // Rss_Open(res_check, index, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("index'[%i] : %llu\n", i, res_check[i]);
        // }
        if ((size & 1)) {
            for (i = 0; i < batch_size; i++) {
                for (size_t s = 0; s < numShares; s++) {
                    ap[s][i * new_size_ceil + new_size] = a[s][i * size + size - 1];
                    index[s][i * new_size_ceil + new_size] = bi[s] * (size - 1);
                }
                // ap[0][i * new_size_ceil + new_size] = a[0][i * size + size - 1];
                // ap[1][i * new_size_ceil + new_size] = a[1][i * size + size - 1];

                // index[0][i * new_size_ceil + new_size] = b1 * (size - 1);
                // index[1][i * new_size_ceil + new_size] = b2 * (size - 1);
            }
            new_size += 1;
        }

        //       printf("\n");
        // Rss_Open(res_check, index, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("index'[%i] : %llu\n", i, res_check[i]);
        // }

        eda_arg_max_helper_time_mp(res, res_index, ap, index, ring_size, new_size, batch_size, nodeNet, timer);

        for (i = 0; i < numShares; i++) {
            delete[] a0[i];
            delete[] a1[i];
            delete[] ap[i];
            delete[] c[i];
            delete[] diff[i];
            delete[] d0[i];
            delete[] index[i];
        }

        delete[] a0;
        delete[] a1;
        delete[] ap;
        delete[] c;
        delete[] diff;
        delete[] d0;
        delete[] index;
        delete[] res_check;
        delete[] bi;

    } else {

        for (int i = 0; i < batch_size; i++) {
            for (size_t s = 0; s < numShares; s++) {
                res[s][i] = a[s][i];
                // res[1][i] = a[1][i];
                res_index[s][i] = 0;
                // res_index[1][i] = 0;
            }
        }
    }
}

void eda_arg_max_helper_time_mp(Lint **res, Lint **res_index, Lint **a, Lint **a_index, uint ring_size, uint size, uint batch_size, NodeNetwork *nodeNet, unsigned long &timer) {
    uint numShares = nodeNet->getNumShares();

    if (size > 1) {

        Lint i, j;
        uint new_size = size >> 1;           // rounding down
        uint new_size_ceil = (size + 1) / 2; // rounding up
        // int pid = nodeNet->getID();

        uint total = size * batch_size;
        // uint total_new = new_size * batch_size;
        uint total_new_ceil = new_size_ceil * batch_size;

        // printf("\nnew_size : %u\n", new_size);
        // printf("new_size_ceil : %u\n", new_size_ceil);
        // printf("size : %u\n", size);

        Lint **a0 = new Lint *[numShares];
        Lint **a1 = new Lint *[numShares];
        Lint **ap = new Lint *[numShares];
        Lint **a0_index = new Lint *[numShares];
        Lint **a1_index = new Lint *[numShares];
        Lint **c = new Lint *[numShares];
        Lint **diff = new Lint *[numShares];
        Lint **d0 = new Lint *[numShares];
        Lint **index = new Lint *[numShares];

        for (i = 0; i < numShares; i++) {
            a0[i] = new Lint[total];
            a0_index[i] = new Lint[total];
            a1[i] = new Lint[total];
            a1_index[i] = new Lint[total];
            ap[i] = new Lint[total];
            c[i] = new Lint[total];
            diff[i] = new Lint[total];
            d0[i] = new Lint[total];
            index[i] = new Lint[total];
        }

        Lint *res_check = new Lint[total];
        // memset(res_check, 0, sizeof(Lint) * total);

        for (j = 0; j < batch_size; j++) {
            for (i = 0; i < new_size; i++) {

                for (size_t s = 0; s < numShares; s++) {
                    // copying the first half of a into a0
                    memcpy(a0[s] + i + j * new_size_ceil, a[s] + i + j * size, sizeof(Lint));
                    // memcpy(a0[1] + i + j * new_size_ceil, a[1] + i + j * size, sizeof(Lint));
                    memcpy(a0_index[s] + i + j * new_size_ceil, a_index[s] + i + j * size, sizeof(Lint));
                    // memcpy(a0_index[1] + i + j * new_size_ceil, a_index[1] + i + j * size, sizeof(Lint));

                    // copying the last half of a into a1
                    memcpy(a1[s] + i + j * new_size_ceil, a[s] + new_size + i + j * size, sizeof(Lint));
                    // memcpy(a1[1] + i + j * new_size_ceil, a[1] + new_size + i + j * size, sizeof(Lint));
                    memcpy(a1_index[s] + i + j * new_size_ceil, a_index[s] + new_size + i + j * size, sizeof(Lint));
                    // memcpy(a1_index[1] + i + j * new_size_ceil, a_index[1] + new_size + i + j * size, sizeof(Lint));
                }
            }
        }

        // Rss_Open(res_check, a0, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("a0[%i] : %llu\n", i, res_check[i]);
        // }
        // printf("\n");

        // Rss_Open(res_check, a1, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("a1[%i] : %llu\n", i, res_check[i]);
        // }

        for (i = 0; i < total_new_ceil; i++) {
            for (size_t s = 0; s < numShares; s++)
                diff[s][i] = a0[s][i] - a1[s][i];
            // diff[1][i] = a0[1][i] - a1[1][i];
        }

        new_Rss_MSB_time_mp(c, diff, total_new_ceil, ring_size, nodeNet, timer);

        // Rss_Open(res_check, c, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("c[%i] : %llu\n", i, res_check[i]);
        // }

        for (i = 0; i < total_new_ceil; i++) {
            for (size_t s = 0; s < numShares; s++)
                diff[s][i] = a1[s][i] - a0[s][i];
            // diff[1][i] = a1[1][i] - a0[1][i];
        }

        Rss_Mult_mp(d0, c, diff, total_new_ceil, ring_size, nodeNet);

        for (i = 0; i < total_new_ceil; i++) {
            for (size_t s = 0; s < numShares; s++) {

                ap[s][i] = d0[s][i] + a0[s][i];
                // ap[1][i] = d0[1][i] + a0[1][i];

                diff[s][i] = a1_index[s][i] - a0_index[s][i];
                // diff[1][i] = a1_index[1][i] - a0_index[1][i];
            }
        }
        Rss_Mult_mp(d0, c, diff, total_new_ceil, ring_size, nodeNet);

        for (i = 0; i < total_new_ceil; i++) {
            for (size_t s = 0; s < numShares; s++)
                index[s][i] = d0[s][i] + a0_index[s][i];
            // index[1][i] = d0[1][i] + a0_index[1][i];
        }

        //       printf("\n");
        // Rss_Open(res_check, ap, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("ap[%i] : %llu\n", i, res_check[i]);
        // }

        if ((size & 1)) {

            for (i = 0; i < batch_size; i++) {
                for (size_t s = 0; s < numShares; s++) {
                    ap[s][i * new_size_ceil + new_size] = a[s][i * size + size - 1];
                    // ap[1][i * new_size_ceil + new_size] = a[1][i * size + size - 1];

                    index[s][i * new_size_ceil + new_size] = a_index[s][i * size + size - 1];
                    // index[1][i * new_size_ceil + new_size] = a_index[1][i * size + size - 1];
                }
            }
            new_size += 1;
        }
        //       printf("\n");
        // Rss_Open(res_check, ap, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("ap'[%i] : %llu\n", i, res_check[i]);
        // }

        //       printf("\n");
        // Rss_Open(res_check, index, total_new_ceil, ring_size, nodeNet);
        // for (i = 0; i < total_new_ceil; i++) {
        // 	printf("index'[%i] : %llu\n", i, res_check[i]);
        // }

        eda_arg_max_helper_time_mp(res, res_index, ap, index, ring_size, new_size, batch_size, nodeNet, timer);

        for (i = 0; i < numShares; i++) {
            delete[] a0[i];
            delete[] a0_index[i];
            delete[] a1[i];
            delete[] a1_index[i];
            delete[] ap[i];

            delete[] c[i];
            delete[] diff[i];
            delete[] d0[i];
            delete[] index[i];
        }

        delete[] a0;
        delete[] a1;
        delete[] ap;
        delete[] a0_index;
        delete[] a1_index;
        delete[] c;
        delete[] diff;
        delete[] d0;
        delete[] index;

        delete[] res_check;

    }

    else {

        for (int i = 0; i < batch_size; i++) {
            for (size_t s = 0; s < numShares; s++) {
                res[s][i] = a[s][i];
                // res[1][i] = a[1][i];
                res_index[s][i] = a_index[s][i];
                // res_index[1][i] = a_index[1][i];
            }
        }
    }
}
