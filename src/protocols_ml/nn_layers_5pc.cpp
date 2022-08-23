#include "nn_layers_5pc.h"

// x = [batch_size, n, n, channels]
// W = [filter, filter, channels]
// first calls pad on the input x to create x_padded
// x_padded = [batch_size, height+2, width+2, channels]
// then computes matrix multiplication
void Rss_depthwise_conv2d_mp(Lint ***x, Lint ***W, Lint ***b, Lint **relu_bunds, Lint **trunc_vals, Lint **input_zp, Lint **weights_zp, Lint **output_zp, uint *n, uint *in_channels, uint out_channels, uint stride, uint batch_size, uint *layer_id, Lint public_M, uint ring_size, NodeNetwork *nodeNet) {
    // cout << "layer_ctr: " << *layer_id << "\t" << "in_dim : " << *n << ", " << *n << ", " << *in_channels << "\n";
    // cout << "out_channels : " << out_channels << endl;

    uint i; // used for loops
    // uint out_size;
    // uint W_size;
    // uint in_size;
    uint pad_size;
    uint numShares = nodeNet->getNumShares();

    Lint **x_padded = new Lint *[numShares];
    pad_size = (*n + 2) * (*n + 2) * (*in_channels) * batch_size;
    // cout << "pad_size : " << pad_size << endl;
    for (i = 0; i < numShares; i++) {

        x_padded[i] = new Lint[pad_size];
        // stride has no impact here
        memset(x_padded[i], 0, sizeof(Lint) * pad_size);
    }

    pad_original(x_padded, x[*layer_id - 1], (*in_channels), *n, *n, 3, stride, batch_size, nodeNet);

    // // "adding" ZERO POINTS
    sub_zp(x_padded, x_padded, input_zp, pad_size / batch_size, batch_size, *layer_id, nodeNet);

    sub_zp(W[*layer_id], W[*layer_id], weights_zp, 3 * 3 * (*in_channels) * (out_channels), 1, *layer_id, nodeNet);

    Rss_dw_matrixmul_mp(x[*layer_id + 0], x_padded, W[*layer_id], *n, *n, (*in_channels), 3, ring_size, batch_size, stride,  nodeNet);

    *n = *n / stride; // stride from previous layer

    // // CHECK STRIDE HERE AND n VALUE
    pw_add_biases(x[*layer_id + 0], x[*layer_id + 0], b[*layer_id], *n, *n, *in_channels, batch_size, nodeNet);
    // // activation

    ReLU_6_alpha_mp(x[*layer_id + 0], x[*layer_id + 0], relu_bunds, (*in_channels) * (*n) * (*n), ring_size, *layer_id, nodeNet);

    Rss_truncPriv_mp(x[*layer_id + 0], x[*layer_id + 0], trunc_vals, public_M, *layer_id, (*in_channels) * (*n) * (*n), ring_size, nodeNet);

    // *layer_id += 1;

    for (i = 0; i < numShares; i++) {
        delete[] x_padded[i];
    }
    delete[] x_padded;
}

void Rss_pointwise_conv2d_mp(Lint ***x, Lint ***W, Lint ***b, Lint **relu_bunds, Lint **trunc_vals, Lint **input_zp, Lint **weights_zp, Lint **output_zp, uint *n, uint *in_channels, uint out_channels, uint stride, uint batch_size, uint *layer_id, Lint public_M, uint ring_size, NodeNetwork *nodeNet) {

    uint i; // used for loops
    uint numShares = nodeNet->getNumShares();

    // cout << "layer_ctr: " << *layer_id << "\t" << "in_dim : " << *n << ", " << *n << ", " << *in_channels << "\n";
    uint pad_size = (*n) * (*n) * (*in_channels) * batch_size;
    Lint **x_padded = new Lint *[numShares];

    for (i = 0; i < numShares; i++) {
        x_padded[i] = new Lint[pad_size];
        memset(x_padded[i], 0, sizeof(Lint) * pad_size);
    }

    extract_patch(x_padded, x[*layer_id - 1], (*in_channels), *n, *n, 1, 1, batch_size, 1, nodeNet);

    sub_zp(x_padded, x_padded, input_zp, ((*in_channels) * (*n) * (*n)), batch_size, *layer_id, nodeNet);

    sub_zp(W[*layer_id], W[*layer_id], weights_zp, 1 * 1 * (*in_channels) * (out_channels), 1, *layer_id, nodeNet);

    // PROBLEM HERE
    Rss_pw_matrixmul_mp(x[*layer_id + 0], x_padded, W[*layer_id], *n, *n, (*in_channels), 1, out_channels, ring_size, batch_size,  nodeNet);

    pw_add_biases(x[*layer_id + 0], x[*layer_id + 0], b[*layer_id], *n, *n, out_channels, batch_size, nodeNet);

    // // activation
    ReLU_6_alpha_mp(x[*layer_id + 0], x[*layer_id + 0], relu_bunds, out_channels * (*n) * (*n), ring_size, *layer_id, nodeNet);

    Rss_truncPriv_mp(x[*layer_id + 0], x[*layer_id + 0], trunc_vals, public_M, *layer_id, out_channels * (*n) * (*n), ring_size, nodeNet);

    // *in_channels = out_channels;
    // *layer_id += 1;

    // *n = *n / stride; // stride from previous layer

    for (i = 0; i < numShares; i++) {
        delete[] x_padded[i];
    }
    delete[] x_padded;
}

