#ifndef STATE_H
#define STATE_H

#include <stdint.h>
#include <memory>

using namespace std;

class State;

typedef shared_ptr<State> State_p;

/**
 * Compressing a char[SIZExSIZE] board state into a list of size uint64_t[SIZE].
 * Where each 32-bit unsigned integer represents one row of the board.
 * This can be used to compare different board states for equality as an
 * implementation of the official KO rule.
 */
class State {
public:
    State(unsigned Size);
    State(unsigned Size, const char * data);

    /**
     * Encodes a char[SIZExSIZE] by encoding each row into trinary.
     * You can then use this classes overloaded operators for comparing
     * different board states.
     * Note, the board MUST be in the clean state,
     * that is to say, 'data' should only contain the values 0, 1, or 2.
     */
    void encode(const char * data);

    bool operator==(const State &other);

    const uint64_t size;
    unique_ptr<uint64_t[]> state;
};

#endif
