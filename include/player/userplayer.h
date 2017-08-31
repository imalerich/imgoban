#ifndef USER_PLAYER_H
#define USER_PLAYER_H

#include "player/player.h"

class UserPlayer : public Player {
public:
	// override virtual functions of 'Player'
	void move_played(Move_p move);
	Move_p gen_move(unsigned color);
	Move_p consider_move(unsigned color);

};

#endif