// need shares of trunc values
// and the public upper bound M
void Rss_dw_pw_conv2d_mp(Lint ***x, Lint ***W, Lint ***b, Lint **relu_bunds, Lint **trunc_vals, Lint **input_zp, Lint **weights_zp, Lint **output_zp, uint *n, uint *in_channels, uint out_channels, uint stride, uint batch_size, uint *layer_id, Lint public_M, uint ring_size, NodeNetwork *nodeNet, unsigned long &timer) {
    // cout<<"DW"<<endl;
    // cout << "layer_ctr: " << *layer_id << "\t" << "in_dim : " << *n << ", " << *n << ", " << *in_channels << "\n";
    uint numShares = nodeNet->getNumShares();

    uint i; // used for loops
    // uint out_size;
    // uint W_size;
    // uint in_size;
    uint pad_size;

    // pad_size = (*n + 2) * (*n + 2) * (*in_channels) * batch_size;
    // out_size = ;
    // W_size = 3*3*(*in_channels)*(out_channels);
    // in_size = ;

    Lint **x_padded = new Lint *[numShares];
    pad_size = (*n + 2) * (*n + 2) * (*in_channels) * batch_size;
    // cout<<"pad_size : "<<pad_size<<endl;
    for (i = 0; i < numShares; i++) {

        // pad_size = (*in_channels) * (3) * (3)*ceil(*n / stride) * ceil(*n / stride) * batch_size;

        x_padded[i] = new Lint[pad_size];
        // stride has no impact here
        memset(x_padded[i], 0, sizeof(Lint) * pad_size);
    }

    pad_original(x_padded, x[*layer_id - 1], (*in_channels), *n, *n, 3, stride, batch_size, nodeNet);

    // "adding" ZERO POINTS
    sub_zp(x_padded, x_padded, input_zp, pad_size / batch_size, batch_size, *layer_id, nodeNet);

    // sub_zp(W[*layer_id], W[*layer_id], weights_zp, 3 * 3 * (*in_channels) * (out_channels), 1, *layer_id, nodeNet);
    sub_zp(W[*layer_id], W[*layer_id], weights_zp, 3 * 3 * (*in_channels), 1, *layer_id, nodeNet);

    Rss_dw_matrixmul_mp(x[*layer_id + 0], x_padded, W[*layer_id], *n, *n, (*in_channels), 3, ring_size, batch_size, stride,  nodeNet);

    *n = *n / stride; // stride from previous layer

    // CHECK STRIDE HERE AND n VALUE
    pw_add_biases(x[*layer_id + 0], x[*layer_id + 0], b[*layer_id], *n, *n, *in_channels, batch_size, nodeNet);

    // activation
    ReLU_6_alpha_timer_mp(x[*layer_id + 0], x[*layer_id + 0], relu_bunds, (*in_channels) * (*n) * (*n) * batch_size, ring_size, *layer_id, nodeNet, timer);

    Rss_truncPriv_time_mp(x[*layer_id + 0], x[*layer_id + 0], trunc_vals, public_M, *layer_id, (*in_channels) * (*n) * (*n) * batch_size, ring_size, nodeNet, timer);

    *layer_id += 1;

    // cout << "layer_ctr: " << *layer_id << "\t" << "in_dim : " << *n << ", " << *n << ", " << *in_channels << "\n";

    for (i = 0; i < numShares; i++) {
        memset(x_padded[i], 0, sizeof(Lint) * pad_size);
    }

    extract_patch(x_padded, x[*layer_id - 1], (*in_channels), *n, *n, 1, 1, batch_size, 1, nodeNet);

    sub_zp(x_padded, x_padded, input_zp, ((*in_channels) * (*n) * (*n)), batch_size, *layer_id, nodeNet);

    sub_zp(W[*layer_id], W[*layer_id], weights_zp, 1 * 1 * (*in_channels) * (out_channels), 1, *layer_id, nodeNet);

    Rss_pw_matrixmul_mp(x[*layer_id + 0], x_padded, W[*layer_id], *n, *n, (*in_channels), 1, out_channels, ring_size, batch_size,  nodeNet);

    pw_add_biases(x[*layer_id + 0], x[*layer_id + 0], b[*layer_id], *n, *n, out_channels, batch_size, nodeNet);

    // // activation
    ReLU_6_alpha_timer_mp(x[*layer_id + 0], x[*layer_id + 0], relu_bunds, out_channels * (*n) * (*n) * batch_size, ring_size, *layer_id, nodeNet, timer);

    Rss_truncPriv_time_mp(x[*layer_id + 0], x[*layer_id + 0], trunc_vals, public_M, *layer_id, out_channels * (*n) * (*n) * batch_size, ring_size, nodeNet, timer);

    *in_channels = out_channels;
    *layer_id += 1;

    // *n = *n / stride; // stride from previous layer

    for (i = 0; i < numShares; i++) {
        delete[] x_padded[i];
    }
    delete[] x_padded;
}

