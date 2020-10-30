#ifndef BALL
#define BALL

#include "matrix.hpp"
#include "vector.hpp"

class Ball
	: public Matrix
{
  private:
	static Ball *balls_[5];
	static Ball *balls2_[10];
	static uint8_t left_score;
	static uint8_t right_score;
	static bool left_new_score;
	static bool right_new_score;

	static uint8_t space_score;
	static bool space_new_score;
	static bool space_game_over;

  	Vector position_;
	uint8_t index_;

  public:
	static uint8_t get_left_score();
	static uint8_t get_right_score();
	static bool get_left_new_score();
	static bool get_right_new_score();
	static bool reset_score();

	static uint8_t get_space_score();
	static bool get_space_new_score();
	static bool reset_space_score();
	static bool get_space_game_over();

	static bool reset_context();

	Ball(uint8_t index);
	Ball(uint8_t game, uint8_t index);
	~Ball();
	void move(Vector &velocity);
	bool move_bullet(Vector &velocity);
	void translate(Vector translation);
	void translate(int16_t x, int16_t y);
	void display();
	Vector get_position();
};

#endif