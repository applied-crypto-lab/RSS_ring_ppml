#include "../include/benchmark_5pc.h"

void benchmark_5pc(NodeNetwork *nodeNet, NodeConfiguration *nodeConfig, char *protocol, uint size, uint batch_size, uint num_iterations) {

    int pid = nodeConfig->getID();
    uint numShares = nodeNet->getNumShares();
    uint totalNumShares = nodeNet->getTotalNumShares();
    // uint threshold = nodeNet->getThreshold();

    int ring_size = nodeNet->RING;

    printf("hello, I am %d\n", pid);

    struct timeval start;
    struct timeval end;
    unsigned long timer = 0;

    __m128i *key_prg;
    uint8_t key_raw[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    key_prg = offline_prg_keyschedule(key_raw);
    // setup prg seed(k1, k2, k3)
    uint8_t k1[] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
    uint8_t k2[] = {0xa2, 0x34, 0x6f, 0x67, 0x10, 0x1b, 0x13, 0xa3, 0x56, 0x45, 0x90, 0xb2, 0x13, 0xe3, 0x23, 0x24};

    Lint **a = new Lint *[numShares];
    Lint **b = new Lint *[numShares];

    Lint **c = new Lint *[numShares];
    Lint **d = new Lint *[numShares];

    for (int i = 0; i < numShares; i++) {
        c[i] = new Lint[size];
        memset(c[i], 0, sizeof(Lint) * size);
        d[i] = new Lint[size];
        memset(d[i], 0, sizeof(Lint) * size);
    }

    Lint **Data1 = new Lint *[totalNumShares];
    Lint **Data2 = new Lint *[totalNumShares];
    for (int i = 0; i < totalNumShares; i++) {
        Data1[i] = new Lint[size];
        memset(Data1[i], 0, sizeof(Lint) * size);
        Data2[i] = new Lint[size];
        memset(Data2[i], 0, sizeof(Lint) * size);
    }

    for (int i = 0; i < size; i++) {
        for (size_t j = 0; j < totalNumShares - 1; j++) {
            prg_aes_ni(Data1[j] + i, k1, key_prg);
            prg_aes_ni(Data2[j] + i, k2, key_prg);
        }

        Data1[totalNumShares - 1][i] = i;
        Data2[totalNumShares - 1][i] = i;

        for (size_t j = 0; j < totalNumShares - 1; j++) {
            Data1[totalNumShares - 1][i] -= Data1[j][i];
            Data2[totalNumShares - 1][i] -= Data2[j][i];
        }
    }

    free(key_prg);
    // indices for {s_1, s_2, s_3, s_4, s_5, s_6}
    uint share_indeces[5][6] = {
        {4, 5, 6, 7, 8, 9}, // p1
        {7, 8, 1, 9, 2, 3}, // p2
        {9, 2, 5, 3, 6, 0}, // p3
        {3, 6, 8, 0, 1, 4}, // p4
        {0, 1, 2, 4, 5, 7}  // p5
    };

    for (size_t i = 0; i < numShares; i++) {
        a[i] = Data1[share_indeces[pid - 1][i]];
        b[i] = Data2[share_indeces[pid - 1][i]];
    }
    if (!strcmp(protocol, "mult")) {

        gettimeofday(&start, NULL); // start timer here
        for (size_t j = 0; j < num_iterations; j++) {
            Rss_Mult_mp(c, a, b, size, ring_size, nodeNet);
        }
        gettimeofday(&end, NULL); // stop timer here
        timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        printf("[%s_5pc] [%u, %i, %u, %u] [%.6lf ms,  %.6lf ms/size,  %lu bytes] \n", protocol, ring_size, size, batch_size, num_iterations, (double)(timer * 0.001) / num_iterations, (double)(timer * 0.001 / size) / num_iterations, nodeNet->getCommunicationInBytes() / num_iterations);

    } else if (!strcmp(protocol, "randbit")) {

        gettimeofday(&start, NULL); // start timer here
        for (size_t j = 0; j < num_iterations; j++) {
            Rss_RandBit_mp(c, size, ring_size, nodeNet);
        }
        gettimeofday(&end, NULL); // stop timer here
        timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        printf("[%s_5pc] [%u, %i, %u, %u] [%.6lf ms,  %.6lf ms/size,  %lu bytes] \n", protocol, ring_size, size, batch_size, num_iterations, (double)(timer * 0.001) / num_iterations, (double)(timer * 0.001 / size) / num_iterations, nodeNet->getCommunicationInBytes() / num_iterations);

    } else if (!strcmp(protocol, "edabit")) {

        gettimeofday(&start, NULL); // start timer here
        for (size_t j = 0; j < num_iterations; j++) {
            Rss_edaBit_mp(c, d, size, ring_size, nodeNet);
        }
        gettimeofday(&end, NULL); // stop timer here
        timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        printf("[%s_5pc] [%u, %i, %u, %u] [%.6lf ms,  %.6lf ms/size,  %lu bytes] \n", protocol, ring_size, size, batch_size, num_iterations, (double)(timer * 0.001) / num_iterations, (double)(timer * 0.001 / size) / num_iterations, nodeNet->getCommunicationInBytes() / num_iterations);

    } else if (!strcmp(protocol, "msb_rb")) {

        gettimeofday(&start, NULL); // start timer here
        for (size_t j = 0; j < num_iterations; j++) {
            Rss_MSB_mp(c, a, size, ring_size, nodeNet);
        }
        gettimeofday(&end, NULL); // stop timer here
        timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        printf("[%s_5pc] [%u, %i, %u, %u] [%.6lf ms,  %.6lf ms/size,  %lu bytes] \n", protocol, ring_size, size, batch_size, num_iterations, (double)(timer * 0.001) / num_iterations, (double)(timer * 0.001 / size) / num_iterations, nodeNet->getCommunicationInBytes() / num_iterations);

    } else if (!strcmp(protocol, "msb_eda")) {

        gettimeofday(&start, NULL); // start timer here
        for (size_t j = 0; j < num_iterations; j++) {
            new_Rss_MSB_mp(c, a, size, ring_size, nodeNet);
        }
        gettimeofday(&end, NULL); // stop timer here
        timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        printf("[%s_5pc] [%u, %i, %u, %u] [%.6lf ms,  %.6lf ms/size,  %lu bytes] \n", protocol, ring_size, size, batch_size, num_iterations, (double)(timer * 0.001) / num_iterations, (double)(timer * 0.001 / size) / num_iterations, nodeNet->getCommunicationInBytes() / num_iterations);

    } else if (!strcmp(protocol, "mat_mult")) {

        // gettimeofday(&start, NULL); // start timer here
        // for (size_t j = 0; j < num_iterations; j++) {
        //     Rss_MatMultArray_batch_mp(c, a, b, int(sqrt(size)), int(sqrt(size)), int(sqrt(size)), ring_size, 1, 1, 1, nodeNet);
        // }
        // gettimeofday(&end, NULL); // stop timer here
        // timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        // printf("[%s_5pc] [%u, %i x %i, %u, %u] [%.6lf ms,  %.6lf ms/size,  %lu bytes] \n", protocol, ring_size, int(sqrt(size)), int(sqrt(size)), batch_size, num_iterations, (double)(timer * 0.001) / num_iterations, (double)(timer * 0.001 / size) / num_iterations, nodeNet->getCommunicationInBytes() / num_iterations);
        timer = 0;
        for (size_t j = 0; j < num_iterations; j++) {
            gettimeofday(&start, NULL); // start timer here
            Rss_MatMultArray_batch_mp(c, a, b, int(sqrt(size)), int(sqrt(size)), int(sqrt(size)), ring_size, 1, 1, 1, nodeNet);
            gettimeofday(&end, NULL); // stop timer here

            unsigned long long temp_time = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

            // printf("[%u] [%.3lf ms]\n", j, (double)(temp_time * 0.001));

            timer += 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        }
        printf("[%s_5pc] [%u, %i x %i, %u, %u] [%.6lf ms,  %.6lf ms/size,  %lu bytes] \n", protocol, ring_size, int(sqrt(size)), int(sqrt(size)), batch_size, num_iterations, (double)(timer * 0.001) / num_iterations, (double)(timer * 0.001 / size) / num_iterations, nodeNet->getCommunicationInBytes() / num_iterations);


    }else{

        printf("Invalid protocol name\n");
        printf("Protocols: mult, randbit, edabit, msb_rb, msb_eda, mat_mult\n");
    }

    // for (size_t i = 0; i < sizes.size(); i++) {

    //     exp_ctr = 0;

    //     total = 100;
    //     gettimeofday(&start, NULL); // start timer here
    //     for (size_t j = 0; j < total; j++) {
    //         Rss_Mult_mp(c, a, b, sizes.at(i), ring_size, nodeNet);
    //     }
    //     gettimeofday(&end, NULL); // stop timer here
    //     timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

    //     exp_times.at(exp_ctr).push_back((double)(timer * 0.001) / total);
    //     exp_ratios.at(exp_ctr).push_back((double)(timer * 0.001 / sizes.at(i)) / total);

    //     exp_ctr++;

    //     total = 100;
    //     gettimeofday(&start, NULL); // start timer here
    //     for (size_t j = 0; j < total; j++) {
    //         Rss_RandBit_mp(c, sizes.at(i), ring_size, nodeNet);
    //     }
    //     gettimeofday(&end, NULL); // stop timer here
    //     timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

    //     exp_times.at(exp_ctr).push_back((double)(timer * 0.001) / total);
    //     exp_ratios.at(exp_ctr).push_back((double)(timer * 0.001 / sizes.at(i)) / total);

    //     exp_ctr++;

    //     total = 100;
    //     gettimeofday(&start, NULL); // start timer here
    //     for (size_t j = 0; j < total; j++) {
    //         Rss_edaBit_mp(c, d, sizes.at(i), ring_size, nodeNet);
    //     }
    //     gettimeofday(&end, NULL); // stop timer here
    //     timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

    //     exp_times.at(exp_ctr).push_back((double)(timer * 0.001) / total);
    //     exp_ratios.at(exp_ctr).push_back((double)(timer * 0.001 / sizes.at(i)) / total);

    //     exp_ctr++;

    //     total = 100;
    //     gettimeofday(&start, NULL); // start timer here
    //     for (size_t j = 0; j < total; j++) {
    //         Rss_MSB_mp(c, a, sizes.at(i), ring_size, nodeNet);
    //     }
    //     gettimeofday(&end, NULL); // stop timer here
    //     timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

    //     exp_times.at(exp_ctr).push_back((double)(timer * 0.001) / total);
    //     exp_ratios.at(exp_ctr).push_back((double)(timer * 0.001 / sizes.at(i)) / total);

    //     exp_ctr++;

    //     total = 100;
    //     gettimeofday(&start, NULL); // start timer here
    //     for (size_t j = 0; j < total; j++) {
    //         new_Rss_MSB_mp(c, a, sizes.at(i), ring_size, nodeNet);
    //     }
    //     gettimeofday(&end, NULL); // stop timer here
    //     timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

    //     exp_times.at(exp_ctr).push_back((double)(timer * 0.001) / total);
    //     exp_ratios.at(exp_ctr).push_back((double)(timer * 0.001 / sizes.at(i)) / total);

    //     exp_ctr++;
    // }

    // for (size_t i = 0; i < matrix_sizes.size(); i++) {

    //     total = 100;
    //     gettimeofday(&start, NULL); // start timer here
    //     for (size_t j = 0; j < total; j++) {
    //         Rss_MatMultArray_batch_mp(c, a, b, matrix_sizes.at(i), matrix_sizes.at(i), matrix_sizes.at(i), ring_size, 1, 1, 1, nodeNet);
    //     }
    //     gettimeofday(&end, NULL); // stop timer here
    //     timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

    //     exp_times.at(exp_ctr).push_back((double)(timer * 0.001) / total);
    //     exp_ratios.at(exp_ctr).push_back((double)(timer * 0.001 / matrix_sizes.at(i)) / total);
    // }

    // write_benchmark(out_fname, exp_names, exp_times, exp_ratios, ring_size);

    for (int i = 0; i < totalNumShares; i++) {

        delete[] Data1[i];
        delete[] Data2[i];
    }

    delete[] Data1;
    delete[] Data2;

    delete[] a;
    delete[] b;

    for (size_t i = 0; i < numShares; i++) {
        delete[] c[i];
        delete[] d[i];
    }

    delete[] c;
    delete[] d;
}
