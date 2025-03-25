#include "RepetitionHistory.h"

RepetitionHistory::RepetitionHistory() = default;

void RepetitionHistory::add_position(ZobristHash hash)
{
	this->_hashes.push_back(hash);
}

void RepetitionHistory::clear()
{
	this->_hashes.clear();
}
uint8_t RepetitionHistory::get_repetition_number(ZobristHash hash)
{
	uint8_t count = 0;

	for (auto hash1 : this->_hashes)
	{
		if (hash == hash1)
		{
			count = count + 1;
		}
	}
	return count;
}


