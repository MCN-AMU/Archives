

unsigned long t_carre = 0;  // to store the current time in ms

int carre_draw_index  = 0;
int carre_erase_index = 0;

const int carre_segments[] = { 1,2,7,6,7,3,4,5,1,2,7,6,7,3,4,5,1,2,7,6,7,3,4,5,1,2,7,6,7,3,4,5,1,2,7,6,7,3,4,5,1,2,7,6,7,3,4,5,1,2,7,6,7,3,4,5,1,2,7,6,7,3,4,5,}; // list of used segments
const int carre_columns[]  = { 0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7,7,7,7,7,6,6,6,6,5,5,5,5,4,4,4,4,3,3,3,3,2,2,2,2,1,1,1,1,0,0,0,0,}; // list of used columns
const int carre_length = 64 ; // 64= nombre d'elements dans la liste


bool can_erase_carre = false;


// init the carre pattern
void carreInit()
{
  t_carre = millis(); // store the current time in ms
  carre_draw_index = 0; // reset the index
  can_erase_carre = false; // do not clear at beginning


  lc.clearDisplay(0);
}


// run the carre pattern
void carreLoop( int tempo ) {


  bool tick = false;

  // update index after 1000 ms
  if ( (millis() - t_carre) > tempo ) {
    t_carre = millis(); // store the current time in ms for the next loop
    tick = true;
  }

  carreDraw(tick);

  if ( can_erase_carre ) carreErase(tick);
  else if (  carre_draw_index > (carre_length / 10) ) can_erase_carre = true; // check if we can erase
}


// draw the carre pattern
void carreDraw( bool tick ) {


  // get segment and column index
  int segment = carre_segments[ carre_draw_index ];
  int column = carre_columns[ carre_draw_index ];

  // adress, colonne, segment
  lc.setLed (0, column, segment, true);


  // update the index (if needed)
  if ( tick ) {
    carre_draw_index++;
    if ( carre_draw_index > carre_length ) carre_draw_index = 0; // loop
  }

}


// erase the carre pattern
void carreErase( bool tick ) {

  // get segment and column index
  int segment = carre_segments[ carre_erase_index ];
  int column = carre_columns[ carre_erase_index ];

  // adress, colonne, segment
  lc.setLed (0, column, segment, false);


  // update the index (if needed)
  if ( tick ) {
    carre_erase_index++;
    if ( carre_erase_index > carre_length ) carre_erase_index = 0; // loop = reste index
  }


}
