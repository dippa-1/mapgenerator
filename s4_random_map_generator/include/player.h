/**
 * @file player.h
 * @author Dominik Helfenstein <st162717@stud.uni-stuttgart.de>
 * @date Jan 12, 2019
 * @brief Brief description of the file
*/

#ifndef INCLUDE_PLAYER_H_
#define INCLUDE_PLAYER_H_


class player {
private:
  bool bot;
  int pos[2];

public:
  player(bool bot, const int* pos);

  bool getBot(void) const;
  const int* getPos(void) const;
};


#endif /* INCLUDE_PLAYER_H_ */
