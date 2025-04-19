
#include "MoveList.hpp"


MoveList::MoveList() {
    this->size = 0;
}
Move &MoveList::operator[](uint8_t index) {
    return this->moves[index];
}
Move MoveList::operator[](uint8_t index) const {
    return this->moves[index];
}

bool MoveList::operator==(const MoveList& other) const
{
    if (size != other.size) return false;
    for (uint8_t i = 0; i < size; ++i)
    {
        if (!(moves[i] == other.moves[i])) return false;
    }
    return true;
}

void MoveList::push(Move move) {
    this->moves[this->size] = move;
    this->size = this->size + 1;
}
uint8_t MoveList::getSize() const {
    return this->size;
}

bool MoveList::hasMoves() const {
	return this->size > 0;
}

