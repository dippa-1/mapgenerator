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
  generateMountains(10U);

}


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
      player p(0, pos);
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

  D(std::cout << "Generated " << mountains.size() << "/" << count << " mountains afer " << tries-1 << " tries." << std::endl;)
}











