#include "PosConstructor.h"

PosConstructor::PosConstructor(Position position, std::shared_ptr<BoardRenderer>& renderer)
    :position(position),
    renderer(renderer)
{
    selected.type = PIECE::NONE;
    selected.side = SIDE::White;
    selected.isWhite = true;
}

bool PosConstructor::isKingAdjacent(uint8_t squareIndex, uint8_t side)
{
    uint8_t enemySide = side == SIDE::White ? SIDE::Black : SIDE::White;

    Bitboard enemyKingBB = position.getPieces().getPieceBitboard(enemySide, PIECE::KING);

    int dx[] = { -1,  0, 1, -1, 1, -1, 0, 1 };
    int dy[] = { -1, -1, -1,  0, 0,  1, 1, 1 };

    int row = squareIndex / 8;
    int col = squareIndex % 8;

    for (int i = 0; i < 8; ++i)
    {
        int r = row + dy[i];
        int c = col + dx[i];

        if (r >= 0 && r < 8 && c >= 0 && c < 8)
        {
            int neighborIndex = r * 8 + c;
            if (BOp::getBit(enemyKingBB, neighborIndex))
                return true;
        }
    }
    return false;
}

bool PosConstructor::canPlacePiece(uint8_t type, uint8_t side, int row, uint8_t squareIndex)
{
    if (type == PIECE::NONE) return false;

    int totalCount = position.countPiecesTotal(side);
    if (totalCount >= 16) {
        renderer->displayWarning("Maximum of 16 pieces per side", 2.0f);
        return false;
    }

    int pawnCount = position.countPieces(PIECE::PAWN, side);
    int kingCount = position.countPieces(PIECE::KING, side);
    int knightCount = position.countPieces(PIECE::KNIGHT, side);
    int bishopCount = position.countPieces(PIECE::BISHOP, side);
    int rookCount = position.countPieces(PIECE::ROOK, side);
    int queenCount = position.countPieces(PIECE::QUEEN, side);

    const int maxPawn = 8;
    const int maxKing = 1;
    const int maxKnight = 2;
    const int maxBishop = 2;
    const int maxRook = 2;
    const int maxQueen = 1;

    int extraKnights = std::max(0, knightCount - maxKnight);
    int extraBishops = std::max(0, bishopCount - maxBishop);
    int extraRooks = std::max(0, rookCount - maxRook);
    int extraQueens = std::max(0, queenCount - maxQueen);

    int totalPromotedUsed = extraKnights + extraBishops + extraRooks + extraQueens;
    int promotionSlotsLeft = 8 - pawnCount - totalPromotedUsed;
    if (promotionSlotsLeft < 0) promotionSlotsLeft = 0;

    switch (type) {
    case PIECE::PAWN:
        if (promotionSlotsLeft <= 0) {
            renderer->displayWarning("No pawns slots left - cannot place more pawns", 2.0f);
            return false;
        }
        if (pawnCount >= maxPawn) {
            renderer->displayWarning("Maximum number of pawns reached", 2.0f);
            return false;
        }
        if (row == 0 || row == 7) {
            renderer->displayWarning("Pawns can't be placed on first or last rank", 2.0f);
            return false;
        }
        return true;

    case PIECE::KING:
        if (kingCount >= maxKing) {
            renderer->displayWarning("Only one king allowed per side", 2.0f);
            return false;
        }
        if (isKingAdjacent(squareIndex, side)) {
            renderer->displayWarning("Too close to enemy king", 2.0f);
            return false;
        }
        return true;

    case PIECE::QUEEN:
        if (queenCount < maxQueen) {
            return true;
        }
        if (promotionSlotsLeft <= 0) {
            renderer->displayWarning("No pawns left for extra queen", 2.0f);
            return false;
        }
        return true;

    case PIECE::ROOK:
        if (rookCount < maxRook) {
            return true;
        }
        if (promotionSlotsLeft <= 0) {
            renderer->displayWarning("No pawns left for extra rook", 2.0f);
            return false;
        }
        return true;

    case PIECE::BISHOP:
        if (bishopCount < maxBishop) {
            return true;
        }
        if (promotionSlotsLeft <= 0) {
            renderer->displayWarning("No pawns left for extra bishop", 2.0f);
            return false;
        }
        return true;

    case PIECE::KNIGHT:
        if (knightCount < maxKnight) {
            return true;
        }
        if (promotionSlotsLeft <= 0) {
            renderer->displayWarning("No pawns left for extra knight", 2.0f);
            return false;
        }
        return true;

    default:
        return false;
    }
}

