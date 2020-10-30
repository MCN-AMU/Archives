#ifndef PAD
#define PAD

#include "ball.hpp"

class Pad
{
  public:
	Ball *child1_;
	Ball *child2_;

	Pad(Ball *child1, Ball *child2);
	~Pad();
	void move(uint8_t x, uint8_t y);
	void display();
};

#endif