#pragma once

#include "ball.hpp"

struct Enemy
{
	static unsigned long last_;
	static unsigned long l_;

	Ball ball_;
	bool live_;
	unsigned long t_;
	unsigned long k_;

	Enemy(uint8_t game, uint8_t index, unsigned long k);	
};