void Rss_conv2d_first_mp(Lint ***x, Lint **input_x, Lint ***W, Lint ***b, Lint **relu_bunds, Lint **trunc_vals, Lint **input_zp, Lint **weights_zp, Lint **output_zp, uint *n, uint kernel, uint *in_channels, uint out_channels, uint stride, uint batch_size, uint *layer_id, Lint public_M, uint ring_size, NodeNetwork *nodeNet, unsigned long &timer) {
    // cout << "layer_ctr: " << *layer_id << "\t" << "in_dim : " << *n << ", " << *n << ", " << *in_channels << "\n";
    struct timeval start;
    struct timeval end;
    unsigned long op_timer = 0;
    uint numShares = nodeNet->getNumShares();

    uint i; // used for loops
    uint pad_size;
    Lint **x_padded = new Lint *[numShares];
    pad_size = (*in_channels) * (kernel) * (kernel)*ceil(*n / stride) * ceil(*n / stride) * batch_size;

    for (i = 0; i < numShares; i++) {
        // x_padded[i] = new Lint[((*in_channels) * (*n) * (*n)) * batch_size];
        // cout<<pad_size<<endl;
        x_padded[i] = new Lint[pad_size];

        memset(x_padded[i], 0, sizeof(Lint) * pad_size);
    }
    gettimeofday(&start, NULL); // start timer here
    extract_patch(x_padded, input_x, (*in_channels), *n, *n, kernel, stride, batch_size, 1, nodeNet);
    *n = *n / stride;
    gettimeofday(&end, NULL); // stop timer here
    op_timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    // printf("extract_patch time = %.6lf\n", (double)(op_timer * 1e-6));

    // "adding" ZERO POINTS
    sub_zp(x_padded, x_padded, input_zp, pad_size, 1, *layer_id, nodeNet);

    sub_zp(W[*layer_id], W[*layer_id], weights_zp, kernel * kernel * (*in_channels) * (out_channels), 1, *layer_id, nodeNet);

    gettimeofday(&start, NULL); // start timer here
    Rss_pw_matrixmul_mp(x[*layer_id], x_padded, W[*layer_id], *n, *n, (*in_channels), kernel, out_channels, ring_size, batch_size,  nodeNet);
    gettimeofday(&end, NULL); // stop timer here
    op_timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    // printf("matrix mul time = %.6lf\n", (double)(op_timer * 1e-6));

    pw_add_biases(x[*layer_id], x[*layer_id], b[*layer_id], *n, *n, out_channels, batch_size, nodeNet);

    add_zp(x[*layer_id + 1], x[*layer_id + 1], output_zp, (*n) * (*n) * (out_channels), batch_size, *layer_id, nodeNet);

    gettimeofday(&start, NULL); // start timer here
    ReLU_6_alpha_timer_mp(x[*layer_id], x[*layer_id], relu_bunds, out_channels * (*n) * (*n) * batch_size, ring_size, *layer_id, nodeNet, timer);
    gettimeofday(&end, NULL); // stop timer here
    op_timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    // printf("ReLU6 time = %.6lf\n", (double)(op_timer * 1e-6));

    gettimeofday(&start, NULL); // start timer here
    Rss_truncPriv_time_mp(x[*layer_id], x[*layer_id], trunc_vals, public_M, *layer_id, out_channels * (*n) * (*n) * batch_size, ring_size, nodeNet, timer);
    gettimeofday(&end, NULL); // stop timer here
    op_timer = 1e6 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    // printf("truncPriv time = %.6lf\n", (double)(op_timer * 1e-6));

    *in_channels = out_channels;
    *layer_id += 1;

    for (i = 0; i < numShares; i++) {
        delete[] x_padded[i];
    }
    delete[] x_padded;
}

