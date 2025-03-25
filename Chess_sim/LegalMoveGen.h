#include "MoveList.h"
#include "PsLegalMoveMaskGen.h"
#include "Position.h"
#pragma once

class LegalMoveGen
{
public:
	static MoveList generate(Position copy, uint8_t side);

private:
	static bool _is_legal(Pieces pieces, Move move, bool en_passant_capture);
	static void _pawn_mask_to_moves(Pieces pieces, BitBoard mask, uint8_t attacker_side, int8_t attacker_index, bool look_for_defender, uint8_t flag, MoveList& moves);
	static void _piece_mask_to_moves(Pieces pieces, BitBoard mask, uint8_t attacker_p, uint8_t attacker_type, uint8_t attacker_side, MoveList& moves);
	static void _add_en_passant_captures(Pieces pieces, uint8_t side, uint8_t en_passant, MoveList& moves);
	static void _add_castling_moves(Pieces pieces, uint8_t side, bool long_castling, bool short_castling, MoveList& moves);
	
	
};