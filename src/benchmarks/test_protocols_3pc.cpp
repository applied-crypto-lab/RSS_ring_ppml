#include "test_protocols_3pc.h"

bool checkbytes(Lint a, Lint b, int bytes) {

    Lint temp_a = 0;
    Lint temp_b = 0;
    memcpy(&temp_a, &a, bytes);
    memcpy(&temp_b, &b, bytes);
    if (temp_a == temp_b) {
        return true;
    }
    return false;
}

bool checkbytes_byte(uint8_t a, uint8_t b, int bytes) {

    uint8_t temp_a = 0;
    uint8_t temp_b = 0;
    memcpy(&temp_a, &a, bytes);
    memcpy(&temp_b, &b, bytes);
    if (temp_a == temp_b) {
        return true;
    }
    return false;
}

void test_protocols_3pc(NodeNetwork *nodeNet, NodeConfiguration *nodeConfig, uint size, uint batch_size) {
    // int bits;
    int i;
    int pid = nodeConfig->getID();
    uint numShares = nodeNet->getNumShares();
    int flag = 0;

    int total;
    int ring_size = nodeNet->RING;

    printf("ring_size = %i\n", ring_size);
    printf("8*sizeof(Lint) = %lu\n", 8 * sizeof(Lint));
    printf("sizeof(Lint) = %lu\n", sizeof(Lint));
    printf("batch_size = %i\n", batch_size);
    printf("numShares : %i \n", numShares);

    // checking if we can access this built-in function
    // uint aaa = _pext_u32((unsigned) ring_size, 0x55555555);

    int bytes = (ring_size + 2 + 8 - 1) / 8;

    struct timeval start;
    struct timeval end;
    unsigned long timer;

    printf("hello, I am %d\n", pid);

    int map[2];
    switch (pid) {
    case 1:
        map[0] = 3;
        map[1] = 2;
        break;
    case 2:
        map[0] = 1;
        map[1] = 3;
        break;
    case 3:
        map[0] = 2;
        map[1] = 1;
        break;
    }

    // setup prg key (will be used by all parties, only for data makeup)
    __m128i *key_prg;
    uint8_t key_raw[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    key_prg = offline_prg_keyschedule(key_raw);
    // setup prg seed(k1, k2, k3)
    uint8_t k1[] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
    uint8_t k2[] = {0xa2, 0x34, 0x6f, 0x67, 0x10, 0x1b, 0x13, 0xa3, 0x56, 0x45, 0x90, 0xb2, 0x13, 0xe3, 0x23, 0x24};

    // make up data
    Lint **a = new Lint *[2];
    Lint **b = new Lint *[2];
    Lint **c = new Lint *[2];
    Lint **d = new Lint *[2];
    Lint **e = new Lint *[2];
    Lint **f = new Lint *[2];

    for (int i = 0; i < 2; i++) {
        c[i] = new Lint[size];
        memset(c[i], 0, sizeof(Lint) * size);
        d[i] = new Lint[size];
        memset(d[i], 0, sizeof(Lint) * size);
        e[i] = new Lint[size];
        memset(e[i], 0, sizeof(Lint) * size);
        f[i] = new Lint[size];
        memset(f[i], 0, sizeof(Lint) * size);
    }

    Lint **Data1;
    Lint **Data2;

    Data1 = new Lint *[3];
    for (int i = 0; i < 3; i++) {
        Data1[i] = new Lint[size];
        memset(Data1[i], 0, sizeof(Lint) * size);
    }

    Data2 = new Lint *[3];
    for (int i = 0; i < 3; i++) {
        Data2[i] = new Lint[size];
        memset(Data2[i], 0, sizeof(Lint) * size);
    }

    for (int i = 0; i < size; i++) {
        prg_aes_ni(Data1[0] + i, k1, key_prg);
        prg_aes_ni(Data1[1] + i, k1, key_prg);
        prg_aes_ni(Data2[0] + i, k2, key_prg);
        prg_aes_ni(Data2[1] + i, k2, key_prg);
        Data1[2][i] = i - Data1[0][i] - Data1[1][i];
        // Data1[2][i] = (i + 510) - Data1[0][i] - Data1[1][i];
        // Data1[2][i] = (i + (1 << 7)) - Data1[0][i] - Data1[1][i];

        Data2[2][i] = i - Data2[0][i] - Data2[1][i];
        // printf("Data[2][%i] : %i\n", i, Data2[2][i]);
    }
    // printf("Data1[0][%i] : %llu\n", 0, Data1[0][0] );
    // printf("Data1[1][%i] : %llu\n", 0, Data1[1][0] );
    free(key_prg);

    Lint *res = new Lint[size];
    memset(res, 0, sizeof(Lint) * size);
    Lint *res2 = new Lint[size];
    memset(res2, 0, sizeof(Lint) * size);
    Lint *res3 = new Lint[size];
    memset(res3, 0, sizeof(Lint) * size);

    Lint *res_check = new Lint[size];
    memset(res_check, 0, sizeof(Lint) * size);
    // assign data
    switch (pid) {
    case 1:
        a[0] = Data1[1];
        a[1] = Data1[2];
        b[0] = Data2[1];
        b[1] = Data2[2];
        break;
    case 2:
        a[0] = Data1[2];
        a[1] = Data1[0];
        b[0] = Data2[2];
        b[1] = Data2[0];
        break;
    case 3:
        a[0] = Data1[0];
        a[1] = Data1[1];
        b[0] = Data2[0];
        b[1] = Data2[1];
        break;
    }

    // unsigned long eda_timer = 0;
    // eda_arg_max_time(c, a, b, ring_size, 1, batch_size, map, nodeNet, eda_timer);

    total = 0;
    printf("\n");
    Rss_EQZ(c, a, size, ring_size, nodeNet);
    Rss_Open(res, c, size, ring_size, nodeNet);

    for (int i = 0; i < size; i++) {

        res_check[i] = Data1[0][i] + Data1[1][i] + Data1[2][i];
        bool check = (res_check[i] == 0);
        // Getting MSB of a -)) {
        if (!(check == res[i] )) {
            printf("EQZ ERROR at %d \n", i);
            printf("res[%i]      : %lu\n", i, res[i]);
            printf("expected[%i] : %u\n", i, check);
            flag = 1;
        }
    }
    if (flag == 0) {
        printf(ANSI_COLOR_GREEN "EQZ Correct!\n" ANSI_COLOR_RESET);
    } else {
        printf(ANSI_COLOR_RED "EQZ error!\n" ANSI_COLOR_RESET);
    }

    // ReLU_6_alpha_timer(c, a, b, 1001, ring_size, 0, nodeNet, timer);

    // // computation test Mult:
    // if (total != 0) {
    //     for (size_t i = 0; i < 3; i++) {
    //         uint temp_size = pow(10, i);
    //         // cout << "temp_size = " << temp_size << endl;
    //         gettimeofday(&start, NULL); // start timer here
    //         for (j = 0; j < total; j++) {
    //             // gettimeofday(&start,NULL); //start timer here
    //             // printf("j = %u\n",j );
    //             Rss_Mult(c, a, b, temp_size, ring_size, map, nodeNet);
    //             // Rss_Mult_fixed_b(c, a, b, 2, size, ring_size, map, nodeNet);
    //         }
    //         gettimeofday(&end, NULL); // stop timer here
    //         timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    //         printf("mult 1e%i = %.6lf ms , ratio %.6lf \n", i, (double)(timer * 0.001) / total, (double)(timer * 0.001 / temp_size) / total);
    //         // printf("Ratio for %d = %.6lf ms\n", temp_size, (double)(timer * 0.001 / temp_size) / total);
    //     }
    // }

    // total = 0;
    // if (total != 0) {
    //     // computation test Mult:
    //     for (size_t i = 0; i < 3; i++) {
    //         uint temp_size = pow(10, i);
    //         // cout << "temp_size = " << temp_size << endl;
    //         gettimeofday(&start, NULL); // start timer here
    //         for (j = 0; j < total; j++) {
    //             Rss_RandBit(c, temp_size, ring_size, map, nodeNet);
    //         }
    //         gettimeofday(&end, NULL); // stop timer here
    //         timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    //         printf("randBit 1e%i = %.6lf ms , ratio %.6lf \n", i, (double)(timer * 0.001) / total, (double)(timer * 0.001 / temp_size) / total);
    //         // printf("Ratio for %d = %.6lf ms\n", temp_size, (double)(timer * 0.001 / temp_size) / total);
    //     }
    // }

    // total = 0;
    // if (total != 0) {
    //     // computation test Mult:
    //     for (size_t i = 0; i < 3; i++) {
    //         uint temp_size = pow(10, i);
    //         // cout << "temp_size = " << temp_size << endl;
    //         gettimeofday(&start, NULL); // start timer here
    //         for (j = 0; j < total; j++) {
    //             Rss_edaBit(c, d, temp_size, ring_size, map, nodeNet);
    //             // Rss_RandBit(c, temp_size, ring_size, map, nodeNet);
    //         }
    //         gettimeofday(&end, NULL); // stop timer here
    //         timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec
    //         printf("edaBit 1e%i = %.6lf ms , ratio %.6lf \n", i, (double)(timer * 0.001) / total, (double)(timer * 0.001 / temp_size) / total);
    //         // printf("Ratio for %d = %.6lf ms\n", temp_size, (double)(timer * 0.001 / temp_size) / total);
    //     }
    // }

    // total = 0;
    // if (total != 0) {
    //     // computation test Mult:
    //     for (size_t i = 0; i < 3; i++) {
    //         uint temp_size = pow(10, i);
    //         // cout << "temp_size = " << temp_size << endl;
    //         gettimeofday(&start, NULL); // start timer here
    //         for (j = 0; j < total; j++) {
    //             Rss_MSB(c, a, temp_size, ring_size, map, nodeNet);
    //             // Rss_RandBit(c, temp_size, ring_size, map, nodeNet);
    //         }
    //         gettimeofday(&end, NULL); // stop timer here
    //         timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    //         printf("msb_rb 1e%i = %.6lf ms , ratio %.6lf \n", i, (double)(timer * 0.001) / total, (double)(timer * 0.001 / temp_size) / total);
    //         // printf("Ratio for %d = %.6lf ms\n", temp_size, (double)(timer * 0.001 / temp_size) / total);
    //     }
    // }

    // total = 0;
    // if (total != 0) {
    //     // computation test Mult:
    //     for (size_t i = 0; i < 3; i++) {
    //         uint temp_size = pow(10, i);
    //         // cout << "temp_size = " << temp_size << endl;
    //         gettimeofday(&start, NULL); // start timer here
    //         for (j = 0; j < total; j++) {
    //             new_Rss_MSB(c, a, temp_size, ring_size, map, nodeNet);
    //             // Rss_RandBit(c, temp_size, ring_size, map, nodeNet);
    //         }
    //         gettimeofday(&end, NULL); // stop timer here
    //         timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    //         printf("msb_eda 1e%i = %.6lf ms , ratio %.6lf \n", i, (double)(timer * 0.001) / total, (double)(timer * 0.001 / temp_size) / total);
    //         // printf("Ratio for %d = %.6lf ms\n", temp_size, (double)(timer * 0.001 / temp_size) / total);
    //     }
    // }

    // uint mat_dims[4] = {100, 10000, 250000, 1000000};

    // total = 1;
    // for (size_t i = 0; i < 4; i++) {
    //     uint temp_size = mat_dims[i];
    //     // cout << "temp_size = " << temp_size << endl;
    //     gettimeofday(&start, NULL); // start timer here
    //     for (j = 0; j < total; j++) {
    //         // gettimeofday(&start,NULL); //start timer here
    //         // printf("j = %u\n",j );
    //         Rss_MatMultArray(c, a, b, sqrt(temp_size), sqrt(temp_size), sqrt(temp_size), ring_size, map, nodeNet);

    //         // Rss_Mult(c, a, b, temp_size, ring_size, map, nodeNet);
    //         // Rss_Mult_fixed_b(c, a, b, 2, size, ring_size, map, nodeNet);
    //     }
    //     gettimeofday(&end, NULL); // stop timer here
    //     timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

    //     printf("matmult %i = %.6lf ms\n", int(sqrt(temp_size)), (double)(timer * 0.001) / total);

    // }

    // computation test open:
    // gettimeofday(&start, NULL); // start timer here
    // Rss_Open(res, a, size,  ring_size, nodeNet);
    // gettimeofday(&end, NULL); // stop timer here

    // timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    // printf("runtime for open with data size %d = %ld us\n", size, timer);

    // // check open
    // for (int i = 0; i < size; i++) {

    //     res_check[i] = Data1[0][i] + Data1[1][i] + Data1[2][i];
    //     res_check[i] = res_check[i] & nodeNet->SHIFT[ring_size];

    //     if (!checkbytes(res[i], res_check[i], bytes) && flag == 0) {
    //         printf("Open ERROR at %d !=   \n", i);
    //         print_1283(res[i]);
    //         print_1283(res_check[i]);
    //         flag = 1;
    //     }
    // }
    // if (flag == 0) {
    //     printf(ANSI_COLOR_GREEN "Open Correct!\n" ANSI_COLOR_RESET);
    // } else {
    //     printf(ANSI_COLOR_RED "Open error!\n" ANSI_COLOR_RESET);
    // }

    // gettimeofday(&start, NULL); // start timer here
    // Rss_Mult(c, a, b, size, ring_size,  nodeNet);
    // gettimeofday(&end, NULL); // stop timer here

    // timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    // printf("runtime for mult with data size %d = %ld us\n", size, timer);

    // Rss_Open(res, c, size, ring_size, nodeNet);

    // // check Mult
    // flag = 0;
    // for (Lint i = 0; i < size; i++) {
    //     // printf("res[%i]  : %llu\n", i, res[i]);

    //     res_check[i] = (i * i) & nodeNet->SHIFT[ring_size];
    //     if (!checkbytes(res[i], res_check[i], bytes) && flag == 0) {
    //         printf("Mult ERROR at %d !=   \n", i);

    //         // printf("expected  : %llu\n", res_check[i]);

    //         print_1283(res[i]);
    //         print_1283(res_check[i]);
    //         flag = 1;
    //     }
    // }
    // if (flag == 0) {
    //     printf(ANSI_COLOR_GREEN "Mult Correct!\n" ANSI_COLOR_RESET);
    // } else {
    //     printf(ANSI_COLOR_RED "Mult error!\n" ANSI_COLOR_RESET);
    // }
    // flag = 0;

    // printf("Testing MultPub...\n");
    // total = 0;

    // gettimeofday(&start, NULL); // start timer here
    // Rss_MultPub(res, a, a, size,  ring_size, nodeNet); // Mulpub squaring

    // gettimeofday(&end, NULL); // stop timer here

    // timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    // printf("Runtime for multpub with data size %d = %.6lf ms\n", size, (double)(timer * 0.001) / total);
    // printf("Ratio for multpub %d = %.6lf ms\n", size, (double)(timer * 0.001 / size) / total);

    // // check MultPub
    // flag = 0;
    // for (Lint i = 0; i < size; i++) {
    //     res_check[i] = (i * i) & nodeNet->SHIFT[ring_size];
    //     if (!checkbytes(res[i], res_check[i], bytes) && flag == 0) {
    //         printf("MultPub ERROR at %d !=   ", i);
    //         print_1283(res[i]);
    //         print_1283(res_check[i]);
    //         flag = 1;
    //     }
    // }
    // if (flag == 0) {
    //     printf(ANSI_COLOR_GREEN "MultPub Correct!\n" ANSI_COLOR_RESET);
    // } else {
    //     printf(ANSI_COLOR_RED "MultPub error!\n" ANSI_COLOR_RESET);
    // }
    // flag = 0;

    // printf("Testing RandBit...\n");
    // total = 1;

    // gettimeofday(&start, NULL); // start timer here
    // Rss_RandBit(c, size, ring_size, nodeNet);
    // gettimeofday(&end, NULL); // stop timer here

    // memset(res, 0, sizeof(Lint) * size);

    // timer = (1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec);
    // printf("runtime for RandBit with data size %d = %f ms\n", size, (float)(timer/total)/1000);

    // Rss_Open(res, c, size,  ring_size, nodeNet);
    // for (int i = 0; i < size; i++) {
    //     // printf("res[%i]  : %llu\n", i, res[i]);
    //     if ((res[i] != 0) && (res[i] != 1)) {
    //         // printf("res[%i]  : %llu\n", i, res[i]);
    //         // printf("RandBit Error at %u\n", i);
    //         flag = 1;
    //     }
    // }
    // if (flag == 0) {
    //     printf(ANSI_COLOR_GREEN "RandBit Correct!\n" ANSI_COLOR_RESET);
    // } else {
    //     printf(ANSI_COLOR_RED "RandBit error!\n" ANSI_COLOR_RESET);
    // }

    // printf("Testing edaBit...\n");
    // gettimeofday(&start,NULL); //start timer here
    // Rss_edaBit(c, d, size, ring_size, nodeNet);
    // gettimeofday(&end, NULL); //stop timer here

    // printf(ANSI_COLOR_YELLOW "Runtime for edaBit with data size  %d = %.6lf ms\n" ANSI_COLOR_RESET, (size), (double)(timer * 0.001) / total);
    // printf(ANSI_COLOR_YELLOW "Ratio for edaBit %d = %.6lf ms\n" ANSI_COLOR_RESET, size, (double)(timer * 0.001 / (size)) / total);

    // Rss_Open(res, c, size, ring_size, nodeNet);
    // Rss_Open_Bitwise(res2, d, size, ring_size, nodeNet);

    // flag = 0;

    // for (int i = 0; i < size; i++) {
    //     // printf("res[%i]  : %llu\n", i, res[i]);
    //     // print_binary(res[i], ring_size);
    //     // printf("res2[%i]  : %llu\n", i, res2[i]);
    //     // print_binary(res2[i], ring_size);
    //     // res_check[i] = GET_BIT_TEST(res_check[i], ring_size - 1);
    //     if (!(res[i] == res2[i])) {
    //         // printf("res[%i]  : %llu\n", i, res[i]);
    //         print_binary(res[i], ring_size);
    //         // printf("res2[%i]  : %llu\n", i, res2[i]);
    //         print_binary(res2[i], ring_size);
    //         printf("edaBit ERROR at %d \n", i);
    //         flag = 1;
    //     }
    // }
    // if (flag == 0) {
    //     printf(ANSI_COLOR_GREEN "edaBit Correct!\n" ANSI_COLOR_RESET);
    // } else {
    //     printf(ANSI_COLOR_RED "edaBit error!\n" ANSI_COLOR_RESET);
    // }

    /*



    total = 0;

    timer = 0;
    for (j = 0; j < total; j++) {
        gettimeofday(&start, NULL); // start timer here
        Rss_Mult(c, a, b, size, ring_size, map, nodeNet);
        // Rss_Mult_fixed_b(c, a, b, 2, size, ring_size, map, nodeNet);

        gettimeofday(&end, NULL); // stop timer here
        timer += 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        unsigned long long temp_time = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        // printf("[mult] %u: time = %.6lf ms\n", j, (double)(temp_time * 1e-3));
    }
    gettimeofday(&end, NULL); // stop timer here
    timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("runtime for mult with data size %d = %.6lf us\n", size, (double)(timer) / total);         // double check time calculation is correct
    // printf("runtime for mult with data size %d = %.6lf ms\n", size, (double)(timer * 0.001) / total); // double check time calculation is correct
    printf("Ratio for %d = %.6lf us\n", size, (double)(timer / size) / total);                        // double check time calculation is correct
    // printf("Ratio for %d = %.6lf ms\n", size, (double)(timer * 0.001 / size) / total);                // double check time calculation is correct

    gettimeofday(&start, NULL); // start timer here
    Rss_Mult(c, a, b, size, ring_size, map, nodeNet);
    gettimeofday(&end, NULL); // stop timer here

    timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("runtime for mult with data size %d = %ld us\n", size, timer);

    Rss_Open(res, c, size, map, ring_size, nodeNet);

    // check Mult
    flag = 0;
    for (Lint i = 0; i < size; i++) {
        // printf("res[%i]  : %llu\n", i, res[i]);

        res_check[i] = (i * i) & nodeNet->SHIFT[ring_size];
        if (!checkbytes(res[i], res_check[i], bytes) && flag == 0) {
            printf("Mult ERROR at %d !=   \n", i);

            // printf("expected  : %llu\n", res_check[i]);

            print_1283(res[i]);
            print_1283(res_check[i]);
            flag = 1;
        }
    }
    if (flag == 0) {
        printf(ANSI_COLOR_GREEN "Mult Correct!\n" ANSI_COLOR_RESET);
    } else {
        printf(ANSI_COLOR_RED "Mult error!\n" ANSI_COLOR_RESET);
    }
    flag = 0;

    printf("Testing MultPub...\n");
    total = 0;

    gettimeofday(&start, NULL); // start timer here
    for (j = 0; j < total; j++) {
        Rss_MultPub(res, a, a, size, map, ring_size, nodeNet); // Mulpub squaring
    }

    gettimeofday(&end, NULL); // stop timer here

    timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("Runtime for multpub with data size %d = %.6lf ms\n", size, (double)(timer * 0.001) / total);
    printf("Ratio for multpub %d = %.6lf ms\n", size, (double)(timer * 0.001 / size) / total);

    // check MultPub
    flag = 0;
    for (Lint i = 0; i < size; i++) {
        res_check[i] = (i * i) & nodeNet->SHIFT[ring_size];
        if (!checkbytes(res[i], res_check[i], bytes) && flag == 0) {
            printf("MultPub ERROR at %d !=   ", i);
            print_1283(res[i]);
            print_1283(res_check[i]);
            flag = 1;
        }
    }
    if (flag == 0) {
        printf(ANSI_COLOR_GREEN "MultPub Correct!\n" ANSI_COLOR_RESET);
    } else {
        printf(ANSI_COLOR_RED "MultPub error!\n" ANSI_COLOR_RESET);
    }
    flag = 0;

    printf("Testing RandBit...\n");
    total = 1;

    gettimeofday(&start, NULL); // start timer here
    // computation test Mult:
    for (j = 0; j < total; j++) {
        // Rss_RandBit(c, size, ring_size, map, nodeNet);
    }
    gettimeofday(&end, NULL); // stop timer here
    timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("Runtime for randbit with data size %d = %.6lf ms\n", size, (double)(timer * 0.001) / total);
    printf("Ratio for randbit %d = %.6lf ms\n", size, (double)(timer * 0.001 / size) / total);

    gettimeofday(&start, NULL); // start timer here
    Rss_RandBit(c, size, ring_size, map, nodeNet);
    gettimeofday(&end, NULL); // stop timer here

    memset(res, 0, sizeof(Lint) * size);

    // timer = (1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec);
    // printf("runtime for RandBit with data size %d = %f ms\n", size, (float)(timer/total)/1000);

    // Rss_Open(res, c, size, map, ring_size - 10, nodeNet);
    Rss_Open(res, c, size, map, ring_size, nodeNet);
    for (int i = 0; i < size; i++) {
        // printf("res[%i]  : %llu\n", i, res[i]);
        if ((res[i] != 0) && (res[i] != 1)) {
            // printf("res[%i]  : %llu\n", i, res[i]);
            // printf("RandBit Error at %u\n", i);
            flag = 1;
        }
    }
    if (flag == 0) {
        printf(ANSI_COLOR_GREEN "RandBit Correct!\n" ANSI_COLOR_RESET);
    } else {
        printf(ANSI_COLOR_RED "RandBit error!\n" ANSI_COLOR_RESET);
    }

    printf("Testing edaBit...\n");
    // gettimeofday(&start,NULL); //start timer here
    // Rss_edaBit(c, d, size/ring_size, ring_size, map, nodeNet);
    // gettimeofday(&end, NULL); //stop timer here

    timer = 0;

    total = 1;
    for (j = 0; j < total; j++) {
        gettimeofday(&start, NULL); // start timer here
        Rss_edaBit(c, d, size, ring_size, map, nodeNet);
        gettimeofday(&end, NULL); // stop timer here

        timer += 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

        unsigned long long temp_time = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        // printf("%u: time = %.6lf\n", j, (double)(temp_time * 1e-3));
    }

    printf(ANSI_COLOR_YELLOW "Runtime for edaBit with data size  %d = %.6lf ms\n" ANSI_COLOR_RESET, (size), (double)(timer * 0.001) / total);
    printf(ANSI_COLOR_YELLOW "Ratio for edaBit %d = %.6lf ms\n" ANSI_COLOR_RESET, size, (double)(timer * 0.001 / (size)) / total);

    Rss_Open(res, c, size, map, ring_size, nodeNet);
    Rss_Open_Bitwise(res2, d, size, map, ring_size, nodeNet);

    flag = 0;

    for (int i = 0; i < size; i++) {
        // printf("res[%i]  : %llu\n", i, res[i]);
        // print_binary(res[i], ring_size);
        // printf("res2[%i]  : %llu\n", i, res2[i]);
        // print_binary(res2[i], ring_size);
        // res_check[i] = GET_BIT_TEST(res_check[i], ring_size - 1);
        if (!(res[i] == res2[i])) {
            printf("res[%i]  : %llu\n", i, res[i]);
            print_binary(res[i], ring_size);
            printf("res2[%i]  : %llu\n", i, res2[i]);
            print_binary(res2[i], ring_size);
            printf("edaBit ERROR at %d \n", i);
            flag = 1;
        }
    }
    if (flag == 0) {
        printf(ANSI_COLOR_GREEN "edaBit Correct!\n" ANSI_COLOR_RESET);
    } else {
        printf(ANSI_COLOR_RED "edaBit error!\n" ANSI_COLOR_RESET);
    }

    total = 1;
    gettimeofday(&start, NULL); // start timer here
    for (j = 0; j < total; j++) {
        Rss_MSB(c, a, size, ring_size, map, nodeNet);
    }
    gettimeofday(&end, NULL); // stop timer here
    timer = (1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec);

    printf("Runtime for MSB with data size  %d = %.6lf ms\n", (size), (double)(timer * 0.001) / total);
    printf("Ratio for MSB %d = %.6lf ms\n", size, (double)(timer * 0.001 / (size)) / total);
    Rss_Open (res, c, size, map, ring_size, nodeNet);

    Rss_Open(res_check, a, size, map, ring_size, nodeNet);
    flag = 0;
    for (int i = 0; i < size; i++) {

        res_check[i] = GET_BIT_TEST(res_check[i], ring_size - 1);
        if (!(res[i] == res_check[i])) {
            // printf("MSB ERROR at %d \n", i);
            // printf("res[%i]  : %llu\n", i, res[i]);
            // print_binary(res[i], ring_size);
            // printf("res_check[%i]  : %llu\n", i, res_check[i]);
            // print_binary(res_check[i], ring_size);
            flag = 1;
        }
    }
    if (flag == 0) {
        printf(ANSI_COLOR_GREEN "MSB Correct!\n" ANSI_COLOR_RESET);
    } else {
        printf(ANSI_COLOR_RED "MSB error!\n" ANSI_COLOR_RESET);
    }

    total = 1;
    gettimeofday(&start, NULL); // start timer here
    for (j = 0; j < total; j++) {
        // test_new_Rss_MSB(c, a, size, ring_size, map, nodeNet);
        new_Rss_MSB(c, a, size, ring_size, map, nodeNet);
    }
    gettimeofday(&end, NULL); // stop timer here
    timer = (1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec);

    printf(ANSI_COLOR_YELLOW "Runtime for newMSB with data size  %d = %.6lf ms\n" ANSI_COLOR_RESET, (size), (double)(timer * 0.001) / total);

    printf(ANSI_COLOR_YELLOW "Ratio for newMSB %d = %.6lf ms\n" ANSI_COLOR_RESET, size, (double)(timer * 0.001 / (size)) / total);
    Rss_Open(res, c, size, map, ring_size, nodeNet);

    Rss_Open(res_check, a, size, map, ring_size, nodeNet);
    flag = 0;
    for (int i = 0; i < size; i++) {

        res_check[i] = GET_BIT_TEST(res_check[i], ring_size - 1);
        if (!(res[i] == res_check[i])) {
            printf("newMSB ERROR at %u \t", i);
            printf("actual:  : %llu\t", res[i]);
            printf("expected  : %llu\n", res_check[i]);
            flag = 1;
        }
    }
    if (flag == 0) {
        printf(ANSI_COLOR_GREEN "newMSB Correct!\n" ANSI_COLOR_RESET);
    } else {
        printf(ANSI_COLOR_RED "newMSB error!\n" ANSI_COLOR_RESET);
    }

    printf("Testing LT.....\n");
    gettimeofday(&start, NULL); // start timer here

    total = 1;
    for (j = 0; j < total; j++) {
        Rss_LT(c, a, b, size, ring_size, map, nodeNet);
    }
    gettimeofday(&end, NULL); // stop timer here

    timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

    printf("Runtime for LT with data size  %d = %.6lf ms\n", (size), (double)(timer * 0.001) / total);
    printf("Ratio for LT %d = %.6lf ms\n", size, (double)(timer * 0.001 / (size)) / total);

    Rss_Open(res, a, size, map, ring_size, nodeNet);
    Rss_Open(res2, b, size, map, ring_size, nodeNet);

    Rss_Open(res3, c, size, map, ring_size, nodeNet);

    flag = 0;

    for (int i = 0; i < size; i++) {
        // Getting MSB of a - b
        Lint c_check = GET_BIT(res[i] - res2[i], ring_size);
        res_check[i] = c_check * (res2[i] - res[i]) + res[i];
        if (!(res3[i] == res_check[i])) {
            // printf("LT ERROR at %d \n", i);
            flag = 1;
        }
    }
    if (flag == 0) {
        printf(ANSI_COLOR_GREEN "LT Correct!\n" ANSI_COLOR_RESET);
    } else {
        printf(ANSI_COLOR_RED "LT error!\n" ANSI_COLOR_RESET);
    }

    printf("Testing newLT.....\n");
    gettimeofday(&start, NULL); // start timer here

    total = 1;
    for (j = 0; j < total; j++) {
        new_Rss_LT(c, a, b, size, ring_size, map, nodeNet);
    }

    gettimeofday(&end, NULL); // stop timer here

    timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("Runtime for newLT with data size  %d = %.6lf ms\n", (size), (double)(timer * 0.001) / total);
    printf("Ratio for newLT %d = %.6lf ms\n", size, (double)(timer * 0.001 / (size)) / total);

    Rss_Open(res, a, size, map, ring_size, nodeNet);
    Rss_Open(res2, b, size, map, ring_size, nodeNet);

    Rss_Open(res3, c, size, map, ring_size, nodeNet);

    flag = 0;

    for (int i = 0; i < size; i++) {
        // Getting MSB of a - b
        Lint c_check = GET_BIT(res[i] - res2[i], ring_size);
        res_check[i] = c_check * (res2[i] - res[i]) + res[i];
        if (!(res3[i] == res_check[i])) {
            // printf("newLT ERROR at %d \n", i);
            flag = 1;
        }
    }
    if (flag == 0) {
        printf(ANSI_COLOR_GREEN "newLT Correct!\n" ANSI_COLOR_RESET);
    } else {
        printf(ANSI_COLOR_RED "newLT error!\n" ANSI_COLOR_RESET);
    }

    memset(res, 0, sizeof(Lint) * size);

    printf("Testing Conversion.....\n");
    // NOTE ///////////////////
    // Must used unsiged long if we want to test more than +1 (assuming ring size of 30)
    gettimeofday(&start, NULL); // start timer here

    total = 1;
    for (j = 0; j < total; j++) {
        Rss_Convert(c, a, size, ring_size, ring_size + 0, map, nodeNet);
    }

    gettimeofday(&end, NULL); // stop timer here

    timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("runtime for Conversion with data size %d = %ld us\n", size, timer);

    Rss_Open(res, c, size, map, ring_size + 0, nodeNet);
    for (Lint i = 0; i < size; i++) {

        res_check[i] = Data1[0][i] + Data1[1][i] + Data1[2][i];
        res_check[i] = res_check[i] & nodeNet->SHIFT[ring_size];
        // printf("res[%i]  : %llu\n", i, res[i]);
        // printf("res_check[%i]  : %llu\n", i, res_check[i]);
        if (!(res[i] == res_check[i])) {
            // if(!checkbytes(res[i], res_check[i], bytes) && flag == 0) {
            // printf("Conversion ERROR at %d\n", i);
            // printf("res[%i]  : %llu\n", i, res[i]);
            // printf("res_check[%i]  : %llu\n\n", i, res_check[i]);
            flag = 1;
        }
    }
    if (flag == 0) {
        printf(ANSI_COLOR_GREEN "Conversion Correct!\n" ANSI_COLOR_RESET);
    } else {
        printf(ANSI_COLOR_RED "Conversion error! " ANSI_COLOR_RESET);
        printf(ANSI_COLOR_RED "Did you use an appropriate ring size?\n" ANSI_COLOR_RESET);
    }

    // printf("Testing newConversion.....\n");
    // // NOTE ///////////////////
    // // Must used unsiged long if we want to test more than +1 (assuming ring size of 30)
    // gettimeofday(&start, NULL); //start timer here
    // total = 0;
    // for (j = 0; j < total; j++) {
    //     new_Rss_Convert(c, a, size, ring_size, ring_size + 0, map, nodeNet);
    // }

    // gettimeofday(&end, NULL); //stop timer here

    // timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    // printf("runtime for newConversion with data size %d = %ld us\n", size, timer);

    // Rss_Open(res, c, size, map, ring_size + 0, nodeNet);
    // flag = 0;
    // for (Lint i = 0; i < size; i++) {

    //     res_check[i] = Data1[0][i] + Data1[1][i] + Data1[2][i];
    //     res_check[i] = res_check[i] & nodeNet->SHIFT[ring_size];
    //     if (!(res[i] == res_check[i])) {
    //         // printf("newConversion ERROR at %d\n", i);
    //         // printf("res[%i]  : %llu\n", i, res[i]);
    //         // printf("res_check[%i]  : %llu\n\n", i, res_check[i]);
    //         flag = 1;
    //     }
    // }
    // // odd iterations are working correctly????
    // if (flag == 0) {
    //     printf(ANSI_COLOR_GREEN "newConversion Correct!\n" ANSI_COLOR_RESET);
    // } else {
    //     printf(ANSI_COLOR_RED "newConversion error! " ANSI_COLOR_RESET);
    //     printf(ANSI_COLOR_RED "Did you use an appropriate ring size?\n" ANSI_COLOR_RESET);
    // }

    Lint m = 5;

    printf("Testing EdaBit_Trunc\n");
    // Rss_edaBit_trunc_test(c, d, e, size, ring_size, m, map, nodeNet);
    Rss_Open(res, c, size, map, ring_size, nodeNet);
    Rss_Open(res2, d, size, map, ring_size, nodeNet);

    flag = 0;
    for (size_t i = 0; i < size; i++) {
        // printf("res[%i]:  %llu\t", i, (res[i] >> m));
        // printf("res[%i]:  \t", i);
        // print_binary(res[i] >> m, ring_size);
        // // printf("res2[%i]: %llu\t", i, res2[i]);
        // printf("res2[%i]: \t", i);
        // print_binary(res2[i], ring_size);

        // if (!((res[i] >> m) == res2[i])) {
        //     printf("--> r and r' don't match\n");
        //     flag = 1;
        // }
    }
    if (flag == 0) {
        printf(ANSI_COLOR_GREEN "EdaBit_Trunc Correct!\n" ANSI_COLOR_RESET);
    } else {
        printf(ANSI_COLOR_RED "EdaBit_Trunc error!\n" ANSI_COLOR_RESET);
    }

    printf("Testing Truncation...\n");
    total = 1;

    gettimeofday(&start, NULL); // start timer here
    for (j = 0; j < total; j++) {
        Rss_truncPr_1(c, a, m, size, ring_size, map, nodeNet);
    }

    gettimeofday(&end, NULL); // stop timer here

    timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("Runtime for TruncPR with data size %d = %.6lf ms\n", size, (double)(timer * 0.001) / total);
    printf("Ratio for TruncPR %d = %.6lf ms\n", size, (double)(timer * 0.001 / size) / total);

    Rss_Open(res, c, size, map, ring_size, nodeNet);
    // Rss_Open(res2, d, size, map, ring_size, nodeNet);
    Rss_Open(res3, a, size, map, ring_size, nodeNet);
    // printf("\n");
    // printf("\n");
    // printf("\n");
    for (size_t i = 0; i < size; i++) {

        res_check[i] = Data1[0][i] + Data1[1][i] + Data1[2][i];
        res_check[i] = (res_check[i] & nodeNet->SHIFT[ring_size]) >> (m);
        // if (!(res[i] == res_check[i] && res2[i] == res_check[i] ) ) {
        // if (!(res[i] == res_check[i]  ) ) {
        // printf("a[%i]          : \t", i );
        // print_binary(res3[i], ring_size);
        // printf("expect[%i]     : \t", i );
        // print_binary(res_check[i], ring_size);

        // printf("actual[%i]     : \t", i );
        // print_binary(res[i], ring_size);

        // printf("------\n");
        flag = 1;
        // }
    }

    // printf("Testing MaxPool.....\n");
    // gettimeofday(&start,NULL); //start timer here
    // MaxPool(c, a, 16, 24, 24, ring_size, map, nodeNet);
    // // MaxPool(c, a, size, ring_size, ring_size + 10, map, nodeNet);
    // gettimeofday(&end,NULL);//stop timer here

    // timer = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
    // printf("runtime for Conversion with data size %d = %ld us\n", size, timer);
    // Rss_Open(res, c, 16*12*12, map, ring_size, nodeNet);
    // for(int i =0; i< 16*12*12; i++) {
    // 	printf("res[%i]  : %llu\n", i, res[i]);
    // }

    // total = 100;
    // printf("Testing SVM.....\n");
    // gettimeofday(&start,NULL); //start timer here
    // for (j = 0; j < total; j++) {

    // 	Rss_MatMultArray_batch(c, a, b, 463, 128, 1, ring_size, batch_size, 0,0, map, nodeNet);

    // 	// for (i = 0; i < 463; i++) {
    // 	// 	Rss_MatMultArray_batch(c, a, b, 1, 128, 1, ring_size, batch_size, 0, map, nodeNet);

    // 	// }
    // 	// arg_max(c, b, a, ring_size, 463, batch_size, map, nodeNet);
    // 	eda_arg_max(c, b, a, ring_size, 463, batch_size, map, nodeNet);

    // }
    // gettimeofday(&end,NULL);//stop timer here
    // timer = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
    // // printf("runtime for SVM with data size %d = %ld us\n", size, (timer/batch_size)/total); //double check time calculation is correct
    // // it should be though

    // // printf("runtime for mult with data size %d = %.6lf us\n", size, (double)(timer)/total); //double check time calculation is correct
    // printf("\nruntime for SVM with data size %d = %.6lf ms\n", total*batch_size, (double)(timer*0.001)); //double check time calculation is correct
    // printf("Average prediction time for data size %d = %.6lf ms\n", total*batch_size, (double)(timer*0.001)/(total*batch_size)); //double check time calculation is correct
    // printf("Ratio for %d = %.6lf us\n", size,  (double)(timer/size)/total); //double check time calculation is correct
    // printf("Ratio for SVM %d = %.6lf ms\n\n", size,  (double)(timer*0.001/size)/total); //double check time calculation is correct
    // // it should be though

    // printf("Testing Max.....\n");
    // gettimeofday(&start,NULL); //start timer here
    // // maximum(c, a, ring_size, size/batch_size, batch_size, map, nodeNet);
    // arg_max(b, c, a, ring_size, size/batch_size, batch_size, map, nodeNet);
    // // MaxPool(c, a, size, ring_size, ring_size + 10, map, nodeNet);
    // gettimeofday(&end,NULL);//stop timer here

    // timer = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
    // printf("runtime for Argmax with data size %d = %ld us\n", size, timer);
    // Rss_Open(res, c, batch_size, map, ring_size, nodeNet);
    // for(int i =0; i< batch_size; i++) {
    // 	printf("res[%i]  : %llu\n", i, res[i]);
    // }

    printf("Testing matmult...\n");

    // total = 1;

    // gettimeofday(&start,NULL); //start timer here
    // computation test Mult:

    // timer = 0;

    uint mat_dims[4] = {100, 10000, 250000, 1000000};

    total = 0;
    for (size_t i = 2; i < 4; i++) {
        uint temp_size = mat_dims[i];
        // cout << "temp_size = " << temp_size << endl;
        gettimeofday(&start, NULL); // start timer here
        for (j = 0; j < total; j++) {
            // gettimeofday(&start,NULL); //start timer here
            // printf("j = %u\n",j );
            Rss_MatMultArray(c, a, b, sqrt(temp_size), sqrt(temp_size), sqrt(temp_size), ring_size, map, nodeNet);

            // Rss_Mult(c, a, b, temp_size, ring_size, map, nodeNet);
            // Rss_Mult_fixed_b(c, a, b, 2, size, ring_size, map, nodeNet);
        }
        gettimeofday(&end, NULL); // stop timer here
        timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

        printf("matmult %i = %.6lf ms\n", int(sqrt(temp_size)), (double)(timer * 0.001) / total);

        // printf("\nruntime for matmult with data temp_size %f = %.6lf ms\n", sqrt(temp_size), (double)(timer * 0.001) / total); // double check time calculation is correct

        // printf("runtime for mult with data temp_size 1e%i = %.6lf ms\n", i, (double)(timer * 0.001) / total);
        // printf("Ratio for %d = %.6lf ms\n", temp_size, (double)(timer * 0.001 / temp_size) / total);
    }

    // for (j = 0; j < total; j++) {
    //     gettimeofday(&start, NULL); // start timer here
    //     // printf("j : %u\n",j);
    //     Rss_MatMultArray(c, a, b, sqrt(size), sqrt(size), sqrt(size), ring_size, map, nodeNet);

    //     gettimeofday(&end, NULL); // stop timer here

    //     timer += 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

    //     unsigned long long temp_time = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    //     printf("%u: time = %.6lf\n", j, (double)(temp_time * 1e-3));

    // }

    // // gettimeofday(&end,NULL);//stop timer here
    // // timer = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
    // printf("\nruntime for matmult with data size %f = %.6lf ms\n", sqrt(size), (double)(timer * 0.001) / total); // double check time calculation is correct
    // // printf("Ratio for %d = %.6lf us\n", size,  (double)(timer/size)/total); //double check time calculation is correct
    // printf("Ratio for matmult %f = %.6lf ms\n\n", sqrt(size), (double)(timer * 0.001 / size) / total); // double check time calculation is correct
    // // it should be though
*/
    delete[] res;
    delete[] res2;
    delete[] res3;
    delete[] res_check;
    for (int i = 0; i < 3; i++) {

        delete[] Data1[i];
        delete[] Data2[i];
    }

    delete[] Data1;
    delete[] Data2;
    delete[] a;
    delete[] b;

    for (i = 0; i < 2; i++) {
        delete[] c[i];
        delete[] d[i];
        delete[] e[i];
        delete[] f[i];
    }

    delete[] c;
    delete[] d;
    delete[] e;
    delete[] f;
    // for (i = 0; i < 2; i++) {
    //     delete [] c[i];
    //     delete [] d[i];
    // }
    // delete [] c;
    // delete [] d;

    // nodeNet->prg_clean();
    //*************************************************************************
    // random performance:

    // int byte = nodeNet->getbytes();
    //  int size_radnom = 1000000;
    //  	Lint *v_b = new Lint [size_radnom];
    //  //memset(v_b,0,sizeof(Lint)*size);

    // gettimeofday(&start,NULL); //start timer here
    // uint8_t *buffer = new uint8_t [bytes*size_radnom];
    // nodeNet->prg_getrandom(1, bytes, size_radnom, buffer);

    // for(int i = 0 ; i<size_radnom; i++){
    // 	//printf("call getrandom ");
    // 	memcpy(v_b+i, buffer+bytes*i, bytes);

    // }
    // delete [] buffer;
    // delete [] v_b;
    // gettimeofday(&end,NULL);//stop timer here
    // timer = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
    // printf("runtime for getradnom with data size %d = %ld us\n", size_radnom, timer);
}