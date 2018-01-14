#include <stdlib.h>
#include <math.h>

#include "goban.h"
#include "constants.h"

Goban::Goban(Ad_Scene_p Scene, unsigned BoardSize) : 
	scene{Scene}, size{BoardSize} {

    addToScene();
}

void Goban::add_graphics_for_stone(Move_p stone, const unsigned current_player) {
    if (!stone || stone->is_on_board()) { return; }

    // get positional information for the stone
    const unsigned GRID_OFF = (getBoardSize() - getGridSize())/2;
    const unsigned SPACING = getGridSize()/(size-1);
    const unsigned SIZE = SPACING * 0.95;
    const auto OFF = SPACING * 0.25f;

    Ad_Rect bounds(
		GRID_OFF + SPACING * stone->x - SIZE/2,
		GRID_OFF + SPACING * stone->y - SIZE/2,
		SIZE, SIZE
    );

    // now that we have the move, we need to add it to our scene
    stone->shadow = make_shared<Ad_GameNode>(SHADOW_IMG, Ad_Rect(
		bounds.get_pos().x - OFF, bounds.get_pos().y - OFF * 0.3,
		bounds.get_size().x + 2 * OFF, bounds.get_size().y + 2 * OFF
    ), 1);

    const char * img = (current_player == SLATE) ? SLATE_IMG : SHELL_IMG;
    stone->stone = make_shared<Ad_GameNode>(img, bounds, 2);

    // add the game nodes to the scene to be rendered
    node->add_child(stone->shadow);
    node->add_child(stone->stone);
}

Ad_Vector2 Goban::grid_coords_for_pos(unsigned x, unsigned y) {
    const unsigned GRID_OFF = (getBoardSize() - getGridSize())/2;
    const unsigned SPACING = getGridSize()/(size-1);
    const unsigned SIZE = SPACING * 0.95;

	return Ad_Vector2(
		GRID_OFF + SPACING * x - SIZE/2,
		GRID_OFF + SPACING * y - SIZE/2);
}

/* ---------------------------------------------------------------------------------------------
 * Create the scene which we will use to render the board.
 * --------------------------------------------------------------------------------------------- */

void Goban::addToScene() {
    const unsigned offset = (SCREEN_W - BOARD_SIZE)/2;

	Ad_GameNode_p koi = make_shared<Ad_GameNode>("../img/koi.png", 
		Ad_Rect(-35, SCREEN_H - 500, 384, 606));
	scene->get_root()->add_child(koi);

	Ad_GameNode_p koi2 = make_shared<Ad_GameNode>("../img/koi.png", 
		Ad_Rect(SCREEN_W + 35, 500, -384, -606));
	scene->get_root()->add_child(koi2);

	const unsigned BORDER_SIZE = 4;
	Ad_GameNode_p border = make_shared<Ad_GameNode>(Ad_Rect(offset - BORDER_SIZE, BOARD_PADDING/2 - BORDER_SIZE,
		BOARD_SIZE + 2 * BORDER_SIZE, BOARD_SIZE + BOARD_DEPTH + 2 * BORDER_SIZE));
	border->set_sprite_bitmap(ad_create_rect_bitmap(
		Ad_Vector2(BOARD_SIZE + 2 * BORDER_SIZE, BOARD_SIZE + 2 * BORDER_SIZE + BOARD_DEPTH), 
		BLACK));
	scene->get_root()->add_child(border);

    Ad_GameNode_p bottom = make_shared<Ad_GameNode>(Ad_Rect(offset, BOARD_SIZE + BOARD_PADDING/2, 
		BOARD_SIZE, BOARD_DEPTH));
    bottom->set_sprite_bitmap(ad_create_rect_bitmap(Ad_Vector2(BOARD_SIZE, BOARD_DEPTH), DARK_COLOR));
    scene->get_root()->add_child(bottom);

    node = make_shared<Ad_GameNode>(Ad_Rect(offset, BOARD_PADDING/2, BOARD_SIZE, BOARD_SIZE));
    node->set_sprite_bitmap(ad_create_rect_bitmap(Ad_Vector2(BOARD_SIZE, BOARD_SIZE), LIGHT_COLOR));
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
    grid->set_sprite_bitmap(ad_create_grid_bitmap(Ad_Vector2(GRID_SIZE, GRID_SIZE), 
	Ad_Vector2(size, size), 1, DARK_COLOR));
    node->add_child(grid);
}

void Goban::addStarsToBoard() {
	// smaller boards won't include the midway points on the corners
	// that larger boards will have, thus only add the points here
	if (size == 9 || size == 13 || (size <= 13 && size % 2 == 1)) {
		addStarPoint(0.5f, 0.5f);
	}

	// anything smaller than 8x8 will only include a center star point
	if (size < 8) { return; }

	size_t count = ((size > 13) && (size % 2 == 1)) ? 3 : 2;
	float * POSITIONS = (float *)malloc(sizeof(float) * count);

	// add count^2 points mirroring the board
	switch (size) {
		case 9:
			POSITIONS[0] = 2.0 / 8.0;
			POSITIONS[1] = 6.0 / 8.0;
			break;
		case 13:
			POSITIONS[0] = 3.0 / 12.0;
			POSITIONS[1] = 9.0 / 12.0;
			break;
		case 19:
			POSITIONS[0] = 3.0  / 18.0;
			POSITIONS[1] = 9.0  / 18.0;
			POSITIONS[2] = 15.0 / 18.0;
			break;
		default:
			if (size > 13 && (size % 2 == 1)) {
				POSITIONS[0] = 3.0 / (float)(size-1);
				POSITIONS[1] = 0.5f;
				POSITIONS[2] = (size-1-3) / (float)(size-1);
			} else if (size > 9) {
				POSITIONS[0] = 3.0 / (size-1);
				POSITIONS[1] = (size-1-3) / (float)(size-1);
			} else {
				POSITIONS[0] = 2.0 / (size-1);
				POSITIONS[1] = (size-1-2) / (float)(size-1);
			}

			break;
	}

    for (size_t i=0; i<count; i++) {
		for (size_t k=0; k<count; k++) {
			addStarPoint(POSITIONS[i], POSITIONS[k]);
	}}

	free(POSITIONS);
}

void Goban::addStarPoint(float x, float y) {
	Ad_GameNode_p star = make_shared<Ad_GameNode>(Ad_Rect(
		getGridSize() * x - (STAR_SIZE/2), 
		getGridSize() * y - (STAR_SIZE/2), 
		STAR_SIZE, STAR_SIZE
	));

	star->set_sprite_bitmap(ad_create_circle_bitmap(STAR_SIZE, DARK_COLOR));
	grid->add_child(star);
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
