#include <stdlib.h>

#include "constants.h"
#include "move.h"

Move::Move(unsigned Player, MoveType Type) : num{0}, x{0}, y{0}, player{Player}, type{Type} {
    stone = nullptr;
    shadow = nullptr;
}

Move::Move(unsigned X, unsigned Y, unsigned Player, MoveType Type) 
	: num{0}, x{X}, y{Y}, player{Player}, type{Type} { 
    stone = nullptr;
    shadow = nullptr;
}

Move::Move(unsigned X, unsigned Y, unsigned Player, MoveType Type, Move_w Parent)
	: parent{Parent}, num{Parent.lock()->num + 1}, x{X}, y{Y}, player{Player}, type{Type} { 
    stone = nullptr;
    shadow = nullptr;
}

Move_p Move::find_stone(unsigned X, unsigned Y) {
    if (X == x && Y == y && is_on_board()) { return shared_from_this(); }
    if (parent.expired()) { return nullptr; }
    return parent.lock()->find_stone(X, Y);
}

Move_p Move::add_child(unsigned X, unsigned Y, unsigned Player, MoveType Type) {
    Move_p child = make_shared<Move>(X, Y, Player, Type, shared_from_this());
    children.push_back(child);
    return child;
}

Move_p Move::add_child(Move_p child) {
    child->parent = shared_from_this();
    children.push_back(child);
    return child;
}

void Move::remove_from_parent() {
    if (parent.expired()) { return; }

    for (auto i = parent.lock()->children.begin(); i != parent.lock()->children.end();) {
	if (i->get() == this) {
	    i = parent.lock()->children.erase(i);
	    parent.reset();
	    break;
	} else {
	    i++;
	}
    }
}

void Move::remove_from_scene() {
    if (shadow) { shadow->remove_from_parent(); shadow = nullptr; }
    if (stone) { stone->remove_from_parent(); stone = nullptr; }
}

void Move::push_captured_stone(Move_p stone) {
    captured.push_back(stone);
}

Move_p Move::pop_captured_stone() {
    if (captured.size() == 0) { return nullptr; }

    Move_p back = captured.back();
    captured.pop_back();
    return back;
}
