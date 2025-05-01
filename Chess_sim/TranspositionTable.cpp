#include "TranspositionTable.h"

TranspositionTable::TranspositionTable() = default;

void TranspositionTable::addEntry(Entry entry) 
{
	auto hashCopy = this->set.find(entry);
	if (hashCopy == this->set.end() or hashCopy->depth < entry.depth) this->set.insert(entry);
}

uint8_t TranspositionTable::tryToFindBestMoveIndex(ZobristHash hash)
{
	auto entry = this->set.find({ hash, 0, 0 });

	if (entry == this->set.end()) return 255;
	return entry->bestMoveIndex;
}