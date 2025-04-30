#pragma once
#include "Pieces.hpp"
#include "PsLegalMoveMaskGen.hpp"
class StaticEvaluator
{
public: 
	int32_t countMaterial(const Pieces pieces);
	int32_t countMobility(const Pieces pieces);
	int32_t countPawnDoubleStructure(const Pieces pieces);
	int32_t countPawnConnectedStructure(const Pieces pieces);
private:

	static constexpr int32_t DoublePawn = -25;
	static constexpr int32_t ConnectedPawn = 12;
	struct Material
	{
		static constexpr int32_t PAWN = 100;
		static constexpr int32_t KNIGHT = 305;
		static constexpr int32_t BISHOP = 333;
		static constexpr int32_t ROOK = 563;
		static constexpr int32_t QUEEN = 950;
	};
	
	struct Mobility
	{
		static constexpr int32_t PAWN = 9;
		static constexpr int32_t KNIGHT = 4;
		static constexpr int32_t BISHOP = 3;
		static constexpr int32_t ROOK = 3;
		static constexpr int32_t QUEEN = 3;
	};
};