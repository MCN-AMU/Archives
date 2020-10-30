#pragma once

#include "enemy.hpp"

class EnemyManager
{
  private:
	Enemy enemies_[5];

  public:
	EnemyManager();
	~EnemyManager();
	void send(uint8_t x);
	void move(int8_t dir_x);
	void display();
};