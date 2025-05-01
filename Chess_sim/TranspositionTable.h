#include <set>
#include "Entry.hpp"

#pragma once

class TranspositionTable
{
public:
	TranspositionTable();

	void addEntry(Entry entry);
	uint8_t tryToFindBestMoveIndex(ZobristHash hash);
	
private:
	std::set<Entry> set;
};

