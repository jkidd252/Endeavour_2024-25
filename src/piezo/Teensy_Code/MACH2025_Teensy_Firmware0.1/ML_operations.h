
#ifndef MODEL_INFERENCE_H
#define MODEL_INFERENCE_H

#include <Arduino.h>
#include "model_weights_CNNmanual.h"


#define INPUT_SIZE 6
#define LAYER1_SIZE 64
#define LAYER2_SIZE 64
#define LAYER3_SIZE 64
#define OUTPUT_CLASSES 8

// === Function declarations ===
float relu(float x);

/**
 * Fully-connected layer computation.
 * 
 * @param input     Pointer to input array.
 * @param output    Pointer to output array.
 * @param weights   Weight matrix [in_size][out_size].
 * @param bias      Bias array [out_size].
 * @param in_size   Number of input neurons.
 * @param out_size  Number of output neurons.
 * @param use_relu  Whether to apply ReLU activation.
 */
void dense_layer(float* input, float* output,
                 const float* weights,
                 const float* bias,
                 int in_size, int out_size,
                 bool use_relu);

/**
 * Applies softmax activation in-place.
 * 
 * @param x    Array of logits.
 * @param size Number of elements.
 */
void softmax(float* x, int size);

/**
 * Runs inference through the full model.
 * 
 * @param input Flattened input array of size INPUT_SIZE.
 * @return      Predicted class index (0–7).
 */
int run_model_inference(float input[]);

#endif