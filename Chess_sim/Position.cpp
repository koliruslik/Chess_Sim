

#include "Position.hpp"


Position::Position() = default;

Position::Position(const std::string& Fen)

{
    std::istringstream fenStream(Fen);
    std::string piecePlacement, activeColor, castlingRights, enPassantStr;
    int halfmoveClock;
    int fullmoveNumber;

    fenStream >> piecePlacement >> activeColor >> castlingRights >> enPassantStr >> halfmoveClock >> fullmoveNumber;

    this->pieces = Pieces(piecePlacement);

    this->moveCtr = (activeColor == "w") ? 0 : 0.5;

    this->wsCastling = castlingRights.find('K') != std::string::npos;
    this->wlCastling = castlingRights.find('Q') != std::string::npos;
    this->bsCastling = castlingRights.find('k') != std::string::npos;
    this->blCastling = castlingRights.find('q') != std::string::npos;

    if (enPassantStr == "-") {
        this->enPassant = 64;
    }
    else {
        int file = enPassantStr[0] - 'a';
        int rank = enPassantStr[1] - '1';
        this->enPassant = rank * 8 + file;
    }

    this->fiftyMovesCtr = halfmoveClock;
    //this->moveCtr = fullmoveNumber + (this->turn == SIDE::Black ? -0.5f : 0.0f);

    this->hash = { this->pieces, this->blackToMove(), this->wlCastling, this->wsCastling, this->blCastling, this->bsCastling };
    this->repetitionHistory.addPosition(this->hash);
}

Position::Position(const std::string& shortFen, uint8_t enPassant, bool wlCastling, bool wsCastling, bool blCastling, bool bsCastling, float moveCtr) {
    this->pieces = { shortFen };
    this->enPassant = enPassant;

    this->wlCastling = wlCastling;
    this->wsCastling = wsCastling;
    this->blCastling = blCastling;
    this->bsCastling = bsCastling;

    this->moveCtr = moveCtr;
    this->hash = { this->pieces, this->blackToMove(), this->wlCastling, this->wsCastling, this->blCastling, this->bsCastling };
    this->repetitionHistory.addPosition(this->hash);
    this->fiftyMovesCtr = 0;
}
std::ostream &operator<<(std::ostream &ostream, const Position& position) {
    ostream << position.pieces << "\n";

    ostream << "En passant: " << (uint32_t)position.enPassant << "\n";
    ostream << "White long castling: " << position.wlCastling << "\n";
    ostream << "White short castling: " << position.wsCastling << "\n";
    ostream << "Black long castling: " << position.blCastling << "\n";
    ostream << "Black short castling: " << position.blCastling << "\n";
    ostream << "Move counter: " << position.moveCtr << "\n";
    ostream << "Zobrist value: " << std::hex << "0x" << position.hash.getValue() << "\n" << std::dec;
    ostream << "Fifty moves counter: " << position.fiftyMovesCtr << "\n";
    ostream << "Threefold repetition counter: " << (uint32_t)position.repetitionHistory.getRepetitionNumber(position.hash);

    return ostream;
}
void Position::move(Move move) {
    this->removePiece(move.getFrom(), move.getAttackerType(), move.getAttackerSide());
    this->addPiece(move.getTo(), move.getAttackerType(), move.getAttackerSide());
    if (move.getDefenderType() != Move::NONE) {
        this->removePiece(move.getTo(), move.getDefenderType(), move.getDefenderSide());
    }

    switch (move.getFlag()) {
        case Move::FLAG::DEFAULT:
            break;

        case Move::FLAG::PAWN_LONG_MOVE:
            this->changeEnPassant((move.getFrom() + move.getTo()) / 2);
            break;
        case Move::FLAG::EN_PASSANT_CAPTURE:
            if (move.getAttackerSide() == SIDE::White) {
                this->removePiece(move.getTo() - 8, PIECE::PAWN, SIDE::Black);
            }
            else {
                this->removePiece(move.getTo() + 8, PIECE::PAWN, SIDE::White);
            }
            break;

        case Move::FLAG::WL_CASTLING:
            this->removePiece(0, PIECE::ROOK, SIDE::White);
            this->addPiece(3, PIECE::ROOK, SIDE::White);
            break;
        case Move::FLAG::WS_CASTLING:
            this->removePiece(7, PIECE::ROOK, SIDE::White);
            this->addPiece(5, PIECE::ROOK, SIDE::White);
            break;
        case Move::FLAG::BL_CASTLING:
            this->removePiece(56, PIECE::ROOK, SIDE::Black);
            this->addPiece(59, PIECE::ROOK, SIDE::Black);
            break;
        case Move::FLAG::BS_CASTLING:
            this->removePiece(63, PIECE::ROOK, SIDE::Black);
            this->addPiece(61, PIECE::ROOK, SIDE::Black);
            break;

        case Move::FLAG::PROMOTE_TO_KNIGHT:
            this->removePiece(move.getTo(), PIECE::PAWN, move.getAttackerSide());
            this->addPiece(move.getTo(), PIECE::KNIGHT, move.getAttackerSide());
            break;
        case Move::FLAG::PROMOTE_TO_BISHOP:
            this->removePiece(move.getTo(), PIECE::PAWN, move.getAttackerSide());
            this->addPiece(move.getTo(), PIECE::BISHOP, move.getAttackerSide());
            break;
        case Move::FLAG::PROMOTE_TO_ROOK:
            this->removePiece(move.getTo(), PIECE::PAWN, move.getAttackerSide());
            this->addPiece(move.getTo(), PIECE::ROOK, move.getAttackerSide());
            break;
        case Move::FLAG::PROMOTE_TO_QUEEN:
            this->removePiece(move.getTo(), PIECE::PAWN, move.getAttackerSide());
            this->addPiece(move.getTo(), PIECE::QUEEN, move.getAttackerSide());
            break;
    }

    this->pieces.updateBitboards();

    if (move.getFlag() != Move::FLAG::PAWN_LONG_MOVE) {
        this->changeEnPassant(Position::NONE);
    }

    switch (move.getFrom()) {
        case 0:
            this->removeWLCastling();
            break;
        case 4:
            this->removeWLCastling();
            this->removeWSCastling();
            break;
        case 7:
            this->removeWSCastling();
            break;
        case 56:
            this->removeBLCastling();
            break;
        case 60:
            this->removeBLCastling();
            this->removeBSCastling();
            break;
        case 63:
            this->removeBSCastling();
            break;
    }

    this->updateMoveCtr();

    this->updateFiftyMovesCtr(move.getAttackerType() == PIECE::PAWN or move.getDefenderType() != Move::NONE);

    if (move.getAttackerType() == PIECE::PAWN or move.getDefenderType() != Move::NONE) {
        this->repetitionHistory.clear();
    }
    this->repetitionHistory.addPosition(this->hash);
}

