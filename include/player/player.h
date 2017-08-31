#ifndef PLAYER_H
#define PLAYER_H

#include "move.h"

class Player;
typedef shared_ptr<Player> Player_p;

class Player {
public:

	/**
	 * \fn virtual void move_was_player();
	 * Inform the player if a move was made.
	 * For users, the board will be enough, so this 
	 * you can safely ignore calls to this method.
	 * However for gtp/remote players, they will need
	 * to know about stones local users make.
	 */
	virtual void move_played(Move_p move) = 0;

	/**
	 * \fn virtual Move_p gen_move(unsigned Player);
	 * Asks the player to make a move on behalf of
	 * the input color. If no move is currently ready,
	 * simply return nullptr.
	 */
	virtual Move_p gen_move(unsigned color) = 0;

	/**
	 * \fn virtual Move_p consider_move(unsigned color);
	 * If the user is considering a move (i.e. mouse over)
	 * draw a preview at that location. If no preview is necessary,
	 * simply return nullptr in this method (this is the default behavior).
	 */
	virtual Move_p consider_move(unsigned color) {
		return nullptr;
	}

};

#endif
