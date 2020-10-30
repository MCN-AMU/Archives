import processing.serial.*;


Serial arduino = null;

void initArduino()
{
    String[] arduinos = Serial.list();
    if ( arduinos.length==0 ) {
        println( "PAS DE ARDUINO CONNECTE ?" );
        println(arduinos);
        exit();
    }

    arduino = new Serial( this, "/dev/cu.Bluetooth-Incoming-Port", 9600 );
    println( "Arduino connect" );
}


boolean boutonArduino()
{

    if ( arduino.available()>0 ) {
        boolean appuyer = arduino.read()==49;
        //println("bouton ON ? "+appuyer);        
        return appuyer;
    }
    return false;
}