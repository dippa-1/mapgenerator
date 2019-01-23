/*
 * mountain.cpp
 *
 *  Created on: 19.01.2019
 *      Author: dominik
 */

#include "mountain.h"

mountain::mountain(int size, const int* pos): size(size){
  this->pos[0] = pos[0];
  this->pos[1] = pos[1];
}


int mountain::getSize(void) const {
  return size;
}


const int* mountain::getPos(void) const {
  return pos;
}
