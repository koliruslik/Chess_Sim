#pragma once
#include "Pieces.hpp"
#include "PsLegalMoveMaskGen.hpp"
#include "PassedPawnMasks.h"
#include "PawnShieldMasks.h"
class StaticEvaluator
{
public: 
	static int32_t countMaterial(const Pieces pieces);
	static int32_t countMobility(const Pieces pieces);
	static int32_t countPawnDoubleStructure(const Pieces pieces);
	static int32_t countPawnConnectedStructure(const Pieces pieces);
	static int32_t countPassedPawnStructure(const Pieces pieces);
	static int32_t countKingSafety(bool wlCastling, bool wsCastling, bool blCastling, bool bsCastling, bool whiteCastlingHappened, bool blackCastlingHappened);
	static int32_t countePawnShieldKingSafety(const Pieces pieces, bool whiteCastlingHappened, bool blackCastlingHappened);
	static int32_t twoBishops(const Pieces pieces);
	static int32_t endgame(const Pieces pieces, bool whiteLeading);
	static int32_t evaluate(
		const Pieces& pieces,
		bool wlCastling, bool wsCastling,
		bool blCastling, bool bsCastling,
		bool whiteCastlingHappened, bool blackCastlingHappened);
	
	struct Material
	{
		static constexpr int32_t PAWN = 100;
		static constexpr int32_t KNIGHT = 305;
		static constexpr int32_t BISHOP = 333;
		static constexpr int32_t ROOK = 563;
		static constexpr int32_t QUEEN = 950;
		static constexpr int32_t TWOBISHOPS = 50;
	};
	
	struct Mobility
	{
		static constexpr int32_t PAWN = 9;
		static constexpr int32_t KNIGHT = 4;
		static constexpr int32_t BISHOP = 3;
		static constexpr int32_t ROOK = 3;
		static constexpr int32_t QUEEN = 3;
	};

	struct  PawnStructure
	{
		static constexpr int32_t DoublePawn = -25;
		static constexpr int32_t ConnectedPawn = 12;
		static constexpr std::array<int32_t, 8> DefaultPawnPromotion = { 0, 0, 0, 0, 10, 20, 30, 0 };
		static constexpr std::array<int32_t, 8> PassedPawnPromotion = { 0, 50, 50, 50, 70, 90, 110, 0 };
	};
	
	struct KingSafety
	{
		static constexpr int32_t CrashedCastling = -50;
		static constexpr int32_t PawnShield = 33;
	};

	struct Endgame
	{
		static constexpr int32_t AttackerKingProximityToDefenderKing = 10;
		static constexpr int32_t DistanceBetweenDefenderKingAndMiddle = 10;
		static constexpr uint8_t MaximumPiecesForEndgame = 8;
	};
	
};