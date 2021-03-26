import cv2
import numpy as np
import os

# Reiterate the process applied to the first image to the whole video
# On récupère les commandes appliquées à la première image dans keypressed.txt pour les appliquer au reste de la vidéo
def Reiteration():
    imageCount = 0
    while True :
        imgNext = cap.read()
        print(imgNext)
        if imgNext == False :
            print("End Of the Program")
            quit()
        imgNext = cv2.cvtColor(imgNext[1], cv2.COLOR_BGR2GRAY)
        dilateState = 0
        erodeState = 0
        treshGval = keyPressed[len(keyPressed)-1]

        # Binarisation, Function dilate & erode Application
        (threshG, imgNext) = cv2.threshold(imgNext, float(treshGval), 255, cv2.THRESH_BINARY)
        for i in range(len(keyPressed)-1):
            tmp = keyPressed[i]
            if i%2 == 0 :
                if tmp == "d":
                    dilateState = 1
                    erodeState = 0
                elif tmp == "e":
                    erodeState = 1
                    dilateState = 0
            else:
                kernel1 = np.ones((int(keyPressed[i]), int(keyPressed[i])), np.uint8)
                if dilateState :
                    imgNext = cv2.dilate(imgNext, kernel1, iterations=1)
                elif erodeState:
                    imgNext = cv2.erode(imgNext, kernel1, iterations=1)
        if not cv2.imwrite("imageFiltered/frame{val}.jpg".format(val=imageCount), imgNext):
            raise Exception("Could not write image")
        # Harris Edge Corner Application
        corners = cv2.cornerHarris(np.float32(imgNext), HarrisParameters[0], HarrisParameters[1], np.float32(HarrisParameters[2]/100))
        img2 = cv2.imread('imageFiltered/frame{val}.jpg'.format(val=imageCount))
        corners2 = cv2.dilate(corners, None, iterations=3)
        img2[corners2 > 0.01 * corners2.max()] = [255, 0, 0]
        np.savetxt("Corners/corners{val}.txt".format(val=imageCount), corners2)
        imageCount = imageCount+1
        print(imageCount)
        cv2.imshow("image", img2)

def nothing(x):
    pass

# Fetch Image from Video
##############################################################################################

# Change initial video directory in here :
#os.chdir("D:\\ESIEA\\4A\\Cap Projet\\NebulaRobot\\VideoInput")

# Change video name in here :
cap = cv2.VideoCapture("LittleFile.mp4")

##############################################################################################

img = cap.read()
img1 = cv2.cvtColor(img[1], cv2.COLOR_BGR2GRAY)

# Instantiate program directory space
dir_path = os.path.dirname(os.path.realpath(__file__))
path0 = os.path.join(dir_path, "imageFiltered")
os.makedirs(path0, exist_ok=True)
path1 = os.path.join(dir_path, "Corners")
os.makedirs(path1, exist_ok=True)

# create State1 TrackBar
cv2.namedWindow('Parameters0')
cv2.resizeWindow("Parameters0",640,240)
cv2.createTrackbar('treshold0','Parameters0',0,255,nothing)
cv2.setTrackbarPos('treshold0','Parameters0',68)
cv2.createTrackbar('kernelSize','Parameters0',0,31,nothing)
cv2.setTrackbarPos('kernelSize','Parameters0',5)

# create State2 TrackBar
def State2TrackBar():
    cv2.namedWindow('Parameters')
    cv2.resizeWindow("Parameters",640,240)
    cv2.createTrackbar('blocksize','Parameters',1,255,nothing)
    cv2.createTrackbar('ksize','Parameters',0,31,nothing)
    cv2.createTrackbar('k','Parameters',4,6,nothing)
    cv2.setTrackbarPos('blocksize','Parameters',2)
    cv2.setTrackbarPos('ksize','Parameters',3)
    cv2.setTrackbarPos('k','Parameters',6)

