#ifndef DISPLAY_H
#define DISPLAY_H

extern ALLEGRO_DISPLAY * ad_display;
extern ALLEGRO_EVENT_QUEUE * ad_event_queue;

/**
 * \fn void set_frame_rate(double frame_rate)
 * \brief Updates the current frame rate to use.
 * \param fps Frame rate to use.
 */
void ad_set_frame_rate(double frame_rate);

/**
 * \fn double get_frame_rate()
 * \brief Gets the current frame rate (defualts to 60).
 * \return The current frame rate.
 */
double ad_get_frame_rate();

/**
 * \fn double get_seconds_per_frame()
 * \brief Returns the applications current seconds per frame.
 * Calculated as 1.0 / frame_rate
 *
 * \return Time of each frame in seconds.
 */
double ad_get_seconds_per_frame();

/**
 * \fn bool ad_init_adelie()
 * \brief Initialize necessary components for Adelie (windowed mode).
 * \return Whether or not the initialization was successful.
 */
bool ad_init(unsigned screen_width, unsigned screen_height);

/**
 * \fn bool ad_init_fullscreen()
 * \brief Initialize necessary components for adelie (fullscreen mode).
 * \return Whether or not the initialization was successful.
 */
bool ad_init_fullscreen();

/**
 * \fn void ad_release()
 * \brief Releases all objects owned by Adelie.
 *
 * Users of adelie will not need to call this function,
 * it will be called automatically by ad_run.
 */
void ad_release();

/**
 * \fn bool create_game_timer()
 * \brief Creates the game control timer using the current frame rate
 *
 * \return Whether or not the update was successful
 */
bool ad_create_game_timer();

/**
 * \fn unsigned ad_get_screen_width()
 * \brief Get the width dimmension of the screen.
 * \return The width of the screen in pixels.
 */
unsigned ad_get_screen_width();

/**
 * \fn unsigned ad_get_screen_height()
 * \brief Get the height dimmension of the screen.
 * \return The height of the screen in pixels.
 */
unsigned ad_get_screen_height();

#endif