void Rss_conv2d_mp(Lint ***x, Lint ***W, Lint ***b, Lint **relu_bunds, Lint **trunc_vals, Lint **input_zp, Lint **weights_zp, Lint **output_zp, uint *n, uint kernel, uint *in_channels, uint out_channels, uint stride, uint batch_size, uint *layer_id, Lint public_M, uint ring_size, NodeNetwork *nodeNet) {
    // cout << "layer_ctr: " << *layer_id << "\t" << "in_dim : " << *n << ", " << *n << ", " << *in_channels << "\n";
    uint numShares = nodeNet->getNumShares();

    uint i; // used for loops
    uint pad_size;
    Lint **x_padded = new Lint *[numShares];
    for (i = 0; i < numShares; i++) {
        pad_size = (*in_channels) * (kernel) * (kernel)*ceil(*n / stride) * ceil(*n / stride) * batch_size;
        // x_padded[i] = new Lint[((*in_channels) * (*n) * (*n)) * batch_size];
        x_padded[i] = new Lint[pad_size];

        memset(x_padded[i], 0, sizeof(Lint) * pad_size);
    }

    extract_patch(x_padded, x[*layer_id], (*in_channels), *n, *n, kernel, stride, batch_size, 1, nodeNet);
    *n = *n / stride; // DOUBLE CHECK THAT THIS IS RIGHT

    // "adding" ZERO POINTS
    // sub_zp(x_padded, x_padded, input_zp, (*n + 2) * (*n + 2) * (*in_channels), batch_size, *layer_id, nodeNet);

    // sub_zp(W[*layer_id], W[*layer_id], weights_zp, kernel * kernel * (*in_channels) * (out_channels), 1, *layer_id, nodeNet);

    Rss_pw_matrixmul_mp(x[*layer_id + 1], x_padded, W[*layer_id], *n, *n, (*in_channels), kernel, out_channels, ring_size, batch_size,  nodeNet);

    pw_add_biases(x[*layer_id + 1], x[*layer_id + 1], b[*layer_id], *n, *n, out_channels, batch_size, nodeNet);

    // not called in standard version because z_3 is always zero
    // add_zp(x[*layer_id+1], x[*layer_id+1], output_zp, (*n) * (*n) * (out_channels), batch_size, *layer_id, nodeNet);

    ReLU_6_alpha_mp(x[*layer_id + 1], x[*layer_id + 1], relu_bunds, out_channels * (*n) * (*n) * batch_size, ring_size, *layer_id, nodeNet);

    Rss_truncPriv_mp(x[*layer_id + 1], x[*layer_id + 1], trunc_vals, public_M, *layer_id, out_channels * (*n) * (*n) * batch_size, ring_size, nodeNet);

    // activation

    *in_channels = out_channels;
    *layer_id += 1;

    // *n = *n / stride; // stride from previous layer

    for (i = 0; i < numShares; i++) {
        delete[] x_padded[i];
    }
    delete[] x_padded;
}

