/**
 * @file main.cpp
 * @author Dominik Helfenstein <st162717@stud.uni-stuttgart.de>
 * @date Jan 12, 2019
 * @brief Brief description of the file
 * exit codes: 1 - functions
 *               2 - map
 *               3 - player
 * The coordinate-system start in the bottom left corner, unlike the normal top left corner!!!
*/

#include "map.h"
#include "player.h"
#include "functions.h"
#include "gdptools.h"


#define MAP_SIZE 1024
#define PLAYERCOUNT 8
#define MAPCOUNT 1


int main(void) {
  std::srand(std::time(0));


  std::vector<map*> maps;
  for (size_t i=0; i<MAPCOUNT; ++i){
    map* m = new map(MAP_SIZE, PLAYERCOUNT);
    maps.push_back(m);
  }

  GDPC* c = draw_maps_combined(maps);

  if (GDPC_destroy(c)) exit(1);

  // Delete all maps
  for (size_t i=0; i<MAPCOUNT; ++i){
    delete maps.at(i);
  }
  maps.clear();

  return 0;
}
