import csv
import numpy as np
import os
import sklearn
from PIL import Image
from sklearn.model_selection import train_test_split
from random import shuffle

from keras.models import Sequential
from keras.layers import Flatten, Dense, Lambda, Cropping2D, Dropout
from keras.layers.convolutional import Conv2D
from keras.layers.pooling import MaxPooling2D
import tensorflow as tf
import keras


lines = []
with open('data/driving_log.csv') as csvfile:
	reader = csv.reader(csvfile)
	for line in reader:
		lines.append(line)

length=len(lines)

lines=lines[1:]

shuffle(lines)

train_samples, validation_samples = train_test_split(lines, test_size=0.2)

def generator(lines, batch_size=32):
	num_samples = len(lines)
	while 1:
		for offset in range(0, num_samples, batch_size):
			batch_samples = lines[offset:offset+batch_size]
			images = []
			measurements = []
			correction = 0.2
			for line in batch_samples:
				
				source_path_center = line[0].split('/')[-1]
				source_path_left = line[1].split('/')[-1]
				source_path_right = line[2].split('/')[-1]
				
				path_center = 'data/IMG/' + source_path_center
				path_left = 'data/IMG/' + source_path_left
				path_right = 'data/IMG/' + source_path_right
				
				image_center = Image.open(path_center)
				image_left = Image.open(path_left)
				image_right = Image.open(path_right)
				
				images.append(np.asarray(image_center))
				images.append(np.asarray(image_left))
				images.append(np.asarray(image_right))
				
				image_center.close()
				image_left.close()
				image_right.close()
				
				measurement = float(line[3])
				measurements.append(measurement)
				measurements.append(measurement+correction)
				measurements.append(measurement-correction)
				
			augmented_images, augmented_measurements = [], []
			for image, measurement in zip(images, measurements):
				augmented_images.append(image)
				augmented_measurements.append(measurement)
				augmented_images.append(np.fliplr(image))
				augmented_measurements.append(measurement*-1.0)
				
			X_train = np.array(augmented_images)
			y_train = np.array(augmented_measurements)

			yield sklearn.utils.shuffle(X_train, y_train)
            
            

train_generator = generator(train_samples)
validation_generator = generator(validation_samples)

sess = tf.Session(config=tf.ConfigProto(log_device_placement=True))

model = Sequential()
model.add(Lambda(lambda x: x / 255.0 - 0.5, input_shape=(160,320,3)))
model.add(Cropping2D(cropping=((70,25), (0,0))))

model.add(Conv2D(24,5,5, activation='relu'))
model.add(MaxPooling2D())
model.add(Dropout(0.1))
model.add(Conv2D(36,5,5, activation='relu'))
model.add(MaxPooling2D())
model.add(Dropout(0.1))
model.add(Conv2D(48,5,5, activation='relu'))
model.add(Dropout(0.1))
model.add(Conv2D(64,3,3, activation='relu'))
model.add(Dropout(0.1))
model.add(Conv2D(64,3,3, activation='relu'))
model.add(MaxPooling2D())
model.add(Dropout(0.1))

model.add(Flatten())
model.add(Dense(100))
model.add(Dropout(0.2))
model.add(Dense(50))
model.add(Dropout(0.2))
model.add(Dense(10))
model.add(Dense(1))

model.compile(loss='mse', optimizer='adam')

model.fit_generator(train_generator,
                    verbose=1,
                    samples_per_epoch= len(6*train_samples),
                    validation_data=validation_generator,
                    nb_val_samples=len(6*validation_samples),
                    epochs=1)

model.save('model.h5')