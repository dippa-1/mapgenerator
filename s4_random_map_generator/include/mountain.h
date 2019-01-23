/*
 * mountain.h
 *
 *  Created on: 19.01.2019
 *      Author: dominik
 */

#ifndef INCLUDE_MOUNTAIN_H_
#define INCLUDE_MOUNTAIN_H_


class mountain {
  private:
    int size;
    //future: int height;
    int pos[2];
    //future: std::vector<resource*> resources;

  public:
    mountain(int size, const int* pos);

    int getSize(void) const;
    const int* getPos(void) const;
};


#endif /* INCLUDE_MOUNTAIN_H_ */
