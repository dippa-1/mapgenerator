/**
 * @file map.h
 * @author Dominik Helfenstein <st162717@stud.uni-stuttgart.de>
 * @date Jan 12, 2019
 * @brief Brief description of the file
*/

#include <iostream>
#include <cmath>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>

#ifndef INCLUDE_MAP_H_
#define INCLUDE_MAP_H_

class player;
class mountain;

class map {
private:
  int size;
  int playercount;
  std::vector<player*> players;
  std::vector<mountain*> mountains;
  int startpositions[2];

public:
  map(int size, int playercount);
  ~map();
  void printc(void) const; //prints to console

  int getSize(void) const;
  int getPlayercount(void) const;
  const int* getStartpositions(void) const;
  const std::vector<player*> getPlayers(void) const;
  const std::vector<mountain*> getMountains(void) const;
};


#endif /* INCLUDE_MAP_H_ */
