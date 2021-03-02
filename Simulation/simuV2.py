# -*- coding: utf-8 -*-
"""
Created on Tue Jan 19 16:27:30 2021

@author: Meyssan KANZARI 
"""


# -*- coding: utf-8 -*-
"""
Created on Tue Jan 12 17:06:43 2021

@author: Meyss
"""
import pygame
import math
import time
import matplotlib
import matplotlib.pyplot as plt
import numpy as np

WIDTH = 1200
HEIGHT = 1200

pygame.init()
SCREEN = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption('Trajectory Robot')
SCREEN.fill((255,255,255))
pygame.display.flip()

# **************************** CAN ***********************************
# resolution CAN
resol = 200

# LSB 15.32 représente la vitesse max de rotation des roues pour notre robot (en réalité c'est la roue la moins rapide de mon robot je devrais faire 2 CAN par robot)
quantum = 15.32/resol 


# **************************** PARAMETRE ROBOT **************************

# position, angle, rayon et largeur du robot
x=[600,600,600]
y=[600,600,600]
x1 = [0,0,0]
y1 = [0,0,0]
theta = [0,61,2]
r = 4
L = 15

# les vitesses sont cappé à 15.32
wg = [15,15,15]
wd = [15,15,15]
owG = [0,0,0]
owD = [0,0,0]

# boucle
boolean = True

# Initialisation timer
t0 = time.perf_counter()
t1 = 0

#***************************** P.I.D **************************************
# gain 
kp = [1,1,1] # proportionnel
ki = [1,1,1] # integral 
kd = [1,1,1] # dérivé
p = [0,0,0]  # P
i = [0,0,0]  # I
d = [0,0,0]  # D
uG = [0,0,0]
uD = [0,0,0]

cpt = 0

# Pas attribué à Te et z
pas = 0.1

# *************************** GESTION COULEUR ********************************
# RECTANGLE
def gradientRect( window, rgb_cm, target_rect ):
    colour_rect = pygame.Surface( ( target_rect.width, target_rect.height ) )       # tiny! 2x2 bitmap
    for i in range(len(rgb_cm)):
        pygame.draw.line(colour_rect,rgb_cm[i],(i,0),(i,target_rect.height))
    window.blit(colour_rect, target_rect)  

# COLOR MAP
cmap = plt.get_cmap('rainbow')
if isinstance(cmap, matplotlib.colors.LinearSegmentedColormap):
    rgb_c = cmap(np.arange(0,cmap.N))
    rgb_cm = []
    for i in rgb_c :
        rgb_cm.append(np.delete([j * 255 for j in i], 3))
elif isinstance(cmap, matplotlib.colors.ListedColormap):
    rgb_cm = cmap.colors  

# PYGAME
stopB = True
stopC = True
stopD = True
mapsize = len(rgb_cm)
rectheight = 25
valx1 = 0
valx2 = 0
valx3 = 0
linel = 30
gradientRect(SCREEN, rgb_cm,pygame.Rect(WIDTH-mapsize,HEIGHT-rectheight,mapsize,rectheight))
gradientRect(SCREEN, rgb_cm,pygame.Rect(WIDTH-mapsize,HEIGHT-(2*rectheight+10),mapsize,rectheight))
gradientRect(SCREEN, rgb_cm,pygame.Rect(WIDTH-mapsize,HEIGHT-(3*rectheight+20),mapsize,rectheight))
pygame.display.flip()
# ****************************************************************************


