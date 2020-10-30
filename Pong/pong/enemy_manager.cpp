#include "enemy_manager.hpp"

EnemyManager::EnemyManager()
	: enemies_{
		  Enemy{2, 5, 500},
		  Enemy{2, 6, 500},
		  Enemy{2, 7, 500},
		  Enemy{2, 8, 500},
		  Enemy{2, 9, 500}}
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::send(uint8_t x)
{
	if (millis() < Enemy::last_)
		return;

	uint8_t i = 0;
	while (i < 5)
	{
		if (!enemies_[i].live_)
			break;
		++i;
	}

	if (i == 5)
		return;

	auto ry = random(0, Matrix::HEIGHT);

	enemies_[i].ball_.translate(x, ry);

	enemies_[i].live_ = true;

	Enemy::last_ = millis() + Enemy::l_;
}

void EnemyManager::move(int8_t dir_x)
{
	for (auto &enemy : enemies_)
	{
		Vector vel{dir_x, 0};

		if (enemy.live_ && millis() > enemy.t_)
		{
			enemy.live_ = enemy.ball_.move_bullet(vel);
			enemy.t_ = millis() + enemy.k_;
		}
	}
}

void EnemyManager::display()
{
	for (auto &enemy : enemies_)
	{
		if (enemy.live_)
			enemy.ball_.display();
	}
}