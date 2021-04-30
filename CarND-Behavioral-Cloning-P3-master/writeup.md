# Behavioral Cloning Project

---

The aims / phases of this project are as follows: 
* Using the simulator to gather knowledge on positive driving conduct. 
* Build, a convolution neural network in Keras that predicts steering angles from images
* Train the model with a training and validation set and test it 
* Test that without leaving the lane, the model drives successfully around track one 
* With a written note, describe the findings

[ds1]: ./examples/ds1.jpg "Dataset Image 1"
[ds2]: ./examples/ds2.jpg "Dataset Image 2"
[ds3]: ./examples/ds3.jpg "Dataset Image 3"
[ds4]: ./examples/ds4.jpg "Dataset Image 4"
[ds5]: ./examples/ds5.jpg "Dataset Image 5"


## Rubric Points

The following files contain my project: 
* model.py, which includes the script used to build and train the model 
* drive.py for autonomous mode to drive the vehicle 
* model.h5 with a qualified neural convolution network 
* The findings are presented by writeup writeup.md

Using the simulator and drive.py file given by Udacity, the car can be moved autonomously across the track by executing
```sh
python drive.py model.h5
```
---

## Model Architecture and Training Strategy

The model.py file includes the training and the convolution neural network saving code. The file displays the pipeline that is used for the model's training and validation.

### 1. Model Architeture

[Nvidia's SelfDrive Car Architecture](https://devblogs.nvidia.com/parallelforall/deep-learning-self-driving-cars/) is used as final model.

The model consists of 9 layers, comprising 5 convolutional layers, a normalization layer, and 3 fully connected layers. 

To add nonlinearity, the model contains RELU layers, and in the model, the data is normalized using a Keras lambda layer.

### 2. Attempts to reduce overfitting in the model

To ensure that the model did not overfit, the model was trained and validated on multiple data sets. By putting it through the simulator and checking that the car will remain on the ground, the model was tested.

### 3. Model parameter tuning

The model used an ADAM optimizer, so that the learning rate was not manually tuned.

### 4. Appropriate training data

Udacity's Simulator collects three images from each video footage frame (Center, Left and Right cameras) 

And capture angles of steering and throttle as well, which are lables.

![Alt text][ds1]
![Alt text][ds2]
![Alt text][ds3]
![Alt text][ds4]
![Alt text][ds5]


## Solution Design Approach

To train the model, I used the Middle, Left and Right Camera photos. I used data augmentation strategy, using numpy flip feature, to feed model with more data. Finally, the data set was shuffled arbitrarily and 20 percent of the data was inserted into a validation set.

The ultimate technique for deriving a vehicle architecture was to use captured images and steering angles to train a deep neural network and to operate a car autonomously on a simulator using the learned model. 

I divided my picture and steering angle data into a training and validation set in order to measure how well the model was performing.

I changed the model that inserted Dropout in Convolution and Fully Connected Layers to fight overfitting.

Then I trained model using my local machine.


## Results

To see how far the car was going along track one, the final move was to run the simulator. 

The car is able to travel autonomously along the track without leaving the road at the end of the process.

