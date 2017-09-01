#ifndef GAME_H
#define GAME_H

#include <memory>
#include <adelie2d/adelie.h>
#include <imGtp/imGtp.h>

using namespace std;

#include "player/player.h"
#include "move.h"
#include "goban.h"
#include "constants.h"
#include "state.h"

enum class GameState { Lobby, Playing, Scoring, Over };

class Game;
typedef shared_ptr<Game> Game_p;

class Game {
public:
    Game(Ad_Scene_p Scene, unsigned BoardSize, float Komi);

	/**
	 * \fn void attach_player(Player_p player, unsigned color);
	 * Attaches the input player to the stone of the input color.
	 * Once a player is attached for black and white, the game will begin.
	 * If a game has already started, this will not reset the game, but it 
	 * will start asking the new player for moves on behalf of 'color'.
	 */
	void attach_player(Player_p player, unsigned color);

    /**
     * \fn void add_stone(unsigned X, unsigned Y);
     * \brief Adds a stone to the move tree at the given position
     * for the current turn player. The current player is then updated.
     * If X, Y are out of bounds of the current board size, this method will do nothing.
     */
    bool add_stone(unsigned X, unsigned Y);

    /**
     * \fn void undo_stone()
     * \brief Removes the current stone from the game tree.
     * The current move will then set to that stones parent 
     * if one exists.
     * Returns false if there are no moves to remove,
     * true in all other cases.
     */
    bool undo_stone();

    /**
     * \fn void pass()
     * \brief Pass the current players turn, this is added 
     * to the move stack and can be undone.
     */
    void pass();

    /**
     * \fn bool is_legal(unsigned X, unsigned Y, unsigned Player);
     * \brief Returns whether or not the input position is legal for the given player.
     */
    bool is_legal(unsigned X, unsigned Y, unsigned Player);

    /**
     * \fn void update()
     * \brief Perform logic updates for the current game. Read mouse events etc.
     */
    void update();

    /**
     * \fn void render()
     * \brief Render additional labels not contained within the scene object.
     */
    void render();

    /**
     * \fn inline unsigned get_stone(unsigned X, unsigned Y)
     * \brief Returns the stone occupying the given location.
     * If the input location is invalid, INVALID is returned.
     * Else, NOPLAYER, SLATE, or SHELL will be returned.
     */
    inline unsigned get_stone(unsigned X, unsigned Y) {
		if (X < 0 || Y < 0 || X >= size || Y >= size) {
			return INVALID;
		}

		return state[X + Y * size];
    }

    operator std::string() const { 
		string out = "";
		for (unsigned i=0; i<2*size-1; i++) { out += "_"; }

		for (unsigned i=0; i<(size * size); i++) {
			if (i % size == 0) { out += "\n"; }
			if (state[i] == NOPLAYER) { out += ". "; }
			if (state[i] == SLATE) { out += "x "; }
			if (state[i] == SHELL) { out += "o "; }
		}

		out += "\n";
		for (unsigned i=0; i<2*size-1; i++) { out += "-"; }
		return out;
    }

	// the size of the board for this game
    const unsigned size;
	// komi (given to white) for this game
	const float komi;
	// game board storing most of the scene information
    Goban_p board;

private:
    const Ad_Scene_p scene;
	GameState game_state;

	// interrfaces to players who we can ask for moves
	Player_p players[2];
    Move_p root_move;
    Move_p current_move;

	// who's turn is it to play?
    unsigned current_player;

	// indicates the position of the most recently played move
	Ad_GameNode_p move_indicator;

	// provides a preview of the position where the user
	// is thinking about making a move, if the player
	// does not respond with a valid position
	// this gamenode will simply be hidden
	Ad_GameNode_p preview_indicator;

	/**
	 * \fn void create_update_move_indicator();
	 * \brief Creates the move indicator if it does not exist,
	 * if it does, the position is updated to match the most recently played 
	 * stone.
	 */
	void create_update_move_indicator();

    /**
     * \fn inline void set_stone(unsigned X, unsigend Y) {
     * \brief Sets the state of the position at the input location.
     * If the location is illegal, this method does nothing.
     */
    inline void set_stone(unsigned X, unsigned Y, unsigned State) {
	if (X >= 0 && Y >= 0 && X < size && Y < size) {
	    state[X + Y * size] = State;
    }}

    /**
     * This is the current state of the game board.
     * Valid entries in this array are:
     * 	NOPLAYER
     * 	SLATE
     * 	SHELL
     * This will be an array of size: SIZExSIZE.
     */
    unique_ptr<char[]> state;

    /**
     * Encoded board state, one for each (displayed) move of the game.
     * As per the KO rule, no new move should create a board state matching
     * an in the games history.
     */
    vector<State_p> history;

    /**
     * \fn unsigned num_liberties()
     * \brief Counts the number of liberties for the given location.
     * If no stone is present, use the 'Player' argument to run a hypothetical check.
     * If a stone is present, set the 'Player' argument to NOPLAYER (default) and
     * the check will be run for the player whos stone occupies the given location.
     * If Player is given, and the stone is occupied, 0 will be returned in error.
     * The 'clean' is used for recursion and should be ignored.
     */
    unsigned num_liberties(unsigned X, unsigned Y, unsigned Player = NOPLAYER, bool clean = true);

    /**
     * \fn bool does_stone_capture(unsigned X, unsigned Y, unsigned Player);
     * \brief Checks whether or not a stone by Player at the given location would capture
     * a neigboring opposing group. If the location is occupied, this method returns false.
     */
    bool does_stone_capture(unsigned X, unsigned Y, unsigned Player);

    /**
     * \fn void remove_group(unsigned X, unsigned Y);
     * \brief Remove the group of stones from the board starting 
     * at the position (X,Y). If there is no stone at position
     * (X,Y), this method does nothing.
     */
    void remove_group(unsigned X, unsigned Y, Move_p killer);

    /**
     * \fn bool is_move_unique(unsigned X, unsigned Y, unsigned Player);
     * \brief If the given stone is added to the board, will it create a unique
     * board state that does not exist in the games history?
     * If the given location is not set to NOPLAYER, this method will return false.
     */
    bool is_move_unique(unsigned X, unsigned Y, unsigned Player);

    /**
     * \fn void swap_turn()
     * \brief Switches the current turn between black and white.
     * Note this is a utility function, and does not actually add a pass
     * move to the move tree.
     */
    void swap_turn();

	/**
	 * \fn void check_and_update_state();
	 * Checks if the current game_state should move
	 * between playing, scoring, and game over.
	 * If a change is necessary, the game_state value will be updated.
	 */
	void check_and_update_state();
};

#endif
