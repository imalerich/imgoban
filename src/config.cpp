#include <fstream>
#include <sstream>

#include "player/roboplayer.h"
#include "player/userplayer.h"
#include "config.h"

unsigned SCREEN_W = 1366;
unsigned SCREEN_H = 768;

using namespace std;

Config::Config(std::string filename) {
	// setup default values
	board_size = 19;
	komi = 7.5;
	// default to human player for black and white
	white_player = "";
	black_player = "";

	frame_rate = 31;

	// read preferences from input config file to override the defaults
	ifstream cfg(filename);
	if (cfg.is_open()) {
		string line;
		while (getline(cfg, line)) {
			read_line(line);
		}

		cfg.close();
	}
}

void Config::read_line(std::string line) {
	// remove all characters after the first comment character
	size_t comment = line.size();
	for (size_t i=0; i<line.size(); i++) { 
		if (line[i] == '#') {
			comment = i;
			break;
	}}

	// remove all characters after & including the comment
	while (line.size() > comment) { line.erase(line.size()-1, 1); }

	// remove leading white space characters
	while (line.size() && isspace(line[0])) { line.erase(0, 1); }

	// ignore empty lines 
	bool empty = true;
	for (size_t i=0; i<line.size(); i++) { empty = empty && isspace(line[i]); }
	if (empty) { return; }

	// read the first word, this is the property the remainder of the string will set
	stringstream stream(line);
	string command;
	stream >> command;

	// read the data stored by the command
	if (command == "BOARD:") {
		stream >> board_size;
	} else if (command == "KOMI:") {
		stream >> komi;
	} else if (command == "WHITE:") {
		// erase the command & leading white space characters
		line.erase(0, command.size());
		while (line.size() && isspace(line[0])) { line.erase(0, 1); }

		white_player = line;
	} else if (command == "BLACK:") {
		// erase the command & leading white space characters
		line.erase(0, command.size());
		while (line.size() && isspace(line[0])) { line.erase(0, 1); }

		black_player = line;
	}

	if (command == "SCREEN_W:") {
		stream >> SCREEN_W;
	} else if (command == "SCREEN_H:") {
		stream >> SCREEN_H;
	} else if (command == "FRAME_RATE:") {
		stream >> frame_rate;
	}
}

Ad_Scene_p Config::create_scene() {
    if (!ad_init(SCREEN_W, SCREEN_H)) {
		return nullptr;
    }

    Ad_Scene_p scene = make_shared<Ad_Scene>(SCREEN_W, SCREEN_H);
    ad_set_frame_rate(frame_rate);
	return scene;
}

Game_p Config::create_game(Ad_Scene_p scene) {
    Game_p g = make_shared<Game>(scene, board_size, komi);

	// if a white engine is specified, run that engine
	if (white_player.size()) {
		g->attach_player(make_shared<RoboPlayer>(g->size, g->komi, white_player), SHELL);
	} else { // otherwise, attach a human player
		g->attach_player(make_shared<UserPlayer>(g->board, g->size), SHELL);
	}

	// if a black engine is specified, run that engine
	if (black_player.size()) {
		g->attach_player(make_shared<RoboPlayer>(g->size, g->komi, black_player), SLATE);
	} else { // otherwise, attach a human player
		g->attach_player(make_shared<UserPlayer>(g->board, g->size), SLATE);
	}

	return g;
}
