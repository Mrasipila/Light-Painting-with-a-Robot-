CC_V01 : Implémentation d’interruptions pour récupérer les valeurs des encodeurs

CC_V02 : Utilisation de la librairie PID_v1

CC_V03 : Utilisation de la librairie Encoder qui remplace l’implémentation des interruptions, librairie PID _v2

CC_V04 : Utilisation des neopixels, librairie FastPID remplace la librairie PID_2, implémentation fonction rotate, forward, arrêt

CC_V05 : PID_v2, tableau de coordonnées, implémentation fonction nextPoint (passer d’un couple de coordonnées à un autre

CC_V06 : tableaux OK pour du rectiligne => PID_v2 sans tableaux et une distance en cm comme condition d’arrêt 

CC_V07 : Implémentation de notre propre correcteur proportionnel et un temps calculé par millis() comme condition d'arrêt

CC_V08 : Force une durée fixe entre deux boucles pour que l'output fasse effet

CC_V09 : Fonction neopixel pour plus de clareté

CC_V10 : Ajout des correcteurs intégral et dérivé et de la fonction wait qui remplace delay

CC_V11 : Implémentation de la fonction initialize() qui "tune" les coefficients en fonction de la consigne
