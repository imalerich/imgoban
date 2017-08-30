#include <cmath>
#include <memory>

#include "game.h"

Game::Game(Ad_Scene_p Scene, unsigned BoardSize) 
	: size{BoardSize}, scene{Scene} {

    board = make_shared<Goban>(scene, BoardSize);
	engine_controller = make_shared<gtp::Controller>(BoardSize, 6.5f);
	engine_controller->attach_engine("");
    root_move = nullptr;
    current_move = nullptr;
	move_indicator = nullptr;
    current_player = SLATE;
	game_state = GameState::Playing;

    // Initialize an empty board.
    state = unique_ptr<char[]>(new char[size * size]);
    for (unsigned i=0; i<(size*size); i++) { state[i] = NOPLAYER; }
}

void Game::update() {
	if (game_state == GameState::Playing) {
		if (current_player == SLATE) {
			// ask the engine to make a move, then add it to the board
			gtp::Vertex v = engine_controller->gen_move(gtp::PLAYER_BLACK);
			if (!v.is_pass()) {
				add_stone(v.get_x(), v.get_y());
			} else {
				pass();
			}

			// if (ad_is_button_released(0)) {
			// 	int x = round((size-1) * (ad_get_mouse_x() - board->grid->get_absolute_pos().x) / board->getGridSize());
			// 	int y = round((size-1) * (ad_get_mouse_y() - board->grid->get_absolute_pos().y) / board->getGridSize());
			// 	add_stone(x, y);

			// } else if (ad_is_button_released(3)) {
			// 	pass();
			// }
		} else {
			// ask the engine to make a move, then add it to the board
			gtp::Vertex v = engine_controller->gen_move(gtp::PLAYER_WHITE);
			if (!v.is_pass()) {
				add_stone(v.get_x(), v.get_y());
			} else {
				pass();
			}
		}
	}

	create_update_move_indicator();
	check_and_update_state();
}

void Game::check_and_update_state() {
	if (game_state == GameState::Playing) {
		// if we have two passes in a row
		// then both players are done playing
		// proceed into scoring the game
		if (current_move->is_pass() && !current_move->parent.expired() &&
				current_move->parent.lock()->is_pass()) {
			game_state = GameState::Scoring;
		}

	} else if (game_state == GameState::Scoring) {
		// TODO
	}
}

void Game::render() {
    board->renderLabels();
}

bool Game::add_stone(unsigned X, unsigned Y) {
    if (!is_legal(X, Y, current_player)) { return false; }

    if (!root_move) { // case: this is the root move of the game
		root_move = make_shared<Move>(X, Y, current_player, MoveType::Stone);
		current_move = root_move;
    } else { // case: moves already exist, add this as a child of the current move
		current_move = current_move->add_child(X, Y, current_player, MoveType::Stone);
    }

    // update the internal board state & scene to include this move
    board->add_graphics_for_stone(current_move, current_player);
    set_stone(X, Y, current_player);

    // encode the current board state, and add it to the history
    history.push_back(make_shared<State>(size, state.get()));

    // we are looking for captured opposing players stones 
    const unsigned opposing = (current_player == SLATE) ? SHELL : SLATE;

    // search neighboring stones, if any have 0 liberties, 
    // remove the group from the board
    for (int x=-1; x<=1; x++) {
		for (int y=-1; y<=1; y++) {
			// we don't need to check these stones
			if (abs(x) == abs(y)) { continue; }

			// check the number of liberties for neighboring opposing stones
			if (get_stone(X+x, Y+y) == opposing && num_liberties(X+x, Y+y) == 0) {
			remove_group(X+x, Y+y, current_move);
    }}}

    cout << (string)(*this) << endl << endl;

	// let the controller know we made a move
	// TODO - refactor this implementation
	if (current_player == SLATE) {
		engine_controller->play_move(X, Y, gtp::PLAYER_BLACK);
	}
    
    swap_turn();
    return true;
}

void Game::create_update_move_indicator() {
	// no current move, no move indicator, nothing to do
	if (!current_move && !move_indicator) { return; }

	// current move is a pass, remove the indicator if necessary and return
	if (current_move && current_move->is_pass()) {
		if (move_indicator) {
			move_indicator->remove_from_parent();
			move_indicator = nullptr;
		}

		return;
	}

	// no current move, but indicator exists, remove it
	if (!current_move && move_indicator) {
		move_indicator->remove_from_parent();
		move_indicator = nullptr;
	}
	
    // get positional information for the stone
    const unsigned SPACING = board->getGridSize()/(size-1);
    const unsigned SIZE = SPACING * 0.225;

	// move indicator currently does not exist
	// don't worry about position yet, that will be set below
	if (current_move && !move_indicator) {
		move_indicator = make_shared<Ad_GameNode>(Ad_Rect(
			0, 0, SIZE, SIZE), 5);
		move_indicator->set_sprite_bitmap(ad_create_rect_bitmap(Ad_Vector2(SIZE, SIZE), INDICATOR_COLOR));
		board->node->add_child(move_indicator);
	}

	// now set the position based on the current move
	move_indicator->set_pos(Ad_Vector2(
		current_move->stone->get_pos().x + current_move->stone->get_size().x/2 - SIZE/2,
		current_move->stone->get_pos().y + current_move->stone->get_size().y/2 - SIZE/2
	));
}

void Game::pass() {
    cout << (current_player == SLATE ? "black" : "white") << " passes" << endl << endl;
    current_move = current_move->add_child(make_shared<Move>(current_player, MoveType::Pass));
    swap_turn();
}

