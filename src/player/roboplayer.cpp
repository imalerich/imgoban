#include <iostream>
#include "player/roboplayer.h"
#include "constants.h"

RoboPlayer::RoboPlayer(unsigned BoardSize, float Komi, std::string engine) {
	// setup the gtp engine
	engine_controller = make_shared<gtp::Controller>(BoardSize, 6.5f);
	engine_controller->attach_engine(engine);
}

void RoboPlayer::move_played(Move_p move) {
	// convert the imgoban player type to a gtp player type
	gtp::Player gtp_player = move->player == SLATE ? 
		gtp::PLAYER_BLACK : gtp::PLAYER_WHITE;
	// then let the controller know about the move
	engine_controller->play_move(move->x, move->y, gtp_player);
}

Move_p RoboPlayer::gen_move(unsigned color) {
	// convert the imgoban player type to a gtp player type
	gtp::Player gtp_player = color == SLATE ? 
		gtp::PLAYER_BLACK : gtp::PLAYER_WHITE;

	// ask the engine for a new move
	gtp::Vertex v = engine_controller->gen_move(gtp_player);

	// create the temporary move object representing the generated move
	if (v.is_pass()) {
		return make_shared<Move>(color, MoveType::Pass);
	} else {
		return make_shared<Move>(v.get_x(), v.get_y(), color, MoveType::Stone);
}}
