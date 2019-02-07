/**
 * @file functions.cpp
 * @author Dominik Helfenstein <st162717@stud.uni-stuttgart.de>
 * @date Jan 12, 2019
 * @brief Brief description of the file
*/

#include "functions.h"
#include "map.h"
#include "player.h"
#include "mountain.h"
#include <cmath>

#define SQR2 1.41421356
#define MAXIMUM_ERRORS 3000
#define PI 3.14159

/**###############################################################################
 * @brief calculates the distance between two points in a cosy
 * @param position 1
 * @param position 2
 */
double distance(const int* pos1, const int* pos2){
  if (pos1 == nullptr || pos2 == nullptr) exit(1);
  double distance = sqrt( pow(pos1[0]-pos2[0], 2) + pow(pos1[1]-pos2[1], 2));

  return distance;
}


/**###############################################################################
 * @brief calculates the length of a vector
 * @param vector
 */
double distance(const double* pos){
  if (pos == nullptr) exit(1);
  double distance = sqrt( pow(pos[0], 2) + pow(pos[1], 2));

  return distance;
}

double distance(const int* pos){
  if (pos == nullptr) exit(1);
  double distance = sqrt( pow(pos[0], 2) + pow(pos[1], 2));

  return distance;
}


/**###############################################################################
 * @brief generates a new random startposition on a map for a player
 * @param Map to generate the startpos for
 * @param Factor for minimum distance between players
 */
const int* new_random_startpos(map& map, double factor){
  if (factor < 0.0) factor = 0.0;

  std::vector<player>& players = map.getPlayers();
  size_t current_players = players.size();

  //Minimum distance between players / water
  const double minimum_startpos_distance = map.getSize() / sqrt(factor*map.getPlayercount());

  const int* pos = new int[2];

  size_t count = 0;
  bool valid_position = false;
  int delta = map.getStartpositions()[1] - map.getStartpositions()[0];

  //find a valid startposition
  while (!valid_position) {

    if(count >= MAXIMUM_ERRORS){
      D(std::cout << "Didn't find a valid startposition after " << count << " iterations. Please retry!" << std::endl;)
      return nullptr;
    }

    ++count;
    delete[] pos;

    //generate Random position
    const int y = (std::rand() % delta) + map.getStartpositions()[0];
    const int x = (std::rand() % delta) + map.getStartpositions()[0];
    const int* pos = new int[2] {x_coord(x, y), y_coord(y)};

    if (current_players > 0) {
      for (size_t i = 0; i < current_players; ++i) {
        if (distance(pos, players.at(i).getPos())
            >= minimum_startpos_distance) {
          if (i == current_players - 1) {
            valid_position = true;
            break;
          } else {
            continue;
          }
        } else {
          break;
        }

      }
    } else {
      valid_position = true;
    }
  }

  D(std::cout << "random count: " << count << ", Position (" << pos[0] << "|" << pos[1] << ")" << std::endl;)


  return pos;
}


/**###############################################################################
 * @brief transforms map-coordinate into display-coordinate
 * @param Coordinate
 */
int y_coord(int y){
  return (int)(y/SQR2);
}


/**###############################################################################
 * @brief transforms map-coordinate into display-coordinate
 * @param x-Coordinate
 * @param y-Coordinate
 */
int x_coord(int x, int y){
  return x + y_coord(y);
}


/**###############################################################################
 * @brief transforms map-coordinates into display-coordinates
 * @param array with coordinates
 */
void convert_to_display_coords(int* mappos){
	mappos[1] = (int)(mappos[1]/SQR2);
	mappos[0] += mappos[1];
}


/**###############################################################################
 * @brief transforms display-coordinates into map-coordinates
 * @param array with coordinates
 */
void convert_to_map_coords(int* displaypos){
	displaypos[0] -= displaypos[1];
	displaypos[1] *= SQR2;
}


/**###############################################################################
 * @brief Draws a map with water around it and the players on it as dots
 * @param map to draw
 */
