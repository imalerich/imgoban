#include <stdio.h>
#include <stdlib.h>
#include <adelie.h>

#define PI 3.14159265f

unsigned SCREEN_W = 1400;
unsigned SCREEN_H = 900;
Ad_Scene_p scene;

bool update();
bool render();
Ad_GameNode_p createBoardNode(Ad_Scene_p scene);

int main(int argc, const char ** argv) {
    if (!ad_init(SCREEN_W, SCREEN_H)) {
	return 1;
    }

    scene = make_shared<Ad_Scene>(SCREEN_W, SCREEN_H);
    createBoardNode(scene);

    ad_set_frame_rate(30);
    return ad_run(update, render);
}

bool update() {
    // TODO
    return true;
}

bool render() {
    scene->render(al_map_rgb(1, 4, 18));
    return true;
}

// TODO - This will need to eventually be in their own file.

Ad_GameNode_p createBoardNode(Ad_Scene_p scene) {
    const unsigned BOARD_DEPTH = 40;
    const unsigned PADDING = 45;
    const unsigned BOARD_SIZE = SCREEN_H - BOARD_DEPTH - PADDING;
    const unsigned offset = (SCREEN_W - BOARD_SIZE)/2;
    ALLEGRO_COLOR DARK_COLOR = al_map_rgb(76, 60, 22);

    Ad_GameNode_p bottom = make_shared<Ad_GameNode>(Ad_Rect(offset, BOARD_SIZE + PADDING/2, BOARD_SIZE, BOARD_DEPTH));
    bottom->set_bitmap(ad_create_rect_bitmap(Ad_Vector2(BOARD_SIZE, BOARD_DEPTH), DARK_COLOR));
    scene->get_root()->add_child(bottom);

    Ad_GameNode_p board = make_shared<Ad_GameNode>(Ad_Rect(offset, PADDING/2, BOARD_SIZE, BOARD_SIZE));
    board->set_bitmap(ad_create_rect_bitmap(Ad_Vector2(BOARD_SIZE, BOARD_SIZE), al_map_rgb(223, 189, 109)));
    scene->get_root()->add_child(board);

    // We need to compute a valid grid size so that each line is visible and evenly spaced.
    const int GAP = ((BOARD_SIZE - 19)/18) - 2;

    const unsigned GRID_SIZE = 19 + 18 * GAP;
    const unsigned GRID_OFF = (BOARD_SIZE - GRID_SIZE)/2;
    Ad_GameNode_p grid = make_shared<Ad_GameNode>(Ad_Rect(GRID_OFF, GRID_OFF, GRID_SIZE, GRID_SIZE));
    grid->set_bitmap(ad_create_grid_bitmap(Ad_Vector2(BOARD_SIZE, BOARD_SIZE), Ad_Vector2(19, 19), 1, DARK_COLOR));
    board->add_child(grid);

    // Now create the star points.
    const unsigned STAR_SIZE = 7;
    const float POSITIONS[] = { 3/18.0f, 9/18.0f, 15/18.0f };
    const auto STAR_BMP = ad_create_circle_bitmap(32, DARK_COLOR);

    for (int i=0; i<3; i++) {
	for (int k=0; k<3; k++) {
	    Ad_GameNode_p star = make_shared<Ad_GameNode>(Ad_Rect(
		GRID_SIZE * POSITIONS[i] - (STAR_SIZE/2), 
		GRID_SIZE * POSITIONS[k] - (STAR_SIZE/2), 
		STAR_SIZE, STAR_SIZE
	    ));

	    star->set_bitmap(STAR_BMP);
	    grid->add_child(star);
	}
    }

    return board;
}
