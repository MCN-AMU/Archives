
import processing.video.*;
import oscP5.*;
import netP5.*;

OscP5 osc;

Movie mov0, mov1, mov2;
Chloe chloe;

float var_chloe;
float speed = 1;

void setup() {
    
    size(960, 540);
    background(0);
    
    mov0 = new Movie(this, "charlotte.mp4");
    mov1 = new Movie(this, "fac.mp4");
    mov2 = new Movie(this, "WM.mp4");

    mov0.loop();
    mov1.loop();
    mov2.loop();

    mov0.volume(1);
    mov1.volume(0.4);
    mov2.volume(1);

    chloe = new Chloe();
    osc = new OscP5(this, 7000);
}

void movieEvent(Movie movie) {
    movie.read();
}

void draw() {    

    image(mov0, 0, 0);
    blend(mov1, 0, 0, 960, 540, 0, 0, 960, 540, ADD);
    blend(mov2, 0, 0, 960, 540, 0, 0, 960, 540, ADD);

    mov0.speed(speed);
    mov1.speed(speed);
    mov2.speed(speed);

    chloe.setMouseX(var_chloe);
    chloe.draw();

    fill(255);
    text(nfc(speed, 2) + "X", 10, 30);
}  

void oscEvent(OscMessage theOscMessage) {
    /* print the address pattern and the typetag of the received OscMessage */
    print("### received an osc message.");
    print(" addrpattern: "+theOscMessage.addrPattern());
    println(" value: "+theOscMessage.get(0).intValue());

    int value= theOscMessage.get(0).intValue();
    if ( theOscMessage.addrPattern().equals("/piezo/one") )
        var_chloe = norm( value, 8, 80) * width;

    if ( theOscMessage.addrPattern().equals("/piezo/two") )
        speed = map(value, 8, 80, 0.1, 3);
    println(speed);
}

void mouseMoved()
{
    var_chloe = mouseX;
    speed = map(mouseX, 0, width, 0.1, 3);
}
