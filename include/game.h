#ifndef GAME_H
#define GAME_H

#include <memory>
#include <adelie2d/adelie.h>

using namespace std;

#include "move.h"
#include "goban.h"

class Game;
typedef shared_ptr<Game> Game_p;

class Game {
public:
    Game(Ad_Scene_p Scene, unsigned BoardSize);

    /**
     * \fn void add_stone(unsigned X, unsigned Y);
     * \brief Adds a stone to the move tree at the given position
     * for the current turn player. The current player is then updated.
     * If X, Y are out of bounds of the current board size, this method will do nothing.
     */
    bool add_stone(unsigned X, unsigned Y);

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

private:
    const unsigned size;
    const Ad_Scene_p scene;

    Goban_p board;
    Move_p root_move;
    Move * current_move;

    unsigned current_player;

    /**
     * \fn void swap_turn()
     * \brief Switches the current turn between black and white.
     * Note this is a utility function, and does not actually add a pass
     * move to the move tree.
     */
    void swap_turn();
};

#endif
