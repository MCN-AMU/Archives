

unsigned long t_serpent = 0;  // to store the current time in ms

int serpent_draw_index  = 0;
int serpent_erase_index = 0;

const int serpent_segments[] = { 1, 2, 3, 0, 5, 6, 1, 2, 3 , 0, 5, 6, 1, 2, 3 , 0, 5, 6, 1, 2, 3 , 0, 5, 6, 1, 2, 3 , 0, 5, 6, 1, 2, 3 , 0, 5, 6, 1, 2, 3 , 0, 5, 6, 1, 2, 3 , 0, 5, 6, 1, 2, 3 , 0, 5, 6, 1 , 2, 0}; // list of used segments
const int serpent_columns[]  = { 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 , 0, 0}; // list of used columns
const int serpent_length = 46 ; // 46= nombre d'elements dans la liste


bool can_erase = false;


// init the serpent pattern
void SerpentInit()
{
  t_serpent = millis(); // store the current time in ms
  serpent_draw_index = 0; // reset the index
  can_erase = false; // do not clear at beginning


  lc.clearDisplay(0);
}


// run the serpent pattern
void SerpentLoop( int tempo ) {


  bool tick = false;

  // update index after 1000 ms
  if ( (millis() - t_serpent) > tempo ) {
    t_serpent = millis(); // store the current time in ms for the next loop
    tick = true;
  }

  SerpentDraw(tick);

  if ( can_erase ) SerpentErase(tick);
  else if (  serpent_draw_index > (serpent_length / 2) ) can_erase = true; // check if we can erase
}


// draw the serpent pattern
void SerpentDraw( bool tick ) {


  // get segment and column index
  int segment = serpent_segments[ serpent_draw_index ];
  int column = serpent_columns[ serpent_draw_index ];

  // adress, colonne, segment
  lc.setLed (0, column, segment, true);


  // update the index (if needed)
  if ( tick ) {
    serpent_draw_index++;
    if ( serpent_draw_index > serpent_length ) serpent_draw_index = 0; // loop
  }

}


// erase the serpent pattern
void SerpentErase( bool tick ) {

  // get segment and column index
  int segment = serpent_segments[ serpent_erase_index ];
  int column = serpent_columns[ serpent_erase_index ];

  // adress, colonne, segment
  lc.setLed (0, column, segment, false);


  // update the index (if needed)
  if ( tick ) {
    serpent_erase_index++;
    if ( serpent_erase_index > serpent_length ) serpent_erase_index = 0; // loop = reste index
  }


}
