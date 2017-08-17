#include <cmath>
#include "state.h"

State::State(unsigned Size) : size{Size} { 
    state = unique_ptr<uint64_t[]>(new uint64_t[size * size]);
    for (unsigned i=0; i<(size*size); i++) { state[i] = 0; }
}

State::State(unsigned Size, const char * data) : size{Size} {
    state = unique_ptr<uint64_t[]>(new uint64_t[size * size]);
    for (unsigned i=0; i<(size*size); i++) { state[i] = 0; }
    encode(data);
}

void State::encode(const char * data) {
    for (unsigned i=0; i<size; i++) {
	state[i] = 0;

	// convert a row of data into a single trinary number
	for (unsigned k=0; k<size; k++) {
	    state[i] += data[i + k*size] * pow(3, k);
	}
    }
}

bool State::operator==(const State &other) {
    // both boards need to be of the same size
    if (other.size != size) { return false; }

    // compare each row (stored as a single number)
    for (unsigned i=0; i<size; i++) {
	if (other.state[i] != state[i]) {
	    return false;
	}
    }

    return true;
}
