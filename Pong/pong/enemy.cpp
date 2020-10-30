#include "enemy.hpp"

unsigned long Enemy::l_ = 500;
unsigned long Enemy::last_ = millis() + Enemy::l_;

Enemy::Enemy(uint8_t game, uint8_t index, unsigned long k)
	: ball_{game, index},
	  k_{k}
{
}