while(boolean):

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            boolean = False
            pygame.quit()
        if event.type == pygame.MOUSEBUTTONUP:
            pos = pygame.mouse.get_pos()
            if pos[0] in range(WIDTH-mapsize,WIDTH) and pos[1] in range(HEIGHT-rectheight,HEIGHT):
                valx1 = pos[0]-(WIDTH-mapsize)
                print("valx1 :", valx1)
            if pos[0] in range(WIDTH-mapsize,WIDTH) and pos[1] in range(HEIGHT-(2*rectheight+10),HEIGHT-rectheight):
                valx2 = pos[0]-(WIDTH-mapsize)
                print("valx2 :", valx2)
            if pos[0] in range(WIDTH-mapsize,WIDTH) and pos[1] in range(HEIGHT-(3*rectheight+20),HEIGHT-(2*rectheight+10)):
                valx3 = pos[0]-(WIDTH-mapsize)
                print("valx3 :", valx3)

    if wg[0] > 16 or wd[0] > 16 or wg[0] < -16 or wd[0] < 0:
        #print("Stop : 0")
        stopB = False

    if wg[1] > 16 or wd[1] > 16 or wg[1] < -16 or wd[1] < 0:
        #print("Stop : 1")
        stopC = False

    if wg[2] > 16 or wd[2] > 16 or wg[2] < -16 or wd[2] < 0:
        #print("Stop : 2")
        stopD = False

    # Color for Robot 1 Trajectory
    color1 = rgb_cm[valx1]

    # Color for Robot 2 Trajectory
    color2 = rgb_cm[valx2]

    # Color for Robot 3 Trajectory
    color3 = rgb_cm[valx3]

    if cpt%75 == 0 and stopB:
        wd[0]-=pas

        # timer
        t = time.perf_counter() - t0

        # ancienne valeur de wG & wD
        owG[0] = wg[0]
        owD[0] = wd[0]

        # Signal des vitesses
        #wg = math.cos(t)
        #wd = math.sin(t)
        print("t = ",t)
        print("wg = ", wg[0], "wd = ", wd[0])

        # PID pour moteur gauche
        t1 = t - t1
        p[0] = wg[0] - 0
        i[0] += wg[0]
        d[0] = (wg[0] - owG[0])/t1
        uG[0] = kp[0]*p[0] + ki[0]*i[0] + kd[0]*d[0] 

        # PID pour moteur droit
        p[0] = wd[0] - 0
        i[0] += wd[0]
        d[0] = (wd[0] - owD[0])/t1
        uD[0] = kp[0]*p[0] + ki[0]*i[0] + kd[0]*d[0]  

        # Paramètres
        v = r*(wg[0] + wd[0]) / 2     # Vitesse moyenne du robot (moyenne de la vitesse des deux roues)
        wk = r*(wg[0] - wd[0]) / L # On note que c'est égale à Dc de l'encodeur une fois multiplié par Te

        # Sortie U vers les moteurs
        if wg[0] >= 0 :
            uL = np.round(uG[0]/quantum)
        else :
            uL = np.round(-uG[0]/quantum)

        if wd[0] >= 0 :
            uR = np.round(uD[0]/quantum)
        else :
            uR = np.round(-uD[0]/quantum)

        Te =  pas # time.perf_counter() - t
        print("V =", v)
        print("uL = ", uL,"uR = ",uR)
        print("Te =", Te)

        # Old Position
        x1[0] = x[0]
        y1[0] = y[0]
        print(np.round(x1[0],2), np.round(y1[0],2))

        # Position
        x[0] = x[0] + v * math.cos(theta[0]) * Te
        y[0] = y[0] + v * math.sin(theta[0]) * Te
        theta[0] = theta[0] + wk * Te
        print(np.round(x[0],2), np.round(y[0],2))

        # Animation
        #pygame.draw.circle(SCREEN,(0,0,0),(x,y),2)
        pygame.draw.line(SCREEN,color1,(x1[0],y1[0]),(x[0],y[0]),linel)
        print("COLOR :",color1)

    if cpt%75 == 0 and stopC:
        wd[1]-=pas

        # timer
        t = time.perf_counter() - t0

        # ancienne valeur de wG & wD
        owG[1] = wg[1]
        owD[1] = wd[1]

        # Signal des vitesses
        #wg = math.cos(t)
        #wd = math.sin(t)
        print("t = ",t)
        print("wg = ", wg[1], "wd = ", wd[1])

        # PID pour moteur gauche
        t1 = t - t1
        p[1] = wg[1] - 0
        i[1] += wg[1]
        d[1] = (wg[0] - owG[0])/t1
        uG[1] = kp[1]*p[1] + ki[1]*i[1] + kd[1]*d[1] 

        # PID pour moteur droit
        p[1] = wd[1] - 0
        i[1] += wd[1]
        d[1] = (wd[1] - owD[1])/t1
        uD[1] = kp[1]*p[1] + ki[1]*i[1] + kd[1]*d[1]  

        # Paramètres
        v = r*(wg[1] + wd[1]) / 2     # Vitesse moyenne du robot (moyenne de la vitesse des deux roues)
        wk = r*(wg[1] - wd[1]) / L # On note que c'est égale à Dc de l'encodeur une fois multiplié par Te

        # Sortie U vers les moteurs
        if wg[1] >= 0 :
            uL = np.round(uG[1]/quantum)
        else :
            uL = np.round(-uG[1]/quantum)

        if wd[1] >= 0 :
            uR = np.round(uD[1]/quantum)
        else :
            uR = np.round(-uD[1]/quantum)

        Te =  pas # time.perf_counter() - t
        print("V =", v)
        print("uL = ", uL,"uR = ",uR)
        print("Te =", Te)

        # Old Position
        x1[1] = x[1]
        y1[1] = y[1]
        print(np.round(x1[1],2), np.round(y1[1],2))

        # Position
        x[1] = x[1] + v * math.cos(theta[1]) * Te
        y[1] = y[1] + v * math.sin(theta[1]) * Te
        theta[1] = theta[1] + wk * Te
        print(np.round(x[1],2), np.round(y[1],2))

        # Animation
        #pygame.draw.circle(SCREEN,(0,0,0),(x,y),2)
        pygame.draw.line(SCREEN,color2,(x1[1],y1[1]),(x[1],y[1]),linel)
        print("COLOR :",color2)
    
    if cpt%75 == 0 and stopD:
        wd[2]-=pas

        # timer
        t = time.perf_counter() - t0

        # ancienne valeur de wG & wD
        owG[2] = wg[2]
        owD[2] = wd[2]

        # Signal des vitesses
        #wg = math.cos(t)
        #wd = math.sin(t)
        print("t = ",t)
        print("wg = ", wg[2], "wd = ", wd[2])

        # PID pour moteur gauche
        t1 = t - t1
        p[2] = wg[2] - 0
        i[2] += wg[2]
        d[2] = (wg[0] - owG[0])/t1
        uG[2] = kp[2]*p[2] + ki[2]*i[2] + kd[2]*d[2] 

        # PID pour moteur droit
        p[2] = wd[2] - 0
        i[2] += wd[2]
        d[2] = (wd[2] - owD[2])/t1
        uD[2] = kp[2]*p[2] + ki[2]*i[2] + kd[2]*d[2]  

        # Paramètres
        v = r*(wg[2] + wd[2]) / 2     # Vitesse moyenne du robot (moyenne de la vitesse des deux roues)
        wk = r*(wg[2] - wd[2]) / L # On note que c'est égale à Dc de l'encodeur une fois multiplié par Te

        # Sortie U vers les moteurs
        if wg[2] >= 0 :
            uL = np.round(uG[2]/quantum)
        else :
            uL = np.round(-uG[2]/quantum)

        if wd[2] >= 0 :
            uR = np.round(uD[2]/quantum)
        else :
            uR = np.round(-uD[2]/quantum)

        Te =  pas # time.perf_counter() - t
        print("V =", v)
        print("uL = ", uL,"uR = ",uR)
        print("Te =", Te)

        # Old Position
        x1[2] = x[2]
        y1[2] = y[2]
        print(np.round(x1[2],2), np.round(y1[2],2))

        # Position
        x[2] = x[2] + v * math.cos(theta[2]) * Te
        y[2] = y[2] + v * math.sin(theta[2]) * Te
        theta[2] = theta[2] + wk * Te
        print(np.round(x[2],2), np.round(y[2],2))

        # Animation
        #pygame.draw.circle(SCREEN,(0,0,0),(x,y),2)
        pygame.draw.line(SCREEN,color3,(x1[2],y1[2]),(x[2],y[2]),linel)
        print("COLOR :",color3)
    cpt+=1
    
    #pygame.time.delay(7)
    pygame.display.flip()
    
    #print("\n")
print("END")
    
    
    