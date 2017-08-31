#ifndef ROBO_PLAYER_H
#define ROBO_PLAYER_H

#include <imGtp/imGtp.h>
#include "player/player.h"

class RoboPlayer : public Player {
public:
	RoboPlayer(unsigned BoardSize, float Komi);

	// override virtual functions of 'Player'
	void move_played(Move_p move);
	Move_p gen_move(unsigned color);
	
private:
	// this is our interface to the current go engine
	gtp::Controller_p engine_controller;
};

#endif
