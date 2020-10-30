#include "bullet.hpp"

unsigned long Bullet::l_ = 500;
unsigned long Bullet::last_ = millis() + Bullet::l_;

Bullet::Bullet(uint8_t game, uint8_t index, unsigned long k)
	: ball_{game, index},
	  k_{k}
{
}