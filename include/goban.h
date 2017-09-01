#ifndef GOBAN_H
#define GOBAN_H

#include <memory>
#include <adelie2d/adelie.h>

#include "move.h"

using namespace std;

class Goban;
typedef shared_ptr<Goban> Goban_p;

class Goban {
public:
    Goban(Ad_Scene_p Scene, unsigned BoardSize);

    void renderLabels();

    inline unsigned getGridSize() {
		// the grid size is square, so just use the x component
		return grid->get_bounds().get_size().x;
    }

    inline unsigned getBoardSize() {
		// the board size is square, so just use the x component
		return node->get_bounds().get_size().x;
    }

	inline unsigned getStoneSize() {
		// stone size is square, only need one component
		return 0.95f * getGridSize()/(size-1);
	}

	/**
	 * \fn Ad_Vector2 grid_coords_for_pos(unsigned x, unsigned y);
	 * Converts the input board coordinates into a stones position
	 * relative to the grid.
	 */
	Ad_Vector2 grid_coords_for_pos(unsigned x, unsigned y);

    /**
     * \fn void add_graphics_for_stone(Move * stone);
     * \brief Given a valid move of type stone with positional information,
     * adds shadow and stone graphics for that move to the current stone
     * and associates those with the input move.
     * If the input move already has a shadow/stone associated with it,
     * this method does nothing.
     */
    void add_graphics_for_stone(Move_p stone, const unsigned current_player);

    Ad_GameNode_p node;
    Ad_GameNode_p grid;

private:
    ALLEGRO_FONT * font;

    const Ad_Scene_p scene;
    const unsigned size;

    void addToScene();
    void addGridToBoard();
    void addStarsToBoard();
};

#endif
