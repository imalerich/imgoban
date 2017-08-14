#ifndef IMAGE_H
#define IMAGE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "display.h"
#include "math/vector2.h"

/**
 * \fn ALLEGRO_BITMAP * ad_create_bitmap_rect(Ad_Vector2 size ALLEGRO_COLOR color)
 * \brief Creates a new bitmap with the input size and color.
 * The given returned bitmap will be registerd to Adelie2d's resources under
 * with a key generated from it's size and color, this key will be preceded by 
 * the identifier 'adelie2d.'. If a bitmap in the resources already exists of the given size and
 * color, it will be returend instead of a new bitmap.
 * \param size The size of the bitmap.
 * \param color The color of the bitmap.
 * \return The newly create bitmap.
 */
ALLEGRO_BITMAP * ad_create_rect_bitmap(Ad_Vector2 size, ALLEGRO_COLOR color);

/**
 * \fn ALLEGRO_BITMAP * ad_create_line_bitmap(Ad_Vector2 size, float angle, ALLEGRO_COLOR color);
 * \brief Creates a new bitmap representing a line.
 * The line will be centered in the image, with the given width and angle.
 * The line will be created using the given color, with transparent pixels used as the background.
 * \param length The length of the line.
 * \param width The width ef the line.
 * \param angle The angle of inclination for the line (in RADIANS).
 * \param color The color of the line.
 * \return The newly created bitmap.
 */
ALLEGRO_BITMAP * ad_create_line_bitmap(unsigned length, unsigned width, float angle, ALLEGRO_COLOR color);

/**
 * TODO
 */
ALLEGRO_BITMAP * ad_create_grid_bitmap(Ad_Vector2 size, Ad_Vector2 segments, unsigned segment_width, ALLEGRO_COLOR color);

/**
 * TODO
 */
ALLEGRO_BITMAP * ad_create_circle_bitmap(float size, ALLEGRO_COLOR color);

#endif
