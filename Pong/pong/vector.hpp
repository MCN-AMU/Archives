#ifndef VECTOR
#define VECTOR

#include <arduino.h>

class Vector
{
  public:
	static Vector get_random(int16_t xmin, int16_t xmax, int16_t ymin, int16_t ymax);

    int16_t x_;
    int16_t y_;

    Vector();
    Vector(int16_t x, int16_t y);
    ~Vector();
    operator +=(Vector const& a);
	bool operator ==(Vector const& a) const;
	Vector operator -(Vector const& a);

};

#endif