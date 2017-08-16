#include <stdlib.h>

#include "constants.h"
#include "move.h"

Move::Move(unsigned X, unsigned Y, unsigned Player, MoveType Type) 
	: parent{nullptr}, num{0}, x{X}, y{Y}, player{Player}, type{Type} { 
    stone = nullptr;
    shadow = nullptr;
}

Move::Move(unsigned X, unsigned Y, unsigned Player, MoveType Type, Move * Parent)
	: parent{Parent}, num{Parent->num + 1}, x{X}, y{Y}, player{Player}, type{Type} { 
    stone = nullptr;
    shadow = nullptr;
}

Move_p Move::add_child(unsigned X, unsigned Y, unsigned Player, MoveType Type) {
    Move_p child = make_shared<Move>(X, Y, Player, Type, this);
    children.push_back(child);
    return child;
}

void Move::remove_from_parent() {
    if (!parent) { return; }

    for (auto i = parent->children.begin(); i != parent->children.end();) {
	if (i->get() == this) {
	    i = parent->children.erase(i);
	    parent = nullptr;
	    break;
	} else {
	    i++;
	}
    }
}
