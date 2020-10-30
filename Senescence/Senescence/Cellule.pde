


class Cellule
{

    String fichier = "";
    PImage image = null;

    EffetServovalve servocompo = null;
    EffetServovalve servodecom = null;
    EffetShift shift = null;

    float x = 0;
    float y = 0;

    boolean active = false;
    int identifiant = 0;

    Cellule( int id, float x, float y )
    {
        this.identifiant = id;
        this.x = x;
        this.y = y;

        // demarrage automatique depuis la dernière sauvegarde de l'image
        fichier =  sketchPath()+"/images/cellule_"+identifiant+".jpg";
        File file = new File( fichier );
        if ( file.exists() ) {
            //println( "nouvelle cellule à partir du fichier: "+fichier );
            copie( loadImage(fichier) );
        }
    }



    // copie + initialise les effets
    void copie( PImage source )
    {
        image = source;
        active = true;

        // sauvegarder l'image
        source.save( fichier );

        // initialisation des effets
        servocompo = new EffetServovalve( image, 50 );
        servodecom = new EffetServovalve( image, 100 );
        shift = new EffetShift( image );
    }


    // copie + recadre + initialise
    void copie( PImage source, int x, int y, int largeur, int hauteur )
    {
        source.copy( source, x, y, largeur, hauteur, 0, 0, source.width, source.height );
        copie( source );
    }



    void dessine()
    {

        // uniquement si j'ai une image
        if ( active ) {

            // dégradé l'image
            //shift.degradation( image, true ); // le deuxième paramètre pour l'effet zigzag (true ou false)
           servocompo.recomposition( image, 200 ); // la deuxième variable est la valeur maximum de recomposition
           servodecom.degradation( image, 0 ); // la deuxième variable est la valeur minimum de décomposition

            // affiche l'image
            image( image, x, y );
        }
    }
}
