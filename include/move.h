#ifndef MOVE_H
#define MOVE_H

#include <memory>
#include <vector>
#include <adelie2d/adelie.h>

using namespace std;

enum class MoveType { Stone, Pass };

class Move;
typedef shared_ptr<Move> Move_p;
typedef vector<Move_p> MoveArray;

class Move {
public:
    Move(unsigned X, unsigned Y, unsigned Player, MoveType Type);
    Move(unsigned X, unsigned Y, unsigned Player, MoveType Type, Move * Parent);

    /**
     * \fn void add_child(unsigned X, unsigned Y, unsigned Player, MoveType Type);
     * \brief Adds a following move to this move in the game tree.
     * This move will be added as a child to this node.
     */
    Move_p add_child(unsigned X, unsigned Y, unsigned Player, MoveType Type);

    /**
     * \fn void remove_from_parent( child);
     * \brief Removes this node from it's parent node (if it exists).
     */
    void remove_from_parent();
    
    // meta data
    Move * parent; /** Parent Move.*/
    const unsigned num; /** Move number = (parent.num + 1). */

    // move information
    const unsigned x; /** x-coordinate of this move */
    const unsigned y; /** y-coordinate of this move */
    const unsigned player; /** Did Black or White Play this Move? */
    const MoveType type; /** What type of move is this? */

    // scene information
    Ad_GameNode_p stone;
    Ad_GameNode_p shadow;

private:
    MoveArray children;
};

#endif
