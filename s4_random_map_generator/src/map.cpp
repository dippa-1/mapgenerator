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
  bool list_full = false;
  size_t count = 0;
  double factor = 0.0;
  while (!list_full) {
    ++count;
    if(count > 1000) {
      std::cout << "Could not generate map after " << count-1 << " attempts!" << std::endl;
      exit(2);
    }
    players.clear();
    for (int i = 0; i < playercount; ++i) {
      factor = 1 + count*0.05;
      const int* pos = new_random_startpos(this, factor);
      if (pos == nullptr){
        delete pos;
        break;
      }
      player* p = new player(0, pos);
      delete pos;
      players.push_back(p);
      if (i+1 == playercount) {
        list_full = true;
      }
    }

  }


  // Generate Mountains


  count = 0;
  list_full = false;
  while (!list_full) {
    ++count;
    if(count > 1000) {
      std::cout << "Could not generate map after " << count-1 << " attempts!" << std::endl;
      exit(2);
    }
    mountains.clear();
    for ( size_t i = 0; i<5; ++i){
      const int* pos = new_random_mountainpos(this);
      mountain* m = new mountain(10, pos);
      mountains.push_back(m);
      for ( size_t j=0; j<10; ++j){
        const int* chain = mountain_chain(this, mountains.at(i*10 + j));
        const int pos2[2] = {mountains.at(i*10 + j)->getPos()[0] + chain[0], mountains.at(i*10 + j)->getPos()[1] + chain[1]};
        mountain* m = new mountain(10, pos2);
        mountains.push_back(m);
      }
      delete pos;

      if (i+1 == 5) {
        list_full = true;
      }
    }
  }

  //std::cout << "Successfully generated map with " << count-1 << " fails!"
  //    << " and with a factor of " << factor << std::endl;
}


map::~map(){
  for (size_t i=0; i<players.size(); ++i){
    delete players.at(i);
    delete mountains.at(i);
  }
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


const std::vector<player*> map::getPlayers(void) const {
  return players;
}


const std::vector<mountain*> map::getMountains(void) const{
  return mountains;
}













