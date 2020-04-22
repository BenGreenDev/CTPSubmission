import numpy as np
import cv2
import json
import math
import matplotlib.image as mpimg
import sys

minLineLength = int(sys.argv[1])
maxLineGap = int(sys.argv[2])
detectionThreshold = int(sys.argv[3])
_minDistToMerge = int(sys.argv[4])
_minMergeAngle = int(sys.argv[5])
imageLocation = sys.argv[6]
saveLocation = sys.argv[7]
shouldLoadSymbols = sys.argv[8]

#default variables for testing script not calling from c# project
# minLineLength = 5
# maxLineGap = 10
# detectionThreshold = 30
# _minDistToMerge = 2
# _minMergeAngle = 5
# imageLocation = "C:\\Users\\ben11\\Documents\\BuildingGenerator\\Test images\\GroundFloorNoSymbols.jpg"
# saveLocation = "C:\\Users\\ben11\\Documents\\BuildingGenerator\\Test images\\"
# shouldLoadSymbols = False


def getLines(lines_in):
    if cv2.__version__ < '3.0':
        return lines_in[0]
    return [l[0] for l in lines_in]


#Function responsible for saving to json along with angle of rotation
def pointstojson(pointlist):
    data = []
    metaData = []
    for point in pointlist:
        startX = point[0][0]
        endX = point[1][0]
        startY = point[0][1]
        endY = point[1][1]

        angleradians = math.atan2(startY - endY, startX - endX)
        angledegrees = math.degrees(angleradians)
        x_m_point = (startX + endX) / 2
        y_m_point = (startY + endY) / 2
        data.append({'startX': int(startX), 'startY': int(startY), 'endX': int(endX), 'endY': int(endY), 'centerpointX': int(x_m_point), 'centerpointY': int(y_m_point), 'angle': int(angledegrees)})

    metaData.append({'shouldLoadSymbols': shouldLoadSymbols})
    with open(saveLocation + '\\data.json', 'w', encoding='utf-8') as f:
        json.dump({'metaData': metaData, 'points': data}, f, ensure_ascii=False, indent=4)


#Our main processing function
def processLines():
    img = cv2.imread(imageLocation)
    img = np.array(img, dtype=np.uint8)
    
    #greyscale
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    kernalSize = 5
    #noise filteration
    blurGray = cv2.GaussianBlur(gray, (kernalSize, kernalSize), 0)

    #Edge detection for reference points for probabilistic hough line analysis
    edges = cv2.Canny(blurGray, 50, 150)

    #Variables here are fed in from the c# application
    discoveredLines = cv2.HoughLinesP(edges, rho=1, theta=np.pi/180, threshold=detectionThreshold,
                            minLineLength=minLineLength, maxLineGap=maxLineGap)

    for line in getLines(discoveredLines):
        leftX, botY, rightX, topY = line
        cv2.line(img, (leftX, botY), (rightX,topY), (0,0,255), 6)

    lines = []
    for line in getLines(discoveredLines):
        lines.append([(line[0], line[1]),(line[2], line[3])])

    # sort lines into horizontal and vertical groupings for merging with one another
    lineX = []
    lineY = []

    for line in lines:
        orientation = math.atan2((line[0][1]-line[1][1]),(line[0][0]-line[1][0]))
        if (abs(math.degrees(orientation)) > 45) and abs(math.degrees(orientation)) < (90+45):
            lineY.append(line)
        else:
            lineX.append(line)

    lineX = sorted(lineX, key=lambda _line: _line[0][0])
    lineY = sorted(lineY, key=lambda _line: _line[0][1])

    # Merge lines that go vertical
    mergeLinesY = mergeLines(lineY)

    #Merge lines that go horizontal
    mergedLinesX = mergeLines(lineX)

    #Put all lines back into one list
    mergedLines = []
    mergedLines.extend(mergedLinesX)
    mergedLines.extend(mergeLinesY)

    # Print how many lines were removed compared to how many were initally found
    print("Number of lines at start", len(lines), " Number of lines after merge ", len(mergedLines))
    mergedLineImg = mpimg.imread(imageLocation)

    #Overlay our discovered lines atop the original image, and resave a copy
    for line in mergedLines:
        cv2.line(mergedLineImg, (line[0][0], line[0][1]), (line[1][0],line[1][1]), (0,0,255), 6)

    cv2.imwrite('mergedLines.jpg', mergedLineImg)

    #Output our JSON data
    pointstojson(mergedLines)

    return mergedLines

