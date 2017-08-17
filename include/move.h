#ifndef MOVE_H
#define MOVE_H

#include <memory>
#include <vector>
#include <adelie2d/adelie.h>

using namespace std;

enum class MoveType { Stone, Pass };

class Move;
typedef shared_ptr<Move> Move_p;
typedef weak_ptr<Move> Move_w;
typedef vector<Move_p> MoveArray;

class Move : public std::enable_shared_from_this<Move> {
public:
    Move(unsigned Player, MoveType Type);
    Move(unsigned X, unsigned Y, unsigned Player, MoveType Type);
    Move(unsigned X, unsigned Y, unsigned Player, MoveType Type, Move_w Parent);

    /**
     * \fn void add_child(unsigned X, unsigned Y, unsigned Player, MoveType Type);
     * \brief Adds a following move to this move in the game tree.
     * This move will be added as a child to this node.
     */
    Move_p add_child(unsigned X, unsigned Y, unsigned Player, MoveType Type);

    /**
     * \fn void add_child(Move_p child);
     * \brief Adds the node pointer as a child move.
     */
    Move_p add_child(Move_p child);

    /**
     * \fn Move_w find_stone(unsigned X, unsigned Y);
     * \brief Recursively search through all parents until one is found at
     * the given location, that stone is then returned.
     * If multiple stones have been played on the given location,
     * the most recent stone will be returned.
     * Only stones currently on the board will be returned.
     * If no stone is found, nullptr will be returned instead.
     */
    Move_p find_stone(unsigned X, unsigned Y);

    /**
     * \fn void remove_from_parent( child);
     * \brief Removes this node from it's parent node (if it exists).
     */
    void remove_from_parent();

    /**
     * \fn void remove_from_scene();
     * \brief Removes the stone and shadow from the
     * scene (if they exist).
     */
    void remove_from_scene();

    /**
     * \fn void push_captured_stone(Move_w stone)
     * \brief Add a reference to a stone captured when this stone was played.
     * These can be removed when this move is undone.
     */
    void push_captured_stone(Move_p stone);

    /**
     * \fn Move_w pop_captured_stone()
     * \brief Remove a captured stone from the captured list,
     * and return the stored reference.
     * If no more stones remain, nullptr will be returned.
     */
    Move_p pop_captured_stone();

    /**
     * \fn inline bool is_on_board() { return stone != nullptr; }
     * \brief A stone is on the board iff it has a graphical representation.
     */
    inline bool is_on_board() { return stone != nullptr; }

    /**
     * \fn inline bool is_captured() { return !is_on_board(); }
     * \brief A stone is considered captured iff it is not on the board.
     */
    inline bool is_captured() { return !is_on_board(); }
    
    // meta data
    Move_w parent; /** Parent Move.*/
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
    MoveArray captured; 
};

#endif
