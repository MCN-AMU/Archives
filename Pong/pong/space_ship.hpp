#ifndef SPACE_SHIP
#define SPACE_SHIP

#include "bullet.hpp"
#include "ball.hpp"


class SpaceShip
{
private:
	Bullet bullets_[3];
	Ball gun;
	Ball body;
	unsigned long k;

public:
	SpaceShip();
	~SpaceShip();
	void move(uint8_t x, uint8_t y);
	void display();
	void shoot(uint8_t x, uint8_t y, bool trigger);
};

#endif

