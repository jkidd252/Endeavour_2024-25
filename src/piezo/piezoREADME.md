# Piezoelectric Machine Learning on Teensy 4.1

## ℹ️ Overview

Repo contains all code used to design/generate the TF model used and its deployment on the Teensy 4.1 microcontroller.

## ⬇️ Flight Firmware / Model Summary

The final flight ready firmware is saved as:
> MACH25_Teensy_Firmware1.0
This file conatins all header files and scripts needed to run the CNN model.
Data collected in flight is stored on an SD card as 'data.txt' with the results of the various data handling algorithms and the ML predicted classes saved in seperate text files.
In the event of a restart/power cycle these files are re-opened and print '# START' is printed to a new line (this was done so that previously collected data wouldnt be lost).
 
Due to limited support for TF on Teensy 4.1 the model weights are saved as in a header file (Model_weights_CNNmanual.h) and a simple model was used.
The model architecture is as follows:
```bash
model_CNN_man = tf.keras.Sequential([
    tf.keras.layers.Input(shape=( x_flat.shape[1], )),
    tf.keras.layers.Dense(64, activation='relu'),
    tf.keras.layers.Dense(64, activation='relu'),
    tf.keras.layers.Dense(64, activation='relu'),
    tf.keras.layers.Dense(num_classes, activation='softmax'),  # 8 classes
])

model_CNN_man.compile(
    optimizer='adam',
    loss='sparse_categorical_crossentropy',  # since y_train is integer labels (0–7)
    metrics=['accuracy']
)
```
This code can also be found in ML/Model_store/Model_CNN_Manual.ipynb

These layers are manually applied to the output layer weights and bias' when running the model, this can be seen in Teensy_Code/MACH2025_Teensy_Firmware1.0.
This method essentially boils down to vecotr/tensor algebra where the input vector, collected data, is multiplied by the weights for some layer and then give an offset (bias).
By doing this the application of the machine learning model requires no complex Tensor operations and can therefor be run on any microcontroller natively (given that the application of the matric multiplication is essentially just floating point maths), this does however reduce the model accuracy.

ML models (generally) take 'windows' of data, ingesting data in this way helps the model to understand the context/origin of the data and therefor make better predictions as a result.
In this project a window size of 3 is used with a stride of 1, this means each wiondow features 3 points and the next window shares one point with its predicessor.
The root mean square of the acceleration and piezoelectric sensor data is used as an input by the model. 
Since the model was trained to take this input various data handling scripts ( windowing.cpp, rms.cpp ) were develped.

The model does not run on a sliding window, i.e. doesnt run live, since we didn't want data collection inhibited by a model error or long computation time.
A sliding window inplemetation could very easily be applied/implemented with this code if the aformentioned issues were addressed.
