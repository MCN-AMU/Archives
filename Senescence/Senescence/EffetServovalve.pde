

//final int NOMBRE_DE_RONGEUR = 100;
final int RAPIDITE_DE_DEGRADATION = 10;    // travail sur la couche alpha (entre 1 et 255 )
final int RAPIDITE_DE_RECOMPOSITION = 20;    // travail sur la couche alpha (entre 1 et 255 )

final int TEMP_EFFET_DEGRADATION = 500; // second
final int TEMP_EFFET_RECOMPOSITION = 500; // second

/////////////////////////////////////////////


class EffetServovalve
{

    // temps de l'effet
    int timeout;

    // liste des (pixels) rongeurs
    int[] rongeurs = new int[0];


    // detection de x zone les plus lumineuses
    EffetServovalve( PImage source, int ronguer )
    {
        PImage gris = source.copy();
        gris.filter(GRAY); // convertion RGB(A) en niveau de gris
        gris.loadPixels();


        // liste des (pixels) rongeurs
        rongeurs = new int[ronguer];

        // liste des pixels à comparer
        ArrayList<Pixel> pixels = new ArrayList<Pixel>();

        // liste les valeurs de niveau de gris pour chaque pixels
        for ( int i=0, nombre_de_pixels=gris.width*gris.height; i<nombre_de_pixels; i++ ) {
            pixels.add( new Pixel(i, gris.pixels[i]) );
        }

        // compare, attention dans l'ordre décroissant : 255 -> 0
        Collections.sort( pixels, new Comparator<Pixel>() {
            public int compare(Pixel pix1, Pixel pix2) {
                return Integer.compare(pix2.couleur, pix1.couleur);
            }
        }
        );

        // on prends les x premiers pixels pour définir la place initiale des rongeurs
        for ( int i=0; i<ronguer; i++ ) {
            rongeurs[i] = pixels.get( i ).index;
        }

        // initialise le temps maximum d'effet
        timeout = millis();
    }


    void degradation( PImage source, int min )
    {

        //if ( (timeout+(TEMP_EFFET_DEGRADATION*1000))<millis() ) return; // ne fait rien si le temps de l'effet est dépasé

        int nombre_de_pixels = source.width*source.height;

        source.loadPixels();
        for ( int i=0; i<rongeurs.length; i++ ) {
            int position = rongeurs[i];
            int aleatoire = (int)random(4);

            if ( aleatoire==0 ) position = max( 0, position - source.width );                   // en haut
            if ( aleatoire==1 ) position = min( position+1, nombre_de_pixels-1);                // a droite
            if ( aleatoire==2 ) position = min( position + source.width, nombre_de_pixels-1 );  // en bas
            if ( aleatoire==3 ) position = max( 0, position - 1);                               // a gauche


            int a = source.pixels[ position ] >> 24 & 0xFF;
            //int r = source.pixels[ position ] >> 16 & 0xFF;
            //int g = source.pixels[ position ] >> 8 & 0xFF;
            //int b = source.pixels[ position ] & 0xFF;

            a = max( min, a-RAPIDITE_DE_DEGRADATION );
            //if ( a>0 ) source.pixels[ position ] = (a<<24) | (r<<16) | (g<<8) | b;
            if ( a>min ) source.pixels[ position ] = (a<<24) | ( source.pixels[position] & 0x00FFFFFF );
            rongeurs[i] = position;
        }
        source.updatePixels();
    }

    void recomposition( PImage source, int max )
    {

        //if ( (timeout+(TEMP_EFFET_RECOMPOSITION*1000))<millis() ) return; // ne fait rien si le temps de l'effet est dépasé

        int nombre_de_pixels = source.width*source.height;

        source.loadPixels();
        for ( int i=0; i<rongeurs.length; i++ ) {
            int position = rongeurs[i];
            int aleatoire = (int)random(4);

            if ( aleatoire==0 ) position = max( 0, position - source.width );                   // en haut
            if ( aleatoire==1 ) position = min( position+1, nombre_de_pixels-1);                // a droite
            if ( aleatoire==2 ) position = min( position + source.width, nombre_de_pixels-1 );  // en bas
            if ( aleatoire==3 ) position = max( 0, position - 1);                               // a gauche


            int a = source.pixels[ position ] >> 24 & 0xFF;
            
            if ( a<max ) {
                a = min( max, a+RAPIDITE_DE_RECOMPOSITION );
                if ( a<=max ) source.pixels[ position ] = (a<<24) | ( source.pixels[position] & 0x00FFFFFF );
            }
            else {
                a = min( 255, a+RAPIDITE_DE_RECOMPOSITION );
                if ( a<=255 ) source.pixels[ position ] = (a<<24) | ( source.pixels[position] & 0x00FFFFFF );
            }
            
            rongeurs[i] = position;
        }
        source.updatePixels();
    }
}

class Pixel
{
    int index;
    int couleur;
    Pixel( int i, int c ) {
        index = i;
        couleur = c;
    }
}