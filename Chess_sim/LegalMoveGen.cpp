#include "LegalMoveGen.h"

bool LegalMoveGen::_is_legal(Pieces pieces, Move move, bool en_passant_capture)
{
	BitBoard::set_0(pieces._piece_bitboards[move._attacker_side][move._attacker_type], move._from);
	BitBoard::set_1(pieces._piece_bitboards[move._attacker_side][move._attacker_type], move._to);
	if(move._defender_type != 255) BitBoard::set_0(pieces._piece_bitboards[move._defender_side][move._defender_type], move._to);
	if (en_passant_capture)
	{
		if (move._attacker_side == Pieces::White) BitBoard::set_0(pieces._piece_bitboards[Pieces::Black][Pieces::Pawn], move._to - 8);
		BitBoard::set_0(pieces._piece_bitboards[Pieces::White][Pieces::Pawn], move._to + 8);
	}

	pieces.uptade_bitboards();

	if(PsLegalMoveMaskGen::is_under_attack(pieces, BitBoard::bsf(pieces._piece_bitboards[move._attacker_side][Pieces::King]), move._attacker_side)) return false;

	return true;
}

void LegalMoveGen::_pawn_mask_to_moves(Pieces pieces, BitBoard mask, uint8_t attacker_side, int8_t attacker_index, bool look_for_defender, uint8_t flag, MoveList& moves)
{
    uint8_t defender_p;
    uint8_t defender_type = 255;

    Move move;

    while (mask != 0)
    {
        defender_p = BitBoard::bsf(mask);
        BitBoard::set_0(mask, defender_p);

        if (look_for_defender)
        {
            defender_type = 255;
            for (uint8_t i = 0; i < 6; i = i + 1) 
            {
                if (BitBoard::get_bit(pieces._piece_bitboards[Pieces::inverse(attacker_side)][i], defender_p)) 
                {
                    defender_type = i;
                    break;
                }
            }
        }

        move = { (uint8_t)(defender_p + attacker_index), defender_p, Pieces::Pawn, attacker_side, defender_type, Pieces::inverse(attacker_side), flag };

        if (LegalMoveGen::_is_legal(pieces, move, false)) 
        {
            if (defender_p < 8 or defender_p > 55)
            {
                moves.push_back({ (uint8_t)(defender_p + attacker_index), defender_p, 0, attacker_side, defender_type, Pieces::inverse(attacker_side), Move::Flag::PromoteToKnight });
                moves.push_back({ (uint8_t)(defender_p + attacker_index), defender_p, 0, attacker_side, defender_type, Pieces::inverse(attacker_side), Move::Flag::PromoteToBishop });
                moves.push_back({ (uint8_t)(defender_p + attacker_index), defender_p, 0, attacker_side, defender_type, Pieces::inverse(attacker_side), Move::Flag::PromoteToRook });
                moves.push_back({ (uint8_t)(defender_p + attacker_index), defender_p, 0, attacker_side, defender_type, Pieces::inverse(attacker_side), Move::Flag::PromoteToQueen });
            }
            else moves.push_back(move);
        }
    }
}

void LegalMoveGen::_piece_mask_to_moves(Pieces pieces, BitBoard mask, uint8_t attacker_p, uint8_t attacker_type, uint8_t attacker_side, MoveList& moves)
{
    uint8_t defender_p;
    uint8_t defender_type;

    Move move;

    while (mask != 0)
    {
        defender_p = BitBoard::bsf(mask);
        BitBoard::set_0(mask, defender_p);

        defender_type = 255;
        for (uint8_t i = 0; i < 6; i = i + 1)
        {
            if (BitBoard::get_bit(pieces._piece_bitboards[Pieces::inverse(attacker_side)][i], defender_p))
            {
                defender_type = i;
                break;
            }
        }
        
        move = { attacker_p, defender_p, attacker_type, attacker_side, defender_type, Pieces::inverse(attacker_side) };

        if (LegalMoveGen::_is_legal(pieces, move, false)) moves.push_back(move);
    }
}

