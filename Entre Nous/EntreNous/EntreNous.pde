
/**
 * EntreVous.pde
 * 
 */

import processing.video.*;
import gab.opencv.*;
import java.awt.*;


// SHOULD MACTH THE MOVIE SIZE
final int MOVIE_W = 1280;
final int MOVIE_H = 720;


// the ratio between the original image and all buffered images use for detection
// hight value means faster is the detection but reduce the definition



final float DETECT_IMAGE_RATIO_W = 7.27f;
final float DETECT_IMAGE_RATIO_H = 5.f;


Movie movie    = null;           // the main video sequence
Movie subtitle = null;           // the subtitle sequence
PGraphics mask = null;           // buffered image to embed the mask
Hole[] holes   = new Hole[0];    // list of detected faces
Capture camera = null;           // the camera object
OpenCV opencv  = null;           // the computer vision engine     


// debug mode, show detection image and informations
boolean debug = true;


void setup()
{
    size(960, 540);
    //fullScreen(2); // arg = number of display (1 for main display, 2 for the second, etc)
    frameRate(60);

    // hide cursor
    noCursor();

    // initialize movie and load sources
    movie = new Movie(this, "oeil600kb.mp4");
    subtitle = new Movie(this, "soustitresfixe.mp4");
    // enable loop mode (and start playing)
    movie.loop();
    subtitle.loop();


    // initialize mask
    //mask = createGraphics(MOVIE_W, MOVIE_H);
    mask = createGraphics(MOVIE_W, 700);
    updateMask();


    // init the face detection
    int camw = int(MOVIE_W/DETECT_IMAGE_RATIO_W);
    int camh = int(MOVIE_H/DETECT_IMAGE_RATIO_H);
    camera = new Capture(this, camw, camh, "FaceTime HD Camera", 30 );
    //camera = new Capture(this, camw, camh, "Logitech HD Webcam C270", 30 );
    opencv = new OpenCV(this, camw, camh  );
    opencv.loadCascade(OpenCV.CASCADE_FRONTALFACE);

    // start the camera
    try {
        camera.start();
    }
    catch(Exception e) {
        println( "available camera");
        printArray( Capture.list() );
        println( "\nuse capture size: "+camw+"x"+camh+"\n" );
        println( "modify the image ratio, ie. DETECT_IMAGE_RATIO_W and/or DETECT_IMAGE_RATIO_H to match one of the size above." );
        System.exit(0);
    }

    // usage
    print("\nusage: PRESS enter TO TOGGLE DEBUG MODE\n" );
}


void draw()
{

    // black background
    background(0);

    // read new image if available (from movies)
    if ( movie!=null && movie.available() ) movie.read();
    if ( subtitle!=null && subtitle.available() ) subtitle.read();


    // blends the pixels in the display window according to a defined mode (read the doc)
    blendMode(SCREEN); // <--- opposite multiply, uses inverse values of the colors. 

    // apply the mask
    movie.mask(mask);


    // display in (layer) order, and scale to window dimensions
    // the main movie in background or the mask depending the play mode
    if ( debug ) image( mask, 0, 0, width, height);
    else image( movie, 0, 0, width, height);

    // the subtitle
    image(subtitle, 0, 0, width, height);


    faceDetect();
}


void faceDetect()
{

    PImage frame = null;

    // grab the current frame from camera
    if ( camera!=null && camera.available() ) camera.read();

    // pass the current frame to opencv
    opencv.loadImage(camera);
    // flip the image
    opencv.flip(OpenCV.HORIZONTAL);

    // get the flipped camera input before the detection process
    if ( debug ) frame = opencv.getOutput();

    // detect all faces and store there area in a list of 'Rectangle' (i.e origine + dimension)
    Rectangle[] faces = opencv.detect();

    // create a hole for each faces
    // define each hole with the associated (face) rectangle properties
    holes = new Hole[faces.length];
    for ( int i=0; i<holes.length; i++ ) {

        float x = faces[i].x + (faces[i].width/2);
        float y = faces[i].y + (faces[i].height/2);
        float size = faces[i].width;

        holes[i] = new Hole( x*DETECT_IMAGE_RATIO_W, y*DETECT_IMAGE_RATIO_H, size*DETECT_IMAGE_RATIO_W );
    }

    // update mask
    updateMask();

    // only on debug mode
    if ( debug ) {

        float x = 20;
        float y = 20;

        // remove blend mode
        blendMode(BLEND);
        // display the camera input
        image( frame, x, y );

        // show detected area
        noFill();
        stroke(255, 0, 0);
        strokeWeight(1);
        for ( int i=0; i<faces.length; i++ ) {
            rect(faces[i].x+x, faces[i].y+y, faces[i].width, faces[i].height);
        }

        // show report on screen
        fill(255, 0, 0);
        text( "FPS: "+frameRate+"\nfaces: "+faces.length, x+10, y+20 );
    }
}

void mousePressed()
{
    holes = new Hole[1]; // create list with one item
    holes[0] = new Hole( mouseX, mouseY, 200 ); // define first item with mouse coordinates
    updateMask();
}
void mouseDragged()
{
    holes[0].x = mouseX;
    holes[0].y = mouseY;
    updateMask();
}

void mouseReleased()
{
    holes = new Hole[0]; // clear list
    updateMask();
}

void keyReleased()
{
    if ( key==ENTER ) debug = !debug;
}


// make mask , ie. movie alpha layer
void updateMask( )
{
    // warning think about scale issue between camera input an movie (mask) dimensions
    mask.beginDraw();
    mask.background(0); // black background = opaque

    // for each holes
    for ( int i=0; i<holes.length; i++ ) {
        mask.noStroke(); // no contour
        mask.fill(255);  // white  = transparent
        mask.ellipse( holes[i].x, holes[i].y, holes[i].size, holes[i].size );
    }
    mask.endDraw();
}
