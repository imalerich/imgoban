#include <adelie2d/adelie.h>
#include <cmath>

#include "player/userplayer.h"

UserPlayer::UserPlayer(Goban_p Goban, unsigned BoardSize) 
	: board{Goban}, size{BoardSize} { }

Move_p UserPlayer::gen_move(unsigned color) {
	// generate a move when a mouse button is released
	if (ad_is_button_released(0)) {
		int x = round((size-1) * (ad_get_mouse_x() - board->grid->get_absolute_pos().x) 
				/ board->getGridSize());
		int y = round((size-1) * (ad_get_mouse_y() - board->grid->get_absolute_pos().y) 
				/ board->getGridSize());
		return make_shared<Move>(x, y, color, MoveType::Stone);
	} else if (ad_is_button_released(1)) {
		return make_shared<Move>(color, MoveType::Pass);
	}

	// else no move is ready
	return nullptr;
}

Move_p UserPlayer::consider_move(unsigned color) {
	// This is not the actual move we are going to play,
	// but it is where the users mouse is currently located at.
	// Have the UI display a semi-transparent stone at this location
	// if that location is available.
	int x = round((size-1) * (ad_get_mouse_x() - board->grid->get_absolute_pos().x) 
			/ board->getGridSize());
	int y = round((size-1) * (ad_get_mouse_y() - board->grid->get_absolute_pos().y) 
			/ board->getGridSize());
	return make_shared<Move>(x, y, color, MoveType::Stone);
}
