#include "ZobristHash.hpp"

#pragma once

struct Entry
{
	Entry();
	Entry(ZobristHash hash, int32_t depth, uint8_t bestMoveIndex);

	friend bool operator <(Entry left, Entry right);

	ZobristHash hash;
	int32_t depth;
	uint8_t bestMoveIndex;
};