GDPC* draw_map(map& map){

  int xmax = x_coord(map.getSize(), map.getSize());
  int ymax = y_coord(map.getSize());

  std::cout << "Drawing a " << xmax << " x " << ymax << " map with " << map.getPlayercount() << " players." << std::endl;

  GDPC* c = GDPC_create(xmax, ymax);
  if (c==nullptr) exit(1);

  GDPC_title(c, "Map preview");

  //Draw parallelogram map Pattern
  for(int i=0; i<=ymax; ++i) {
    GDPC_line(c, ymax-i, 0, 0, ymax-i);
    GDPC_line(c, xmax-i, ymax, xmax, ymax-i);
  }

  //Draw water
  GDPC_color(c, 0, 127, 255);
  //horizontal
  for(int i=0; i<10; ++i) {
    GDPC_line(c, ymax-i, i, xmax-i, i);
    GDPC_line(c, i, ymax-i, xmax-ymax+i, ymax-i);
  }
  //diagonal
  for(int i=0; i<=14; ++i) {
    GDPC_line(c, xmax-i, 0, xmax-ymax-i, ymax);
    GDPC_line(c, i, ymax, ymax+i, 0);
  }

  draw_Players(map, c);
  draw_Mountains(map, c);


  return c;
}


/**###############################################################################
 * @brief draws Players on canvas
 * @param map to draw players from
 * @param canvas to draw on
 * @return canvas
 */
GDPC* draw_Players(map& map, GDPC* c){
  if(c==nullptr) return nullptr;

  //int xmax = x_coord(map.getSize(), map.getSize());
  int ymax = y_coord(map.getSize());

  //Draw Player Startpoints
  GDPC_color(c, 0, 127, 0);
  for(int i=0; i<map.getPlayercount(); ++i){
    const int* posp = map.getPlayers()[i].getPos();
    int pos[2] = {posp[0], posp[1]};

    GDPC_circle(c, pos[0], ymax-pos[1], 3, 1);
  }


  return c;
}


/**###############################################################################
 * @brief Draws all mountains on a canvas
 * @param map to draw mountains from
 * @param canvas to draw on
 * @return canvas
 */
GDPC* draw_Mountains(map& map, GDPC* c){
  if(c==nullptr) return nullptr;

  //int xmax = x_coord(map.getSize(), map.getSize());
  int ymax = y_coord(map.getSize());

  //Draw Mountains
  GDPC_color(c, 127, 127, 127);
  std::vector<mountain>& mountains = map.getMountains();
  D(std::cout << "there are amount of mountains: " << mountains.size() << std::endl;)
  for(size_t i=0; i < mountains.size(); ++i){
    const int* posp = mountains.at(i).getPos();
    int pos[2] = {posp[0], posp[1]};

    D(std::cout << "mountain position: " << pos[0] << "," << pos[1] << std::endl;)
    GDPC_circle(c, pos[0], ymax-pos[1], mountains[i].getSize(), 1);
  }


  return c;
}



/**###############################################################################
 * @brief Draws 1 Map and all players as dots in it
 * @param vector with map pointers
 * @return canvas of the window
 */
GDPC* draw_maps_combined(std::vector<map>& maps){
  GDPC* c = draw_map(maps.at(0));
  if (c==nullptr) return nullptr;

  for (size_t i=1; i<maps.size(); ++i){

    std::vector<player>& players = maps.at(i).getPlayers();
    size_t current_players = players.size();

    int ymax = y_coord(maps.at(i).getSize());

    GDPC_color(c, std::rand() % 255, std::rand() % 255, std::rand() % 255);

    //Draw players
    for(size_t j=0; j<current_players; ++j){

      const int* posp = maps.at(i).getPlayers()[j].getPos();
      int pos[2] = {posp[0], posp[1]};

      //GDPC_square(c, pos[0], ymax-pos[1], 9, 9);
      GDPC_circle(c, pos[0], ymax-pos[1], 3, 1);
    }

    //Draw mountains
    GDPC_color(c, 127, 127, 127);
    std::vector<mountain>& mountains = maps.at(i).getMountains();
    D(std::cout << "[DRAW] Amount of mountains: " << mountains.size() << std::endl;)
    for(size_t j=0; j < mountains.size(); ++j){

      const int* posp = mountains.at(j).getPos();
      D(std::cout << "[DRAW] Mountainposition: " << posp[0] << "|" << posp[1] << std::endl;)
      int pos[2] = {posp[0], posp[1]};

      GDPC_circle(c, pos[0], ymax-pos[1], mountains[j].getSize(), 1);
    }
  }

  return c;
}


/**###############################################################################
 * @brief generates a new random position for a mountain on a map
 * @param map
 * @return valid position for a mountain
 */
