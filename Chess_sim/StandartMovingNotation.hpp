#pragma once

#include <string>
#include <cstdint>
#include <cctype>

#include "Move.hpp"
#include "Pieces.hpp"
#include "Position.hpp"
#include "PsLegalMoveMaskGen.hpp"

class StandartMovingNotation
{
public:
	explicit StandartMovingNotation(const std::string& notatedMove, Position position);

	Move StandartNotationMove() const;
    uint8_t SquareToIndex(char file, char rank);
    uint8_t PieceCharToType(char piece);
    static void showPossibleMoves(Pieces pieces, uint8_t square, uint8_t side);
private:
    uint8_t from;
    uint8_t to;
    uint8_t attackerType;
    uint8_t attackerSide;
    uint8_t defenderType;
    uint8_t defenderSide;
    uint8_t flag;
};