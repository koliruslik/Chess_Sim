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

	ZobristHash();
	ZobristHash(const Pieces& pieces, bool black_move, bool w_l_castling, bool w_s_castling, bool b_l_castling, bool b_s_castling);

	bool operator==(const ZobristHash& other) const { return this->_hash == other._hash; }

	uint64_t getHash() const { return _hash; }
	uint64_t invert_move() const;
	uint64_t invert_white_long_castling() const;
	uint64_t invert_white_short_castling() const;
	uint64_t invert_black_long_castling() const;
	uint64_t invert_black_short_castling() const;
	uint64_t invert_piece(uint8_t square, uint8_t side, uint8_t type) const;
private:
	uint64_t _hash;
	std::array<std::array<std::array<uint64_t, 6>, 2>, 64> Constants;
	uint64_t BlackMove, WhiteLongCastling, WhiteShortCastling, BlackLongCastling, BlackShortCastling;

	uint64_t next_random(uint64_t previous) const { return Multiplier * previous + Summand; }
	void calc_constants();
	uint64_t calculate_hash(const Pieces& pieces, bool black_move);

	
};