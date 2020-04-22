import numpy as np
import matplotlib.pyplot as plt
import os
import cv2
import random
import pickle

DATADIR = "BlueprintIcons"
CATAGORIES = ["wall", "door", "window"]


for category in CATAGORIES:
    # for each catagory file in our path
    path = os.path.join(DATADIR, category)
    for img in os.listdir(path):
        img_array = cv2.imread(os.path.join(path, img), cv2.IMREAD_GRAYSCALE)
        plt.imshow(img_array, cmap="gray")
        plt.show()
        break
    break

# Resize all images to the same size
IMG_SIZE = 50
new_array = cv2.resize(img_array, (IMG_SIZE, IMG_SIZE))
plt.imshow(new_array, cmap="gray")
plt.show()

# create training data
training_data = []


def create_training_data():
    for category2 in CATAGORIES:
        # for each catagory file in our path
        path2 = os.path.join(DATADIR, category2)
        class_num = CATAGORIES.index(category2)
        for img2 in os.listdir(path2):
            try:
                img_array2 = cv2.imread(os.path.join(path2, img2), cv2.IMREAD_GRAYSCALE)
                new_array2 = cv2.resize(img_array2, (IMG_SIZE, IMG_SIZE))
                training_data.append([new_array2, class_num])
            except Exception as e:
                pass


create_training_data()

random.shuffle(training_data)

# Array of images as grayscale, and identifiers
X = []
Y = []

for features, label in training_data:
    X.append(features)
    Y.append(label)
X = np.array(X).reshape(-1, IMG_SIZE, IMG_SIZE, 1)

# Save our data
pickle_out = open("X.pickle", "wb")
pickle.dump(X, pickle_out)
pickle_out.close()

pickle_out = open("Y.pickle", "wb")
pickle.dump(Y, pickle_out)
pickle_out.close()

print("Finished")