bool Game::undo_stone() {
    if (!current_move) { return false; }
    current_move->remove_from_scene();

    // update the internal board state to remove this move
    if (current_move->type == MoveType::Stone) {
		set_stone(current_move->x, current_move->y, NOPLAYER);
		history.pop_back();
    }

    // remove this move from the tree & set parent as the new current
    Move_w parent = current_move->parent;
    current_move->remove_from_parent();

    // loop through each removed stone by this play, and add them back to the board
    for (Move_p killed = current_move->pop_captured_stone(); 
			killed; killed = current_move->pop_captured_stone()) {
		// add the killed stone back to the board
		board->add_graphics_for_stone(killed, current_player);
		set_stone(killed->x, killed->y, killed->player);
    }

    // swap back to the current turn
    (parent.expired()) ? current_move = nullptr : current_move = parent.lock();
    if (current_move == nullptr) { root_move = nullptr; }

    cout << (string)(*this) << endl << endl;

    swap_turn();
    return true;
}

bool Game::is_legal(unsigned X, unsigned Y, unsigned Player) {
    if (get_stone(X, Y) != NOPLAYER) { return false; }
    if (num_liberties(X, Y, Player) == 0 && !does_stone_capture(X, Y, Player)) { return false; }
    if (!is_move_unique(X, Y, Player)) { return false; };
    return (X < size) && (Y < size);
}

unsigned Game::num_liberties(unsigned X, unsigned Y, unsigned Player, bool clean) {
    // base case - already checked or invalid stone, do nothing
    if (get_stone(X, Y) != NOPLAYER && get_stone(X, Y) != SLATE && get_stone(X, Y) != SHELL) { return 0; }
    // we want to run an invalid hypothetical, return 0 in error
    if (Player != NOPLAYER && get_stone(X, Y) != NOPLAYER) { return 0; }

    // if there is no stone, use the hypothetical, else use the current stone
    const unsigned original = get_stone(X, Y);
    const unsigned current = (get_stone(X, Y) == NOPLAYER) ? Player : get_stone(X, Y);
    set_stone(X, Y, (current == SLATE) ? SLATE_CHECKED : SHELL_CHECKED); // set the board state to checked

    for (int x=-1; x<=1; x++) {
		for (int y=-1; y<=1; y++) {
			// we don't need to check these stones
			if (abs(x) == abs(y)) { continue; }
			// found an empty space, set this as a liberty
			if (get_stone(X+x, Y+y) == NOPLAYER) { set_stone(X+x, Y+y, LIBERTY); }
			// found a friendly stone, recurse to that friendly stone, but do not clean the board
			// this should NEVER be a hypothetical test, and will not need to clean changes
			if (get_stone(X+x, Y+y) == current) { num_liberties(X+x, Y+y, NOPLAYER, false); }
    }}

    // count and return the liberties
    unsigned liberties = 0;
    if (clean) { // reset the temporary board states to their proper values
		for (unsigned i=0; i<(size * size); i++) {
			liberties += (state[i] == LIBERTY);
			state[i] = (state[i] == LIBERTY) ? NOPLAYER : state[i];
			state[i] = (state[i] == SLATE_CHECKED) ? SLATE : state[i];
			state[i] = (state[i] == SHELL_CHECKED) ? SHELL : state[i];
		}

		set_stone(X, Y, original);
    }

    return liberties; // only the root call of recursion needs to return anything
}

bool Game::does_stone_capture(unsigned X, unsigned Y, unsigned Player) {
    // we need an empty location to perform this test
    if (get_stone(X, Y) != NOPLAYER) { return false; }
    set_stone(X, Y, Player);

    // we are looking for the opposing players stones
    const unsigned opposing = current_player == SLATE ? SHELL : SLATE;

    // check adjacent stones
    for (int x=-1; x<=1; x++) {
		for (int y=-1; y<=1; y++) {
			// we don't need to check these stones
			if (abs(x) == abs(y)) { continue; }

			// check the number of libertios for neighboring opposing stones
			if (get_stone(X+x, Y+y) == opposing && num_liberties(X+x, Y+y) == 0) {
			set_stone(X, Y, NOPLAYER);
			return true;
    }}}

    // test is done, reset the position to noplayer
    set_stone(X, Y, NOPLAYER);
    return false;
}

void Game::remove_group(unsigned X, unsigned Y, Move_p killer) {
    // Who do we need to remove stones for? Is there even a stone at this position?
    unsigned remove_for = get_stone(X, Y);
    if (remove_for != SLATE && remove_for != SHELL) { return; }

    // first we're going to need the actual move reference
    Move_p m = current_move->find_stone(X, Y);
    if (!m) { 
		cerr << "Could not find valid stone in move hierarchy!" << endl;
		exit(1);
		return; 
    }

    // remove this stones graphics (keep it in the hierarchy)
    // and remove it from the current board state
    m->remove_from_scene();
    set_stone(X, Y, NOPLAYER);
    killer->push_captured_stone(m);

    // recurse to adjacent stones (of the current players)
    for (int x=-1; x<=1; x++) {
		for (int y=-1; y<=1; y++) {
			// we don't need to check these stones
			if (abs(x) == abs(y)) { continue; }
			if (get_stone(X+x, Y+y) == remove_for) { remove_group(X+x, Y+y, killer); }
    }}
}

bool Game::is_move_unique(unsigned X, unsigned Y, unsigned Player) {
    if (get_stone(X, Y) != NOPLAYER) { return false; }
    set_stone(X, Y, Player); // temporarily add the stone to the board
    State check(size, state.get());

    for (auto const state : history) {
		if (*state == check) {
			set_stone(X, Y, NOPLAYER);
			return false;
    }}

    // reset the board state
    set_stone(X, Y, NOPLAYER);
    return true;
}

void Game::swap_turn() {
    current_player = (current_player == SLATE) ? SHELL : SLATE;
}
