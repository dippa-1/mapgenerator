/**
 * @file map.h
 * @author Dominik Helfenstein <st162717@stud.uni-stuttgart.de>
 * @date Jan 12, 2019
 * @brief Brief description of the file
*/

#ifndef INCLUDE_MAP_H_
#define INCLUDE_MAP_H_

#include <iostream>
#include <cmath>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>

class player;
class mountain;

class map {
private:
  int size;
  int playercount;
  std::vector<player> players;
  std::vector<mountain> mountains;
  int startpositions[2];

  void generatePlayers(void);
  void generateMountains(const size_t count);

public:
  map(int size, int playercount);
  map(int size, std::vector<player>& players);
  map(int size, std::vector<player>& players, std::vector<mountain>& mountains);
  ~map();
  void printc(void) const; //prints to console

  int getSize(void) const;
  int getPlayercount(void) const;
  const int* getStartpositions(void) const;
  std::vector<player>& getPlayers(void) ;
  std::vector<mountain>& getMountains(void) ;

  void addMountain(mountain m);
};


#endif /* INCLUDE_MAP_H_ */
