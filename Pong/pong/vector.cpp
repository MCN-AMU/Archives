#include "vector.hpp"
#include "Matrix.hpp"

Vector Vector::get_random(int16_t xmin, int16_t xmax, int16_t ymin, int16_t ymax)
{
	int16_t cx = Matrix::WIDTH / 2 + 1;
	int16_t cy = Matrix::HEIGHT / 2 + 1;

	int16_t rx = cx;
	while (rx == cx)
	{
		rx = random(xmin, xmax);
	}
	
	int16_t ry = cy;
	while (ry == cy)
	{
		ry = random(ymin, ymax);
	}

	return Vector{rx, ry};
}

Vector::Vector()
{
}

Vector::Vector(int16_t x, int16_t y)
{
    x_ = x;
    y_ = y;
}

Vector::~Vector()
{
}

Vector::operator+=(Vector const &a)
{
    x_ += a.x_;
    y_ += a.y_;
}

bool Vector::operator==(Vector const& a) const
{
	return x_ == a.x_ && y_ == a.y_;
}

Vector Vector::operator-(Vector const& a)
{
	Vector vector{};
	vector.x_ = x_ - a.x_;
	vector.y_ = x_ - a.y_;
	return vector;
}