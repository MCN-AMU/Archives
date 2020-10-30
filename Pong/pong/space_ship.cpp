#include "space_ship.hpp"

SpaceShip::SpaceShip()
	: gun{2, 0},
	  body{2, 1},
	  bullets_{Bullet{2, 2, 500}, Bullet{2, 3, 500}, Bullet{2, 4, 500}}
{
}

SpaceShip::~SpaceShip()
{
}

void SpaceShip::move(uint8_t x, uint8_t y)
{
	if (x == 0)
	{
		gun.translate(x + 1, y);

		for (auto &bullet : bullets_)
		{
			if (bullet.shot_ && millis() > bullet.t_)
			{
				Vector vel{1, 0};
				bullet.shot_ = bullet.ball_.move_bullet(vel);
				bullet.t_ = millis() + bullet.k_;
			}
		}
	}
	else if (x == Matrix::WIDTH)
	{
		gun.translate(x - 1, y);

		for (auto &bullet : bullets_)
		{
			if (bullet.shot_ && millis() > bullet.t_)
			{
				Vector vel{-1, 0};
				bullet.shot_ = bullet.ball_.move_bullet(vel);
				bullet.t_ = millis() + bullet.k_;
			}
		}
	}
	body.translate(x, y);
}

void SpaceShip::display()
{
	gun.display();
	body.display();

	for (auto &bullet : bullets_)
	{
		if (bullet.shot_)
			bullet.ball_.display();
	}
}

void SpaceShip::shoot(uint8_t x, uint8_t y, bool trigger)
{
	if (millis() < Bullet::last_)
		return;

	if (!trigger)
		return;
	
	uint8_t i = 0;	
	while(i < 3)
	{
		if (!bullets_[i].shot_)
			break;
		++i;
	}

	if (i == 3)
		return;

	if (x == 0)
	{
		bullets_[i].ball_.translate(x + 2, y);
	}
	else if (x == Matrix::WIDTH)
	{
		bullets_[i].ball_.translate(x - 2, y);
	}

	bullets_[i].shot_ = true;
	Bullet::last_ = millis() + Bullet::l_;
}
