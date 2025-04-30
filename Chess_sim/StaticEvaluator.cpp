#include "StaticEvaluator.h"

int32_t StaticEvaluator::countMaterial(const Pieces pieces)
{
	int32_t material = 0;
	material = material + StaticEvaluator::Material::PAWN * (BOp::count1(pieces.getPieceBitboard(SIDE::White, PIECE::PAWN)) 
															-BOp::count1(pieces.getPieceBitboard(SIDE::Black, PIECE::PAWN)));

	material = material + StaticEvaluator::Material::KNIGHT * (BOp::count1(pieces.getPieceBitboard(SIDE::White, PIECE::KNIGHT)) 
															  -BOp::count1(pieces.getPieceBitboard(SIDE::Black, PIECE::KNIGHT)));

	material = material + StaticEvaluator::Material::BISHOP * (BOp::count1(pieces.getPieceBitboard(SIDE::White, PIECE::BISHOP)) 
															  -BOp::count1(pieces.getPieceBitboard(SIDE::Black, PIECE::BISHOP)));

	material = material + StaticEvaluator::Material::ROOK * (BOp::count1(pieces.getPieceBitboard(SIDE::White, PIECE::ROOK)) 
															-BOp::count1(pieces.getPieceBitboard(SIDE::Black, PIECE::ROOK)));

	material = material + StaticEvaluator::Material::QUEEN * (BOp::count1(pieces.getPieceBitboard(SIDE::White, PIECE::QUEEN)) 
															 -BOp::count1(pieces.getPieceBitboard(SIDE::Black, PIECE::QUEEN)));

	return material;
}

int32_t StaticEvaluator::countMobility(const Pieces pieces)
{
	int32_t mobility = 0;

	std::array<std::array<Bitboard, 6>, 2> iterationMasks = pieces.getPieceBitboards();
	uint8_t index;

	int32_t knightMobility = 0;
	int32_t bishopMobility = 0;
	int32_t rookMobility = 0;
	int32_t queenMobility = 0;
	
	while (iterationMasks[SIDE::White][PIECE::KNIGHT])
	{
		index = BOp::bsf(iterationMasks[SIDE::White][PIECE::KNIGHT]);
		BOp::set0(iterationMasks[SIDE::White][PIECE::KNIGHT], index);
		knightMobility = knightMobility + BOp::count1(PsLegalMoveMaskGen::generateKnightMask(pieces,index,SIDE::White,false));
	}
	while (iterationMasks[SIDE::White][PIECE::BISHOP])
	{
		index = BOp::bsf(iterationMasks[SIDE::White][PIECE::BISHOP]);
		BOp::set0(iterationMasks[SIDE::White][PIECE::BISHOP], index);
		bishopMobility = bishopMobility + BOp::count1(PsLegalMoveMaskGen::generateBishopMask(pieces, index, SIDE::White, false));
	}
	while (iterationMasks[SIDE::White][PIECE::ROOK])
	{
		index = BOp::bsf(iterationMasks[SIDE::White][PIECE::ROOK]);
		BOp::set0(iterationMasks[SIDE::White][PIECE::ROOK], index);
		rookMobility = rookMobility + BOp::count1(PsLegalMoveMaskGen::generateRookMask(pieces, index, SIDE::White, false));
	}
	while (iterationMasks[SIDE::White][PIECE::QUEEN])
	{
		index = BOp::bsf(iterationMasks[SIDE::White][PIECE::QUEEN]);
		BOp::set0(iterationMasks[SIDE::White][PIECE::QUEEN], index);
		queenMobility = queenMobility + BOp::count1(PsLegalMoveMaskGen::generateQueenMask(pieces, index, SIDE::White, false));
	}
	while (iterationMasks[SIDE::Black][PIECE::KNIGHT])
	{
		index = BOp::bsf(iterationMasks[SIDE::Black][PIECE::KNIGHT]);
		BOp::set0(iterationMasks[SIDE::Black][PIECE::KNIGHT], index);
		knightMobility = knightMobility - BOp::count1(PsLegalMoveMaskGen::generateKnightMask(pieces, index, SIDE::Black, false));
	}
	while (iterationMasks[SIDE::Black][PIECE::BISHOP])
	{
		index = BOp::bsf(iterationMasks[SIDE::Black][PIECE::BISHOP]);
		BOp::set0(iterationMasks[SIDE::Black][PIECE::BISHOP], index);
		bishopMobility = bishopMobility - BOp::count1(PsLegalMoveMaskGen::generateBishopMask(pieces, index, SIDE::Black, false));
	}
	while (iterationMasks[SIDE::Black][PIECE::ROOK])
	{
		index = BOp::bsf(iterationMasks[SIDE::Black][PIECE::ROOK]);
		BOp::set0(iterationMasks[SIDE::Black][PIECE::ROOK], index);
		rookMobility = rookMobility - BOp::count1(PsLegalMoveMaskGen::generateRookMask(pieces, index, SIDE::Black, false));
	}
	while (iterationMasks[SIDE::Black][PIECE::QUEEN])
	{
		index = BOp::bsf(iterationMasks[SIDE::Black][PIECE::QUEEN]);
		BOp::set0(iterationMasks[SIDE::Black][PIECE::QUEEN], index);
		queenMobility = queenMobility - BOp::count1(PsLegalMoveMaskGen::generateQueenMask(pieces, index, SIDE::Black, false));
	}
	mobility = mobility + StaticEvaluator::Mobility::KNIGHT * knightMobility;
	mobility = mobility + StaticEvaluator::Mobility::BISHOP * bishopMobility;
	mobility = mobility + StaticEvaluator::Mobility::ROOK * rookMobility;
	mobility = mobility + StaticEvaluator::Mobility::QUEEN * queenMobility;

	return mobility;
}

