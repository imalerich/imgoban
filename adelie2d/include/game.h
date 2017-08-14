#ifndef GAME_H
#define GAME_H

/**
 * \fn int ad_run(int (update *)(), int (render *)());
 * \brief The main game loop.
 * 
 * \param update Pointer to the application update function. 
 * \param render Pointer to the application render function.
 */
int ad_run(bool (* update)(), bool (* render)());

/**
 * \fn double ad_get_run_time()
 * \brief Get the current application run time.
 * \return Current in game time (in seconds).
 */
double ad_get_run_time();

/**
 * \fn unsigned ad_get_run_time_mas()
 * \brief Get the current application run time.
 * \return Current in game time (in milliseconds).
 */
unsigned ad_get_run_time_ms();

#endif
