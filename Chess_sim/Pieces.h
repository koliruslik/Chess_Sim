#include "BitBoard.h"

#pragma once

#ifndef PIECES_H
#define PIECES_H



class Pieces 
{
public:
    Pieces();
    Pieces(const std::string& short_fen);

    std::array<std::array<BitBoard, 6>, 2> _piece_bitboards{};
    std::array<BitBoard, 2> _side_bitboards{};
    std::array<BitBoard, 2> _inversion_side_bitboards{};
    BitBoard _all;
    BitBoard _empty;

    void uptade_bitboards();
    static uint8_t inverse(uint8_t side);

    void PrintPieces();
    
    static constexpr uint8_t Pawn = 0;
    static constexpr uint8_t Knight = 1;
    static constexpr uint8_t Bishop = 2;
    static constexpr uint8_t Rook = 3;
    static constexpr uint8_t Queen = 4;
    static constexpr uint8_t King = 5;

    static constexpr uint8_t White = 0;
    static constexpr uint8_t Black = 1;
    
};
#endif // PIECES_H
