#include "ball.hpp"

Ball *Ball::balls_[5] = {nullptr};
Ball *Ball::balls2_[10] = {nullptr};
uint8_t Ball::left_score = 0;
uint8_t Ball::right_score = 0;
bool Ball::left_new_score = false;
bool Ball::right_new_score = false;

uint8_t Ball::space_score = 0;
bool Ball::space_new_score = false;
bool Ball::space_game_over = false;

bool Ball::get_space_game_over()
{
	return Ball::space_game_over;
}

uint8_t Ball::get_space_score()
{
	return Ball::space_score;
}

bool Ball::get_space_new_score()
{
	return Ball::space_new_score;
}

bool Ball::reset_space_score()
{
	Ball::space_score = 0;
}

uint8_t Ball::get_left_score()
{
	return Ball::left_score;
}

uint8_t Ball::get_right_score()
{
	return Ball::right_score;
}

bool Ball::get_left_new_score()
{
	return Ball::left_new_score;
}

bool Ball::get_right_new_score()
{
	return Ball::right_new_score;
}

bool Ball::reset_score()
{
	Ball::left_score = Ball::right_score = 0;
}

bool Ball::reset_context()
{
	Ball::left_new_score = Ball::right_new_score = false;
	Ball::space_new_score = false;
	Ball::space_game_over = false;
}

Ball::Ball(uint8_t index)
	: index_{index}
{
	Ball::balls_[index] = this;
	translate(Vector::get_random(1, Matrix::WIDTH, 0, Matrix::HEIGHT));
}

Ball::Ball(uint8_t game, uint8_t index)
	: index_{index}
{
	switch (game)
	{
	case 1:
		Ball::balls_[index] = this;
		translate(Vector::get_random(1, Matrix::WIDTH, 0, Matrix::HEIGHT));
	case 2:
		Ball::balls2_[index] = this;
		break;

	default:
		break;
	}
}

Ball::~Ball()
{
}

void Ball::move(Vector &velocity)
{
	position_ += velocity;

	if (position_.x_ < 0)
	{
		position_.x_ = -1;
		velocity.x_ *= -1;
		position_.x_ += velocity.x_;
	}

	if (position_.x_ > WIDTH)
	{
		position_.x_ = WIDTH + 1;
		velocity.x_ *= -1;
		position_.x_ += velocity.x_;
	}

	if (position_.y_ < 0)
	{
		position_.y_ = -1;
		velocity.y_ *= -1;
		position_.y_ += velocity.y_;
	}

	if (position_.y_ > HEIGHT)
	{
		position_.y_ = HEIGHT + 1;
		velocity.y_ *= -1;
		position_.y_ += velocity.y_;
	}

	for (auto &&ball : Ball::balls_)
	{
		if (ball == this)
			continue;

		if (ball->position_ == position_)
		{
			velocity.x_ *= -1;
			position_.x_ += velocity.x_ * 2;
		}
	}

	if (position_.x_ <= 0)
	{
		++Ball::left_score;
		Ball::left_new_score = true;
	}
	else if (position_.x_ >= WIDTH)
	{
		++Ball::right_score;
		Ball::right_new_score = true;
	}
}

void Ball::translate(Vector translation)
{
	if (translation.x_ < 0)
		position_ = Vector{0, translation.y_};
	else if (translation.x_ > WIDTH)
		position_ = Vector{WIDTH, translation.y_};
	else if (translation.y_ < 0)
		position_ = Vector{translation.x_, 0};
	else if (translation.y_ > HEIGHT)
		position_ = Vector{translation.x_, HEIGHT};
	else
		position_ = translation;
}

void Ball::translate(int16_t x, int16_t y)
{
	Vector translation{x, y};
	translate(translation);
}

void Ball::display()
{
	enable(position_.x_, position_.y_);
}

bool Ball::move_bullet(Vector &velocity)
{
	position_ += velocity;

	if (position_.x_ < 0 || position_.x_ > WIDTH)
	{
		return false;
	}

	if (position_.y_ < 0)
	{
		position_.y_ = -1;
		velocity.y_ *= -1;
		position_.y_ += velocity.y_;
	}

	if (position_.y_ > HEIGHT)
	{
		position_.y_ = HEIGHT + 1;
		velocity.y_ *= -1;
		position_.y_ += velocity.y_;
	}

	for (size_t i = 0; i < 5; i++)
	{
		if (balls2_[i] == this)
			continue;

		if (balls2_[i]->position_ == position_)
		{
			if (balls2_[i]->index_ < 2)
			{
				space_game_over = true;
				Serial.print("Game over: ");
				Serial.println(space_game_over);
			}
			else
			{
				space_score++;
				space_new_score = true;
				Serial.print("Score: ");
				Serial.println(space_score);
			}
			return false;
		}
	}

	return true;
}