
#include <vector>
#include "ZobristHash.hpp"


#pragma once


class RepetitionHistory {
public:
    RepetitionHistory();

    RepetitionHistory& operator=(const RepetitionHistory& other);

    void addPosition(ZobristHash hash);
    void clear();
    [[nodiscard]] uint8_t getRepetitionNumber(ZobristHash hash) const;
private:
    std::vector<ZobristHash> hashes;
};