#include "StaticEvaluator.h"
#include "Move.hpp"
#include "MoveList.hpp"
#pragma once

class MoveSorter
{
public:
	static int32_t evaluateMove(Pieces pieces, Move move);
	static MoveList sort(Pieces pieces, MoveList moves);
};
