

// RECEIVE AND TREAT OSC MESSAGES FROM PD (INTERNALLY) AND TOUCHOSC (REMOTELY THROUGH Wi-Fi)


import oscP5.*;
import netP5.*;

OscP5 osc;                 // the OSC object 
NetAddress pd_location;    // the Pd location (on the network)

int bgd_color = 0;
float ball_x  = 0;
boolean square_visible = false;



void setup()
{
    size(400, 400);
    frameRate(25);

    // start oscP5, listening for incoming messages at port 54321
    osc = new OscP5(this, 54321);

    /* pd_location is a NetAddress. a NetAddress takes 2 parameters,
     * an ip address and a port number. pd_location is used as parameter in
     * oscP5.send() when sending osc packets to another computer, device, 
     * application. usage see below. for testing purposes the listening port
     * and the port of the remote location address are the same, hence you will
     * send messages back to this sketch.
     */
    pd_location = new NetAddress("127.0.0.1", 12345);
}


void draw()
{
    // use a gray background color,
    // should be defined by the arduino connected potentiometer
    background(bgd_color); 

    // draw a circle (ball) at the X position sent by PureData
    noStroke();
    fill(255, 0, 0);
    ellipse(ball_x, height/2, 20, 20);

    // show or hide a green square
    // according to the TouchOSC toggle status
    if ( square_visible ) {
        fill(0, 255, 0);
        rect( width/2, height/2, 100, 100);
    }
}



// incoming osc message are forwarded to the oscEvent method
void oscEvent(OscMessage msg)
{

    // print message content
    print("--> received an osc message, address pattern: "+msg.addrPattern());
    if ( msg.typetag().equals("i") ) println(" arg int: "+msg.get(0).intValue());
    if ( msg.typetag().equals("f") ) println(" arg float: "+msg.get(0).floatValue());
    if ( msg.typetag().equals("s") ) println(" arg string: "+msg.get(0).stringValue());


    // work with the arduino potentiometer value
    if ( msg.addrPattern().equals("/arduino/sensor") ) {
        bgd_color = (int)map(msg.get(0).intValue(), 0, 1023, 0, 255 );    // data mapping , what the value from one frame of reference in another
        //bgd_color = (int)norm( osc.get(0).intValue(), 0, 1023) * 255;   // alternative using normalisation
    }

    // ball position
    if ( msg.addrPattern().equals("/ball/position") ) {
        //if ( osc.typetag().equals("i") ) ball_x = osc.get(0).intValue() * width;
        //if ( osc.typetag().equals("f") )  ball_x = osc.get(0).floatValue() * width;
        ball_x = (msg.typetag().equals("i") ? ball_x = msg.get(0).intValue() :msg.get(0).floatValue()) * width;
    }

    // TouchOSC toggle
    if ( msg.addrPattern().equals("/1/toggle1") ) {
        
        square_visible = msg.get(0).floatValue()==1.0; // turn square visible/hidden

        // send square status to PD
        msg = new OscMessage("/test"); // <---- be caution the variable msg is now attached to the outgoing message, not the incomming OSC message
        msg.add(square_visible); // add an int to the osc message
        osc.send(msg, pd_location);
        
        // be sure msg is no longer used
        return;
    }
}
