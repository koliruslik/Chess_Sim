

#include <cctype>
#include "Bitboard.hpp"
#include "GameEnums.h"

#pragma once




class Pieces {
public:
    Pieces();
    Pieces(const std::string& shortFen);

    friend std::ostream& operator <<(std::ostream& ostream, Pieces pieces);
    bool operator==(const Pieces& other) const
    {
        return pieceBitboards == other.pieceBitboards &&
            sideBitboards == other.sideBitboards &&
            invSideBitboards == other.invSideBitboards &&
            all == other.all &&
            empty == other.empty;
    }

    Pieces& operator=(const Pieces& other);

    void updateBitboards();

    void setPieceBitboard(uint8_t side, uint8_t piece, Bitboard bb);

    [[nodiscard]] std::array<std::array<Bitboard, 6>, 2> getPieceBitboards() const;
    [[nodiscard]] Bitboard getPieceBitboard(uint8_t side, uint8_t piece) const;
    [[nodiscard]] Bitboard getSideBitboard(uint8_t side) const;
    [[nodiscard]] Bitboard getInvSideBitboard(uint8_t side) const;
    [[nodiscard]] Bitboard getAllBitboard() const;
    [[nodiscard]] Bitboard getEmptyBitboard() const;

    static uint8_t inverse(uint8_t side);
private:
    std::array<std::array<Bitboard, 6>, 2> pieceBitboards{};
    std::array<Bitboard, 2> sideBitboards{};
    std::array<Bitboard, 2> invSideBitboards{};
    Bitboard all;
    Bitboard empty;
};