void PosConstructor::SelectSquare(bool isPlacing)
{
    int col = mousePos.x / squareSize;
    int row = boardSize - (mousePos.y / squareSize);
    if (col >= 0 && col < boardSize && row >= 0 && row < boardSize)
    {
        std::string square = std::string(1, char('a' + col)) + std::string(1, char('1' + row));
        uint8_t squareIndex = Btrans::squareToIndex(square);

        if (isPlacing)
        {
            if (selected.type != PIECE::NONE)
            {
                if (!canPlacePiece(selected.type, selected.side, row, squareIndex)) return;

                uint8_t existingSide = position.getPieceSideAt(squareIndex);
                uint8_t existingTypeWhite = position.getPieceTypeAt(squareIndex, SIDE::White);
                uint8_t existingTypeBlack = position.getPieceTypeAt(squareIndex, SIDE::Black);

                if (existingTypeWhite != Position::NONE && existingTypeBlack == Position::NONE)
                {
                    position.deletePiece(squareIndex, existingTypeWhite, SIDE::White);
                }
                else if (existingTypeBlack != Position::NONE && existingTypeWhite == Position::NONE)
                {
                    position.deletePiece(squareIndex, existingTypeBlack, SIDE::Black);
                }

                position.placePiece(squareIndex, selected.type, selected.side);
            }
        }
        else
        {
            uint8_t side = position.getPieceSideAt(squareIndex);
            uint8_t typeW = position.getPieceTypeAt(squareIndex, SIDE::White);
            uint8_t typeB = position.getPieceTypeAt(squareIndex, SIDE::Black);
            if (typeW != Position::NONE && typeB == Position::NONE)
            {
                position.deletePiece(squareIndex, typeW, side);

            }
            else if (typeB != Position::NONE && typeW == Position::NONE)
            {
                position.deletePiece(squareIndex, typeB, side);
            }
        }
    }
    else
    {
        //std::cout << "Clicked outside the board!\n";
    }
    //system("cls");
    //std::cout << position.toFEN();
    
}

void PosConstructor::SelectPiece()
{
    int pressedKey = GetKeyPressed();
    switch (pressedKey)
    {
    case KEY_P:
        selected.type = PIECE::PAWN;
        selected.strType = "Pawn";
        std::cout << "PAWN selected\n";
        break;

    case KEY_N:
        selected.type = PIECE::KNIGHT;
        selected.strType = "Knight";
        std::cout << "KNIGHT selected\n";
        break;

    case KEY_B:
        selected.type = PIECE::BISHOP;
        selected.strType = "Bishop";
        std::cout << "BISHOP selected\n";
        break;

    case KEY_R:
        selected.type = PIECE::ROOK;
        selected.strType = "Rook";
        std::cout << "ROOK selected\n";
        break;

    case KEY_Q:
        selected.type = PIECE::QUEEN;
        selected.strType = "Queen";
        std::cout << "QUEEN selected\n";
        break;

    case KEY_K:
        selected.type = PIECE::KING;
        selected.strType = "King";
        std::cout << "KING selected\n";
        break;

    case KEY_TAB:
        selected.isWhite = !selected.isWhite;
        selected.side = selected.isWhite ? SIDE::White : SIDE::Black;
        std::cout << (selected.isWhite ? "White pieces selected\n" : "Black pieces selected\n");
        break;

    case KEY_C:
        selected.type = NONE;
        selected.isWhite = true;
        std::cout << "clear selection\n";
        break;

    case KEY_H:
        showInfo = !showInfo;
        break;

    default:
        break;
    }

}

void PosConstructor::update()
{
	renderer->setTheme(theme);
	renderer->drawBoard(position, 255, 255, 255, 255);
    if (selected.type != PIECE::NONE) renderer->drawPieceAtCursor(selected.type, selected.side,scale);
    if(showInfo) renderer->drawInfo(5, 5, 20,{
        "Secelted piece: " + selected.strType,
        "p - Pawn",
        "n - Knight",
        "b - Bishop",
        "r - Rook",
        "q - Queen",
        "k - King",
        "c - Clear selection",
        "TAB - Change color", 
        "h - Hide info"});
    wKingCounter = position.countPieces(PIECE::KING, SIDE::White);
    bKingCounter = position.countPieces(PIECE::KING, SIDE::Black);
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        mousePos = GetMousePosition();
        SelectSquare(true);
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        mousePos = GetMousePosition();
        SelectSquare(false);
    }
    if (IsKeyPressed(KEY_Q))
    {
        std::cout << "Q PRESSED\n";
        result = MenuResult::ESCMenu;
    }
    if (IsMouseButtonUp(MOUSE_BUTTON_LEFT))
    {
        for (; scale <= MAXSCALE; scale += DIFF);
    }
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        for (; scale >= MINSCALE; scale -= DIFF);
    }
    
    SelectPiece();
}
