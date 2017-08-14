#ifndef VECTOR2_H
#define VECTOR2_H

class Ad_Vector2 {
public:
	double x;
	double y;

	Ad_Vector2(double x = 0, double y = 0);

	Ad_Vector2 operator+(const Ad_Vector2 &v);
	Ad_Vector2 operator-(const Ad_Vector2 &v);
	Ad_Vector2 operator*(const double &s);
	Ad_Vector2 operator/(const double &s);
	bool operator==(const Ad_Vector2 &s);
};

#endif
