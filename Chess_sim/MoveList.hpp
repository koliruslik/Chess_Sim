
#include <array>
#include <sstream>
#include "Move.hpp"

#pragma once


class MoveList {
public:
    MoveList();

    Move &operator[](uint8_t index);
    Move operator[](uint8_t index) const;
    bool operator==(const MoveList& other) const;

    void push(Move move);
    void clear();
    [[nodiscard]] uint8_t getSize() const;

    bool hasMoves() const;

    void printMoves() const;
    void readMovesFromFile(std::string filePath);
    void saveToFile(std::string& annotation, float moveCtr, std::string filePath);
private:
    std::array<Move, 220> moves{};
    uint8_t size;
};