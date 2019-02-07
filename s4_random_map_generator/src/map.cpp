/**
 * @file map.cpp
 * @author Dominik Helfenstein <st162717@stud.uni-stuttgart.de>
 * @date Jan 12, 2019
 * @brief Brief description of the file
*/

#include "map.h"
#include "player.h"
#include "functions.h"
#include "mountain.h"
#include <cmath>

#define RANDOM_CHANCE 4

map::map(int size, int playercount)
  : size(size), playercount(playercount){

  // calculate the minimum distance between each player
  double minimum_startpos_distance = size / sqrt(2.0*playercount);

  // calculate the range of startposition-coordinates
  const int watersize = 10;
  //min and max
  startpositions[0] = watersize + (int)(minimum_startpos_distance/4.0);
  startpositions[1] = size - startpositions[0];


  // create Players
  generatePlayers();

  // Generate Mountains
  generateMountains(5U);

  // create botmountains
  for(int i=0; i<playercount; ++i){
	  if(players[i].getBot()){
		  const int dir[2] = {size/2 - players[i].getPos()[0], size/2 - players[i].getPos()[1]};
		  std::cout << "Direction of Botmountain: " << dir[0] << "|" << dir[1] << std::endl;
		  create_botmountain(*this, players[i].getPos(), dir);
	  }
  }

}


map::map(int size, std::vector<player>& players): size(size), players(players), playercount(players.size()){
	generateMountains(5U);
}


map::map(int size, std::vector<player>& players, std::vector<mountain>& mountains)
	: size(size), players(players), mountains(mountains), playercount(players.size())
{}


map::~map(){
  players.clear();
  mountains.clear();
}


void map::printc(void) const {
  std::cout << "Map size is " << getSize()
      << " with " << getPlayercount() << " Players." << std::endl
      << "  The possible startpositions range between (" << startpositions[0] << "|" << startpositions[1]
      << ") and (" << startpositions[0] << "|" << startpositions[1] << ")"
      << std::endl;
}


int map::getSize(void) const{
  return size;
}


int map::getPlayercount(void) const {
  return playercount;
}


const int* map::getStartpositions(void) const {
  return startpositions;
}


std::vector<player>& map::getPlayers(void)  {
  return players;
}


std::vector<mountain>& map::getMountains(void) {
  return mountains;
}


void map::generatePlayers(){

  size_t tries = 0;
  double factor = 0.0;
  while (players.size() != (size_t)playercount) {
    ++tries;
    if(tries > 1000) {
      D(std::cout << "Could not generate players after " << tries-1 << " tries!" << std::endl;)
      exit(2);
    }
    players.clear();
    for (int i = 0; i < playercount; ++i) {
      factor = 1 + tries*0.05;
      const int* pos = new_random_startpos(*this, factor);
      if (pos == nullptr){
        break;
      }
      player p(i % 2, pos);
      players.push_back(p);
    }

  }
}


void map::generateMountains(const size_t count) {
  size_t tries = 0;
  while (mountains.size() != count) {
    ++tries;
    if(tries > 3000) {
      std::cout << "Could not generate " << count << " mountains after " << tries-1 << " tries!" << std::endl;
      exit(2);
    }
    mountains.clear();
    for ( size_t i = 0; i<count-tries+1; ++i){
      const int* pos = new_random_mountainpos(*this);
      if(pos == nullptr) break;
      mountain m(10, pos);
      mountains.push_back(m);
    }
  }

  // now generate mountain-chains
  /*const double minimum_player_distance = 1.5 * sqrt(size);
  const int distance_to_water = 10;
  for(size_t i = 0; i<count; ++i) {
	  std::vector<mountain> m_chain;
	  m_chain.push_back(mountains[i]);
	  for(size_t j=0; (std::rand() % RANDOM_CHANCE) >= 1; ++j){
		  const int* pos_origin = m_chain[j].getPos();
		  const int* direction = mountain_chain(*this, m_chain[j]);
		  const int pos[2] = {pos_origin[0] + direction[0], pos_origin[1] + direction[1]};
		  if(distance(pos, players.at(i).getPos()) >= minimum_player_distance
			//	  && x_coord(pos[0], pos[1])
		  	  //	  < x_coord(size-distance_to_water-5, size-distance_to_water-5)
				//  && x_coord(pos[0], pos[1])
				//  > x_coord(distance_to_water+5, distance_to_water+5)
				//  && y_coord(pos[1])
		  	  	//  < y_coord(size-distance_to_water-5)
				 // && y_coord(pos[1])
		  	  	  //> y_coord(distance_to_water+5))

			  && pos[0]
	  	  	  < size-2*distance_to_water
			  && pos[0]
			  > 2*distance_to_water
			  && pos[1]
	  	  	  < size-2*distance_to_water
			  && pos[1]
	  	  	  > 2*distance_to_water)
		  {
			  mountain m(10, pos);
			  m_chain.push_back(m);
			  mountains.push_back(m);
		  }
		  else{
			  break;
		  }
	  }
  }*/

  D(std::cout << "Generated " << mountains.size() << "/" << count << " mountains afer " << tries-1 << " tries." << std::endl;)
}


void map::addMountain(mountain m){
	mountains.push_back(m);
}










