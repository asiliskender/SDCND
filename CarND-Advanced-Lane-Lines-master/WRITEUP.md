# Self-Driving Car Engineer Nanodegree Program
## Advanced Lane Finding Project

The goals / steps of this project are the following:

* Calculate the matrix of camera calibration and coefficients of distortion given a set of chessboard images. 
* For raw images, apply a distortion correction. 
* To construct a thresholded binary image, use color transformations, gradients, etc. 
* Apply a transformation perspective to right binary images ("birds-eye view"). 
* Detect and match lane pixels to locate the lane boundary.
* Assess the curvature relative to the middle of the lane and vehicle location. 
* Warp the limits of the detected lane back to the original image. 
* Visual display of lane boundaries and numerical calculation of the curvature of the lane and the location of the vehicle.

[//]: # (Image References)

[im01]: ./output_images/1.png "Chessboard Calibration"
[im02]: ./output_images/2.png "Undistorted Chessboard"
[im03]: ./output_images/3.png "Undistorted Dashcam Image"
[im04]: ./output_images/4.png "Perspective Transform"
[im05]: ./output_images/5.png "Colorspace Exploration"
[im06]: ./output_images/6.png "Colorspace Exploration"
[im07]: ./output_images/7.png "Colorspace Exploration"
[im08]: ./output_images/8.png "Processing Pipeline for All Test Images"
[im09]: ./output_images/9.png "Sliding Window Polyfit"
[im10]: ./output_images/10.png "Polyfit Using Previous Fit"
[im11]: ./output_images/11.png "Data Drawn onto Original Image"


[video1]: ./project_video_output.mp4 "Video"

---
### Writeup / README

#### 1. Provide a Writeup / README that includes all the rubric points and how you addressed each one.  You can submit your writeup as markdown or pdf.  [Here](https://github.com/udacity/CarND-Advanced-Lane-Lines/blob/master/writeup_template.md) is a template writeup for this project you can use as a guide and a starting point.  

You're reading it!

### Camera Calibration

#### 1. Briefly state how you computed the camera matrix and distortion coefficients. Provide an example of a distortion corrected calibration image.

The code for this step is contained in the first two code cells of the Jupyter notebook `project.ipynb`.  

The 'findChessboardCorners' and 'calibrateCamera' OpenCV functions are the backbone of the calibration of the picture. Arrays of object points, corresponding to the position (essentially indices) of a chessboard's internal corners, and image points, the inner chessboard corners' pixel positions determined by 'findChessboardCorners', are fed to 'calibrateCamera' which returns calibration and distortion coefficients of the camera. The OpenCV 'undistort'function can then be used to reverse the effects of distortion on any image created by the same camera.

![alt text][im01]

*Note: Some of the images of the chessboard do not appear because the desired number of internal corners could not be identified by 'findChessboardCorners'.* 

The picture below shows the effects of applying 'undistort' to one of the chessboard images, using the calibration and distortion coefficients:

![alt text][im02]

### Pipeline (single images)

#### 1. Provide an example of a distortion-corrected image.

The picture below displays the effects of the application of 'undistort' to one of the photos of the project dashcam: 

![alt text][im03]

The effect of 'undistort' is slight, but the difference in the shape of the car's hood at the bottom corners of the picture can be perceived.

#### 2. Describe how (and identify where in your code) you used color transforms, gradients or other methods to create a thresholded binary image.  Provide an example of a binary image result.

In multiple color spaces, I researched several mixtures of sobel gradient thresholds and color channel thresholds. These are clearly identified in the notebook of Jupyter. An example of the mixture of sobel magnitude and direction thresholds is given below: 

The image below shows different channels with three different color spaces for the same image:

![alt text][im05]
![alt text][im06]
![alt text][im07]


Ultimately, I wanted to use only the HLS color space L channel to isolate white lines and the LAB colorspace B channel to isolate yellow lines. In my pipeline, I did not use any gradient thresholds. Nevertheless, I finely tuned the threshold for each channel to be minimally tolerant of lighting changes.As part of this, I have chosen to normalize the maximum values of the HLS L channel and the LAB B channel (presumably occupied by lane lines) to 255 because, depending on the lighting conditions, the values of the lane lines inside these channels can differ. 


*Note: if the maximum value for an image was less than 175, the B channel was not normalized, meaning that no yellow was actually present in the image. The resulting threshold binary image was otherwise very noisy.* 

The results of applying the binary thresholding pipeline to different sample images are given below:

![alt text][im08]

#### 3. Describe how (and identify where in your code) you performed a perspective transform and provide an example of a transformed image.

The function 'unwarp()' takes a picture ('img'), as well as source ('src') and destination ('dst') points as inputs. I decided to hardcode the points of the source and destination.

I had considered identifying source and destination points programmatically, but I felt that I would get better results by carefully choosing points using one of the 'straight-lines' test images for comparison and assuming that the direction of the camera would remain constant and that the road in the videos would remain relatively flat. The picture below shows the effects of the transformation of perspective:

![alt text][im04]

#### 4. Describe how (and identify where in your code) you identified lane-line pixels and fit their positions with a polynomial?

Sliding-window-polyfit and polyfit-using-prev-fit functions define lane lines and fit both right and left lane lines with a second order polynomial. A histogram of the lower half of the picture is calculated by the first of these and finds the bottom-most x position (or "base") of the left and right lane lines. These positions were originally defined from the local limit of the left and right halves of the histogram, but I changed these to quarters of the histogram only left and right of the midpoint in my final execution. This helped to exclude lines from neighboring lanes. The feature then defines fifteen windows from which lane pixels can be detected, each based from the window below on the midpoint of the pixels. This "follows" the lane lines up to the top of the binary image effectively, and speeds up the processing by looking only for active pixels over a small part of the image.Pixels belonging to each lane line are identified and each set of pixels is equipped with a second order polynomial by the Numpy' polyfit()' process. The following picture illustrates how this method works:

![alt text][im09]


The 'polyfit' function effectively performs the same job, but alleviates most of the search process complexity by leveraging a previous fit (for example, from a previous video frame) and then looking for lane pixels within a certain range of that fit. The image below demonstrates this - the green shaded area is the range from the previous fit, and the yellow lines and red and blue pixels are from the current image:

![alt text][im10]

#### 5. Describe how (and identify where in your code) you calculated the radius of curvature of the lane and the position of the vehicle with respect to center.

```
curve_radius = ((1 + (2*fit[0]*y_0*y_meters_per_pixel + fit[1])**2)**1.5) / np.absolute(2*fit[0])
```
In this case,' fit[0]' is the first coefficient of the second order polynomial fit (y-squared coefficient), and 'fit[1]' is the second coefficient of (y). In the image on which the curvature measurement is based,' y 0' is the y position (the bottom-most y - the position of the car in the image - was chosen).The factor used to convert pixels to meters is 'y-meters-per-pixel.' This conversion was also used to produce a new fit in meter terms with coefficients. 

The location of the vehicle with respect to the middle of the lane shall be determined by means of the following code lines:
```
lane_center_position = (r_fit_x_int + l_fit_x_int) /2
center_dist = (car_position - lane_center_position) * x_meters_per_pix
```
The right and left fits are the x-intercepts of 'r fit x int' and 'l fit x int' respectively. This includes measuring the fit at the highest y value (719, in this case - the bottom of the image) since the minimum y value is actually at the top of the image (otherwise, the constant coefficient of each fit would have sufficed).The position of the car is the distinction between these intercept points and the midpoint of the picture (assuming that the camera is mounted at the center of the vehicle).

#### 6. Provide an example image of your result plotted back down onto the road such that the lane area is identified clearly.

Based on plots of left and right fits, a polygon is created, warped back to the perspective of the original image using the 'Minv' inverse perspective matrix and overlaid on the original image. An example of the effects of the 'draw lane' function is the picture below.

![alt text][im11]

---

### Pipeline (video)

#### 1. Provide a link to your final video output.  Your pipeline should perform reasonably well on the entire project video (wobbly lines are ok but no catastrophic failures that would cause the car to drive off the road!).

Here's a [link to my video result](./project_video_output.mp4)

---

### Discussion

#### 1. Briefly discuss any problems / issues you faced in your implementation of this project.  Where will your pipeline likely fail?  What could you do to make it more robust?

The issues I faced were almost entirely due to lighting conditions, shadows, discoloration, etc. To get the pipeline to function well on the original project film, except on the lighter-gray bridge parts that constituted the most complicated sections of the video, it was not difficult to dial in threshold parameters.

It was attempting to apply the same pipeline to the video of the challenge that faced the greatest challenge (ahem). Lane lines do not always have the same pixel value when white lines are not properly contrasted with the rest of the image. This would undoubtedly be a concern in the snow or in a case where a bright white car was driving between dull white lanes, for instance. 

It was of utmost significance to create a pipeline from which lane lines can be accurately established, but it also helped to smooth the video output by averaging the last 'n' found good matches. If the left and right base points are not a certain distance apart (within any tolerance) under the assumption that the lane width would stay relatively constant, my approach also invalidates fits.
