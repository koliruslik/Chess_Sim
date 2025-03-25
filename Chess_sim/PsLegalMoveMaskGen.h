#include "BitBoard.h"
#include "Pieces.h"

#pragma once

class PsLegalMoveMaskGen
{
public:
	static BitBoard generate_knight_mask(Pieces pieces, uint8_t p, uint8_t side, bool only_captures);
	static BitBoard generate_king_mask(Pieces pieces, uint8_t p, uint8_t side, bool only_captures);
	static BitBoard generate_bishop_mask(Pieces pieces, uint8_t p, uint8_t side, bool only_captures);
	static BitBoard generate_rook_mask(Pieces pieces, uint8_t p, uint8_t side, bool only_captures);
	static BitBoard generate_queen_mask(Pieces pieces, uint8_t p, uint8_t side, bool only_captures);

	static BitBoard generate_pawn_default_mask(Pieces pieces, uint8_t side);
	static BitBoard generate_pawn_long_mask(Pieces pieces, uint8_t side);
	static BitBoard generate_pawn_left_captures_mask(Pieces pieces, uint8_t side, bool include_all_possible_captures);
	static BitBoard generate_pawn_right_captures_mask(Pieces pieces, uint8_t side, bool include_all_possible_captures);

	static BitBoard _calc_ray(Pieces pieces, uint8_t p, uint8_t side, bool only_captures, int8_t direction, bool bsr);

	static bool is_under_attack(Pieces pieces, uint8_t p, uint8_t side);
};

namespace KingMasks
{
	constexpr uint8_t abs_subtract(uint8_t left, uint8_t right);
	std::array<BitBoard, 64> calc_masks();
	extern std::array<BitBoard, 64> Masks;
}

namespace KnightMasks
{
	constexpr uint8_t abs_subtract(uint8_t left, uint8_t right);
	std::array<BitBoard, 64> calc_masks();
	extern std::array<BitBoard, 64> Masks;
}

namespace SliderMasks
{
	struct Direction
	{
		static constexpr int8_t North = 0;
		static constexpr int8_t South = 1;
		static constexpr int8_t West = 2;
		static constexpr int8_t East = 3;

		static constexpr int8_t NorthWest = 4;
		static constexpr int8_t NorthEast = 5;
		static constexpr int8_t SouthWest = 6;
		static constexpr int8_t SouthEast = 7;
	};

	BitBoard calc_mask(uint8_t p, int8_t direction);
	std::array<std::array<BitBoard, 8>, 64 > calc_masks();
	extern std::array<std::array<BitBoard, 8>, 64> Masks;
}