#include "benchmark_7pc.h"

void benchmark_7pc(NodeNetwork *nodeNet, NodeConfiguration *nodeConfig, char *protocol, uint size, uint batch_size, uint num_iterations) {

    int pid = nodeConfig->getID();
    uint numShares = nodeNet->getNumShares();
    uint totalNumShares = nodeNet->getTotalNumShares();
    int ring_size = nodeNet->RING;
    printf("hello, I am %d\n", pid);

    if ((ring_size == 30 and sizeof(Lint) * 8 == 32) or (ring_size == 60 and sizeof(Lint) * 8 == 64)) {
        /* code */
    }

    struct timeval start;
    struct timeval end;
    unsigned long timer;

    __m128i *key_prg;
    uint8_t key_raw[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    key_prg = offline_prg_keyschedule(key_raw);
    // setup prg seed(k1, k2, k3)
    uint8_t k1[] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
    uint8_t k2[] = {0xa2, 0x34, 0x6f, 0x67, 0x10, 0x1b, 0x13, 0xa3, 0x56, 0x45, 0x90, 0xb2, 0x13, 0xe3, 0x23, 0x24};

    printf("init start\n");

    Lint *res = new Lint[size];
    memset(res, 0, sizeof(Lint) * size);
    Lint *res_check = new Lint[size];
    memset(res_check, 0, sizeof(Lint) * size);
    printf("init res\n");

    Lint **a = new Lint *[numShares];
    Lint **b = new Lint *[numShares];
    Lint **c = new Lint *[numShares];
    Lint **d = new Lint *[numShares];

    for (int i = 0; i < numShares; i++) {
        a[i] = new Lint[size];
        memset(a[i], 0, sizeof(Lint) * size);
        b[i] = new Lint[size];
        memset(b[i], 0, sizeof(Lint) * size);

        c[i] = new Lint[size];
        memset(c[i], 0, sizeof(Lint) * size);
        d[i] = new Lint[size];
        memset(d[i], 0, sizeof(Lint) * size);
    }
    printf("init a,b,c,d\n");

    // Lint **Data1;
    // Lint **Data2;

    // Data1 = new Lint *[totalNumShares];
    // Data2 = new Lint *[totalNumShares];
    // for (int i = 0; i < totalNumShares; i++) {
    //     Data1[i] = new Lint[size];
    //     memset(Data1[i], 0, sizeof(Lint) * size);
    //     Data2[i] = new Lint[size];
    //     memset(Data2[i], 0, sizeof(Lint) * size);
    // }

    // for (int i = 0; i < size; i++) {
    //     for (size_t j = 0; j < totalNumShares - 1; j++) {
    //         prg_aes_ni(Data1[j] + i, k1, key_prg);
    //         prg_aes_ni(Data2[j] + i, k2, key_prg);
    //     }

    //     Data1[totalNumShares - 1][i] = i;
    //     Data2[totalNumShares - 1][i] = i;

    //     for (size_t j = 0; j < totalNumShares - 1; j++) {
    //         Data1[totalNumShares - 1][i] -= Data1[j][i];
    //         Data2[totalNumShares - 1][i] -= Data2[j][i];
    //     }
    // }
    printf("init data\n");

    free(key_prg);
    // indices for {s_1, s_2, s_3, s_4, s_5, s_6}
    uint share_indeces[7][20] = {
        {15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34},
        {25, 26, 27, 5, 28, 29, 6, 30, 7, 8, 31, 32, 9, 33, 10, 11, 34, 12, 13, 14},
        {31, 32, 9, 19, 33, 10, 20, 11, 21, 1, 34, 12, 22, 13, 23, 2, 14, 24, 3, 4},
        {34, 12, 22, 28, 13, 23, 29, 2, 6, 16, 14, 24, 30, 3, 7, 17, 4, 8, 18, 0},
        {14, 24, 30, 33, 3, 7, 10, 17, 20, 26, 4, 8, 11, 18, 21, 27, 0, 1, 5, 15},
        {4, 8, 11, 13, 18, 21, 23, 27, 29, 32, 0, 1, 2, 5, 6, 9, 15, 16, 19, 25},
        {0, 1, 2, 3, 5, 6, 7, 9, 10, 12, 15, 16, 17, 19, 20, 22, 25, 26, 28, 31}};
    // uint share_indeces_p2[6] = {4,5,6,7,8,9}
    // for (size_t i = 0; i < numShares; i++) {
    //     a[i] = Data1[share_indeces[pid - 1][i]];
    //     b[i] = Data2[share_indeces[pid - 1][i]];
    // }
    printf("setting a,b\n");

    printf("Starting computation of protocol %s...\n", protocol);
    if (!strcmp(protocol, "mult")) {
        timer = 0;

        // gettimeofday(&start, NULL); // start timer here
        for (size_t j = 0; j < num_iterations; j++) {

            gettimeofday(&start, NULL); // start timer here
            Rss_Mult_7pc(c, a, b, size, ring_size, nodeNet);
            // Rss_MatMultArray_batch_7pc(c, a, b, int(sqrt(size)), int(sqrt(size)), int(sqrt(size)), ring_size, 1, 1, 1, nodeNet);
            gettimeofday(&end, NULL); // stop timer here

            unsigned long long temp_time = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

            printf("[%u] [%.3lf ms]\n", j, (double)(temp_time * 0.001));

            timer += 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;


        }
        // gettimeofday(&end, NULL); // stop timer here
        timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        printf("[%s_7pc] [%u, %i, %u, %u] [%.6lf ms,  %.6lf ms/size,  %lu bytes] \n", protocol, ring_size, size, batch_size, num_iterations, (double)(timer * 0.001) / num_iterations, (double)(timer * 0.001 / size) / num_iterations, nodeNet->getCommunicationInBytes() / num_iterations);
    } else if (!strcmp(protocol, "mulpub")) {

        gettimeofday(&start, NULL); // start timer here
        for (size_t j = 0; j < num_iterations; j++) {
            printf("MPUB START\n");
            Rss_MultPub_7pc(res, a, b, size, ring_size, nodeNet);
            printf("MPUB END\n");
        }
        gettimeofday(&end, NULL); // stop timer here
        timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        printf("[%s_7pc] [%u, %i, %u, %u] [%.6lf ms,  %.6lf ms/size,  %lu bytes] \n", protocol, ring_size, size, batch_size, num_iterations, (double)(timer * 0.001) / num_iterations, (double)(timer * 0.001 / size) / num_iterations, nodeNet->getCommunicationInBytes() / num_iterations);
    }

    else if (!strcmp(protocol, "randbit")) {

        gettimeofday(&start, NULL); // start timer here
        for (size_t j = 0; j < num_iterations; j++) {
            Rss_RandBit_7pc(c, size, ring_size, nodeNet);
        }
        gettimeofday(&end, NULL); // stop timer here
        timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        printf("[%s_7pc] [%u, %i, %u, %u] [%.6lf ms,  %.6lf ms/size,  %lu bytes] \n", protocol, ring_size, size, batch_size, num_iterations, (double)(timer * 0.001) / num_iterations, (double)(timer * 0.001 / size) / num_iterations, nodeNet->getCommunicationInBytes() / num_iterations);
    }

    else if (!strcmp(protocol, "edabit")) {

        gettimeofday(&start, NULL); // start timer here
        for (size_t j = 0; j < num_iterations; j++) {
            Rss_edaBit_7pc(c, d, size, ring_size, nodeNet);
        }
        gettimeofday(&end, NULL); // stop timer here
        timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        printf("[%s_7pc] [%u, %i, %u, %u] [%.6lf ms,  %.6lf ms/size,  %lu bytes] \n", protocol, ring_size, size, batch_size, num_iterations, (double)(timer * 0.001) / num_iterations, (double)(timer * 0.001 / size) / num_iterations, nodeNet->getCommunicationInBytes() / num_iterations);
    } else if (!strcmp(protocol, "msb_rb")) {

        gettimeofday(&start, NULL); // start timer here
        for (size_t j = 0; j < num_iterations; j++) {
            Rss_MSB_7pc(c, a, size, ring_size, nodeNet);
        }
        gettimeofday(&end, NULL); // stop timer here
        timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        printf("[%s_7pc] [%u, %i, %u, %u] [%.6lf ms,  %.6lf ms/size,  %lu bytes] \n", protocol, ring_size, size, batch_size, num_iterations, (double)(timer * 0.001) / num_iterations, (double)(timer * 0.001 / size) / num_iterations, nodeNet->getCommunicationInBytes() / num_iterations);
    } else if (!strcmp(protocol, "msb_eda")) {

        gettimeofday(&start, NULL); // start timer here
        for (size_t j = 0; j < num_iterations; j++) {
            new_Rss_MSB_7pc(c, a, size, ring_size, nodeNet);
        }
        gettimeofday(&end, NULL); // stop timer here
        timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        printf("[%s_7pc] [%u, %i, %u, %u] [%.6lf ms,  %.6lf ms/size,  %lu bytes] \n", protocol, ring_size, size, batch_size, num_iterations, (double)(timer * 0.001) / num_iterations, (double)(timer * 0.001 / size) / num_iterations, nodeNet->getCommunicationInBytes() / num_iterations);
    }

    else if (!strcmp(protocol, "mat_mult")) {
        timer = 0;
        for (size_t j = 0; j < num_iterations; j++) {
            gettimeofday(&start, NULL); // start timer here
            Rss_MatMultArray_batch_7pc(c, a, b, int(sqrt(size)), int(sqrt(size)), int(sqrt(size)), ring_size, 1, 1, 1, nodeNet);
            gettimeofday(&end, NULL); // stop timer here

            unsigned long long temp_time = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

            // printf("[%u] [%.3lf ms]\n", j, (double)(temp_time * 0.001));

            timer += 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        }
        printf("[%s_7pc] [%u, %i x %i, %u, %u] [%.6lf ms,  %.6lf ms/size,  %lu bytes] \n", protocol, ring_size, int(sqrt(size)), int(sqrt(size)), batch_size, num_iterations, (double)(timer * 0.001) / num_iterations, (double)(timer * 0.001 / size) / num_iterations, nodeNet->getCommunicationInBytes() / num_iterations);
    } else {

        printf("Invalid protocol name\n");
        printf("Protocols: mult, randbit, edabit, msb_rb, msb_eda, mat_mult\n");
    }
    // printf("before final benchmark delete\n");

    delete[] res;
    delete[] res_check;
    // for (int i = 0; i < totalNumShares; i++) {
    //     delete[] Data1[i];
    //     delete[] Data2[i];
    // }

    // delete[] Data1;
    // delete[] Data2;

    for (int i = 0; i < numShares; i++) {
        delete[] a[i];
        delete[] b[i];
        delete[] c[i];
        delete[] d[i];
    }
    delete[] a;
    delete[] b;
    delete[] c;
    delete[] d;
}