#include <array>
#include "Bitboard.hpp"

#pragma once

namespace PawnShieldMasks
{
	static consteval std::array<Bitboard, 64> calcWhitePawnShieldMasks()
	{
		std::array<Bitboard, 64> masks{};

		for (uint8_t x = 0; x < 8; x = x + 1)
		{
			for (uint8_t y = 0; y < 7; y = y + 1)
			{
				if (x != 0) BOp::set1(masks[y * 8 + x], (y + 1) * 8 + x - 1);
				if (x != 7) BOp::set1(masks[y * 8 + x], (y + 1) * 8 + x + 1);
				BOp::set1(masks[y * 8 + x], (y + 1) * 8 + x);

				if (y != 6)
				{
					if (x != 0) BOp::set1(masks[y * 8 + x], (y + 2) * 8 + x - 1);
					if (x != 7) BOp::set1(masks[y * 8 + x], (y + 2) * 8 + x + 1);
					BOp::set1(masks[y * 8 + x], (y + 2) * 8 + x);
				}
			}
		}
		return masks;
	}

	static consteval std::array<Bitboard, 64> calcBlackPawnShieldMasks()
	{
		std::array<Bitboard, 64> masks{};
		for (uint8_t x = 0; x < 8; x = x + 1)
		{
			for (uint8_t y = 1; y < 8; y = y + 1)
			{
				if (x != 0) BOp::set1(masks[y * 8 + x], (y - 1) * 8 + x - 1);
				if (x != 7) BOp::set1(masks[y * 8 + x], (y - 1) * 8 + x + 1);
				BOp::set1(masks[y * 8 + x], (y - 1) * 8 + x);
				if (y != 1)
				{
					if (x != 0) BOp::set1(masks[y * 8 + x], (y - 2) * 8 + x - 1);
					if (x != 7) BOp::set1(masks[y * 8 + x], (y - 2) * 8 + x + 1);
					BOp::set1(masks[y * 8 + x], (y - 2) * 8 + x);
				}
			}
		}
		return masks;
	}

	static constexpr std::array<Bitboard, 64> WHITE_PAWN_SHIELD_MASKS = calcWhitePawnShieldMasks();
	static constexpr std::array<Bitboard, 64> BLACK_PAWN_SHIELD_MASKS = calcBlackPawnShieldMasks();
}