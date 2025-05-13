#include <string>
#include <vector>
#include <bit>

#include "Pieces.hpp"

#pragma once

namespace Btrans
{
    inline uint8_t squareToIndex(const std::string& square)
    {
        if (square.length() != 2) return -1;
        char file = square[0];
        char rank = square[1];

        if (file < 'a' || file > 'h' || rank < '1' || rank > '8') return -1;

        return (rank - '1') * 8 + (file - 'a');
    }

    inline std::string indexToSquare(uint8_t index)
    {
        if (index < 0 || index > 63) return "??";

        char file = 'a' + (index % 8);
        char rank = '1' + (index / 8);

        return std::string(1, file) + std::string(1, rank);
    }

    inline std::pair<int, int> indexToRowCol(uint8_t index)
    {
        if (index < 0 || index > 63) return { -1, -1 };
        int col = index % 8;
        int row = 7 - (index / 8);
        return { row, col };
    }

    inline char pieceToChar(uint8_t piece)
    {
        switch (piece)
        {
        case PIECE::PAWN: return 'p';
        case PIECE::KNIGHT: return 'n';
        case PIECE::BISHOP: return 'b';
        case PIECE::ROOK: return 'r';
        case PIECE::QUEEN: return 'q';
        case PIECE::KING: return 'k';
        default: return '\0';
        }
    }
    
    inline PIECE charToPiece(char pieceChar)
    {
        switch (pieceChar)
        {
        case 'p': return PIECE::PAWN;
        case 'n': return PIECE::KNIGHT;
        case 'b': return PIECE::BISHOP;
        case 'r': return PIECE::ROOK;
        case 'q': return PIECE::QUEEN;
        case 'k': return PIECE::KING;
        default: return PIECE::NONE;
        }
    }
}