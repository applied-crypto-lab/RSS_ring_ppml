#include "eda_rb_pregen_3pc.h"

void eda_rb_pregen_3pc(NodeNetwork *nodeNet, NodeConfiguration *nodeConfig, uint num_iterations, uint batch_size, uint alpha_index, uint input_dim, uint num_discarded, string path) {
    std::cout << "starting eda rb pregen" << std::endl;
    uint numShares = nodeNet->getNumShares();

    if (8 * sizeof(Lint) != 32) {
        printf(ANSI_COLOR_RED "ERROR: sizeof(Lint) != 32.\nRecompile with unsigned.\nExiting....\n" ANSI_COLOR_RESET);
        exit(0);
    }
    uint ring_size = nodeNet->RING;

    vector<uint> batch_sizes = {1, 10};
    vector<uint> input_sizes = {128, 160, 192, 224};
    vector<double> alphas = {0.25, 0.5, 0.75, 1.0};
    double alpha_param = stod(alpha_string.at(alpha_index));

    vector<string> exp_params = read_fnames(path);

    int batch_idx = distance(batch_sizes.begin(), find(batch_sizes.begin(), batch_sizes.end(), batch_size));
    int input_dim_idx = distance(input_sizes.begin(), find(input_sizes.begin(), input_sizes.end(), input_dim));

    struct timeval start;
    struct timeval end;
    unsigned long long timer = 0, temp_time = 0;

    // cout<<batch_idx<<endl;
    // cout<<input_dim_idx<<endl;

    // cout<<input_dim<<", "<<alpha_index<<", "<<batch_size<<endl;
    string match_string = exp_params.at((input_dim_idx * batch_sizes.size() * alphas.size()) + (alpha_index * batch_sizes.size()) + batch_idx);

    cout << match_string << endl;

    string tmp;
    stringstream ss(match_string);
    vector<uint> params;

    while (getline(ss, tmp, ',')) {
        params.push_back(stoi(tmp));
    }
    // {
    //     cout<<var<<endl;
    // }
    uint num_eda = params.at(3);
    uint num_rb = params.at(4);

    // may need to update after tuning, and may be different for 5pc
    int opt_rb_batch = 100000;
    int opt_eda_batch = 100000;

    Lint **out_eda = new Lint *[numShares];
    Lint **out_eda_bw = new Lint *[numShares];
    Lint **out_rb = new Lint *[numShares];
    for (size_t i = 0; i < numShares; i++) {
        out_eda[i] = new Lint[opt_eda_batch];
        out_eda_bw[i] = new Lint[opt_eda_batch];
        out_rb[i] = new Lint[opt_rb_batch];
    }

    for (size_t j = 0; j < num_iterations; j++) {

        int temp_rb_size = num_rb;
        int temp_eda_size = num_eda;

        gettimeofday(&start, NULL); // start timer here
        while (true) {
            if (temp_rb_size - opt_rb_batch > 0) {
                // printf("RB : opt_size = %i\n",opt_rb_batch);
                Rss_RandBit(out_rb, opt_rb_batch, ring_size, nodeNet);
                temp_rb_size -= opt_rb_batch;
            } else {
                // printf("RB : remainder = %i\n", temp_rb_size);
                Rss_RandBit(out_rb, temp_rb_size, ring_size, nodeNet);
                break;
            }
        }
        while (true) {
            if (temp_eda_size - opt_eda_batch > 0) {
                // printf("EDA : opt_size = %i\n",opt_eda_batch);
                Rss_edaBit(out_eda, out_eda_bw, opt_eda_batch, ring_size, nodeNet);
                temp_eda_size -= opt_eda_batch;
            } else {
                // printf("EDA : remainder = %i\n", temp_eda_size);
                Rss_edaBit(out_eda, out_eda_bw, temp_eda_size, ring_size, nodeNet);
                break;
            }
        }
        gettimeofday(&end, NULL); // stop timer here
        temp_time = 1e6*(end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        printf("[%zu] [%u, %.2lf, %u, %u, %u] [%.6lf s] \n", j, input_dim, alpha_param, batch_size, num_iterations, num_discarded, (double)(temp_time * 1e-6) );
            
        if (j >= num_discarded) {

            timer += 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        }

    }
    printf("[%s_3pc] [%u, %.2lf, %u, %u, %u] [%.6lf s] \n", "eda_rb_pregen", input_dim, alpha_param, batch_size, num_iterations, num_discarded, (double)(timer * 1e-6) / (double)(num_iterations - num_discarded));


    for (size_t i = 0; i < numShares; i++) {
        delete[] out_eda[i];
        delete[] out_eda_bw[i];
        delete[] out_rb[i];
    }

    delete[] out_eda;
    delete[] out_eda_bw;
    delete[] out_rb;
}