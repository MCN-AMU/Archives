#ifndef MATRIX
#define MATRIX

#include <arduino.h>

class Matrix
{
  public:
    static const uint8_t R6;
    static const uint8_t R5;
    static const uint8_t R7;
    static const uint8_t C2;
    static const uint8_t C5;
    static const uint8_t R3;
    static const uint8_t R1;
    static const uint8_t C4;
    static const uint8_t C3;
    static const uint8_t C1;
    static const uint8_t R2;
    static const uint8_t R4;
    static const uint8_t COLUMN_COUNT;
    static const uint8_t ROW_COUNT;
    static const uint8_t COLUMNS[5];
    static const uint8_t ROWS[7];
    static const uint8_t HEIGHT;
    static const uint8_t WIDTH;

    static void init();
    static void reset();
	static void all();
	static void one(uint8_t x, uint8_t y);
	static void wait(unsigned long duration);

    Matrix();
    ~Matrix();
    void on(uint8_t row, uint8_t column);
    void off(uint8_t row, uint8_t column);
    void enable(uint8_t x, uint8_t y);
};

#endif