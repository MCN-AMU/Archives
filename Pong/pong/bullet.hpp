#pragma once

#include "ball.hpp"

struct Bullet
{
	static unsigned long last_;
	static unsigned long l_;

	Ball ball_;
	bool shot_;
	unsigned long t_;
	unsigned long k_;

	Bullet(uint8_t game, uint8_t index, unsigned long k);	
};
