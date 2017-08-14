#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <allegro5/allegro.h>

/**
 * \fn void ad_init_key_states()
 * \brief Initializes the current, prev, and key_states arrays.
 */
void ad_init_key_states();

/**
 * \fn void ad_clear_keys()
 * \brief Clears the keyboard to read the new round of input.
 * Called before reading keys for the current frame.
 */
void ad_clear_keys();

/**
 * \fn void ad_press_key(int keycode)
 * \brief Inputs the key to the current array in state key_down.
 * Called to fill out the current state arrays, these changes must
 * be applied with ad_apply_keys before they can be
 * tested with the ad_is_key_*() functions.
 * \param keycode The key the user has pressed.
 */
void ad_press_key(int keycode);

/**
 * \fn void ad_release_key(int keycode)
 * \brief Inputs the to the current array in state key_up.
 * Called to fill out the current states arrays, these changes must
 * be applied with ad_apply keys before they can be
 * tested with the ad_is_key_*() functions.
 * \param keycode The key the user has released.
 */
void ad_release_key(int keycode);

/**
 * \fn void ad_apply_keys()
 * \brief Builds the key_states array based on the prev and current keyboard states.
 * After clearing the current array to the prev array with ad_clear_keys() and
 * applying the current key states with ad_press_key, this will fill out the 
 * current key_states based on the two in order to test key_down, key_pressed, key_up
 * and key_released.
 */
void ad_apply_keys();

/**
 * \fn bool ad_is_key_down(int keycode)
 * \brief Returns true if the input key is down.
 * \param keycode The keycode to test.
 * \return Whether or not the input key is down.
 */
bool ad_is_key_down(int keycode);

/**
 * \fn bool ad_is_key_pressed(int keycode)
 * \brief Returns true if the user has just pressed the input key.
 * \param keycode The keycode to test.
 * \return Whether or not the input key was just pressed.
 */
bool ad_is_key_pressed(int keycode);

/**
 * \fn bool ad_is_key_up(int keycode)
 * \brief Returns true if the input key is up.
 * \param keycode The keycode to test.
 * \return Whether or not the input key is up.
 */
bool ad_is_key_up(int keycode);

/**
 * \fn bool ad_is_key_released(int keycode)
 * \brief Returns true if the user has just released the input key.
 * \param keycode The keycode to test.
 * \return Whether or not the input key was just released.
 */
bool ad_is_key_released(int keycode);

#endif
