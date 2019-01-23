/**
 * @file player.cpp
 * @author Dominik Helfenstein <st162717@stud.uni-stuttgart.de>
 * @date Jan 12, 2019
 * @brief Brief description of the file
*/

#include "player.h"

player::player(bool bot, const int* pos)
  : bot(bot){
  this->pos[0] = pos[0];
  this->pos[1] = pos[1];
}

const int* player::getPos(void) const {
  return pos;
}


bool player::getBot(void) const {
  return bot;
}
