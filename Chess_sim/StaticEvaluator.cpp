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
		iterationMasks[SIDE::White][PIECE::KNIGHT] = BOp::set0(iterationMasks[SIDE::White][PIECE::KNIGHT], index);
		knightMobility = knightMobility + BOp::count1(PsLegalMoveMaskGen::generateKnightMask(pieces,index,SIDE::White,false));
	}
	while (iterationMasks[SIDE::White][PIECE::BISHOP])
	{
		index = BOp::bsf(iterationMasks[SIDE::White][PIECE::BISHOP]);
		iterationMasks[SIDE::White][PIECE::BISHOP] = BOp::set0(iterationMasks[SIDE::White][PIECE::BISHOP], index);
		bishopMobility = bishopMobility + BOp::count1(PsLegalMoveMaskGen::generateBishopMask(pieces, index, SIDE::White, false));
	}
	while (iterationMasks[SIDE::White][PIECE::ROOK])
	{
		index = BOp::bsf(iterationMasks[SIDE::White][PIECE::ROOK]);
		iterationMasks[SIDE::White][PIECE::ROOK] = BOp::set0(iterationMasks[SIDE::White][PIECE::ROOK], index);
		rookMobility = rookMobility + BOp::count1(PsLegalMoveMaskGen::generateRookMask(pieces, index, SIDE::White, false));
	}
	while (iterationMasks[SIDE::White][PIECE::QUEEN])
	{
		index = BOp::bsf(iterationMasks[SIDE::White][PIECE::QUEEN]);
		iterationMasks[SIDE::White][PIECE::QUEEN] = BOp::set0(iterationMasks[SIDE::White][PIECE::QUEEN], index);
		queenMobility = queenMobility + BOp::count1(PsLegalMoveMaskGen::generateQueenMask(pieces, index, SIDE::White, false));
	}
	while (iterationMasks[SIDE::Black][PIECE::KNIGHT])
	{
		index = BOp::bsf(iterationMasks[SIDE::Black][PIECE::KNIGHT]);
		iterationMasks[SIDE::Black][PIECE::KNIGHT] = BOp::set0(iterationMasks[SIDE::Black][PIECE::KNIGHT], index);
		knightMobility = knightMobility - BOp::count1(PsLegalMoveMaskGen::generateKnightMask(pieces, index, SIDE::Black, false));
	}
	while (iterationMasks[SIDE::Black][PIECE::BISHOP])
	{
		index = BOp::bsf(iterationMasks[SIDE::Black][PIECE::BISHOP]);
		iterationMasks[SIDE::Black][PIECE::BISHOP] = BOp::set0(iterationMasks[SIDE::Black][PIECE::BISHOP], index);
		bishopMobility = bishopMobility - BOp::count1(PsLegalMoveMaskGen::generateBishopMask(pieces, index, SIDE::Black, false));
	}
	while (iterationMasks[SIDE::Black][PIECE::ROOK])
	{
		index = BOp::bsf(iterationMasks[SIDE::Black][PIECE::ROOK]);
		iterationMasks[SIDE::Black][PIECE::ROOK] = BOp::set0(iterationMasks[SIDE::Black][PIECE::ROOK], index);
		rookMobility = rookMobility - BOp::count1(PsLegalMoveMaskGen::generateRookMask(pieces, index, SIDE::Black, false));
	}
	while (iterationMasks[SIDE::Black][PIECE::QUEEN])
	{
		index = BOp::bsf(iterationMasks[SIDE::Black][PIECE::QUEEN]);
		iterationMasks[SIDE::Black][PIECE::QUEEN] = BOp::set0(iterationMasks[SIDE::Black][PIECE::QUEEN], index);
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

	doublePawn = doublePawn + StaticEvaluator::PawnStructure::DoublePawn * doublePawnCtr;

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

	connectedPawn = connectedPawn + StaticEvaluator::PawnStructure::ConnectedPawn * connectedPawnCtr;

	return connectedPawn;
}

int32_t StaticEvaluator::countPassedPawnStructure(Pieces pieces)
{
	int32_t pawnPromotion = 0;

	Bitboard whitePawns = pieces.getPieceBitboard(SIDE::White, PIECE::PAWN);
	Bitboard blackPawns = pieces.getPieceBitboard(SIDE::Black, PIECE::PAWN);

	uint8_t index; 

	while (whitePawns)
	{
		index = BOp::bsf(whitePawns);
		whitePawns = BOp::set0(whitePawns, index);
		
		if (PassedPawnMasks::White_PASSED_PAWN_MASKS[index] & pieces.getPieceBitboard(SIDE::Black, PIECE::PAWN))
		{
			pawnPromotion = pawnPromotion + StaticEvaluator::PawnStructure::DefaultPawnPromotion[index/8];
		}
		else pawnPromotion = pawnPromotion + StaticEvaluator::PawnStructure::PassedPawnPromotion[index / 8];
	}
	while (blackPawns)
	{
		index = BOp::bsf(blackPawns);
		blackPawns = BOp::set0(blackPawns, index);
		if (PassedPawnMasks::Black_PASSED_PAWN_MASKS[index] & pieces.getPieceBitboard(SIDE::White, PIECE::PAWN))
		{
			pawnPromotion = pawnPromotion - StaticEvaluator::PawnStructure::DefaultPawnPromotion[7 - index / 8];
		}
		else pawnPromotion = pawnPromotion - StaticEvaluator::PawnStructure::PassedPawnPromotion[7 - index / 8];
	}

	return pawnPromotion;
}

