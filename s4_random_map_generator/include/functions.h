/**
 * @file functions.h
 * @author Dominik Helfenstein <st162717@stud.uni-stuttgart.de>
 * @date Jan 12, 2019
 * @brief Brief description of the file
*/

//#include <iostream>
#include "gdpc.h"
#include <vector>

#ifndef INCLUDE_FUNCTIONS_H_
#define INCLUDE_FUNCTIONS_H_

class map;
class mountain;

double distance(const int* pos1, const int* pos2);
double distance(const double* pos);
const int* new_random_startpos(map* map, double factor);
int y_coord(int y);
int x_coord(int x, int y);
GDPC* draw_map(map* map);
GDPC* draw_maps_combined(std::vector<map*> maps);
const int* new_random_mountainpos(map* map);
const int* mountain_chain(map* map, mountain* mountain);



#endif /* INCLUDE_FUNCTIONS_H_ */
