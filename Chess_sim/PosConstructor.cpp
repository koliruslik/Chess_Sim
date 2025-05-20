#include "PosConstructor.h"

PosConstructor::PosConstructor(Position position) 
    : position(position)
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
                if (wKingCounter >= 1 && selected.side == SIDE::White && selected.type == PIECE::KING)
                {
                    selected.type = PIECE::NONE;
                    std::cout << "Could not be more than 1 king, DESELECTING\n";
                    return;
                }
                else if (bKingCounter >= 1 && selected.side == SIDE::Black && selected.type == PIECE::KING)
                {
                    selected.type = PIECE::NONE;
                    std::cout << "Could not be more than 1 king, DESELECTING\n";
                    return;
                }

                if (selected.type == PIECE::KING && isKingAdjacent(squareIndex, selected.side))
                {
                    selected.type = PIECE::NONE;
                    std::cout << "Too close to enemy king. DESELECTING\n";
                    return;
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
    if (IsKeyPressed(KEY_ONE)) {
        selected.type = PIECE::PAWN;              
        std::cout << "PAWN selected\n";
    }
    if (IsKeyPressed(KEY_TWO)) {
        selected.type = PIECE::KNIGHT;           
        std::cout << "KNIGHT selected\n";
    }
    if (IsKeyPressed(KEY_THREE)) {
        selected.type = PIECE::BISHOP;
        std::cout << "BISHOP selected\n";
    }
    if (IsKeyPressed(KEY_FOUR)) {
        selected.type = PIECE::ROOK;
        std::cout << "ROOK selected\n";
    }
    if (IsKeyPressed(KEY_FIVE)) {
        selected.type = PIECE::QUEEN;
        std::cout << "QUEEN selected\n";
    }
    if (IsKeyPressed(KEY_SIX)) {
        selected.type = PIECE::KING;
        
        std::cout << "KING selected\n";
    }

  
    if (IsKeyPressed(KEY_SEVEN)) {
        selected.isWhite = !selected.isWhite;
        selected.side = selected.isWhite ? SIDE::White : SIDE::Black;
        std::cout << (selected.isWhite ? "White pieces selected\n" : "Black pieces selected\n");
    }
    if (IsKeyPressed(KEY_EIGHT))
    {
        selected.type = NONE;
        selected.isWhite = true;
        std::cout << "clear selection\n";
    }
}

void PosConstructor::update()
{
	bRenderer.setTheme(theme);
	bRenderer.drawBoard(position, 255, 255, 255, 255);
    wKingCounter = position.countPieces(PIECE::KING, SIDE::White);
    bKingCounter = position.countPieces(PIECE::KING, SIDE::Black);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
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
    
    SelectPiece();
}
