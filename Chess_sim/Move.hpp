
#include <cstdint>
#include <string>
#include <fstream>

#include "BitBoard.hpp"
#include "Pieces.hpp"
#include "BTrans.h"

#pragma once


class Move {
public:
    Move();
    Move(uint8_t from, uint8_t to, uint8_t attackerType, uint8_t attackerSide, uint8_t defenderType, uint8_t defenderSide, uint8_t flag = Move::FLAG::DEFAULT);

    void setFrom(uint8_t newFrom);
    void setTo(uint8_t newTo);
    void setAttackerType(uint8_t newAttackerType);
    void setAttackerSide(uint8_t newAttackerSide);
    void setDefenderType(uint8_t newDefenderType);
    void setDefenderSide(uint8_t newDefenderSide);
    void setFlag(uint8_t newFlag);
    static Move CreateCastling(bool kingside, uint8_t side);
    [[nodiscard]] uint8_t getFrom() const;
    [[nodiscard]] uint8_t getTo() const;
    [[nodiscard]] uint8_t getAttackerType() const;
    [[nodiscard]] uint8_t getAttackerSide() const;
    [[nodiscard]] uint8_t getDefenderType() const;
    [[nodiscard]] uint8_t getDefenderSide() const;
    [[nodiscard]] uint8_t getFlag() const;

    bool operator==(const Move& other) const;
    Move& operator=(const Move& other);
    friend std::ostream& operator <<(std::ostream& ostream, const Move& move);
    
    void ToFile(std::string& annotation, float moveCtr, std::string filePath) const;
    static bool isPromotion(const std::string& condition);

    static Move strToMove(const std::string& moveStr, uint8_t flag);
    enum FLAG {
        DEFAULT,

        PAWN_LONG_MOVE,
        EN_PASSANT_CAPTURE,

        WL_CASTLING,
        WS_CASTLING,
        BL_CASTLING,
        BS_CASTLING,

        PROMOTE_TO_KNIGHT,
        PROMOTE_TO_BISHOP,
        PROMOTE_TO_ROOK,
        PROMOTE_TO_QUEEN
    };
    static constexpr uint8_t NONE = 255;
private:
    uint8_t from;
    uint8_t to;
    uint8_t attackerType;
    uint8_t attackerSide;
    uint8_t defenderType;
    uint8_t defenderSide;
    uint8_t flag;

    std::string condition;
    
};