int32_t StaticEvaluator::countKingSafety(bool wlCastling, bool wsCastling, bool blCastling, bool bsCastling, bool whiteCastlingHappened, bool blackCastlingHappened)
{
	int32_t crashedCastling = 0;
	if (!whiteCastlingHappened)
	{
		if (!wlCastling) crashedCastling = crashedCastling + StaticEvaluator::KingSafety::CrashedCastling;
		if (!wsCastling) crashedCastling = crashedCastling + StaticEvaluator::KingSafety::CrashedCastling;
	}

	if (!blackCastlingHappened)
	{
		if (!blCastling) crashedCastling = crashedCastling - StaticEvaluator::KingSafety::CrashedCastling;
		if (!bsCastling) crashedCastling = crashedCastling - StaticEvaluator::KingSafety::CrashedCastling;
	}

	return crashedCastling;
}

int32_t StaticEvaluator::countePawnShieldKingSafety(const Pieces pieces, bool whiteCastlingHappened, bool blackCastlingHappened)
{
	int32_t pawnShield = 0;

	int32_t pawnShieldCtr = 0;

	if (whiteCastlingHappened)
	{
		Bitboard whitePawns = pieces.getPieceBitboard(SIDE::White, PIECE::PAWN);
		Bitboard whitePawnShield = PawnShieldMasks::WHITE_PAWN_SHIELD_MASKS[BOp::bsf(pieces.getPieceBitboard(SIDE::White, PIECE::KING))];
		pawnShieldCtr = pawnShieldCtr + BOp::count1(whitePawns & whitePawnShield);
	}

	if (blackCastlingHappened)
	{
		Bitboard blackPawns = pieces.getPieceBitboard(SIDE::Black, PIECE::PAWN);
		Bitboard blackPawnShield = PawnShieldMasks::BLACK_PAWN_SHIELD_MASKS[BOp::bsf(pieces.getPieceBitboard(SIDE::Black, PIECE::KING))];
		pawnShieldCtr = pawnShieldCtr - BOp::count1(blackPawns & blackPawnShield);
	}

	pawnShield = pawnShield + StaticEvaluator::KingSafety::PawnShield * pawnShieldCtr;

	return pawnShield;
}

int32_t StaticEvaluator::twoBishops(Pieces pieces)
{
	int32_t twoBishops = 0;

	if (BOp::count1(pieces.getPieceBitboard(SIDE::White, PIECE::BISHOP)) >= 2)
	{
		twoBishops = twoBishops + StaticEvaluator::Material::TWOBISHOPS;
	}
	if (BOp::count1(pieces.getPieceBitboard(SIDE::Black, PIECE::BISHOP)) >= 2)
	{
		twoBishops = twoBishops - StaticEvaluator::Material::TWOBISHOPS;
	}

	return twoBishops;
}

int32_t StaticEvaluator::endgame(const Pieces pieces, bool whiteLeading)
{
	int32_t endgame = 0;

	if (BOp::count1(pieces.getAllBitboard()) > StaticEvaluator::Endgame::MaximumPiecesForEndgame) return endgame;

	uint8_t attackerSide;
	uint8_t defenderSide;

	if (whiteLeading)
	{
		attackerSide = SIDE::White;
		defenderSide = SIDE::Black;
	}
	else
	{
		attackerSide = SIDE::Black;
		defenderSide = SIDE::White;
	}

	uint8_t attackerKingP = BOp::bsf(pieces.getPieceBitboard(attackerSide, PIECE::KING));
	uint8_t attackerKingX = attackerKingP % 8;
	uint8_t attackerKingY = attackerKingP / 8;

	uint8_t defenderKingP = BOp::bsf(pieces.getPieceBitboard(defenderSide, PIECE::KING));
	uint8_t defenderKingX = defenderKingP % 8;
	uint8_t defenderKingY = defenderKingP / 8;

	endgame = endgame + StaticEvaluator::Endgame::AttackerKingProximityToDefenderKing * (16 - std::abs(attackerKingX - defenderKingX) - std::abs(attackerKingY - defenderKingY));
	endgame = endgame - StaticEvaluator::Endgame::DistanceBetweenDefenderKingAndMiddle * (std::abs(defenderKingX - 3) + std::abs(defenderKingY - 4));

	if (!whiteLeading) endgame = -endgame;
	return endgame;
}

int32_t StaticEvaluator::evaluate(
	const Pieces& pieces,
	bool wlCastling, bool wsCastling,
	bool blCastling, bool bsCastling,
	bool whiteCastlingHappened, bool blackCastlingHappened)
{
	int32_t evaluation = 0;
	//std::cout << "Evaluation started" << std::endl;	

	evaluation += countMaterial(pieces);
	//std::cout << "Material evaluation finished" << std::endl;

	evaluation += countMobility(pieces);
	//std::cout << "Mobility evaluation finished" << std::endl;

	evaluation += countPawnDoubleStructure(pieces);
	//std::cout << "Double pawn evaluation finished" << std::endl;

	evaluation += countPawnConnectedStructure(pieces);
	//std::cout << "Connected pawn evaluation finished" << std::endl;

	evaluation += countPassedPawnStructure(pieces);
	//std::cout << "Passed pawn evaluation finished" << std::endl;

	evaluation += countKingSafety(wlCastling, wsCastling, blCastling, bsCastling, whiteCastlingHappened, blackCastlingHappened);
	//std::cout << "King safety evaluation finished" << std::endl;

	evaluation += countePawnShieldKingSafety(pieces, whiteCastlingHappened, blackCastlingHappened);
	//std::cout << "Pawn shield evaluation finished" << std::endl;

	evaluation += twoBishops(pieces);
	//std::cout << "Two bishops evaluation finished" << std::endl;

	bool whiteLeading = countMaterial(pieces) > 0;
	evaluation += endgame(pieces, whiteLeading);
	//std::cout << "Evatuation finished" << std::endl;
	return evaluation;
}