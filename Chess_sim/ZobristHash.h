#include <cstdint>
#include <array>
#include "Pieces.h"

#pragma once

class  ZobristHash
{
public:
	static constexpr uint64_t Seed = 0x98f107;
	static constexpr uint64_t Multiplier = 0x71abc9;
	static constexpr uint64_t Summand = 0xff1b3f;

	ZobristHash()
	{
		calc_constants();
	}
	ZobristHash(Pieces pieces, bool black_move, bool white_long_castling, bool white_short_castling, bool black_long_castling, bool black_short_castling);
	uint64_t next_random(uint64_t previous) const
	{
		return Multiplier * previous + Summand;
	}
	const std::array<std::array<std::array<uint64_t, 6>, 2>, 64>& get_constants() const
	{
		return Constants;
	}


	uint64_t getBlackMove() const { return BlackMove; }
	uint64_t getWhiteLongCastling() const { return WhiteLongCastling; }
	uint64_t getWhiteShortCastling() const { return WhiteShortCastling; }
	uint64_t getBlackLongCastling() const { return BlackLongCastling; }
	uint64_t getBlackShortCastling() const { return BlackShortCastling; }

private:
	uint64_t _hash;
	void calc_constants()
	{
		uint64_t previous = Seed;

		for (uint8_t square = 0; square < 64; square = square + 1)
		{
			for (uint8_t side = 0; side < 2; side = side + 1)
			{
				for (uint8_t type = 0; type < 6; type = type + 1)
				{
					previous = ZobristHash::next_random(previous);
					Constants[square][side][type] = previous;
				}
			}
		}
		BlackMove = next_random(Constants[63][1][5]);
		WhiteLongCastling = next_random(BlackMove);
		WhiteShortCastling = next_random(WhiteLongCastling);
		BlackLongCastling = next_random(WhiteShortCastling);
		BlackShortCastling = next_random(BlackLongCastling);
	}
	uint64_t calculate_hash(const Pieces& pieces, bool black_move)
	{
		uint64_t h = 0;

		if (black_move) 
		{
			h ^= BlackMove;
		}

		for (uint8_t square = 0; square < 64; ++square) 
		{
			for (uint8_t side = 0; side < 2; ++side)
			{
				for (uint8_t type = 0; type < 6; ++type) 
				{
					if (BitBoard::get_bit(pieces._piece_bitboards[side][type], square)) 
					{
						h ^= Constants[square][side][type];
						break;
					}
				}
			}
		}
		return h;
	}
	uint64_t invert_move() const;
	uint64_t invert_white_long_castling() const;
	uint64_t invert_white_short_castling() const;
	uint64_t invert_black_long_castling() const;
	uint64_t invert_black_short_castling() const;
	uint64_t invert_piece(uint8_t square, uint8_t side, uint8_t type) const;

	std::array<std::array<std::array<uint64_t, 6>, 2>, 64> Constants;
	uint64_t BlackMove;
	uint64_t WhiteLongCastling;
	uint64_t WhiteShortCastling;
	uint64_t BlackLongCastling;
	uint64_t BlackShortCastling;
};