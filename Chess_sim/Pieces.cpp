
#include "Pieces.hpp"


Pieces::Pieces() = default;
Pieces::Pieces(const std::string& shortFen) {
    uint8_t x = 0;
    uint8_t y = 7;

    uint8_t side;

    for (auto buff : shortFen) {
        if (buff == '/') {
            x = 0;
            y = y - 1;
        }
        else if (std::isdigit(buff)) {
            x = x + buff - '0';
        }
        else {
            if (std::isupper(buff)) {
                buff = (char)std::tolower(buff);
                side = SIDE::White;
            }
            else {
                side = SIDE::Black;
            }

            switch (buff) {
                case 'p':
                    this->pieceBitboards[side][PIECE::PAWN] = BOp::set1(this->pieceBitboards[side][PIECE::PAWN], y * 8 + x);
                    break;
                case 'n':
                    this->pieceBitboards[side][PIECE::KNIGHT] = BOp::set1(this->pieceBitboards[side][PIECE::KNIGHT], y * 8 + x);
                    break;
                case 'b':
                    this->pieceBitboards[side][PIECE::BISHOP] = BOp::set1(this->pieceBitboards[side][PIECE::BISHOP], y * 8 + x);
                    break;
                case 'r':
                    this->pieceBitboards[side][PIECE::ROOK] = BOp::set1(this->pieceBitboards[side][PIECE::ROOK], y * 8 + x);
                    break;
                case 'q':
                    this->pieceBitboards[side][PIECE::QUEEN] = BOp::set1(this->pieceBitboards[side][PIECE::QUEEN], y * 8 + x);
                    break;
                case 'k':
                    this->pieceBitboards[side][PIECE::KING] = BOp::set1(this->pieceBitboards[side][PIECE::KING], y * 8 + x);
                    break;
            }

            x = x + 1;
        }
    }

    this->updateBitboards();
}
std::ostream &operator<<(std::ostream &ostream, Pieces pieces) {
    for (int8_t y = 7; y >= 0; y = y - 1) {
        for (uint8_t x = 0; x < 8; x = x + 1) {
            ostream << "|  ";

            uint8_t index = y * 8 + x;

            if (BOp::getBit(pieces.pieceBitboards[SIDE::White][PIECE::PAWN], index)) {
                ostream << "P";
            }
            else if (BOp::getBit(pieces.pieceBitboards[SIDE::White][PIECE::KNIGHT], index)) {
                ostream << "N";
            }
            else if (BOp::getBit(pieces.pieceBitboards[SIDE::White][PIECE::BISHOP], index)) {
                ostream << "B";
            }
            else if (BOp::getBit(pieces.pieceBitboards[SIDE::White][PIECE::ROOK], index)) {
                ostream << "R";
            }
            else if (BOp::getBit(pieces.pieceBitboards[SIDE::White][PIECE::QUEEN], index)) {
                ostream << "Q";
            }
            else if (BOp::getBit(pieces.pieceBitboards[SIDE::White][PIECE::KING], index)) {
                ostream << "K";
            }
            else if (BOp::getBit(pieces.pieceBitboards[SIDE::Black][PIECE::PAWN], index)) {
                ostream << "p";
            }
            else if (BOp::getBit(pieces.pieceBitboards[SIDE::Black][PIECE::KNIGHT], index)) {
                ostream << "n";
            }
            else if (BOp::getBit(pieces.pieceBitboards[SIDE::Black][PIECE::BISHOP], index)) {
                ostream << "b";
            }
            else if (BOp::getBit(pieces.pieceBitboards[SIDE::Black][PIECE::ROOK], index)) {
                ostream << "r";
            }
            else if (BOp::getBit(pieces.pieceBitboards[SIDE::Black][PIECE::QUEEN], index)) {
                ostream << "q";
            }
            else if (BOp::getBit(pieces.pieceBitboards[SIDE::Black][PIECE::KING], index)) {
                ostream << "k";
            }
            else {
                ostream << " ";
            }

            ostream << " ";
        }
        ostream << "|";
        if (y != 0) {
            ostream << "\n";
        }
    }
    ostream << "\n";

    return ostream;
}
void Pieces::updateBitboards() {
    this->sideBitboards[SIDE::White] = this->pieceBitboards[SIDE::White][PIECE::PAWN] |
                                         this->pieceBitboards[SIDE::White][PIECE::KNIGHT] |
                                         this->pieceBitboards[SIDE::White][PIECE::BISHOP] |
                                         this->pieceBitboards[SIDE::White][PIECE::ROOK] |
                                         this->pieceBitboards[SIDE::White][PIECE::QUEEN] |
                                         this->pieceBitboards[SIDE::White][PIECE::KING];

    this->sideBitboards[SIDE::Black] = this->pieceBitboards[SIDE::Black][PIECE::PAWN] |
                                         this->pieceBitboards[SIDE::Black][PIECE::KNIGHT] |
                                         this->pieceBitboards[SIDE::Black][PIECE::BISHOP] |
                                         this->pieceBitboards[SIDE::Black][PIECE::ROOK] |
                                         this->pieceBitboards[SIDE::Black][PIECE::QUEEN] |
                                         this->pieceBitboards[SIDE::Black][PIECE::KING];

    this->invSideBitboards[SIDE::White] = ~this->sideBitboards[SIDE::White];
    this->invSideBitboards[SIDE::Black] = ~this->sideBitboards[SIDE::Black];

    this->all = this->sideBitboards[SIDE::White] | this->sideBitboards[SIDE::Black];
    this->empty = ~this->all;
}
void Pieces::setPieceBitboard(uint8_t side, uint8_t piece, Bitboard bb) {
    this->pieceBitboards[side][piece] = bb;
}
std::array<std::array<Bitboard, 6>, 2> Pieces::getPieceBitboards() const {
    return this->pieceBitboards;
}
Bitboard Pieces::getPieceBitboard(uint8_t side, uint8_t piece) const {
    return this->pieceBitboards[side][piece];
}
Bitboard Pieces::getSideBitboard(uint8_t side) const {
    return this->sideBitboards[side];
}
Bitboard Pieces::getInvSideBitboard(uint8_t side) const {
    return this->invSideBitboards[side];
}
Bitboard Pieces::getAllBitboard() const {
    return this->all;
}
Bitboard Pieces::getEmptyBitboard() const {
    return this->empty;
}
uint8_t Pieces::inverse(uint8_t side) {
    return !side;
}