void LegalMoveGen::_add_en_passant_captures(Pieces pieces, uint8_t side, uint8_t en_passant, MoveList& moves)
{
    if (en_passant == 255) return;

    Move move;

    if (side == Pieces::White) {
        if (en_passant % 8 != 7 and BitBoard::get_bit(pieces._piece_bitboards[Pieces::White][Pieces::Pawn], en_passant - 7)) {
            move = { (uint8_t)(en_passant - 7), en_passant, Pieces::Pawn, Pieces::White, 255, 255, Move::Flag::EnPassantCapture };
            if (LegalMoveGen::_is_legal(pieces, move, true)) moves.push_back(move);
        }
        if (en_passant % 8 != 0 and BitBoard::get_bit(pieces._piece_bitboards[Pieces::White][Pieces::Pawn], en_passant - 9)) {
            move = { (uint8_t)(en_passant - 9), en_passant, Pieces::Pawn, Pieces::White, 255, 255, Move::Flag::EnPassantCapture };
            if (LegalMoveGen::_is_legal(pieces, move, true)) moves.push_back(move);
        }
    }
    else {
        if (en_passant % 8 != 0 and BitBoard::get_bit(pieces._piece_bitboards[Pieces::Black][Pieces::Pawn], en_passant + 7)) {
            move = { (uint8_t)(en_passant + 7), en_passant, Pieces::Pawn, Pieces::Black, 255, 255, Move::Flag::EnPassantCapture };
            if (LegalMoveGen::_is_legal(pieces, move, true)) moves.push_back(move);
        }
        if (en_passant % 8 != 7 and BitBoard::get_bit(pieces._piece_bitboards[Pieces::Black][Pieces::Pawn], en_passant + 9)) {
            move = { (uint8_t)(en_passant + 9), en_passant, Pieces::Pawn, Pieces::Black, 255, 255, Move::Flag::EnPassantCapture };
            if (LegalMoveGen::_is_legal(pieces, move, true)) moves.push_back(move);
        }
    }
}
void LegalMoveGen::_add_castling_moves(Pieces pieces, uint8_t side, bool long_castling, bool short_castling, MoveList& moves)
{
    uint8_t index;
    uint8_t long_castling_flag;
    uint8_t short_castling_flag;
    if (side == Pieces::White) {
        index = 0;
        long_castling_flag = Move::Flag::WhiteLongCastling;
        short_castling_flag = Move::Flag::WhiteShortCastling;
    }
    else {
        index = 56;
        long_castling_flag = Move::Flag::BlackLongCastling;
        short_castling_flag = Move::Flag::BlackShortCastling;
    }

    if (long_castling and BitBoard::get_bit(pieces._piece_bitboards[side][Pieces::Rook], 0 + index) and BitBoard::get_bit(pieces._empty, 1 + index) and BitBoard::get_bit(pieces._empty, 2 + index) and BitBoard::get_bit(pieces._empty, 3 + index)) {
        if (!PsLegalMoveMaskGen::is_under_attack(pieces, BitBoard::bsf(pieces._piece_bitboards[side][Pieces::King]), side) and !PsLegalMoveMaskGen::is_under_attack(pieces, 2 + index, side) and !PsLegalMoveMaskGen::is_under_attack(pieces, 3 + index, side)) moves.push_back({ (uint8_t)(4 + index), (uint8_t)(2 + index), Pieces::King, side, 255, 255, long_castling_flag });
    }
    if (short_castling and BitBoard::get_bit(pieces._piece_bitboards[side][Pieces::Rook], 7 + index) and BitBoard::get_bit(pieces._empty, 5 + index) and BitBoard::get_bit(pieces._empty, 6 + index)) {
        if (!PsLegalMoveMaskGen::is_under_attack(pieces, BitBoard::bsf(pieces._piece_bitboards[side][Pieces::King]), side) and !PsLegalMoveMaskGen::is_under_attack(pieces, 5 + index, side) and !PsLegalMoveMaskGen::is_under_attack(pieces, 6 + index, side)) moves.push_back({ (uint8_t)(4 + index), (uint8_t)(6 + index), Pieces::King, side, 255, 255, short_castling_flag });
    }
}

MoveList LegalMoveGen::generate(Position copy, uint8_t side)
{
    MoveList moves;

    for (uint8_t i = 0; i < 64; ++i) {
        if (BitBoard::get_bit(copy._pieces._piece_bitboards[side][Pieces::Pawn], i)) {
            _pawn_mask_to_moves(copy._pieces, BitBoard::get_bit(copy._pieces._piece_bitboards[side][Pieces::Pawn], i), side, i, true, 0, moves);
        }
    }

    for (uint8_t piece_type = Pieces::Knight; piece_type <= Pieces::King; ++piece_type) {
        for (uint8_t i = 0; i < 64; ++i) {
            if (BitBoard::get_bit(copy._pieces._piece_bitboards[side][piece_type], i)) {
                _piece_mask_to_moves(copy._pieces, BitBoard::get_bit(copy._pieces._piece_bitboards[side][piece_type], i), i, piece_type, side, moves);
            }
        }
    }

    _add_en_passant_captures(copy._pieces, side, copy._en_passant, moves);

    _add_castling_moves(copy._pieces, side, true, true, moves);

    return moves;
}