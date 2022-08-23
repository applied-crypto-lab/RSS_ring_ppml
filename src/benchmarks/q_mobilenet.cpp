#include "../include/q_mobilenet.h"

void q_mobilenet(NodeNetwork *nodeNet, NodeConfiguration *nodeConfig, string in_model_path, uint num_iterations, uint batch_size, uint alpha_index, uint input_dim, uint num_discarded) {
    // batch_size = num_iterations;

    string mobile_path = in_model_path + "/q_mobile/";
    // string mobile_path = "/projects/academic/mblanton/models/q_mobile/";

    // uint test_im = 0;

    string labels_path = mobile_path + "labels.txt";
    string model_structure = mobile_path + "model_shapes.csv";

    string interim_layer = "intermediate_layers.csv";

    if (8 * sizeof(Lint) != 32) {
        printf(ANSI_COLOR_RED "ERROR: sizeof(Lint) != 32.\nRecompile with unsigned.\nExiting....\n" ANSI_COLOR_RESET);
        exit(0);
    }
    struct timeval start;
    struct timeval end;

    struct timeval start_layer;
    struct timeval end_layer;

    unsigned long timer = 0;
    unsigned long timer_layer = 0;
    unsigned long eda_timer = 0;
    unsigned long eda_run_timer = 0;

    // double alpha = stod(alpha_string.at(alpha_index));
    double alpha_param = stod(alpha_string.at(alpha_index));
    uint ring_size = nodeNet->RING;

    uint num_layers = 28;

    Lint public_M = 16; // public upper bound for private truncation

    string model_path = mobile_path + "all_models/mobilenet_v1_" + alpha_string.at(alpha_index) + "_" + to_string(input_dim) + "/";
    string model_path_csv = model_path + "csv/";

    string image_path = mobile_path + "input_image/" + to_string(input_dim) + "/";

    vector<string> class_labels = read_fnames(labels_path);

    // all the name files in q_mobile
    // vector<string> all_names = read_fnames_with_mobile_path(mobile_path + "name_paths.txt", mobile_path);

    vector<string> all_names = read_fnames_with_mobile_path(mobile_path, mobile_path + "name_paths.txt");
    cout << "mobile_path + name_paths.txt : " << mobile_path + "name_paths.txt" << endl;
    cout << "all_names.at(0) : " << all_names.at(0) << endl;


    // vector<string> all_names = read_fnames(mobile_path + "name_paths.txt");
    cout << "all_names.size() : " << all_names.size() << endl;

    uint n, in_channels, layer_ctr;

    string bias_shapes = mobile_path + "bias_shapes.csv";
    // string labels_path = mobile_path + "labels.txt";

    vector<vector<int>> model_shapes = read_model_shape(model_structure);
    vector<vector<int>> bias_dim = read_model_shape(bias_shapes);
    vector<vector<int>> layer_dim = read_model_shape(model_path + interim_layer);

    scaleModel(model_shapes, alpha_param);
    scaleBiases(bias_dim, alpha_param);
    scaleIntermediateLayers(layer_dim, alpha_param);

    Lint ***weights = new Lint **[num_layers];
    Lint ***biases = new Lint **[num_layers];
    for (size_t i = 0; i < num_layers; i++) {
        weights[i] = new Lint *[2];
        biases[i] = new Lint *[2];
        uint weight_size = model_shapes.at(i).at(0) * model_shapes.at(i).at(1) * model_shapes.at(i).at(2) * model_shapes.at(i).at(3);

        for (size_t j = 0; j < 2; j++) {

            weights[i][j] = new Lint[weight_size];
            biases[i][j] = new Lint[bias_dim.at(i).at(0)];
        }
    }
    uint current_layer;

    Lint ***intermediate_x = new Lint **[num_layers + 1];
    for (size_t i = 0; i < num_layers + 1; i++) {
        intermediate_x[i] = new Lint *[2];

        current_layer = layer_dim.at(i).at(0) * layer_dim.at(i).at(1) * layer_dim.at(i).at(2) * layer_dim.at(i).at(3);

        // cout << i << " - current_layer: " << current_layer * batch_size << endl;
        for (size_t j = 0; j < 2; j++) {
            intermediate_x[i][j] = new Lint[batch_size * current_layer];

            memset(intermediate_x[i][j], 0, sizeof(Lint) * batch_size * current_layer);
        }
    }

    Lint **relu_bounds = new Lint *[2];
    Lint **trunc_vals = new Lint *[2];
    Lint **input_zp = new Lint *[2];
    Lint **weights_zp = new Lint *[2];
    Lint **output_zp = new Lint *[2];

    Lint **input_x = new Lint *[2];
    Lint **output_x = new Lint *[2];

    Lint **final_x = new Lint *[2];     // used for computing argmax
    Lint **final_x_ind = new Lint *[2]; // used for computing argmax
    for (size_t i = 0; i < 2; i++) {

        // its num_layers-1 here since the last conv layer has no activation
        relu_bounds[i] = new Lint[num_layers];
        trunc_vals[i] = new Lint[num_layers]; // private values we truncate by at end of each layer
        input_zp[i] = new Lint[num_layers];
        weights_zp[i] = new Lint[num_layers];
        output_zp[i] = new Lint[num_layers];

        // dimensions are always input_dim^2*3*batch_size
        input_x[i] = new Lint[input_dim * input_dim * 3 * batch_size];
        output_x[i] = new Lint[1001 * batch_size];

        // top 1
        final_x[i] = new Lint[batch_size];
        final_x_ind[i] = new Lint[batch_size];
    }

    // checking correctness
    cout << "setup begin" << endl;

    q_fullSetup(input_x, weights, biases, relu_bounds, trunc_vals, input_zp, weights_zp, output_zp, model_path, image_path, all_names, model_shapes, bias_dim, alpha_param, input_dim, public_M, ring_size, batch_size, nodeNet);
    cout << "setup end" << endl;

    for (size_t k = 0; k < num_iterations; k++) {
        // cout << "k : " << k << endl;
        layer_ctr = 0;
        n = input_dim;
        in_channels = 3;

        eda_timer = 0;

        // cout << "----" << endl;
        gettimeofday(&start, NULL); // start timer here
        // cout<<"layer_ctr : "<<layer_ctr<<endl;

        // gettimeofday(&start_layer, NULL); //start timer here

        Rss_conv2d_first(intermediate_x, input_x, weights, biases, relu_bounds, trunc_vals, input_zp, weights_zp, output_zp, &n, 3, &in_channels, alpha_param * 32, 2, batch_size, &layer_ctr, public_M, ring_size, nodeNet, eda_timer);
        // gettimeofday(&end_layer, NULL); //stop timer here
        timer_layer = 1e6 * (end_layer.tv_sec - start_layer.tv_sec) + end_layer.tv_usec - start_layer.tv_usec;
        // printf("--- layer_%u time = %.6lf\n", layer_ctr, (double)(timer_layer * 1e-6));
        // printf("%u, %.6lf, %.6lf \n", batch_size, (double)(timer * 1e-6) / (double)(num_iterations), (double)(eda_timer * 1e-6) / (double)(num_iterations));

        // cout<<"layer_ctr : "<<layer_ctr<<endl;
        // gettimeofday(&start_layer, NULL); //start timer here
        Rss_dw_pw_conv2d(intermediate_x, weights, biases, relu_bounds, trunc_vals, input_zp, weights_zp, output_zp, &n, &in_channels, alpha_param * 64, 1, batch_size, &layer_ctr, public_M, ring_size, nodeNet, eda_timer);
        gettimeofday(&end_layer, NULL); // stop timer here
        // timer_layer = 1e6 * (end_layer.tv_sec - start_layer.tv_sec) + end_layer.tv_usec - start_layer.tv_usec;
        // printf("--- layer_%u time = %.6lf\n", layer_ctr, (double)(timer_layer * 1e-6));

        // cout<<"layer_ctr : "<<layer_ctr<<endl;

        Rss_dw_pw_conv2d(intermediate_x, weights, biases, relu_bounds, trunc_vals, input_zp, weights_zp, output_zp, &n, &in_channels, alpha_param * 128, 2, batch_size, &layer_ctr, public_M, ring_size, nodeNet, eda_timer);
        // cout<<"layer_ctr : "<<layer_ctr<<endl;

        Rss_dw_pw_conv2d(intermediate_x, weights, biases, relu_bounds, trunc_vals, input_zp, weights_zp, output_zp, &n, &in_channels, alpha_param * 128, 1, batch_size, &layer_ctr, public_M, ring_size, nodeNet, eda_timer);
        // cout<<"layer_ctr : "<<layer_ctr<<endl;

        Rss_dw_pw_conv2d(intermediate_x, weights, biases, relu_bounds, trunc_vals, input_zp, weights_zp, output_zp, &n, &in_channels, alpha_param * 256, 2, batch_size, &layer_ctr, public_M, ring_size, nodeNet, eda_timer);
        // cout<<"layer_ctr : "<<layer_ctr<<endl;

        Rss_dw_pw_conv2d(intermediate_x, weights, biases, relu_bounds, trunc_vals, input_zp, weights_zp, output_zp, &n, &in_channels, alpha_param * 256, 1, batch_size, &layer_ctr, public_M, ring_size, nodeNet, eda_timer);
        // cout<<"layer_ctr : "<<layer_ctr<<endl;

        Rss_dw_pw_conv2d(intermediate_x, weights, biases, relu_bounds, trunc_vals, input_zp, weights_zp, output_zp, &n, &in_channels, alpha_param * 512, 2, batch_size, &layer_ctr, public_M, ring_size, nodeNet, eda_timer);
        // cout<<"layer_ctr : "<<layer_ctr<<endl;

        Rss_dw_pw_conv2d(intermediate_x, weights, biases, relu_bounds, trunc_vals, input_zp, weights_zp, output_zp, &n, &in_channels, alpha_param * 512, 1, batch_size, &layer_ctr, public_M, ring_size, nodeNet, eda_timer);
        // cout<<"layer_ctr : "<<layer_ctr<<endl;

        Rss_dw_pw_conv2d(intermediate_x, weights, biases, relu_bounds, trunc_vals, input_zp, weights_zp, output_zp, &n, &in_channels, alpha_param * 512, 1, batch_size, &layer_ctr, public_M, ring_size, nodeNet, eda_timer);
        // cout<<"layer_ctr : "<<layer_ctr<<endl;

        Rss_dw_pw_conv2d(intermediate_x, weights, biases, relu_bounds, trunc_vals, input_zp, weights_zp, output_zp, &n, &in_channels, alpha_param * 512, 1, batch_size, &layer_ctr, public_M, ring_size, nodeNet, eda_timer);
        // cout<<"layer_ctr : "<<layer_ctr<<endl;

        Rss_dw_pw_conv2d(intermediate_x, weights, biases, relu_bounds, trunc_vals, input_zp, weights_zp, output_zp, &n, &in_channels, alpha_param * 512, 1, batch_size, &layer_ctr, public_M, ring_size, nodeNet, eda_timer);
        // cout<<"layer_ctr : "<<layer_ctr<<endl;

        Rss_dw_pw_conv2d(intermediate_x, weights, biases, relu_bounds, trunc_vals, input_zp, weights_zp, output_zp, &n, &in_channels, alpha_param * 512, 1, batch_size, &layer_ctr, public_M, ring_size, nodeNet, eda_timer);
        // cout<<"layer_ctr : "<<layer_ctr<<endl;

        Rss_dw_pw_conv2d(intermediate_x, weights, biases, relu_bounds, trunc_vals, input_zp, weights_zp, output_zp, &n, &in_channels, alpha_param * 1024, 2, batch_size, &layer_ctr, public_M, ring_size, nodeNet, eda_timer);
        // cout<<"layer_ctr : "<<layer_ctr<<endl;

        Rss_dw_pw_conv2d(intermediate_x, weights, biases, relu_bounds, trunc_vals, input_zp, weights_zp, output_zp, &n, &in_channels, alpha_param * 1024, 1, batch_size, &layer_ctr, public_M, ring_size, nodeNet, eda_timer);
        // cout<<"layer_ctr : "<<layer_ctr<<endl;

        Rss_avg_pool_trunc(intermediate_x[layer_ctr], intermediate_x[layer_ctr - 1], in_channels, &n, 0, 0, batch_size, ring_size, nodeNet, eda_timer);
        // cout<<"layer_ctr : "<<layer_ctr<<endl;

        Rss_conv2d_final_layer(intermediate_x, output_x, weights, biases, relu_bounds, trunc_vals, input_zp, weights_zp, output_zp, &n, 1, &in_channels, 1001, 1, batch_size, &layer_ctr, public_M, ring_size, nodeNet, eda_timer);
        // cout<<"layer_ctr : "<<layer_ctr<<endl;

        // cout << "layer_ctr: " << layer_ctr << "\t" << "in_dim : " << n << ", " << n << ", " << in_channels << "\n";

        // arg_max(final_x, final_x_ind, intermediate_x[layer_ctr], ring_size, 1001, batch_size,  nodeNet);
        // std::cout << "intermediate_x["<<layer_ctr<<"] = "<< intermediate_x[layer_ctr] << std::endl;
        eda_arg_max_time(final_x, final_x_ind, intermediate_x[layer_ctr], ring_size, 1001, batch_size, nodeNet, eda_timer);

        gettimeofday(&end, NULL); // stop timer here

        unsigned long long temp_time = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;

        printf("[%zu] [%u, %.2lf, %u, %u, %u] [%.6lf s] \n", k, input_dim, alpha_param, batch_size, num_iterations, num_discarded, (double)(temp_time * 1e-6));

        if (k >= num_discarded) {

            timer += 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
            eda_run_timer += eda_timer;
        }
    }

    // printf("Total runtime for mobilenet with n = %u, alpha = %.2f:  %.6lf s\n", input_dim, alpha_param, (double)(timer * 1e-6));
    // printf("Average prediction time for mobilenet with n = %u, alpha = %.2f:  %.6lf s\n", input_dim, alpha_param, (double)(timer * 1e-6) / (double)(num_iterations));
    // printf("edaBit generation time: %.6lf s\n", (double)(eda_timer * 1e-6) / (double)(num_iterations));

    // printf("batch, total, edaBit\n");
    printf("[%s_3pc] [%u, %.2lf, %u, %u, %u] [%.6lf s,  %.6lf s,  %lu bytes] \n", "q_mobilenet", input_dim, alpha_param, batch_size, num_iterations, num_discarded, (double)(timer * 1e-6) / (double)(num_iterations - num_discarded), (double)(eda_run_timer * 1e-6) / (double)(num_iterations - num_discarded), nodeNet->getCommunicationInBytes() / (num_iterations ));

    // printf(ANSI_COLOR_YELLOW "%u, %.2lf, %u, %.6lf, %.6lf \n" ANSI_COLOR_RESET, input_dim, alpha_param, batch_size, (double)(timer * 1e-6) / (double)(num_iterations - num_discarded), (double)(eda_run_timer * 1e-6) / (double)(num_iterations - num_discarded));

    // CLEANUP

    for (size_t i = 0; i < num_layers; i++) {
        for (size_t j = 0; j < 2; j++) {
            delete[] weights[i][j];
            delete[] biases[i][j];
        }
        delete[] weights[i];
        delete[] biases[i];
    }
    // cout<<"num_layers + 1"<<num_layers + 1<<endl;
    for (size_t i = 0; i < num_layers + 1; i++) {
        // cout<<"i : "<<i<<endl;
        for (size_t j = 0; j < 2; j++) {
            delete[] intermediate_x[i][j];
        }
        delete[] intermediate_x[i];
    }

    delete[] weights;
    delete[] biases;
    delete[] intermediate_x;

    for (size_t i = 0; i < 2; i++) {
        delete[] relu_bounds[i];
        delete[] trunc_vals[i];
        delete[] input_zp[i];
        delete[] weights_zp[i];
        delete[] output_zp[i];

        delete[] input_x[i];
        delete[] output_x[i];

        delete[] final_x[i];
        delete[] final_x_ind[i];
    }

    delete[] relu_bounds;
    delete[] trunc_vals;
    delete[] input_zp;
    delete[] weights_zp;
    delete[] output_zp;

    delete[] input_x;
    delete[] output_x;

    delete[] final_x;
    delete[] final_x_ind;
}