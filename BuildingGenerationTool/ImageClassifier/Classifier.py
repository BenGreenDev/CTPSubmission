import cv2
import tensorflow as tf
import os
import numpy as np
import matplotlib.pyplot as plt
import json
import math
import sys

imageHeight = int(sys.argv[1])
imageWidth = int(sys.argv[2])
imageDirectory = sys.argv[3]
saveDirectory = sys.argv[4]

strideAmount = 10
CATEGORIES = ["wall", "door", "window"]

def getCatagories():
    directory_list = list()
    for root, dirs, files in os.walk("BlueprintIcons", topdown=False):
        for name in dirs:
            directory_list.append(name)

    catagories = np.asarray(directory_list)
    return catagories

def prepare(filepath):
    IMG_SIZE = 50
    img_array = cv2.imread(filepath, cv2.IMREAD_GRAYSCALE)
    new_array = cv2.resize(img_array, (IMG_SIZE, IMG_SIZE))
    return new_array.reshape(-1, IMG_SIZE, IMG_SIZE, 1)


def identify():
    model = tf.keras.models.load_model("CNN.model")
    prediction = model.predict([prepare('jj.jpg')])
    currentMax = 0
    currentMaxVal = 0
    currentIndex = 0

    for pred in prediction[0]:
        if pred > currentMax:
            currentMax = currentIndex
            currentMaxVal = pred
        currentIndex = currentIndex + 1

    print(prediction[0])
    print(currentMaxVal)
    if currentMaxVal > 0.8:
        result = CATEGORIES[currentMax]
    else:
        result = CATEGORIES[2]

    return result


def identifySegment(listOfImages):
    model = tf.keras.models.load_model("CNN.model")

    pointsFound = []

    for img2 in listOfImages:
        try:
            print("Checking Image " + img2)
            prediction = model.predict([prepare(img2)])
            currentMax = 0
            currentMaxVal = 0
            currentIndex = 0

            print(prediction)
            for pred in prediction[0]:
                if pred > currentMaxVal:
                    currentMax = currentIndex
                    currentMaxVal = pred
                currentIndex = currentIndex + 1

            #Detect if windows and doors probable beyond 0.85
            if prediction[0][2] > 0.85:
                result = CATEGORIES[2]

                fileNameNoExtension = img2.split(".")[0]
                fileNameNoRoot = fileNameNoExtension.split("/")[1]
                yPos = fileNameNoRoot.split("-")[0]
                xPos = fileNameNoRoot.split("-")[1]
                pointsFound.append([result, 2, int(xPos), int(yPos), prediction[0][2]])
            elif prediction[0][1] > 0.85 and abs(prediction[0][0] - prediction[0][1]) < 0/3:
                result = CATEGORIES[1]

                fileNameNoExtension = img2.split(".")[0]
                fileNameNoRoot = fileNameNoExtension.split("/")[1]
                yPos = fileNameNoRoot.split("-")[0]
                xPos = fileNameNoRoot.split("-")[1]
                pointsFound.append([result, 1, int(xPos), int(yPos), prediction[0][2]])

        except Exception as e:
            pass

    print(pointsFound)
    removeDuplicated(pointsFound)

# Simulate k-means clustering with x and y coordinates
# Need to change value here to load from the JSON
def removeDuplicated(pointsFound):

    finalList = []
    removedIndex = []
    for index, foundObject in enumerate(pointsFound):
        xPositions = []
        yPositions = []
        xPositions.append(foundObject[3])
        yPositions.append(foundObject[2])

        hasBeenCheckedAlready = False
        if index in removedIndex:
            hasBeenCheckedAlready = True

        if not hasBeenCheckedAlready:
            for idx, otherObjects in enumerate(pointsFound):
                if abs(foundObject[2] - otherObjects[2]) < strideAmount * (imageHeight / strideAmount) and abs(foundObject[3] - otherObjects[3]) < strideAmount * (imageWidth / strideAmount):
                    yPositions.append(otherObjects[2])
                    xPositions.append(otherObjects[3])
                    removedIndex.append(idx)

            foundObject[2] = Average(yPositions)
            foundObject[3] = Average(xPositions)
            finalList.append(foundObject)

    printfoundsegments(finalList)


def Average(lst):
    return sum(lst) / len(lst)


def printfoundsegments(pointsFound):
    data = []

    for symbol in pointsFound:
        data.append({'objectFound': symbol[0], 'enumNumber': symbol[1], 'xPos': symbol[2], 'yPos': symbol[3]})

        with open(saveDirectory + '\\symbols.json', 'w', encoding='utf-8') as f:
            json.dump({'symbolsFound': data}, f, ensure_ascii=False, indent=4)


def sliceImage(height, width, stride_height, stride_width, filepath):
    img = cv2.imread(filepath)
    img_height, img_width = img.shape[:2]

    num_y_increments = (img_height / height) * (height / stride_height)
    num_x_increments = (img_width / width) * (width / stride_width)

    num_y_increments = math.ceil(num_y_increments)
    num_x_increments = math.ceil(num_x_increments)

    print(num_y_increments)
    print(num_x_increments)
    listOfImages = []
    i = 0

    for y in range(0, num_y_increments):
        for x in range(0, num_x_increments):
            print("current y : " + str(y*stride_height) + " current x : " + str(x*stride_width))
            crop_img = img[(y*stride_height):(y*stride_height) + height, (x*stride_width):(x*stride_width)+width]
            cv2.imwrite("SlicedImages/" + '' + str(y * stride_height) + "-" + str(x * stride_width) + ".jpg", crop_img)
            listOfImages.append("SlicedImages/" + '' + str(y * stride_height) + "-" + str(x * stride_width) + ".jpg")
            i += 1

    print(identifySegment(listOfImages))


sliceImage(imageHeight,imageWidth,strideAmount,strideAmount,imageDirectory)