const int* new_random_mountainpos(map& map){

  std::vector<mountain>& mountains = map.getMountains();
  std::vector<player>& players = map.getPlayers();
  size_t current_mountains = mountains.size();
  size_t current_players = players.size();

  //Minimum distance between mountains
  const double minimum_mountain_distance = 6.0 * sqrt(map.getSize());
  //Minimum distance between mountain and player
  const double minimum_player_distance = 1.5 * sqrt(map.getSize());

  const int* pos = new int[2];

  size_t count = 0;
  bool valid_position = false;
  const int distance_to_water = 30;
  int delta = map.getSize() - 2*distance_to_water; // 50 wegen Wasser

  //find a valid startposition
  while (!valid_position) {

    if(count >= MAXIMUM_ERRORS){
      D(std::cout << "Didn't find a valid mountainposition after " << count << " iterations. Please retry!" << std::endl;)
      return nullptr;
    }

    ++count;
    delete[] pos;

    //generate VERY GENERAL random position on map
    const int y = (std::rand() % delta) + distance_to_water;
    const int x = (std::rand() % delta) + distance_to_water;
    const int* pos = new int[2] {x_coord(x, y), y_coord(y)};

    //Mountain-condition
    bool mountain_condition = false;
    if (current_mountains == 0)
      mountain_condition = true;
    for (size_t i = 0; i < current_mountains && !mountain_condition; ++i) {
      if (distance(pos, mountains.at(i).getPos()) >= minimum_mountain_distance) {
        if (i == current_mountains - 1) {
          mountain_condition = true;
          break;
        } else {
          continue;
        }
      } else { // mountaincondition not fulfilled!
        break;
      }
    }

    //Player-condition
    for (size_t i = 0; i < current_players && mountain_condition; ++i) {
      if (distance(pos, players.at(i).getPos()) >= minimum_player_distance) {

        if (i == current_players - 1) {
          valid_position = true;
          break;
        } else {
          continue;
        }
      } else { // player condition not fulfilled !
        break;
      }
    }
  }

  //Debugging
  D(std::cout << "[Mountain] Random count: " << count << ", Position (" << pos[0] << "|" << pos[1] << ")" << std::endl;)


  return pos;
}


/**
 * @brief gesucht ist ein algorithmus, der den Berg in die Richtung fortpflanzt, in der die wenigsten Spieler sind
 *        Ansatz: Die Summe aller Vektoren von Berg zu Spieler, jeweils durch distanz² geteilt
 */
const int* mountain_chain(map& map, mountain& mountain){

  std::vector<player>& players = map.getPlayers();
  size_t playercount = map.getPlayercount();

  double x = 0;
  double y = 0;

  for( size_t i=0; i<playercount; ++i){
    double factor = pow(distance(mountain.getPos(), players.at(i).getPos()), 2);
    x += (mountain.getPos()[0]*mountain.getPos()[0] - players.at(i).getPos()[0]*players.at(i).getPos()[0]) / factor;
    y += (mountain.getPos()[1]*mountain.getPos()[1] - players.at(i).getPos()[1]*players.at(i).getPos()[1]) / factor;
  }

  const int* direction = new int[2]{(int)x, (int)y};
  const double* direction2 = new double[2]{x, y};
  const int* normal_direction = new int[2]{(int)(mountain.getSize()*x/distance(direction2)), (int)(mountain.getSize()*y/distance(direction2))};

  D(std::cout << "Direction of mountain-chain: [" << normal_direction[0] << " | " << normal_direction[1] << "]" << std::endl;)
  delete []direction;

  return normal_direction;
}


/**
 * @brief Creates a mountain circle for bots with 3 coal, 2 iron, 1 stone, 1 gold
 * @param position the circle should be around; should be at startposition of bot
 * @param direction the circle-part will be in, a 2-dimensional vector with the length of the distance
 */
void create_botmountain(map& map, const int* pos, const int* direction){

	int x1 = 1;
	int y1 = 1;
	int x2 = direction[0];
	int y2 = direction[1];

	double dot = x1*x2 + y1*y2;
	double det = x1*y2 - y1*x2;
	double angle;

	if(direction[0] >= 0 && direction[1] >= 0){
		angle = atan2(dot, det);
	}
	else if(direction[0] >= 0 && direction[1] < 0){
		angle = atan2(det, dot);
	}
	else if(direction[0] < 0 && direction[1] < 0){
		angle = atan2(dot, det) - PI/2.0;
	}
	else if(direction[0] < 0 && direction[1] >= 0){
		angle = 2.0*PI + atan2(dot, det);
	}

	std::cout << "Phi is " << 180.0/PI * angle << std::endl;

	//move 90degrees to the right
	for(int i=-40; i<40; i+=10){
		double anglenow = angle + i*PI/180.0;
		std::cout << "Phinow: " << anglenow *180/PI << std::endl;
		const int currentpos[2] = {(int)(pos[0]+30.0*sin(anglenow)), (int)(pos[1]+30.0*cos(anglenow))};

		mountain m(5, currentpos);
		map.addMountain(m);
	}

}










