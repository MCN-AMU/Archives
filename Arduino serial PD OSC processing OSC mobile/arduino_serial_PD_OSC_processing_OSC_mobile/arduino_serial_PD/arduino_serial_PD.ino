/**
    Send analog sensor values and listen for incoming message
    open arduino_serial_PD_OSC_processing_OSC_mobile.pd patch
*/

int POTAR = 2;        // analog pin 2
int LED_RED = 13;     // digital pin 13

void setup()
{
  // start serial port at 9600 baud (bits per second)
  Serial.begin(9600);

  // configures the specified pin to behave as an output
  pinMode(LED_RED, OUTPUT);
  digitalWrite(LED_RED, LOW);

}

void loop()
{

  // get sensors values
  int pval = analogRead( POTAR );   // read potentometer value

  // send values on serial port
  Serial.print("potar ");  Serial.println( pval );

  // arbitrary delay to limit data to serial port (milliseconds)
  delay(100);
}


// listening for message
void serialEvent()
{
  // if there's any serial available, read it:
  while ( Serial.available() > 0 ) {

    // message format :  (first byte) LED number + (second byte) LED status + newline
    // look for the next valid integer in the incoming serial stream
    int led = Serial.parseInt();
    int val = Serial.parseInt();

    // look for the newline. That's the end of your sentence:
    if ( Serial.read() == '\n' ) {
      digitalWrite(led, val == 0 ? LOW : HIGH); // turn LED on or off
    }

    // send LED status
    int state = digitalRead(led);
    Serial.println( "the LED " + String(led) + " is " + (state == LOW ? "OFF" : "ON") );
  }
}
