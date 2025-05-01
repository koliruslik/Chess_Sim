#include "MoveSorter.hpp"

MoveList MoveSorter::sort(Pieces pieces, MoveList moves)
{
	for (uint8_t i = 0; i < moves.getSize() - 1; i = i + 1)
	{
		for (uint8_t j = 0; j < moves.getSize() - i - 1; j = j + 1)
		{
			if(MoveSorter::evaluateMove(pieces, moves[j]) < MoveSorter::evaluateMove(pieces, moves[j + 1]))
			{
				Move temp = moves[j];
				moves[j] = moves[j + 1];
				moves[j + 1] = temp;
			}
		}
	}
	return moves;
}

int32_t MoveSorter::evaluateMove(Pieces pieces, Move move)
{
	int32_t evaluation = 0;

	if (move.getAttackerType() != PIECE::PAWN)
	{
		Bitboard opponentPawnAttacks = PsLegalMoveMaskGen::generatePawnsLeftCapturesMask(pieces, move.getDefenderSide(), true)
									  |PsLegalMoveMaskGen::generatePawnsRightCapturesMask(pieces, move.getDefenderSide(), true);
		if (BOp::getBit(opponentPawnAttacks, move.getTo()))
		{
			switch (move.getAttackerType())
			{
				case PIECE::KNIGHT: evaluation = evaluation - StaticEvaluator::Material::KNIGHT; break;
				case PIECE::BISHOP: evaluation = evaluation - StaticEvaluator::Material::BISHOP; break;
				case PIECE::ROOK: evaluation = evaluation - StaticEvaluator::Material::ROOK; break;
				case PIECE::QUEEN: evaluation = evaluation - StaticEvaluator::Material::QUEEN; break;

			}
		}
	}

	if (move.getDefenderType() != 255)
	{
		switch (move.getDefenderType())
		{
			case PIECE::PAWN: evaluation = evaluation + 1000 * StaticEvaluator::Material::PAWN; break;
			case PIECE::KNIGHT: evaluation = evaluation + 1000 * StaticEvaluator::Material::KNIGHT; break;
			case PIECE::BISHOP: evaluation = evaluation + 1000 * StaticEvaluator::Material::BISHOP; break;
			case PIECE::ROOK: evaluation = evaluation + 1000 * StaticEvaluator::Material::ROOK; break;
			case PIECE::QUEEN: evaluation = evaluation + 1000 * StaticEvaluator::Material::QUEEN; break;
		}
		switch (move.getAttackerType())
		{
			case PIECE::PAWN: evaluation = evaluation - StaticEvaluator::Material::PAWN; break;
			case PIECE::KNIGHT: evaluation = evaluation - StaticEvaluator::Material::KNIGHT; break;
			case PIECE::BISHOP: evaluation = evaluation - StaticEvaluator::Material::BISHOP; break;
			case PIECE::ROOK: evaluation = evaluation - StaticEvaluator::Material::ROOK; break;
			case PIECE::QUEEN: evaluation = evaluation - StaticEvaluator::Material::QUEEN; break;
		}
	}
	
	return evaluation;
}