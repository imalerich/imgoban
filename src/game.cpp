#include <cmath>
#include <memory>

#include "game.h"

Game::Game(Ad_Scene_p Scene, unsigned BoardSize, float Komi) 
	: size{BoardSize}, komi{Komi}, scene{Scene} {

    board = make_shared<Goban>(scene, BoardSize);
    root_move = nullptr;
    current_move = nullptr;
	move_indicator = nullptr;
    current_player = SLATE;
	game_state = GameState::Lobby;

	players[0] = nullptr;
	players[1] = nullptr;

	preview_indicator = make_shared<Ad_GameNode>(SLATE_PREV_IMG, Ad_Rect(
		0, 0, board->getStoneSize(), board->getStoneSize()
	), 5);
	board->node->add_child(preview_indicator);
	preview_indicator->hidden = true;

    // Initialize an empty board.
    state = unique_ptr<char[]>(new char[size * size]);
    for (unsigned i=0; i<(size*size); i++) { state[i] = NOPLAYER; }
}

void Game::attach_player(Player_p player, unsigned color) {
	const unsigned c = (color+1) % 2;
	players[c] = player;
}

void Game::update() {
	// if we don't have two players attached yet, don't do anything
	if (!players[0] && !players[1]) { return; }
	// once we have two players assigned, leave the lobby and start playing
	if (game_state == GameState::Lobby) { game_state = GameState::Playing; }

	if (game_state == GameState::Playing) {
		const unsigned c = (current_player+1) % 2;
		const unsigned n = (current_player+2) % 2;

		// check if we should render the preview indicator
		Move_p p = players[c]->consider_move(current_player);
		if (p && get_stone(p->x, p->y) == NOPLAYER) {
			const char * img = (current_player == SLATE) ? 
				SLATE_PREV_IMG : SHELL_PREV_IMG;
			preview_indicator->load_sprite(img);
			preview_indicator->hidden = false;
			preview_indicator->set_pos(board->grid_coords_for_pos(p->x, p->y));
		} else { preview_indicator->hidden = true; }

		Move_p move = players[c]->gen_move(current_player);
		if (move && !move->is_pass()) {
			if (add_stone(move->x, move->y)) {
				players[n]->move_played(current_move);
			}
		} else if (move && move->is_pass()) {
			pass();
			players[n]->move_played(current_move);
		}
	} else if (game_state == GameState::Scoring) {
		// TODO - clean up
		if (ad_is_button_released(0)) {
			// get the coordinates of a stone in the group we wish to kill
			int x = round((size-1) * (ad_get_mouse_x() - board->grid->get_absolute_pos().x) 
					/ board->getGridSize());
			int y = round((size-1) * (ad_get_mouse_y() - board->grid->get_absolute_pos().y) 
					/ board->getGridSize());
			toggle_dead(x, y);
		}
	} else if (game_state == GameState::Over) {
		static bool has_output_score = false;

		// TODO: This sucks, fix it, I'm on a plane and don't have time.
		if (!has_output_score) {
			// finalize dead stones
			for (unsigned i=0; i<(size*size); i++) {
				state[i] = (state[i] == SLATE_DEAD || state[i] == SHELL_DEAD) ?
					NOPLAYER : state[i];
			}

			// now score the board, this will output territory
			// eventuall we'll need to do this another way
			score_board();
			
			// only do this once
			has_output_score = true;
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
		if (current_move &&
				current_move->is_pass() && 
				!current_move->parent.expired() &&
				current_move->parent.lock()->is_pass()) {
			game_state = GameState::Scoring;
			preview_indicator->hidden = true;
		}

	} else if (game_state == GameState::Scoring) {
		if (ad_is_button_released(1)) {
			game_state = GameState::Over;
		}
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
		move_indicator->set_sprite_bitmap(
			ad_create_rect_bitmap(Ad_Vector2(SIZE, SIZE), INDICATOR_COLOR));
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
	if (!root_move) {
		root_move = make_shared<Move>(current_player, MoveType::Pass);
		current_move = root_move;
	} else {
		current_move = current_move->add_child(make_shared<Move>(current_player, MoveType::Pass));
	}

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

void Game::toggle_dead(unsigned X, unsigned Y) {
	// make sure we have a valid stone that can be marked as dead
	unsigned remove_for = get_stone(X, Y);
	if (remove_for != SLATE 
		&& remove_for != SHELL
		&& remove_for != SLATE_DEAD
		&& remove_for != SHELL_DEAD) { return; }

	// if the stone is already dead, we need to set it to alive
	bool dead = remove_for == SLATE_DEAD || remove_for == SHELL_DEAD;

	// get the reference to the move
	Move_p m = current_move->find_stone(X, Y);
	if (!m) {
		cerr << "Could not find valid stone in move hierarchy!" << endl;
		exit(1);
		return;
	}

	// make the stone transparent and update the current state
	// we should also hide the shadow, would look weird with a transparent stone
	if (!dead) {
		m->shadow->hidden = true;
		m->stone->load_sprite((remove_for == SLATE) ? SLATE_PREV_IMG : SHELL_PREV_IMG);
		set_stone(X, Y, (remove_for == SLATE) ? SLATE_DEAD : SHELL_DEAD);
	} else {
		m->shadow->hidden = false;
		m->stone->load_sprite((remove_for == SLATE_DEAD) ? SLATE_IMG : SHELL_IMG);
		set_stone(X, Y, (remove_for == SLATE_DEAD) ? SLATE : SHELL);
	}

    // recurse to adjacent stones (of the current players)
    for (int x=-1; x<=1; x++) {
		for (int y=-1; y<=1; y++) {
			// we don't need to check these stones
			if (abs(x) == abs(y)) { continue; }
			if (get_stone(X+x, Y+y) == remove_for) { toggle_dead(X+x, Y+y); }
    }}
}

void Game::score_board() {
	// game must be complete in order to score
	if (game_state != GameState::Over) { return; }

	// TODO - Do this somewhere else where we can 
	// TODO - include komi and captures
	unsigned black_territory = 0;
	unsigned white_territory =  0;

	// iterate over each position
	for (unsigned x=0; x<size; x++) {
		for (unsigned y=0; y<size; y++) {
			// only evaluate territory for open spaces
			if (get_stone(x,y) != NOPLAYER) { continue; }

			bool reach_w = can_reach_player(x, y, SHELL);
			bool reach_b = can_reach_player(x, y, SLATE);

			if (reach_w && !reach_b) {
				set_stone(x, y, SHELL_TERRITORY);
				++white_territory;
			} else if (reach_b && !reach_w) {
				set_stone(x, y, SLATE_TERRITORY);
				++black_territory;
			} else {
				set_stone(x, y, NO_TERRITORY);
	}}}

	cout << "Black has " << black_territory << " Points of Territory." << endl;
	cout << "White has " << white_territory << " Points of Territory." << endl;
    cout << (string)(*this) << endl << endl;
}

bool Game::can_reach_player(unsigned X, unsigned Y, unsigned Player, bool clean) {
	// game must be complete in order to score
	if (game_state != GameState::Over) { return false; }
	if (get_stone(X,Y) == Player) { return true; }
	set_stone(X, Y, NOPLAYER_CHECKED);

	// recurse to adjacent stones
	bool can_reach = false; // can an adjacent position reach?
	for (int x=-1; x<=1; x++) {
		for (int y=-1; y<=1; y++) {
			if (abs(x) == abs(y)) { continue; }
			if (get_stone(X+x, Y+y) == Player) { 
				if (clean) { set_stone(X, Y, NOPLAYER);
				return true; 
			}

			if (get_stone(X+x, Y+y) == NOPLAYER) {
				can_reach = can_reach || can_reach_player(X+x, Y+y, Player, false);
	}}}}

	// reset marked empty spaces
    if (clean) {
		for (unsigned i=0; i<(size * size); i++) {
			state[i] = (state[i] == NOPLAYER_CHECKED) ? NOPLAYER : state[i];
	}}

	return can_reach;
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
