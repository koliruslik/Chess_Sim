#include "StandartMovingNotation.hpp"
#include <bitset>
StandartMovingNotation::StandartMovingNotation(const std::string& notatedMove, Position position)
{
    from = Position::NONE;
    to = Position::NONE;
    attackerType = Position::NONE;
    attackerSide = Position::NONE;
    defenderType = Position::NONE;
    defenderSide = Position::NONE;
    flag = Move::FLAG::DEFAULT;


    size_t len = notatedMove.length();
    if (len < 2) return;

    size_t pos = 0;
    char pieceChar = 'P';
    if (std::isupper(notatedMove[0])) {
        pieceChar = notatedMove[0]; 
        pos = 1;
    }

    if (std::fmod(position.getMoveCtr(), 1.0f) == 0.0f) {
        attackerSide = SIDE::White;
        defenderSide = SIDE::Black;
    }
    else {
        attackerSide = SIDE::Black;
        defenderSide = SIDE::White;
    }

    if (notatedMove == "O-O") {
        flag = (attackerSide == SIDE::White) ? Move::FLAG::WS_CASTLING : Move::FLAG::BS_CASTLING;
        return;
    }
    if (notatedMove == "O-O-O") {
        flag = (attackerSide == SIDE::White) ? Move::FLAG::WL_CASTLING : Move::FLAG::BL_CASTLING;
        return;
    }

    bool isCapture = (notatedMove.find('x') != std::string::npos);
    if (isCapture) pos++;

    char fileFrom = '\0', rankFrom = '\0';
    if (std::islower(notatedMove[pos]) && std::isdigit(notatedMove[pos + 1])) {
        fileFrom = notatedMove[pos];
        rankFrom = notatedMove[pos + 1];
        pos += 2;
    }


    char fileTo = notatedMove[pos + 2];
    char rankTo = notatedMove[pos + 3];

    if (fileFrom != '\0') {
        from = SquareToIndex(fileFrom, rankFrom);
    }
    to = SquareToIndex(fileTo, rankTo);
    attackerType = PieceCharToType(pieceChar);

    if (attackerType == PIECE::PAWN && isCapture && rankTo == ((attackerSide == SIDE::White) ? '6' : '3'))
    {
        flag = Move::FLAG::EN_PASSANT_CAPTURE;
    }
    if (attackerType == PIECE::PAWN && (rankTo == '8' || rankTo == '1') && len > 4)
    {
        switch (notatedMove[4])
        {
        case 'N': flag = Move::FLAG::PROMOTE_TO_KNIGHT; break;
        case 'B': flag = Move::FLAG::PROMOTE_TO_BISHOP; break;
        case 'R': flag = Move::FLAG::PROMOTE_TO_ROOK; break;
        case 'Q': flag = Move::FLAG::PROMOTE_TO_QUEEN; break;
        }
    }
    if (attackerType == PIECE::PAWN && std::abs(rankTo - rankFrom) == 2)
    {
        flag = Move::FLAG::PAWN_LONG_MOVE;
    }
    if (isCapture)
    {
        defenderType = position.getPieceTypeAt(to, defenderSide);
    }
}

uint8_t StandartMovingNotation::SquareToIndex(char file, char rank)
{
    return (rank - '1') * 8 + (file - 'a');
}

uint8_t StandartMovingNotation::PieceCharToType(char pieceChar)
{
    switch (std::toupper(pieceChar))
    {
        case 'P': return PIECE::PAWN;
        case 'N': return PIECE::KNIGHT;
        case 'B': return PIECE::BISHOP;
        case 'R': return PIECE::ROOK;
        case 'Q': return PIECE::QUEEN;
        case 'K': return PIECE::KING;
        default: return 0;
    }
}

Move StandartMovingNotation::StandartNotationMove() const
{
    return Move(this->from, this->to, this->attackerType, this->attackerSide, this->defenderType, this->defenderSide, this->flag);
}

void StandartMovingNotation::showPossibleMoves(Pieces pieces, uint8_t square, uint8_t side) {
    std::cout << "showPossibleMoves called for square: " << (int)square << " and side: " << (int)side << std::endl;

    Bitboard moveMask = 0;

    std::cout << "Checking piece type at square: " << (int)square << " for side: " << (int)side << std::endl;

    if (BOp::getBit(pieces.getPieceBitboard(side, PIECE::PAWN), square)) {
        std::cout << "Pawn detected at square: " << (int)square << std::endl;
        moveMask = PsLegalMoveMaskGen::generatePawnsDefaultMask(pieces, side) |
            PsLegalMoveMaskGen::generatePawnsLongMask(pieces, side) |
            PsLegalMoveMaskGen::generatePawnsLeftCapturesMask(pieces, side, false) |
            PsLegalMoveMaskGen::generatePawnsRightCapturesMask(pieces, side, false);
    }
    else if (BOp::getBit(pieces.getPieceBitboard(side, PIECE::KNIGHT), square)) {
        std::cout << "Knight detected at square: " << (int)square << std::endl;
        moveMask = PsLegalMoveMaskGen::generateKnightMask(pieces, square, side, false);
    }
    else if (BOp::getBit(pieces.getPieceBitboard(side, PIECE::BISHOP), square)) {
        std::cout << "Bishop detected at square: " << (int)square << std::endl;
        moveMask = PsLegalMoveMaskGen::generateBishopMask(pieces, square, side, false);
    }
    else if (BOp::getBit(pieces.getPieceBitboard(side, PIECE::ROOK), square)) {
        std::cout << "Rook detected at square: " << (int)square << std::endl;
        moveMask = PsLegalMoveMaskGen::generateRookMask(pieces, square, side, false);
    }
    else if (BOp::getBit(pieces.getPieceBitboard(side, PIECE::QUEEN), square)) {
        std::cout << "Queen detected at square: " << (int)square << std::endl;
        moveMask = PsLegalMoveMaskGen::generateQueenMask(pieces, square, side, false);
    }
    else if (BOp::getBit(pieces.getPieceBitboard(side, PIECE::KING), square)) {
        std::cout << "King detected at square: " << (int)square << std::endl;
        moveMask = PsLegalMoveMaskGen::generateKingMask(pieces, square, side, false);
    }
    else {
        std::cout << "No piece detected at square: " << (int)square << std::endl;
    }

    // Вывод битовой маски возможных ходов
    std::cout << "Generated moveMask: " << std::bitset<64>(moveMask) << std::endl;

    // Выводим список доступных ходов
    std::cout << "Possible moves on: " << (int)square << ":" << std::endl;
    bool foundMoves = false;
    for (int i = 0; i < 64; i++) {
        if (BOp::getBit(moveMask, i)) {
            std::cout << " - " << i << std::endl; // Можешь заменить на координаты шахматной доски
            foundMoves = true;
        }
    }
    if (!foundMoves) {
        std::cout << "No moves found!" << std::endl;
    }
}


//