#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <adelie2d/adelie.h>

#include "constants.h"
#include "game.h"

class Config {
public:
	/**
	 * Generate a configuration object using the properties 
	 * stored in the input config file.
	 */
	Config(std::string filename);

	/**
	 * Constructs a secen from the scene configuration data.
	 * This can then be used to call the 'create_game' method.
	 */
	Ad_Scene_p create_scene();

	/**
	 * Constructs a new game using the settings
	 * stored in this configuration object.
	 */
	Game_p create_game(Ad_Scene_p scene);

private:
	void read_line(std::string line);

	size_t frame_rate; /**< Adelie render frame rate. */

	size_t board_size; /**< Board size. */
	float komi; /**< Komi given to white. */
	std::string white_player; /**< Engine to run white, empty if human. */
	std::string black_player; /**< Engine to run black, empty if human. */
};

#endif
