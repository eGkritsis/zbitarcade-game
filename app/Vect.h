#pragma once
class Vect
{
private:
	float x, y;
public:
	Vect();
	Vect(float x, float y);
	float getX();
	float getY();
	void setX(float x);
	void setY(float y);
	Vect operator+(const Vect& v);
	Vect operator-(const Vect& v);
	Vect operator*(float num);
};