# kernels
kernel = np.ones((5,5),np.uint8)

# States
state1 = 1
state2 = 0
tmpstate2 = 0

# On enregistre les données pour ensuite pouvoir appliquer la même configuration appliqué à l'image 1, sur le
# reste des images de la vidéo, il est important de choisir une vidéo aillant une propriété "homogène" d'arrière plan
# saved Data
imgSaved = img1
keyPressed = []
binaryConversionSeuil = 0
HarrisParameters = []

# L'appuie bouton n'est pas géré en interruption, veuillez resté appuyer longtemps sur les touches ou réitérer l'appui
while True:
    key = cv2.waitKey(1)
    # State 1 : Choose the correct Treshold for grey scale initialisation
    if state1 :
        # Image gray scale conversion
        seuil = cv2.getTrackbarPos('treshold0', 'Parameters0')
        (thresh, img) = cv2.threshold(img1, seuil, 255, cv2.THRESH_BINARY)

        kernelSize = cv2.getTrackbarPos('kernelSize', 'Parameters0')
        kernel1 = np.ones((kernelSize, kernelSize), np.uint8)
        print(kernelSize)
        print(kernel1)
        # When D is pressed execute dilatation on current image, on enregistre l'appui et la valeur du kernel appliqué
        if key == ord('d'):
            img1 = cv2.dilate(img, kernel1, iterations=1)
            keyPressed.append("d")
            keyPressed.append(f'{kernelSize}')
        # When E is pressed execute erode on current image, on enregistre l'appui et la valeur du kernel appliqué
        if key == ord('e'):
            img1 = cv2.erode(img, kernel1, iterations=1)
            keyPressed.append("e")
            keyPressed.append(f'{kernelSize}')
        # When N is pressed we go to next step, on enregistre l'image
        if key == ord('n'):
            binaryConversionSeuil = seuil
            imgSaved = img1
            cv2.imwrite("frame.jpg",imgSaved)
            cv2.destroyWindow('Parameters0')
            state1 = 0
            state2 = 1
        # When R is pressed, on réinitialise l'image
        if key == ord('r'):
            img1 = imgSaved
            keyPressed = []
        # image visualisation
        cv2.imshow("image", img)
    # State 2 : Choose the correct Tresholds for Harris Corner Algorithm
    elif state2 :
        if tmpstate2 == 0 :
            State2TrackBar()
        # Retrieving parameter from Parameters window && initialising Harris Algorithm
        blocksize = cv2.getTrackbarPos('blocksize', 'Parameters')
        ksize = cv2.getTrackbarPos('ksize', 'Parameters')
        k = cv2.getTrackbarPos('k', 'Parameters')
        if ksize%2 == 0 :
            ksize += 1
            cv2.setTrackbarPos('ksize', 'Parameters', ksize)
        if blocksize <= 0:
            blocksize += 1
            cv2.setTrackbarPos('blocksize', 'Parameters', blocksize)
        corners = cv2.cornerHarris(np.float32(img), blocksize, ksize, np.float32(k/100))

        img2 = cv2.imread('frame.jpg')

        corners2 = cv2.dilate(corners, None, iterations=3)
        img2[corners2 > 0.01 * corners2.max()] = [255, 0, 0]
        # image visualisation
        cv2.imshow("image", img2)
        tmpstate2 +=1
        if key == ord('n'):
            # Save Harris Parameter
            HarrisParameters.append(blocksize)
            HarrisParameters.append(ksize)
            HarrisParameters.append(k)
            # Save Corner of first image into corners2.txt
            np.savetxt("corners2.txt",corners2)
            # Save function applied to first image and "seuil" value into corners2.txt
            tmp = f'{binaryConversionSeuil}'
            keyPressed.append(tmp)
            np.savetxt("keypressed.txt", keyPressed, fmt="%s")
            # Reiteration du process sur le reste des images de la vidéo
            cv2.destroyWindow('Parameters')
            Reiteration()

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break