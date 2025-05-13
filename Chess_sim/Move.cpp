#include "Move.hpp"

Move::Move() = default;
Move::Move(uint8_t from, uint8_t to, uint8_t attackerType, uint8_t attackerSide, uint8_t defenderType, uint8_t defenderSide, uint8_t flag) {
    this->from = from;
    this->to = to;
    this->attackerType = attackerType;
    this->attackerSide = attackerSide;
    this->defenderType = defenderType;
    this->defenderSide = defenderSide;
    this->flag = flag;
}
void Move::setFrom(uint8_t newFrom) {
    from = newFrom;
}
void Move::setTo(uint8_t newTo) {
    to = newTo;
}
void Move::setAttackerType(uint8_t newAttackerType) {
    attackerType = newAttackerType;
}
void Move::setAttackerSide(uint8_t newAttackerSide) {
    attackerSide = newAttackerSide;
}
void Move::setDefenderType(uint8_t newDefenderType) {
    defenderType = newDefenderType;
}
void Move::setDefenderSide(uint8_t newDefenderSide) {
    defenderSide = newDefenderSide;
}
void Move::setFlag(uint8_t newFlag) {
    flag = newFlag;
}
uint8_t Move::getFrom() const {
    return from;
}
uint8_t Move::getTo() const {
    return to;
}
uint8_t Move::getAttackerType() const {
    return attackerType;
}
uint8_t Move::getAttackerSide() const {
    return attackerSide;
}
uint8_t Move::getDefenderType() const {
    return defenderType;
}
uint8_t Move::getDefenderSide() const {
    return defenderSide;
}
uint8_t Move::getFlag() const {
    return flag;
}

Move Move::CreateCastling(bool kingside, uint8_t side)
{
    uint8_t from, to, flag;

    if (side == SIDE::White)
    {
        from = 4; // e1
        to = kingside ? 6 : 2; // g1 або c1
        flag = kingside ? Move::FLAG::WS_CASTLING : Move::FLAG::WL_CASTLING;
    }
    else
    {
        from = 60; // e8
        to = kingside ? 62 : 58; // g8 або c8
        flag = kingside ? Move::FLAG::BS_CASTLING : Move::FLAG::BL_CASTLING;
    }

    return Move(from, to, PIECE::KING, side, 0, 0, flag);
}
bool Move::isPromotion(const std::string& condition)
{
    static const std::vector<std::string> promotionOptions = { "=Q", "=R", "=B", "=N" };
    return std::find(promotionOptions.begin(), promotionOptions.end(), condition) != promotionOptions.end();
}

void Move::ToFile( std::string& annotation, float moveCtr, std::string filePath) const
{
    
    std::ofstream file(filePath, std::ios::app | std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Unable to open file\n";
        return;
    }

    int fullMoveNumber = moveCtr + 0.5;
    bool isWhiteMove = !(moveCtr == static_cast<int>(moveCtr));
    file << fullMoveNumber << ". " << *this << annotation << " " << (int)getFlag() << "\n";
    file.close();
}

Move Move::strToMove(const std::string& moveStr, uint8_t flag)
{
    uint8_t attackerSide = 0;
    uint8_t attackerType = 0;
    uint8_t defenderSide = 0;
    uint8_t defenderType = Move::NONE;
    uint8_t from, to;

    attackerSide = (flag == Move::FLAG::WS_CASTLING || flag == Move::FLAG::WL_CASTLING) ? SIDE::White :
        (flag == Move::FLAG::BS_CASTLING || flag == Move::FLAG::BL_CASTLING) ? SIDE::Black : 0;

    if (moveStr == "0-0")
    {
        return Move::CreateCastling(true, attackerSide);
    }
    if (moveStr == "0-0-0")
    {
        return Move::CreateCastling(false, attackerSide);
    }

    std::string part = moveStr;
    bool isCapture = moveStr.find('x') != std::string::npos;
    if (isCapture) {
        part.erase(moveStr.find('x'), 1);

       
        defenderType = Btrans::charToPiece(tolower(part[3]));
    }
    if (flag == 2) {
        defenderType = PIECE::PAWN;  
    }

    char pieceChar = part[0];
    attackerType = Btrans::charToPiece(tolower(pieceChar));

    attackerSide = isupper(pieceChar) ? SIDE::White : SIDE::Black;

    std::string fromSquare = part.substr(1, 2);
    std::string toSquare = (isCapture) ? part.substr(4, 3) : part.substr(3, 2);

    from = Btrans::squareToIndex(fromSquare);
    to = Btrans::squareToIndex(toSquare);

    defenderSide = (attackerSide == SIDE::White) ? SIDE::Black : SIDE::White;

    return Move(from, to, attackerType, attackerSide, defenderType, defenderSide, flag);
}
std::ostream& operator<<(std::ostream& ostream, const Move& move) {
    std::string from = Btrans::indexToSquare(move.getFrom());
    std::string to = Btrans::indexToSquare(move.getTo());
    char attackerType = Btrans::pieceToChar(move.getAttackerType());
    char defenderType = Btrans::pieceToChar(move.getDefenderType());
    uint8_t attackerSide = move.getAttackerSide();
    uint8_t defenderSide = move.getDefenderSide();
    uint8_t flag = move.getFlag();


    if (attackerSide == SIDE::White) {
        attackerType = toupper(attackerType);
    }
    else if (attackerSide == SIDE::Black) {
        attackerType = tolower(attackerType);
    }

    if (defenderSide == SIDE::White && defenderType != '\0') {
        defenderType = toupper(defenderType);
    }
    else if (defenderSide == SIDE::Black && defenderType != '\0') {
        defenderType = tolower(defenderType);
    }


    if (flag == Move::FLAG::WL_CASTLING || flag == Move::FLAG::BL_CASTLING)
        ostream << "0-0-0";
    else if (flag == Move::FLAG::WS_CASTLING || flag == Move::FLAG::BS_CASTLING)
        ostream << "0-0";
    else if (defenderType != '\0' || flag == Move::FLAG::EN_PASSANT_CAPTURE)
        ostream << attackerType << from << "x" << defenderType << to;
    else
        ostream << attackerType << from << to;

    return ostream;
}

bool Move::operator==(const Move& other) const {
    return from == other.from &&
        to == other.to &&
        attackerType == other.attackerType &&
        attackerSide == other.attackerSide &&
        defenderType == other.defenderType &&
        defenderSide == other.defenderSide &&
        flag == other.flag;
}

Move& Move::operator=(const Move& other)
{
    if (this != &other)
    {
        from = other.from;
        to = other.to;
        attackerType = other.attackerType;
        attackerSide = other.attackerSide;
        defenderType = other.defenderType;
        defenderSide = other.defenderSide;
        flag = other.flag;
    }
    return *this;
}