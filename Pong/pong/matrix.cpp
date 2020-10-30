#include "matrix.hpp"

const uint8_t Matrix::R6 = 13;
const uint8_t Matrix::R5 = 12;
const uint8_t Matrix::R7 = 11;
const uint8_t Matrix::C2 = 10;
const uint8_t Matrix::C5 = 9;
const uint8_t Matrix::R3 = 8;
const uint8_t Matrix::R1 = 7;
const uint8_t Matrix::C4 = 6;
const uint8_t Matrix::C3 = 5;
const uint8_t Matrix::C1 = 4;
const uint8_t Matrix::R2 = 3;
const uint8_t Matrix::R4 = 2;
const uint8_t Matrix::COLUMN_COUNT = 5;
const uint8_t Matrix::ROW_COUNT = 7;
const uint8_t Matrix::COLUMNS[5] = {C1, C2, C3, C4, C5};
const uint8_t Matrix::ROWS[7] = {R1, R2, R3, R4, R5, R6, R7};
const uint8_t Matrix::HEIGHT = 4;
const uint8_t Matrix::WIDTH = 6;

void Matrix::init()
{
    for (auto &&column : Matrix::COLUMNS)
    {
        pinMode(column, OUTPUT);
    }
    for (auto &&row : Matrix::ROWS)
    {
        pinMode(row, OUTPUT);
    }
}

void Matrix::reset()
{
    for (auto &&column : Matrix::COLUMNS)
    {
        digitalWrite(column, LOW);
    }
    for (auto &&row : Matrix::ROWS)
    {
        digitalWrite(row, HIGH);
    }
}

void Matrix::all()
{
    for (auto &&column : Matrix::COLUMNS)
    {
        digitalWrite(column, HIGH);
    }
    for (auto &&row : Matrix::ROWS)
    {
        digitalWrite(row, LOW);
    }
}

void Matrix::one(uint8_t x, uint8_t y)
{
    digitalWrite(COLUMNS[y], HIGH);
    digitalWrite(ROWS[x], LOW);
    delay(1);
    digitalWrite(COLUMNS[y], LOW);
    digitalWrite(ROWS[x], HIGH);
}

Matrix::Matrix()
{
}

Matrix::~Matrix()
{
}

void Matrix::on(uint8_t row, uint8_t column)
{
    digitalWrite(COLUMNS[column], HIGH);
    digitalWrite(ROWS[row], LOW);
}

void Matrix::off(uint8_t row, uint8_t column)
{
    digitalWrite(COLUMNS[column], LOW);
    digitalWrite(ROWS[row], HIGH);
}

void Matrix::enable(uint8_t x, uint8_t y)
{
    on(x, y);
    delay(1);
    off(x, y);
}

void Matrix::wait(unsigned long duration)
{
	delay(duration);
}