#pragma once

#include "BitBoard.h"
#include "Pieces.h"
#include "ZobristHash.h"
#include "RepetitionHistory.h"
#include "Move.h"

class Position
{
public:
	Position(const std::string& short_fen, uint8_t en_passant, bool w_l_castling, bool w_s_castling, bool b_l_castling, bool b_s_castling, float move_ctr);
	Pieces _pieces;
	uint8_t _en_passant;

	bool _white_long_castling;
	bool _white_short_castling;
	bool _black_long_castling;
	bool _black_short_castling;

	bool _white_castling_happened;
	bool _black_castling_happened;

	float _move_counter;
	ZobristHash _hash;
	float _fifty_moves_counter;
	RepetitionHistory _repetition_history;
	void move(const Move& move);
private:
	void _add_piece(uint8_t square, uint8_t type, uint8_t side);
	void _remove_piece(uint8_t square, uint8_t type, uint8_t side);
	void _change_en_passant(uint8_t en_passant);
	void _remove_white_long_castling();
	void _remove_white_short_castling();
	void _remove_black_long_castling();
	void _remove_black_short_castling();
	void _update_move_counter();
	void _update_fifty_moves_counter(bool break_event);
};