
final int TAILLE_TAMPON = 10; // TAILLE_TAMPON du tampon
final int RAYON = 30;    //décalage par rapport au pîxel source

final int MARGE_H = 10; // attention par securité > RAYON + TAILLE_TAMPON
final int MARGE_V = 10; // cette régle n'est pas absolue

final boolean USE_SOURCE = false; // utiliser l'image source (si true) comme source ou l'image déjà dégradée
final boolean COPIE = true; // si true n'utilise pas le mode de fusion ci-dessous (copie simple de  laz zone ), si false l'inverse
// mode de fusion, mettre COPY au dessus à false
//  BLEND, ADD, SUBTRACT, LIGHTEST, DARKEST, DIFFERENCE, EXCLUSION, MULTIPLY, SCREEN, OVERLAY, HARD_LIGHT, SOFT_LIGHT, DODGE, BURN
final int MODE_FUSION = ADD;


/////////////////////////////////////////////


class EffetShift
{

    PImage source;

    int px = 0; // position x du pixel référent
    int py = 0; // position y du pixel référent
    int dx = 0; // position x de destination
    int dy = 0; // position y de destination

    EffetShift( PImage source )
    {
        this.source = source;

        px = (int)random( source.width-TAILLE_TAMPON-(MARGE_V*2) ) + MARGE_V;
        py = (int)random( source.height-TAILLE_TAMPON-(MARGE_H*2) ) + MARGE_H;

        float angle = radians( random(360) );
        dx = px + int( cos(angle) * RAYON );
        dy = py + int( sin(angle) * RAYON );
    }

    void degradation( PImage image, boolean zigzag )
    {

        int sx = zigzag ? dx : (int)random( image.width-TAILLE_TAMPON-(MARGE_V*2) ) + MARGE_V;
        int sy = zigzag ? dy : (int)random( image.height-TAILLE_TAMPON-(MARGE_H*2) ) + MARGE_H;

        float angle = radians( random(360) );
        dx = sx + int( cos(angle) * RAYON );
        dy = sy + int( sin(angle) * RAYON );

        // copie simple de la zone
        if ( COPIE ) {
            if ( USE_SOURCE ) image.copy( source, sx, sy, TAILLE_TAMPON, TAILLE_TAMPON, dx, dy, TAILLE_TAMPON, TAILLE_TAMPON );
            else image.copy( image, sx, sy, TAILLE_TAMPON, TAILLE_TAMPON, dx, dy, TAILLE_TAMPON, TAILLE_TAMPON );
        } 
        // copie avec fusion de la zone 
        else {
            if ( USE_SOURCE ) image.blend( source, sx, sy, TAILLE_TAMPON, TAILLE_TAMPON, dx, dy, TAILLE_TAMPON, TAILLE_TAMPON, MODE_FUSION );
            else image.blend( image, sx, sy, TAILLE_TAMPON, TAILLE_TAMPON, dx, dy, TAILLE_TAMPON, TAILLE_TAMPON, MODE_FUSION );
        }
    }
}