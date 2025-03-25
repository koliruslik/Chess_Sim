#include "ZobristHash.h"

ZobristHash::ZobristHash()
{
	calc_constants();
	_hash = 0;
}

ZobristHash::ZobristHash(const Pieces& pieces, bool black_move, bool w_l_castling, bool w_s_castling, bool b_l_castling, bool b_s_castling)
{
	calc_constants();
	_hash = calculate_hash(pieces, black_move);

	if (w_l_castling) _hash ^= WhiteLongCastling;
	if (w_s_castling) _hash ^= WhiteShortCastling;
	if (b_l_castling) _hash ^= BlackLongCastling;
	if (b_s_castling) _hash ^= BlackShortCastling;
}

void ZobristHash::calc_constants()
{
	uint64_t previous = Seed;
	for (uint8_t square = 0; square < 64; ++square)
		for (uint8_t side = 0; side < 2; ++side)
			for (uint8_t type = 0; type < 6; ++type)
			{
				previous = next_random(previous);
				Constants[square][side][type] = previous;
			}
	BlackMove = next_random(Constants[63][1][5]);
	WhiteLongCastling = next_random(BlackMove);
	WhiteShortCastling = next_random(WhiteLongCastling);
	BlackLongCastling = next_random(WhiteShortCastling);
	BlackShortCastling = next_random(BlackLongCastling);
}

uint64_t ZobristHash::calculate_hash(const Pieces& pieces, bool black_move)
{
	uint64_t h = black_move ? BlackMove : 0;

	for (uint8_t square = 0; square < 64; ++square)
		for (uint8_t side = 0; side < 2; ++side)
			for (uint8_t type = 0; type < 6; ++type)
				if (BitBoard::get_bit(pieces._piece_bitboards[side][type], square))
				{
					h ^= Constants[square][side][type];
					break;
				}

	return h;
}

uint64_t ZobristHash::invert_move() const
{
	return _hash ^ BlackMove;
}
uint64_t ZobristHash::invert_white_long_castling() const
{
	return _hash ^ WhiteLongCastling;
}
uint64_t ZobristHash::invert_white_short_castling() const
{
	return _hash ^ WhiteShortCastling;
}
uint64_t ZobristHash::invert_black_long_castling() const
{
	return _hash ^ BlackLongCastling;
}
uint64_t ZobristHash::invert_black_short_castling() const
{
	return _hash ^ BlackShortCastling;
}
uint64_t ZobristHash::invert_piece(uint8_t square, uint8_t side, uint8_t type) const
{
	return _hash ^ Constants[square][side][type];
}