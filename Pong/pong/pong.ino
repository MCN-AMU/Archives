#include <arduino.h>

#include "matrix.hpp"
#include "ball.hpp"
#include "pad.hpp"
#include "space_ship.hpp"
#include "enemy_manager.hpp"

// échelle de resistance.
// Physical computing. Livre.

Vector velocity{};

Ball ball{0};
Ball c1{1};
Ball c2{2};
Ball c3{3};
Ball c4{4};
Pad left_pad{&c1, &c2};
Pad right_pad{&c3, &c4};

SpaceShip ship{};
EnemyManager manager{};

unsigned long FREQ = 1000;
const uint8_t MAX_SCORE = 5;
unsigned long bom = 0;
bool left_pause = false;
bool right_pause = false;
unsigned long k_ = 3000;
unsigned long t_ = 0;

void setup()
{
	Serial.begin(9600);

	randomSeed(analogRead(5));

	ball = Ball(0);

	auto rx = 0;
	while (rx == 0)
	{
		rx = random(-1, 2);
	}

	auto ry = 0;
	while (ry == 0)
	{
		ry = random(-1, 2);
	}

	velocity = Vector{rx, ry};

	Matrix::init();
}

void loop()
{
	if (!left_pause && !right_pause)
	{
		uint8_t v1 = map(analogRead(0), 0, 1023, 0, Matrix::HEIGHT);
		uint8_t v2 = map(analogRead(1), 0, 1023, 0, Matrix::HEIGHT);

		left_pad.move(0, v1);
		right_pad.move(Matrix::WIDTH, v2);

		FREQ = 1000 /  map(analogRead(2), 0, 1023, 1, 100);
		if (millis() > bom + FREQ)
		{
			ball.move(velocity);
			bom = millis();
		}

		left_pause = Ball::get_left_new_score();
		right_pause = Ball::get_right_new_score();
		t_ = millis() + k_;

		left_pad.display();
		right_pad.display();
		ball.display();
	}

	if (left_pause)
	{
		uint8_t v = map(analogRead(1), 0, 1023, 0, Matrix::HEIGHT + 1);
		uint8_t trigger = map(analogRead(0), 0, 1023, 0, 3) > 0;

		manager.send(0);
		manager.move(1);
		manager.display();

		ship.move(Matrix::WIDTH, v);
		ship.shoot(Matrix::WIDTH, v, trigger);
		ship.display();

		if (Ball::get_space_new_score())
			t_ = millis() + k_;

		// left_pause = millis() < t_ || !Ball::get_space_game_over();
		left_pause = false;
	}

	if (right_pause)
	{
		uint8_t v = map(analogRead(0), 0, 1023, 0, Matrix::HEIGHT + 1);
		uint8_t trigger = map(analogRead(1), 0, 1023, 0, 3) > 0;

		manager.send(Matrix::WIDTH);
		manager.move(-1);
		manager.display();

		ship.move(0, v);
		ship.shoot(0, v, trigger);
		ship.display();

		if (Ball::get_space_new_score())
			t_ = millis() + k_;

		right_pause = millis() < t_ || !Ball::get_space_game_over();

		right_pause = false;
	}

	Matrix::reset();
	Ball::reset_context();
}
