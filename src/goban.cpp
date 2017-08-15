#include <stdlib.h>

#include "goban.h"
#include "constants.h"

Goban::Goban(Ad_Scene_p Scene, unsigned BoardSize) : 
	scene{Scene}, size{BoardSize} {

    addToScene();
    randInit();
}

unsigned Goban::getGridSize() {
    return grid->get_bounds().get_size().x;
}

unsigned Goban::getBoardSize() {
    return node->get_bounds().get_size().x;
}

/* ---------------------------------------------------------------------------------------------
 * Create the scene which we will use to render the board.
 * --------------------------------------------------------------------------------------------- */

void Goban::addToScene() {
    const unsigned offset = (SCREEN_W - BOARD_SIZE)/2;

    Ad_GameNode_p bottom = make_shared<Ad_GameNode>(Ad_Rect(offset, BOARD_SIZE + BOARD_PADDING/2, 
	BOARD_SIZE, BOARD_DEPTH));
    bottom->set_bitmap(ad_create_rect_bitmap(Ad_Vector2(BOARD_SIZE, BOARD_DEPTH), DARK_COLOR));
    scene->get_root()->add_child(bottom);

    node = make_shared<Ad_GameNode>(Ad_Rect(offset, BOARD_PADDING/2, BOARD_SIZE, BOARD_SIZE));
    node->set_bitmap(ad_create_rect_bitmap(Ad_Vector2(BOARD_SIZE, BOARD_SIZE), LIGHT_COLOR));
    scene->get_root()->add_child(node);

    addGridToBoard();
    addStarsToBoard();

    if (!(font = al_load_ttf_font("../fonts/OpenSans-Regular.ttf", 12, 0))) {
	cerr << "Failed to load board font at: ../fonts/OpenSans-Regular.ttf" << endl;
	exit(1);
    }
}

void Goban::addGridToBoard() {
    // We need to compute a valid grid size so that each line is visible and evenly spaced.
    const int GAP = (int)((BOARD_SIZE * 0.9 - size)/(size-1));

    const unsigned GRID_SIZE = size + (size-1) * GAP;
    const unsigned GRID_OFF = (BOARD_SIZE - GRID_SIZE)/2;
    grid = make_shared<Ad_GameNode>(Ad_Rect(GRID_OFF, GRID_OFF, GRID_SIZE, GRID_SIZE));
    grid->set_bitmap(ad_create_grid_bitmap(Ad_Vector2(GRID_SIZE, GRID_SIZE), 
	Ad_Vector2(size, size), 1, DARK_COLOR));
    node->add_child(grid);
}

void Goban::addStarsToBoard() {
    const float POSITIONS[] = { 3/18.0f, 9/18.0f, 15/18.0f };
    const auto STAR_BMP = ad_create_circle_bitmap(32, DARK_COLOR);

    for (int i=0; i<3; i++) {
	for (int k=0; k<3; k++) {
	    Ad_GameNode_p star = make_shared<Ad_GameNode>(Ad_Rect(
		getGridSize() * POSITIONS[i] - (STAR_SIZE/2), 
		getGridSize() * POSITIONS[k] - (STAR_SIZE/2), 
		STAR_SIZE, STAR_SIZE
	    ));

	    star->set_bitmap(STAR_BMP);
	    grid->add_child(star);
	}
    }
}

void Goban::randInit() {
    const unsigned GRID_OFF = (getBoardSize() - getGridSize())/2;
    const unsigned SPACING = getGridSize()/(size-1);

    for (unsigned y=0; y<size; y++) {
	for (unsigned x=0; x<size; x++) {
	    unsigned r = rand() % 4;
	    Ad_Vector2 pos(
		GRID_OFF + SPACING * x - SPACING/2,
		GRID_OFF + SPACING * y - SPACING/2
	    );
	    if (r < 2) {
		// There will be a stone, so add the drop shadow.
		Ad_GameNode_p shadow = make_shared<Ad_GameNode>("../img/shadow.png", Ad_Vector2(
		    pos.x - (52 - SPACING)/2 + 5,
		    pos.y - (52 - SPACING)/2 + 5
		));
		node->add_child(shadow);
	    }

	    if (r == 0) {
		// Black Stone.
		Ad_GameNode_p shadow = make_shared<Ad_GameNode>("../img/slate.png", pos);
		node->add_child(shadow);
	    } else if (r == 1) {
		// White Stone.
		Ad_GameNode_p shadow = make_shared<Ad_GameNode>("../img/shell.png", pos);
		node->add_child(shadow);
	    } // Else no stone.
	}
    }
}


/* ---------------------------------------------------------------------------------------------
 * Render the text labels indicating position.
 * --------------------------------------------------------------------------------------------- */

void Goban::renderLabels() {
    const unsigned GRID_OFF = (getBoardSize() - getGridSize())/2;

    for (unsigned i=0; i<size; i++) {
	auto x = node->get_bounds().get_pos().x + GRID_OFF + (getGridSize()/(size-1) * i);
	auto y = node->get_bounds().get_pos().y + 4;
	al_draw_text(font, DARK_COLOR, x, y, ALLEGRO_ALIGN_CENTRE, string(1, 'A' + i).c_str());
    }

    for (unsigned i=0; i<size; i++) {
	auto x = node->get_bounds().get_pos().x + GRID_OFF + (getGridSize()/(size-1) * i);
	auto y = node->get_bounds().get_pos().y + 2 * GRID_OFF + getGridSize() - 20;
	al_draw_text(font, DARK_COLOR, x, y, ALLEGRO_ALIGN_CENTRE, string(1, 'A' + i).c_str());
    }

    for (unsigned i=0; i<size; i++) {
	int pos = size - i;
	auto x = node->get_bounds().get_pos().x + 4;
	auto y = node->get_bounds().get_pos().y + GRID_OFF + (getGridSize()/(size-1) * i) - 6;
	al_draw_text(font, DARK_COLOR, x, y, ALLEGRO_ALIGN_LEFT, to_string(pos).c_str());
    }

    for (unsigned i=0; i<size; i++) {
	int pos = size - i;
	auto x = node->get_bounds().get_pos().x + 2 * GRID_OFF + getGridSize() - 6;
	auto y = node->get_bounds().get_pos().y + GRID_OFF + (getGridSize()/(size-1) * i) - 6;
	al_draw_text(font, DARK_COLOR, x, y, ALLEGRO_ALIGN_RIGHT, to_string(pos).c_str());
    }
}