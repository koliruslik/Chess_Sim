#include <chrono>
#include <future>
#include <iomanip>
#include <thread>
#include "LegalMoveGen.hpp"
#include "MoveSorter.hpp"
#include "TranspositionTable.h"
#include "OpeningBook.h"


#pragma once


class AI
{
public:
	AI();
	AI(const std::string& openingBookPath);

	Move proccessBestMove(const Position& position, uint8_t side, int32_t min_ms, int32_t max_ms, bool debugMode);
private:
	OpeningBook openingBook;

	static std::tuple<int32_t, Move> bestMove(const Position& position, uint8_t side, int32_t depth, TranspositionTable& tt);

	static std::tuple<int32_t, Move> alphaBetaMin(Position position, int32_t alpha, int32_t beta, int32_t depth_left, int32_t depth_current, TranspositionTable& tt);
	static std::tuple<int32_t, Move> alphaBetaMax(Position position, int32_t alpha, int32_t beta, int32_t depth_left, int32_t depth_current, TranspositionTable& tt);
	static int32_t alphaBetaMinOnlyCaptures(const Position& position, int32_t alpha, int32_t beta, int32_t depth_current);
	static int32_t alphaBetaMaxOnlyCaptures(const Position& position, int32_t alpha, int32_t beta, int32_t depth_current);
	struct Infinity
	{
		static constexpr int32_t Negative = -1e+9;
		static constexpr int32_t Positive = 1e+9;
	};
};