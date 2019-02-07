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
#include "mountain.h"
#include "gdptools.h"


#define MAP_SIZE 704
#define PLAYERCOUNT 8
#define MAPCOUNT 1

#if (MAPCOUNT<2)
#define DRAW(m) GDPC* c = draw_map(m[0]);
#else
#define DRAW(m) GDPC* c = draw_maps_combined(m);
#endif


int main(void) {
  std::srand(std::time(0));


  std::vector<map> maps;
  /*for (size_t i=0; i<MAPCOUNT; ++i){
    map m(MAP_SIZE, PLAYERCOUNT);
    maps.push_back(m);
  }*/

  std::vector<player> players;
  for(int i=0; i<PLAYERCOUNT; ++i){
	  const int pos[2] = {50+(int)i*MAP_SIZE/PLAYERCOUNT, 50+(int)i*MAP_SIZE/PLAYERCOUNT};
	  players.push_back(player(0, pos));
  }
  maps.push_back(map(MAP_SIZE, players));

  DRAW(maps);

  if (GDPC_destroy(c)) exit(0);

  maps.clear();

  return 0;
}