void Position::moveList(MoveList moves)
{
    for (uint8_t i = 0; i < moves.getSize(); ++i) {
        this->move(moves[i]);
    }
}
Pieces Position::getPieces() const {
    return this->pieces;
}
uint8_t Position::getEnPassant() const {
    return this->enPassant;
}
bool Position::getWLCastling() const {
    return this->wlCastling;
}
bool Position::getWSCastling() const {
    return this->wsCastling;
}
bool Position::getBLCastling() const {
    return this->blCastling;
}
bool Position::getBSCastling() const {
    return this->bsCastling;
}
bool Position::whiteToMove() const {
    return !this->blackToMove();
}
bool Position::blackToMove() const {
    return (this->moveCtr - std::floor(this->moveCtr) > 1e-4);
}
ZobristHash Position::getHash() const {
    return this->hash;
}
bool Position::fiftyMovesRuleDraw() const {
    return (this->fiftyMovesCtr == 50);
}
bool Position::threefoldRepetitionDraw() const {
    return (this->repetitionHistory.getRepetitionNumber(this->hash) == 3);
}
void Position::addPiece(uint8_t square, uint8_t type, uint8_t side) {
    if (!BOp::getBit(this->pieces.getPieceBitboard(side, type), square)) {
        this->pieces.setPieceBitboard(side, type, BOp::set1(this->pieces.getPieceBitboard(side, type), square));
        this->hash.invertPiece(square, type, side);
    }
}
void Position::removePiece(uint8_t square, uint8_t type, uint8_t side) {
    if (BOp::getBit(this->pieces.getPieceBitboard(side, type), square)) {
        this->pieces.setPieceBitboard(side, type, BOp::set0(this->pieces.getPieceBitboard(side, type), square));
        this->hash.invertPiece(square, type, side);
    }
}
void Position::changeEnPassant(uint8_t en_passant) {
    this->enPassant = en_passant;
}
void Position::removeWLCastling() {
    if (this->wlCastling) {
        this->wlCastling = false;
        this->hash.invertWLCastling();
    }
}
void Position::removeWSCastling() {
    if (this->wsCastling) {
        this->wsCastling = false;
        this->hash.invertWSCastling();
    }
}
void Position::removeBLCastling() {
    if (this->blCastling) {
        this->blCastling = false;
        this->hash.invertBLCastling();
    }
}
void Position::removeBSCastling() {
    if (this->bsCastling) {
        this->bsCastling = false;
        this->hash.invertBSCastling();
    }
}
void Position::updateMoveCtr() {
    this->moveCtr = this->moveCtr + 0.5f;
    this->hash.invertMove();
}
void Position::updateFiftyMovesCtr(bool breakEvent) {
    if (breakEvent) {
        this->fiftyMovesCtr = 0;
    }
    else {
        this->fiftyMovesCtr = this->fiftyMovesCtr + .5f;
    }
}
uint8_t Position::getPieceTypeAt(uint8_t square, uint8_t side) const 
{
    for (uint8_t type = PIECE::PAWN; type <= PIECE::KING; type++) 
    {
        if (BOp::getBit(this->pieces.getPieceBitboard(side, type), square)) {
            return type;
        }
    }
    return Position::NONE;
}

