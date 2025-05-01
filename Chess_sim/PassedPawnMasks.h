#include <array>
#include "BitBoard.hpp"

#pragma once

namespace PassedPawnMasks
{
	static std::array<Bitboard, 64> calcWhitePassedPawnMasks()
	{
		std::array<Bitboard, 64> masks{};

		for (uint8_t x = 0; x < 8; x = x + 1)
		{
			for (uint8_t y = 0; y < 8; y = y + 1)
			{
				for (uint8_t y1 = y + 1; y1 < 8; y1 = y1 + 1)
				{
					if (x != 0) BOp::set1(masks[y * 8 + x], y1 * 8 + x - 1);
					if (x != 7) BOp::set1(masks[y * 8 + x], y1 * 8 + x + 1);
					BOp::set1(masks[y * 8 + x], y1 * 8 + x);
				}
			}
		}
		return masks;
	}

	static std::array<Bitboard, 64> calcBlackPassedPawnMasks()
	{
		std::array<Bitboard, 64> masks{};
		for (uint8_t x = 0; x < 8; x = x + 1)
		{
			for (uint8_t y = 0; y < 8; y = y + 1)
			{
				for (uint8_t y1 = y - 1; y1 > 0; y1 = y1 - 1)
				{
					if (x != 0) BOp::set1(masks[y * 8 + x], y1 * 8 + x - 1);
					if (x != 7) BOp::set1(masks[y * 8 + x], y1 * 8 + x + 1);
					BOp::set1(masks[y * 8 + x], y1 * 8 + x);
				}
			}
		}
		return masks;
	}

	static std::array<Bitboard, 64> White_PASSED_PAWN_MASKS = calcWhitePassedPawnMasks();
	static std::array<Bitboard, 64> Black_PASSED_PAWN_MASKS = calcBlackPassedPawnMasks();
}