

unsigned long t_spirale = 0;  // to store the current time in ms

int spirale_draw_index  = 0;
int spirale_erase_index = 0;

const int spirale_segments[] = { 1,1,1,1,1,1,1,1,2,3,4,4,4,4,4,4,4,4,5,7,7,7,7,7,7,7,7,2,1,1,1,1,1,1,1,2,7,7,7,7,7,7,7,3,4,4,4,4,4,4,4,4,5,6}; // list of used segments
const int spirale_columns[]  = { 7,6,5,4,3,2,1,0,0,0,0,1,2,3,4,5,6,7,7,7,6,5,4,3,2,1,0,0,0,1,2,3,4,5,6,7,6,5,4,3,2,1,0,0,0,1,2,3,4,5,6,7,7,7}; // list of used columns
const int spirale_length = 55 ; // 38= nombre d'elements dans la liste


bool can_erase_spirale = false;


// init the spirale pattern
void spiraleInit()
{
  t_spirale = millis(); // store the current time in ms
  spirale_draw_index = 0; // reset the index
  can_erase_spirale = false; // do not clear at beginning


  lc.clearDisplay(0);
}


// run the spirale pattern
void spiraleLoop( int tempo ) {


  bool tick = false;

  // update index after 1000 ms
  if ( (millis() - t_spirale) > tempo ) {
    t_spirale = millis(); // store the current time in ms for the next loop
    tick = true;
  }

  spiraleDraw(tick);

  if ( can_erase_spirale ) spiraleErase(tick);
  else if (  spirale_draw_index > (spirale_length / 5) ) can_erase_spirale = true; // check if we can erase
}


// draw the spirale pattern
void spiraleDraw( bool tick ) {


  // get segment and column index
  int segment = spirale_segments[ spirale_draw_index ];
  int column = spirale_columns[ spirale_draw_index ];

  // adress, colonne, segment
  lc.setLed (0, column, segment, true);


  // update the index (if needed)
  if ( tick ) {
    spirale_draw_index++;
    if ( spirale_draw_index > spirale_length ) spirale_draw_index = 0; // loop
  }

}


// erase the spirale pattern
void spiraleErase( bool tick ) {

  // get segment and column index
  int segment = spirale_segments[ spirale_erase_index ];
  int column = spirale_columns[ spirale_erase_index ];

  // adress, colonne, segment
  lc.setLed (0, column, segment, false);


  // update the index (if needed)
  if ( tick ) {
    spirale_erase_index++;
    if ( spirale_erase_index > spirale_length ) spirale_erase_index = 0; // loop = reste index
  }


}