def mergeLines(lines):
    finalLines = []
    combinedLines = []
    #Defaults 10 and 45
    minDistToMerge = _minDistToMerge
    minAngle = _minMergeAngle

    #Iterate through all lines and create a new group
    for line in lines:
        create_new_group = True
        group_updated = False

        #If orientation of a line is less than the min angle difference of a line in a group, check if it is close enough to be added to the group
        for lineGroup in combinedLines:
            for linesInGroup in lineGroup:
                if getDistance(linesInGroup, line) < minDistToMerge:
                    #Angle of current line and lines in group
                    orientation_i = math.atan2((line[0][1]-line[1][1]),(line[0][0]-line[1][0]))
                    orientation_j = math.atan2((linesInGroup[0][1]-linesInGroup[1][1]),(linesInGroup[0][0]-linesInGroup[1][0]))

                    if int(abs(abs(math.degrees(orientation_i)) - abs(math.degrees(orientation_j)))) < minAngle:
                        lineGroup.append(line)

                        create_new_group = False
                        group_updated = True
                        break

            if group_updated:
                break

        #If line found with a similar orientation to other lines is also close enough, add line to group and append to combinedLine list
        if (create_new_group):
            newLineGroup = []
            newLineGroup.append(line)

            for idx, linesInGroup in enumerate(lines):
                # check the distance between lines
                if getDistance(linesInGroup, line) < minDistToMerge:
                    # check the angle between lines
                    orientation_i = math.atan2((line[0][1]-line[1][1]),(line[0][0]-line[1][0]))
                    orientation_j = math.atan2((linesInGroup[0][1]-linesInGroup[1][1]),(linesInGroup[0][0]-linesInGroup[1][0]))

                    if int(abs(abs(math.degrees(orientation_i)) - abs(math.degrees(orientation_j)))) < minAngle:
                        newLineGroup.append(linesInGroup)

            combinedLines.append(newLineGroup)

    #Take all grouped lines, and return just the start and end position as a new line
    for lineGroup in combinedLines:
        finalLines.append(mergeLineSegments(lineGroup))

    return finalLines


def mergeLineSegments(lines):
    if(len(lines) == 1):
        return lines[0]

    #Use first line to gain an understanding of what this line groups orientation is
    firstLine = lines[0]

    orientation = math.atan2((firstLine[0][1]-firstLine[1][1]),(firstLine[0][0]-firstLine[1][0]))

    linePoints = []
    for line in lines:
        linePoints.append(line[0])
        linePoints.append(line[1])

    if (abs(math.degrees(orientation)) > 45) and abs(math.degrees(orientation)) < (90+45):
        linePoints = sorted(linePoints, key=lambda point: point[1])
    else:
        linePoints = sorted(linePoints, key=lambda point: point[0])

    return [linePoints[0], linePoints[len(linePoints)-1]]


#Get vector magnitude of the line
#https://stackoverflow.com/questions/32502494/get-the-magnitude-of-a-vector-x-y
#magnitude = math.sqrt(sum(v**2 for v in vector))
def getLineMagnitude(x1, y1, x2, y2):
    lineMagnitude = math.sqrt(math.pow((x2 - x1), 2) + math.pow((y2 - y1), 2))
    return lineMagnitude


#Distance point line
#https://brilliant.org/wiki/dot-product-distance-between-point-and-a-line/
#https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
def DotProductPointDistance(px, py, x1, y1, x2, y2):
    LineMag = getLineMagnitude(x1, y1, x2, y2)

    #If line magnitude is ludicrously small, ignore this point for it's probably just a dot / noise
    if LineMag < 0.00000001:
        DotProductPointDistance = 9999
        return DotProductPointDistance

    u1 = (((px - x1) * (x2 - x1)) + ((py - y1) * (y2 - y1)))
    u = u1 / (LineMag * LineMag)

    if (u < 0.00001) or (u > 1):
        #Does not fall within line segment
        ix = getLineMagnitude(px, py, x1, y1)
        iy = getLineMagnitude(px, py, x2, y2)
        if ix > iy:
            DotProductPointDistance = iy
        else:
            DotProductPointDistance = ix
    else:
        # Intersecting point is on the line, use the formula
        ix = x1 + u * (x2 - x1)
        iy = y1 + u * (y2 - y1)
        DotProductPointDistance = getLineMagnitude(px, py, ix, iy)

    return DotProductPointDistance


def getDistance(line1, line2):
    distance1 = DotProductPointDistance(line1[0][0], line1[0][1],
                              line2[0][0], line2[0][1], line2[1][0], line2[1][1])
    distance2 = DotProductPointDistance(line1[1][0], line1[1][1],
                              line2[0][0], line2[0][1], line2[1][0], line2[1][1])
    distance3 = DotProductPointDistance(line2[0][0], line2[0][1],
                              line1[0][0], line1[0][1], line1[1][0], line1[1][1])
    distance4 = DotProductPointDistance(line2[1][0], line2[1][1],
                              line1[0][0], line1[0][1], line1[1][0], line1[1][1])

    return min(distance1,distance2,distance3,distance4)


processLines()
