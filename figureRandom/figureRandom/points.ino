

unsigned long t_points = 0;  // to store the current time in ms

int points_draw_index  = 0;
int points_erase_index = 0;

const int points_segments[] = { 0, 0, 0, 0, 0, 0, 0, 0}; // list of used segments
const int points_columns[]  = { 4, 3, 2, 5, 1, 6, 0, 7}; // list of used columns
const int points_length = 8 ; // 8= nombre d'elements dans la liste


bool can_erase_points = false;


// init the points pattern
void PointsInit()
{
  t_points = millis(); // store the current time in ms
  points_draw_index = 0; // reset the index pour recommencer la boucle
  can_erase_points = false; // do not clear at beginning


  lc.clearDisplay(0);
}


// run the points pattern
void pointsLoop( int tempo ) {


  bool tick = false;

  // update index after 1000 ms
  if ( (millis() - t_points) > tempo ) {
    t_points = millis(); // store the current time in ms for the next loop
    tick = true;
  }

  pointsDraw(tick);

  if ( can_erase_points ) pointsErase(tick);
  else if (  points_draw_index > (points_length / 1,1)) can_erase_points = true; // check if we can erase
}


// draw the points pattern
void pointsDraw( bool tick ) {


  // get segment and column index
  int segment = points_segments[ points_draw_index ];
  int column = points_columns[ points_draw_index ];

  // adress, colonne, segment
  lc.setLed (0, column, segment, true);


  // update the index (if needed)
  if ( tick )
    points_draw_index++;
  if ( points_draw_index > points_length ) points_draw_index = 0; // loop

 

}


// erase the points pattern
void pointsErase( bool tick ) {

  // get segment and column index
  int segment = points_segments[ points_erase_index ];
  int column = points_columns[ points_erase_index ];

  // adress, colonne, segment
  lc.setLed (0, column, segment, false);


  // update the index (if needed)
  if ( tick ) {
    points_erase_index++;
    if ( points_erase_index > points_length ) points_erase_index = 0; // loop = reste index
  }


}
