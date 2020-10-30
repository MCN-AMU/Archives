import processing.video.*;
import processing.sound.*;
import java.util.*;



final int NOMBRE_ECRAN = 1;

//////////////////////////////////////////////////////


// camera
Capture camera = null;

// lecture audio
//SoundFile son = null;
Movie son = null;

// matrice
int n_colonne = 5 * NOMBRE_ECRAN;
int n_rangee  = 4;

int prochaine_cellule = 0;
int largeur_cellule = 0;
int hauteur_cellule = 0;

Cellule[] cellules = null;

boolean debug = false;
int last = 0;


void setup()
{

    fullScreen(0); // 0 = last les écrans, sinon l'identifiant de l'écran
    frameRate(24); // cadence du programme image/seconde
    background(0);
    noCursor();

    //son = new SoundFile(this, "salle_son.wav");
    //son = new Movie(this, "salle_son.wav");
    //son.loop();

    initCamera();
    initArduino();
    initMatrice();
}

void initCamera()
{

    String[] cameras = camera.list();
    if ( cameras.length==0 ) {
        println( "PAS DE CAMERA CONNECTE ?" );
        println( Capture.list() ); // affiche la liste des cameras connectées
        exit();
    }

    // initialisation de la camera 
    camera = new Capture(this, 320, 240); // si une seule camera
    //camera = new Capture(this, width, height, "HD Webcam C615", 30);
    camera.start();
}

void initMatrice()
{


    largeur_cellule = width  / n_colonne;
    hauteur_cellule = height / n_rangee;


    // création de la liste des cellules en fonction de la matrice
    cellules = new Cellule[ n_colonne*n_rangee ];

    // définir la position des cellules dans la matrice
    // de manière ordonnée
    for ( int i=0; i<cellules.length; i++ ) {

        float x = (i%n_colonne) * largeur_cellule;
        float y = (i/n_colonne) * hauteur_cellule;

        cellules[i] = new Cellule( i, x, y);
    }

    // mélanger les cellules
    cellules = (Cellule[])shuffle( cellules );
}

void draw()
{
    background(0); // fond noir

    // si le bouton est appuyée
    if ( boutonArduino() ) ajouteImage();

    stroke(255);
    for ( int i=0; i<cellules.length; i++ ) {
        cellules[i].dessine();
        if ( debug ) {
            line( cellules[i].x, 0, cellules[i].x, height );
            line( 0, cellules[i].y, width, cellules[i].y );
        }
    }


    if ( debug ) {

        // cadre de l'ensemble des ecrans
        stroke(255);
        noFill();
        rect( 0, 0, width-1, height-1 );

        // affichage des infos
        noStroke();
        fill( 0, 128 );
        rect( 0, 0, 300, 40 );
        fill( 255 );
        text( "FPS: "+frameRate+"\ntimeout: "+max(0, last-millis()), 10, 20 );
    }
}

void keyReleased()
{
    if ( key==' ' ) ajouteImage();
    if ( key=='!' ) debug =!debug;
}

void ajouteImage()
{
    camera.read();

    // mise à l'échelle de l'image de la camera
    PImage source = camera.copy();
    source.resize( largeur_cellule, hauteur_cellule );

    // copy de l'image de la camera dans la cellule
    cellules[ prochaine_cellule ].copie( source, 0, 0, largeur_cellule, hauteur_cellule );

    // mise à jour de la prochaine cellule active
    prochaine_cellule = prochaine_cellule +1;
    if ( prochaine_cellule>=cellules.length ) prochaine_cellule = 0;

    last = millis()+(TEMP_EFFET_DEGRADATION*1000);
}



// mélange aléatoiremengt les élémnts dans une liste
Object[] shuffle( Object[] list )
{
    Collections.shuffle( Arrays.asList(list) ); 
    return list;
}