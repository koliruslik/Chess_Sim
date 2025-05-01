#include <fstream>
#include <sstream>
#include <tuple>
#include <vector>
#include <algorithm>
#include "LegalMoveGen.hpp"


#pragma once

class OpeningBook 
{
public:

	OpeningBook();
	OpeningBook(const std::string& path);

	std::tuple<Move, int32_t> tryToFindMove(const Position& position);

private:
	std::vector<std::vector<Move>> moves;

};


