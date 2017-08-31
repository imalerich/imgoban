#ifndef USER_PLAYER_H
#define USER_PLAYER_H

#include "player/player.h"
#include "goban.h"
#include "move.h"

class UserPlayer : public Player {
public:
	UserPlayer(Goban_p Goban, unsigned BoardSize);

	// override virtual functions of 'Player'
	Move_p gen_move(unsigned color);
	Move_p consider_move(unsigned color);

	// need a reference to the board so we can do
	// some math with the users cursor
	const Goban_p board;
	// size of the board (i.e. 9, 13, 19)
	const unsigned size;
};

#endif
