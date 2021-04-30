**Abstract**

This project shows how to classify german traffic signs using a modified LeNet neuronal network. 

The following are the phases in this project: 
* Launch the Collection of Data (see below for links to the project data set) 
* The data collection discusses, summarizes and visualizes 
* Model, train and test a model 
* Use the model to make new image predictions 
* Analyze the new photographs' softmax probabilities 


[//]: # (Image References)

[labels_with_examples]: ./fig/labels_with_examples.png "Labels and example images"
[traffic_signs_orig]: ./fig/traffic_signs_orig.png "Traffic Signs"
[learning]: ./fig/learning.png "Validation Accuracy per Epoche"
[prediction_probabilities_with_examples]: ./fig/prediction_probabilities_with_examples.png "Traffic Sign Prediction with Examples"
[prediction_probabilities_with_barcharts]: ./fig/prediction_probabilities_with_barcharts.png "Traffic Sign Prediction with Bar Charts"

### Data Set Summary & Exploration
#### 1. Dataset Summary

Numpy library is used to calculate summary statistics of the traffic
signs dataset:

* The size of training set is 34799
* The size of the validation set is 4410
* The size of test set is 12630
* The shape of a traffic sign image is (32, 32, 3)
* The number of unique classes/labels in the data set is 43

#### 2. Exploratory Visualization
For each label in the training set, the following figure shows one example picture.

![alt text][labels_with_examples]



### Design and Test a Model Architecture

#### 1. Preprocessing

I wanted to transform the images to grayscale as a first move because many images were pretty dark in the testing and contained very little color and the grayscaling decreases the number of features and therefore reduces the time of execution. In addition, with grayscaling of the pictures, some analysis papers have shown strong results.

Then I normalized the image using the '(pixel - 128)/ 128' method, which transforms each pixel's int values to [0,255] float values with a range of [-1,1]

#### 2. Model Architecture

The model architecture is the LeNet model architecture.

#### 3. Model Training
To train the model, I used an Adam optimizer and the following hyperparameters:
* batch size: 64
* number of epochs: 150
* learning rate: 0.0006
* Variables were initialized using the truncated normal distribution with mu = 0.0 and sigma = 0.1
* keep probalbility of the dropout layer: 0.5


Final results were:
* training set accuracy of 100.0%
* validation set accuracy of 94.6%
* test set accuracy of 92.9%

![alt text][Learning]

### Test a Model on New Images
#### 1. Acquiring New Images
Here are some German traffic signs I find on the internet:
![alt text][traffic_signs_orig]

#### 2. Performance on New Images
Results:

![alt text][traffic_signs_orig]

The algorithm was able to accurately guess 5 of the 5 road signals, offering 100 percent precision.


#### 3. Model Certainty - Softmax Probabilities
The top five softmax probabilities of the projections on the captured images are issued in the following images.

![alt text][prediction_probabilities_with_barcharts]

In the next picture, the comprehensive probabilities and examples of the top five softmax predictions are given.

![alt text][prediction_probabilities_with_examples]

### Possible Future Work
#### 1. Augmentation of Training Data

Augmenting the training set may further increase the efficiency of the model. Popular techniques of data augmentation include rotation, translation, zoom, flips, jitter addition, and/or disruption of colour. For the bulk of image processing operations, I would use [OpenCV](https://opencv.org).