uint8_t Position::getPieceSideAt(uint8_t square) const {
    for (uint8_t side = SIDE::White; side <= SIDE::Black; side++) {
        for (uint8_t type = PIECE::PAWN; type <= PIECE::KING; type++) {
            if (BOp::getBit(this->pieces.getPieceBitboard(side, type), square)) {
                return side;
            }
        }
    }
    return SIDE::None;
}

uint8_t Position::getSideToMove() const
{
    return (moveCtr == static_cast<int>(moveCtr)) ? SIDE::White : SIDE::Black;
}
uint8_t Position::getOpponentSide()const
{
	return (moveCtr == static_cast<int>(moveCtr)) ? SIDE::Black : SIDE::White;
}

uint8_t Position::countPieces(uint8_t type, uint8_t side) const
{
    Bitboard bb = pieces.getPieceBitboard(side, type);
    return BOp::count1(bb);
}

int Position::countPiecesTotal(uint8_t side) const
{
    int total = 0;
    for (uint8_t piece = PIECE::PAWN; piece <= PIECE::KING; ++piece)
    {
        total += countPieces(static_cast<PIECE>(piece), side);
    }
    return total;
}

std::string Position::toFEN() const {
    std::string fen;

    for (int rank = 7; rank >= 0; --rank) {
        int emptyCount = 0;
        for (int file = 0; file < 8; ++file) {
            uint8_t square = rank * 8 + file;
            char pieceChar = 0;

            for (uint8_t side = SIDE::White; side <= SIDE::Black && pieceChar == 0; ++side) {
                for (uint8_t type = PIECE::PAWN; type <= PIECE::KING; ++type) {
                    if (BOp::getBit(pieces.getPieceBitboard(side, type), square)) {
                        static const char fenChars[] = { 'P', 'N', 'B', 'R', 'Q', 'K' };
                        pieceChar = (side == SIDE::White) ? fenChars[type] : std::tolower(fenChars[type]);
                        break;
                    }
                }
            }

            if (pieceChar) {
                if (emptyCount > 0) {
                    fen += std::to_string(emptyCount);
                    emptyCount = 0;
                }
                fen += pieceChar;
            }
            else {
                ++emptyCount;
            }
        }

        if (emptyCount > 0) {
            fen += std::to_string(emptyCount);
        }
        if (rank > 0) {
            fen += '/';
        }
    }

    fen += ' ';
    fen += (getSideToMove() == SIDE::White) ? 'w' : 'b';

    fen += ' ';
    std::string castling;
    if (wlCastling) castling += 'Q';
    if (wsCastling) castling += 'K';
    if (blCastling) castling += 'q';
    if (bsCastling) castling += 'k';
    fen += (castling.empty() ? "-" : castling);

    fen += ' ';
    fen += (enPassant != NONE) ? Btrans::indexToSquare(enPassant) : "-";

    fen += ' ';
    fen += std::to_string(static_cast<int>(fiftyMovesCtr));

    fen += ' ';
    fen += std::to_string(static_cast<int>(moveCtr));

    return fen;
}

void Position::placePiece(uint8_t square, uint8_t type, uint8_t side)
{
    this->addPiece(square, type, side);
}

void Position::deletePiece(uint8_t square, uint8_t type, uint8_t side)
{
    this->removePiece(square, type, side);
}

Position& Position::operator=(const Position& other) 
{
    if (this == &other) { 
        return *this;
    }

    enPassant = other.enPassant;
    wlCastling = other.wlCastling;
    wsCastling = other.wsCastling;
    blCastling = other.blCastling;
    bsCastling = other.bsCastling;
    moveCtr = other.moveCtr;
    fiftyMovesCtr = other.fiftyMovesCtr;
    hash = other.hash;

    repetitionHistory = other.repetitionHistory;
    pieces = other.pieces;
	wTime = other.wTime;
	bTime = other.bTime;
    return *this;
}

void Position::save(const std::string& filePath) const
{
    std::ofstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Error" << filePath << std::endl;
        return;
    }

    std::string fenPos = this->toFEN();
    file << fenPos;
	file << std::endl;
	file << wTime << " " << bTime << std::endl;
	std::cout << "Save: " << wTime << " " << bTime << std::endl;
    file.close();
}

Position Position::load(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Error " << filePath << std::endl;
        
        return Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }

    std::string fenLine;
    std::getline(file, fenLine);

    if (fenLine.empty())
    {
        std::cerr << "Error: empty file " << filePath << std::endl;
		Position defPos = Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
		defPos.setTime(defPos.defaultTime,defPos.defaultTime);
        return Position("8/8/8/8/8/8/8/8 w - - 0 1");
    }

	std::string timeLine;
    bool hasTimeLine = static_cast<bool>(std::getline(file, timeLine));

    file.close();

    Position pos(fenLine);

    int whiteTime = 0;
    int blackTime = 0;

    
    
    if (hasTimeLine)
    {
        std::istringstream timeStream(timeLine);
        timeStream >> whiteTime >> blackTime;
        pos.setTime(whiteTime, blackTime);
        std::cout << whiteTime << " " << blackTime << std::endl;
    }
    else
    {
		pos.setTime(pos.defaultTime, pos.defaultTime);
    }
    
	
    return pos;  
}

