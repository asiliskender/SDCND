# **Finding Lane Lines on the Road** 

---

**Finding Lane Lines on the Road**

The goals / steps of this project are the following:
* Make a pipeline that finds lane lines on the road
* Reflect on your work in a written report


[//]: # (Image References)

[image1]: ./examples/grayscale.jpg "Grayscale"

---

### Reflection

### 1. Pipeline

1. Covert the image to grayscale.
2. Apply Gaussian blur to make the noises in image smooth.
3. Apply canny detection to detect the edge of the image.
4. Mask the region of interest out of the image.
5. Apply hough algorithm to detect lines in the image.
6. Merge the original image and lines generated at step 5.

In order to draw a single line on the left and right lanes, draw_lines() function is modified. All the slope and bias are computed about left and right lines, then unnecessary resutls are deleted. At last mean of the slopes and bias are calculated and a line according to the mean slope and bias is drawed.

### 2. Identify potential shortcomings with your current pipeline

One potential shortcoming would be robustness, it can only detect the line in the Region of interest.

Other potential shortcoming is cury roads and lanemarks.


### 3. Suggest possible improvements to your pipeline

A possible improvement would be to change the parameters (a second or third degree polynom) to overcome some situations.

Another potential improvement could be to use other technology to do this job, ie. deep learning techniques.
