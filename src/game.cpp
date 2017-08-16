#include <cmath>
#include "game.h"
#include "constants.h"

Game::Game(Ad_Scene_p Scene, unsigned BoardSize) 
	: size{BoardSize}, scene{Scene} {

    board = make_shared<Goban>(scene, BoardSize);
    root_move = nullptr;
    current_move = nullptr;
    current_player = SHELL;
}

void Game::update() {
    if (ad_is_button_released(0)) {
	int x = round((size-1) * (ad_get_mouse_x() - board->grid->get_absolute_pos().x) / board->getGridSize());
	int y = round((size-1) * (ad_get_mouse_y() - board->grid->get_absolute_pos().y) / board->getGridSize());
	add_stone(x, y);
    }
}

void Game::render() {
    board->renderLabels();
}

bool Game::add_stone(unsigned X, unsigned Y) {
    if (!is_legal(X, Y, current_player)) { return false; }

    if (!current_move) { // case: this is the root move of the game
	root_move = make_shared<Move>(X, Y, current_player, MoveType::Stone);
	current_move = root_move.get();
    } else { // case: moves already exist, add this as a child of the current move
	current_move = current_move->add_child(X, Y, current_player, MoveType::Stone).get();
    }

    // get positional information for the stone
    const unsigned GRID_OFF = (board->getBoardSize() - board->getGridSize())/2;
    const unsigned SPACING = board->getGridSize()/(size-1);
    const unsigned SIZE = SPACING * 0.95;
    const auto OFF = SPACING * 0.25f;

    Ad_Rect bounds(
	GRID_OFF + SPACING * X - SIZE/2,
	GRID_OFF + SPACING * Y - SIZE/2,
	SIZE, SIZE
    );

    // now that we have the move, we need to add it to our scene
    current_move->shadow = make_shared<Ad_GameNode>("../img/shadow.png", Ad_Rect(
	bounds.get_pos().x - OFF, bounds.get_pos().y - OFF * 0.3,
	bounds.get_size().x + 2 * OFF, bounds.get_size().y + 2 * OFF
    ), 1);

    const char * img = (current_player == SLATE) ? "../img/slate.png" : "../img/shell.png";
    current_move->stone = make_shared<Ad_GameNode>(img, bounds, 2);

    // add the game nodes to the scene to be rendered
    board->node->add_child(current_move->shadow);
    board->node->add_child(current_move->stone);

    swap_turn();
    return true;
}

bool Game::is_legal(unsigned X, unsigned Y, unsigned Player) {
    // TODO: 2. Suicide.
    // TODO: 3. KO.

    for (Move * tmp = current_move; tmp; tmp = tmp->parent) {
	if (X == tmp->x && Y == tmp->y) { return false; }
    }

    return (X < size) && (Y < size);
}

void Game::swap_turn() {
    current_player = (current_player == SLATE) ? SHELL : SLATE;
}