void Rss_conv2d_final_layer_mp(Lint ***x, Lint **output_x, Lint ***W, Lint ***b, Lint **relu_bunds, Lint **trunc_vals, Lint **input_zp, Lint **weights_zp, Lint **output_zp, uint *n, uint kernel, uint *in_channels, uint out_channels, uint stride, uint batch_size, uint *layer_id, Lint public_M, uint ring_size, NodeNetwork *nodeNet, unsigned long &timer) {
    // cout << "layer_ctr: " << *layer_id << "\t" << "in_dim : " << *n << ", " << *n << ", " << *in_channels << "\n";
    uint numShares = nodeNet->getNumShares();

    uint i; // used for loops
    uint pad_size;

    pad_size = (*in_channels) * (kernel) * (kernel)*ceil(*n / stride) * ceil(*n / stride) * batch_size;
    // cout << "pad_size : " << pad_size << endl;

    Lint **x_padded = new Lint *[numShares];
    for (i = 0; i < numShares; i++) {
        // x_padded[i] = new Lint[((*in_channels) * (*n) * (*n)) * batch_size];
        x_padded[i] = new Lint[pad_size];

        memset(x_padded[i], 0, sizeof(Lint) * pad_size);
    }

    extract_patch(x_padded, x[*layer_id - 1], (*in_channels), *n, *n, kernel, stride, batch_size, 1, nodeNet);
    *n = *n / stride;

    // "adding" ZERO POINTS
    sub_zp(x_padded, x_padded, input_zp, pad_size, 1, *layer_id, nodeNet);

    sub_zp(W[*layer_id], W[*layer_id], weights_zp, kernel * kernel * (*in_channels) * (out_channels), 1, *layer_id, nodeNet);

    Rss_pw_matrixmul_mp(output_x, x_padded, W[*layer_id], *n, *n, (*in_channels), kernel, out_channels, ring_size, batch_size,  nodeNet);

    // cout<<"pw_mult_end"<<endl;
    pw_add_biases(output_x, output_x, b[*layer_id], *n, *n, out_channels, batch_size, nodeNet);
    // cout<<"biases  end "<<endl;

    add_zp(output_x, output_x, output_zp, (*n) * (*n) * (out_channels), batch_size, *layer_id, nodeNet);
    // cout<<"add_zp  end "<<end, nodeNetl;

    ReLU_6_alpha_timer_mp(output_x, output_x, relu_bunds, out_channels * (*n) * (*n) * batch_size, ring_size, *layer_id, nodeNet, timer);

    // cout<<"relu_6  end "<<endl;
    Rss_truncPriv_time_mp(output_x, output_x, trunc_vals, public_M, *layer_id, out_channels * (*n) * (*n) * batch_size, ring_size, nodeNet, timer);
    //    cout<<"truncPriv  end "<<endl;

    // activation

    *in_channels = out_channels;
    *layer_id += 1;

    // *n = *n / stride; // stride from previous layer

    for (i = 0; i < numShares; i++) {
        delete[] x_padded[i];
    }
    delete[] x_padded;
}

void Rss_conv2d_no_act_mp(Lint **res, Lint **x, Lint **W, Lint **b, Lint **relu_bunds, uint *n, uint kernel, uint *in_channels, uint out_channels, uint stride, uint ring_size, uint batch_size, uint *layer_id, NodeNetwork *nodeNet) {

    uint i; // used for loops
    uint numShares = nodeNet->getNumShares();

    Lint **x_padded = new Lint *[numShares];
    for (i = 0; i < numShares; i++) {
        x_padded[i] = new Lint[((*in_channels) * (*n) * (*n)) * batch_size];
    }

    extract_patch(x_padded, x, (*in_channels), *n, *n, kernel, stride, batch_size, 1, nodeNet);
    *n = *n / stride; // DOUBLE CHECK THAT THIS IS RIGHT

    Rss_pw_matrixmul_mp(res, x_padded, W, *n, *n, (*in_channels), kernel, out_channels, ring_size, batch_size,  nodeNet);

    pw_add_biases(res, res, b, *n, *n, out_channels, batch_size, nodeNet);

    *in_channels = out_channels;
    *layer_id += 1;

    // *n = *n / stride; // stride from previous layer

    for (i = 0; i < numShares; i++) {
        delete[] x_padded[i];
    }
    delete[] x_padded;
}
