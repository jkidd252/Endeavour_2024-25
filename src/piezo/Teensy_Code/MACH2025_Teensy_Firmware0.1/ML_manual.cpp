#include "model_weights_CNNmanual.h"
#include <math.h>

float relu(float x) {
    return x > 0 ? x : 0;
}

void dense_layer(float* input, float* output, const float* weights, const float* bias, int in_size, int out_size, bool use_relu) {
    for (int i = 0; i < out_size; i++) {
        output[i] = bias[i];
        for (int j = 0; j < in_size; j++) {
            output[i] += input[j] * weights[j * out_size + i];
        }
        if (use_relu) output[i] = relu(output[i]);
    }
}

void softmax(float* x, int size) {
    float max_val = x[0];
    for (int i = 1; i < size; i++) {
        if (x[i] > max_val) max_val = x[i];
    }
    float sum = 0.0f;
    for (int i = 0; i < size; i++) {
        x[i] = expf(x[i] - max_val);  // for numerical stability
        sum += x[i];
    }
    for (int i = 0; i < size; i++) {
        x[i] /= sum;
    }
}

int run_model_inference(float input[]) {
    // 3 Dense(64), ReLU + Dense(8), softmax
    float layer1[64];
    float layer2[64];
    float layer3[64];
    float logits[8];

    dense_layer(input, layer1, dense_0_weights, dense_0_bias, 6, 64, true);
    dense_layer(layer1, layer2, dense_1_weights, dense_1_bias, 64, 64, true);
    dense_layer(layer2, layer3, dense_2_weights, dense_2_bias, 64, 64, true);
    dense_layer(layer3, logits, dense_3_weights, dense_3_bias, 64, 8, false);

    softmax(logits, 8);

    // Get predicted class
    int max_i = 0;
    float max_val = logits[0];
    for (int i = 1; i < 8; ++i) {
        if (logits[i] > max_val) {
            max_val = logits[i];
            max_i = i;
        }
    }
    return max_i;
}