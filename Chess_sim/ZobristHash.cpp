#include "ZobristHash.h"

ZobristHash::ZobristHash() = default;
ZobristHash::ZobristHash(Pieces pieces, bool black_move, bool white_long_castling, bool white_short_castling, bool black_long_castling, bool black_short_castling)
{
	this->_hash = 0;
	if (black_move) this->invert_move();
	if (white_long_castling) this->invert_white_long_castling();
	if (white_short_castling) this->invert_white_short_castling();
	if (black_long_castling) this->invert_black_long_castling();
	if (black_short_castling) this->invert_black_short_castling();

	uint8_t side;
	for (uint8_t square = 0; square < 64; square = square + 1)
	{
		if (BitBoard::get_bit(pieces._side_bitboards[Pieces::White], square)) side = Pieces::White;
		else if (BitBoard::get_bit(pieces._side_bitboards[Pieces::Black], square)) side = Pieces::Black;
		else continue;

		for (uint8_t type = 0; type < 6; type = type + 1)
		{
			if (BitBoard::get_bit(pieces._piece_bitboards[side][type], square))
			{
				this->invert_piece(square, side, type);
				break;
			}
		}
	}
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