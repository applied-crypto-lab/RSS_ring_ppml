#include "mpc_util.h"

void print_binary(Lint n, uint size){
    uint temp = size-1;
    int i = size-1;
    uint b;
    while(i !=-1) {
        b = GET_BIT(n, temp);
        printf("%u", b);
        temp--;
        i -= 1;
    }
    printf("\n");
}

double absMax(double *input, uint size){
    double max = abs(input[0]);
    for (size_t i = 1; i < size; i++) {
        if (abs(input[i]) > max) {
            max = abs(input[i]);
        }
    }
    return max;
}

double absMax(std::vector<double> input){
    double max = abs(input[0]);
    for (size_t i = 1; i < input.size(); i++) {
        if (abs(input[i]) > max) {
            max = abs(input[i]);
        }
    }
    return max;
}


double getScale(uint bound, double *input, uint size){
    double abs_max = absMax(input, size);
    return ((double) bound / abs_max);
}

double getScale(uint bound, std::vector<double> x){

    double abs_max = absMax(x);
    return ((double) bound / abs_max);
}

void q_applyScale(sLint *output, double *input, double scale, uint size){
    for (size_t i = 0; i < size ; i++) {
        // y.at(i) = scale * x.at(i);
        output[i] = input[i] * scale;
    }
}