int32_t StaticEvaluator::countPawnDoubleStructure(Pieces pieces)
{
	int32_t doublePawn = 0;

	int32_t doublePawnCtr = 0;

	uint8_t whitePawns;
	uint8_t blackPawns;

	for (uint8_t x = 0; x < 8; x = x + 1)
	{
		whitePawns = BOp::count1(pieces.getPieceBitboard(SIDE::White, PIECE::PAWN) & BColumns::COLUMNS[x]);
		blackPawns = BOp::count1(pieces.getPieceBitboard(SIDE::Black, PIECE::PAWN) & BColumns::COLUMNS[x]);

		doublePawnCtr = doublePawnCtr + std::max(0, whitePawns - 1);
		doublePawnCtr = doublePawnCtr - std::max(0, blackPawns - 1);
	}

	doublePawn = doublePawn + StaticEvaluator::DoublePawn * doublePawnCtr;

	return doublePawn;
}

int32_t StaticEvaluator::countPawnConnectedStructure(Pieces pieces)
{
	int32_t connectedPawn = 0;
	
	int32_t connectedPawnCtr = 0;

	Bitboard whiteCaptures = PsLegalMoveMaskGen::generatePawnsLeftCapturesMask(pieces, SIDE::White, true)
							|PsLegalMoveMaskGen::generatePawnsRightCapturesMask(pieces, SIDE::White, true);

	Bitboard blackCaptures = PsLegalMoveMaskGen::generatePawnsLeftCapturesMask(pieces, SIDE::Black, true)
							|PsLegalMoveMaskGen::generatePawnsRightCapturesMask(pieces, SIDE::Black, true);
	
	connectedPawnCtr = connectedPawnCtr + BOp::count1(whiteCaptures & pieces.getPieceBitboard(SIDE::White, PIECE::PAWN));
	connectedPawnCtr = connectedPawnCtr - BOp::count1(blackCaptures & pieces.getPieceBitboard(SIDE::Black, PIECE::PAWN));

	connectedPawn = connectedPawn + StaticEvaluator::ConnectedPawn * connectedPawnCtr;

	return connectedPawn;
}