
#include <array>
#include "Move.hpp"


#pragma once


class MoveList {
public:
    MoveList();

    Move &operator[](uint8_t index);
    Move operator[](uint8_t index) const;
    bool operator==(const MoveList& other) const;

    void push(Move move);
    [[nodiscard]] uint8_t getSize() const;

    bool hasMoves() const;
   
private:
    std::array<Move, 220> moves{};
    uint8_t size;
};