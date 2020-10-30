/*

 MATRICE
 travailler dans une liste de pixels, ou emplacement sur l'écran
 
 int pixel = (y*largeur) + x;
 int x = pixel%largeur;
 int y = pixel/largeur;
 
 
 ///////////////////////////////////////////////////////
 
 TRIGONOMÉTRIE
 emplacement d'un point en fontion d'un autre
 
 float angle = radians(degree)
 int x = int( cos(angle) * rayon );
 int y = int( sin(angle) * rayon );
 
 
 ///////////////////////////////////////////////////////
 
 MANIPULATION DES COULEURS
 (bit shifting)
 
 RGBA =  alpha << 24 | red << 16 | green << 8 | blue
 RGBA = ( alpha << 24 ) | ( RGB & 0x00FFFFFF )
 
 a = RGBA >> 24 & 0xFF;
 r = RGBA >> 16 & 0xFF;
 g = RGBA >> 8 & 0xFF;
 b = RGBA  & 0xFF;
 
 
 */