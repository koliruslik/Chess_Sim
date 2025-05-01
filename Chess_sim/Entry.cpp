#include "Entry.hpp"

Entry::Entry(ZobristHash hash, int32_t depth, uint8_t bestMoveIndex)
{
	this->hash = hash;
	this->depth = depth;
	this->bestMoveIndex = bestMoveIndex;
}

bool operator <(Entry left, Entry right)
{
	return (left.hash < right.hash);
}