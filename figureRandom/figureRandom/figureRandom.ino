
// Lien Vimeo
// https://vimeo.com/316366965


//LES REFERENCES

//http://www.ardumotive.com/8-digit-7seg-display-en.html
//https://skyduino.wordpress.com/2012/05/01/arduino-de-electronique-avec-capteur-tilt-et-afficheur-7-segments/
//https://learn.adafruit.com/tilt-sensor/using-a-tilt-sensor
//https://www.carnetdumaker.net/articles/mesurer-une-distance-avec-un-capteur-ultrason-hc-sr04-et-une-carte-arduino-genuino/
//https://www.instructables.com/id/Using-a-4-digit-7-segment-display-with-arduino/




//We always have to include the library
#include "LedControl.h"

#define ANIM_SNAKE   0
#define ANIM_RANDOM  1
#define ANIM_POINTS  2
#define ANIM_SPIRALE 3
#define ANIM_CARRE   4

int anim = ANIM_RANDOM;
LedControl lc = LedControl(12, 13, 10, 1);

//telemetre Ultrason

const unsigned long MESURE_TIMEOUT = 25000UL;
const float SOUND_SPEED = 340.0 / 1000;
const byte ULTRASON_TRIGGER  = 3; //prise de mesure
const byte ULTRASON_ECHO = 4; //definir la vitesse du son

//LDR
int LDR = 5;
int valeurLDR = 0;
#define LUMINOSITE_MINIMUM 20
bool arret = false;


//Tilt
int tilt = 2;
bool lecture; //La lecture actuelle de la broche d'entrée
bool precedent = false; // La lecture précédente de la broche d'entrée
long temps = 0;
long rebond = 50;





void setup() {
  Serial.begin (9600);
  pinMode(ULTRASON_TRIGGER, OUTPUT);
  //LDR
  pinMode(LDR, INPUT);
  digitalWrite (ULTRASON_TRIGGER, LOW);
  pinMode(ULTRASON_ECHO, INPUT);
  lc.shutdown(0, false);
  lc.setIntensity(0, 2);  /* Set the brightness to a medium values */
  lc.clearDisplay(0);   /* and clear the display */
  pinMode (tilt , INPUT);


}


void loop() {
  //lecture LDR
  valeurLDR = analogRead (LDR);

  //Lancement telemetre ultrason
  digitalWrite (ULTRASON_TRIGGER, HIGH);
  delayMicroseconds (10);
  digitalWrite (ULTRASON_TRIGGER, LOW);
  //Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho
  long mesure = pulseIn (ULTRASON_ECHO, HIGH, MESURE_TIMEOUT);
  //Calcul la distance à partir du temps mesuré
  float distance_mm = mesure / 2.0 * SOUND_SPEED;


  // intensité LCD selon LDR
  int lcdPower = constrain(valeurLDR, 600, 933); //a partir de là, lcdPower est compris entre 600 et 933
  lcdPower = map(lcdPower, 600, 933, 15, 0);
  lc.setIntensity(0, lcdPower);



  int tempo = constrain(distance_mm, 30, 1100);




  //Animation
  precedent = lecture;
  lecture = digitalRead(tilt);


  if (lecture != precedent) {
    Serial.println("tilt");
    anim = randomTilt();
    lc.clearDisplay(0);
    if ( anim==ANIM_SNAKE ) SerpentInit();
    if ( anim==ANIM_POINTS ) PointsInit();
    if ( anim==ANIM_CARRE ) carreInit();
    if ( anim==ANIM_SPIRALE ) spiraleInit();
    delay (10);
  }


  switch ( anim ) {

    // random
    case ANIM_RANDOM :
      tempo = map(tempo, 30, 1100, 1, 50); // data mapping
      figureRandom(10, 25, tempo);
      break;

    // snake
    case ANIM_SNAKE :
      tempo = map(tempo, 30, 1100, 1, 1000); // data mapping
      SerpentLoop(tempo);
      break;

    //points
    case ANIM_POINTS :
      tempo = map(tempo, 30, 1100, 1, 500); // data mapping
      pointsLoop(tempo);
      break;

    //Spirale
    case ANIM_SPIRALE :
      tempo = map(tempo, 30, 1100, 1, 1000); // data mapping
      spiraleLoop(tempo);
      break;
    //Carre
    case ANIM_CARRE :
      tempo = map(tempo, 30, 1100, 1, 1000); // data mapping
      carreLoop(tempo);
      break;
  }


  // delay(10);

  Serial.print ("distance_mm ");
  Serial.print (distance_mm);
  Serial.print (", valeurLDR ");
  Serial.print (valeurLDR);
  Serial.print (", LDR ");
  Serial.print (LDR);
  Serial.print (", lcdPower ");
  Serial.print (lcdPower);
  Serial.print (", tempo ");
  Serial.print (tempo);
  Serial.print(", anim ");
  Serial.println (anim);
  Serial.println();

}///////////////////end of loop



////pick random number on tilt event
int randomTilt() {
  int result = -1;
  result = random(0, 5);
  return result;
}

void figureRandom( int freq, int count, int rate )
{
  lc.clearDisplay(0);

  for ( int i = 0; i < count; i++ ) {
    int row = random(0, 8);
    int col = random(0, 8 );
    lc.setLed (0, row, col, true);
    delay(rate);
  }

}



