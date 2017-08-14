#ifndef RECT_H
#define RECT_H

#include <stdbool.h>
#include "vector2.h"

class Ad_Rect {
public:
	Ad_Rect(double x = 0, double y = 0, double width = 0, double height = 0);
	Ad_Rect(Ad_Vector2 pos, Ad_Vector2 size);

	/**
	 * \fn Ad_Vector2 get_pos()
	 * \brief Returns the position of this rect.
	 * \return The position of this rect.
	 */
	inline Ad_Vector2 get_pos() {
		return pos;
	}

	/**
	 * \fn Ad_Vector2 get_size()
	 * \brief Returns the size of this rect.
	 * \return The size of this rect.
	 */
	inline Ad_Vector2 get_size() {
		return size;
	}

	/**
	 * \fn bool does_intersect(const Ad_Rect &rect)
	 * \brief Determines whether the edges of the two rects intersect.
	 * \param rect The other to check for intersections with this rect.
	 * \return Whether or not the edges of the two rects intersect.
	 */
	bool does_intersect(const Ad_Rect &rect);

	/**
	 * \fn bool does_overlap(const Ad_Rect &rect)
	 * \brief Determines whether the two rects overlap.
	 * \param rect The other to check for intersections with this rect.
	 * \return Whether or not this rect and the input rect overlap.
	 */
	bool does_overlap(const Ad_Rect &rect);

	Ad_Rect operator+(const Ad_Vector2 &r);
	Ad_Rect operator-(const Ad_Vector2 &r);
	Ad_Rect operator*(const double &s);
	Ad_Rect operator/(const double &s);
	bool operator==(const Ad_Rect &r);

private:
	Ad_Vector2 pos;
	Ad_Vector2 size;
};

#endif
