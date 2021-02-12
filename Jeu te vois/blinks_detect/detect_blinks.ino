#define LED     2
#define TIMEOUT 500        // delay to turn off the LED, in milliseconds


int total = 0;             // blinks count
String status = "";        // eyes status
boolean isEnable = false;  // something is activated after eye's blink, LED is turned ON for our example

unsigned long duration;    // eye close duration
unsigned long t_closed;    // time variable to store the elapsed time since the eyes were closed
unsigned long t_enable;    // time variable to store the elapsed time since the LED is turned ON


void setup()
{

  // define pin mode as output
  pinMode(LED, OUTPUT);
  pinMode(13, OUTPUT);

  // turn ON the LED,
  // meaning the program is running
  digitalWrite(13, HIGH);

  // open serial connection
  Serial.begin(9600);
  Serial.println("Arduino is running ...");

  // init time variables
  t_closed = millis();
  t_enable  = millis();
}

void loop()
{

  // tant que des données arrivent sur le port série
  // lit les données charactère par charactère sur le port série
  while ( Serial.available() ) {
    char c = Serial.read();

    switch (c) {

      // welcome message
      case 'W' :
        Serial.println("Welcome Python!");
        return;

      // no detection
      case '*' :
        status = "not detected";
        break;

      // rejected
      case '2' :
        status = "rejected";
        break;

      // eye closed
      case '0' :
        status = "closed";
        t_closed = millis();
        break;

      // eye opened valid
      case '1' :

        // blink
        if ( status == "closed" ) {
          digitalWrite(LED, HIGH); // LED ON

          total += 1;
          t_enable = millis();
          duration = millis() - t_closed;
          isEnable = true;

          char buffer[256];
          sprintf(buffer, "blink count: %i, eyes closed during: %i ms", total, duration);
          Serial.println(buffer);
        }

        status = "opened";
        break;
    }

    Serial.println(status);
  }


  // if enabled and time is gone, disable or turn OFF the led
  //if ( isEnable && (t_enable + t_elapsed <= millis()) ) { // using timeout
  if ( isEnable && (t_enable + duration <= millis()) ) { // using eye close duration
    digitalWrite(LED, LOW);
    isEnable = false;
  }

  // wait a bit (ms)
  delay(10);
}
