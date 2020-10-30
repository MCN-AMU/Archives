#include "pad.hpp"

Pad::Pad(Ball *child1, Ball *child2)
	: child1_{child1},
	  child2_{child2}
{
}

Pad::~Pad()
{
}

void Pad::move(uint8_t x, uint8_t y)
{
	child1_->translate(x, y);
	child2_->translate(x, y + 1);
}

void Pad::display()
{
	child1_->display();
	child2_->display();
}