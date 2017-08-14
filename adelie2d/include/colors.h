#ifndef COLORS_H
#define COLORS_H

#include <allegro5/allegro.h>

#define BLACK al_map_rgb(0, 0, 0)
#define WHITE al_map_rgb(255, 255, 255)

#define DARK_GREY al_map_rgb(33, 33, 33)
#define MID_GREY al_map_rgb(128, 128, 128)
#define LIGHT_GREY al_map_rgb(201, 201, 201)

#define RED al_map_rgb(255, 0, 0)
#define LIME al_map_rgb(0, 255, 0)
#define BLUE al_map_rgb(0, 0, 255)

#define ORANGE al_map_rgb(255, 165, 0)
#define GREEN al_map_rgb(0, 128, 0)
#define INDIGO al_map_rgb(75, 0, 130)
#define PURPLE al_map_rgb(128, 0, 128)

#define YELLOW al_map_rgb(255, 255, 0)
#define CYAN al_map_rgb(0, 255, 255)
#define MAGENTA al_map_rgb(255, 0, 255)

/**
 * \fn ALLEGRO_COLOR mix_colors(ALLEGRO_COLOR c0, ALLEGRO_COLOR c1, double factor)
 * \brief Returns a combination of the input colors, weighted by factor.
 *
 * The return value is computed as:
 * 	factor * c0 + (1 - factor) * c1
 *
 * \param c0 First color to be mixed.
 * \param c1 Second color to be mixed.
 * \param factor Weighting applied to the input colors.
 * \return Resulting color of the mixture.
 */
ALLEGRO_COLOR ad_mix_colors(ALLEGRO_COLOR c0, ALLEGRO_COLOR c1, double factor);

#endif
