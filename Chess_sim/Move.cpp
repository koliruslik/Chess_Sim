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

bool Move::operator==(const Move& other) const {
	return from == other.from &&
		to == other.to &&
		attackerType == other.attackerType &&
		attackerSide == other.attackerSide &&
		defenderType == other.defenderType &&
		defenderSide == other.defenderSide &&
		flag == other.flag;
}

bool Move::isPromotion(const std::string& condition)
{
    static const std::vector<std::string> promotionOptions = { "=Q", "=R", "=B", "=N" };
    return std::find(promotionOptions.begin(), promotionOptions.end(), condition) != promotionOptions.end();
}

void Move::Print(std::string annotation, float moveCtr) const
{
    int fullMoveNumber = moveCtr + 0.5;
    bool isWhiteMove = !(moveCtr == static_cast<int>(moveCtr));
    if (isWhiteMove)
        std::cout << "\n" << fullMoveNumber << ". " << *this;
    else
        std::cout << "\t" << *this;

    std::cout << annotation;
}

void Move::ToFile( std::string& annotation, float moveCtr, std::string filePath) const
{
    
    std::ofstream file(filePath, std::ios::app);
    if (!file.is_open())
    {
        std::cerr << "Unable to open file\n";
        return;
    }

    int fullMoveNumber = moveCtr + 0.5;
    bool isWhiteMove = !(moveCtr == static_cast<int>(moveCtr));
    if (isWhiteMove)
        file << "\n" << fullMoveNumber << ". " << *this;
    else
        file << "\t" << *this;

    file << annotation;
    file.close();
}

std::ostream& operator <<(std::ostream& ostream, const Move& move) {
	std::string from = Btrans::indexToSquare(move.getFrom());
	std::string to = Btrans::indexToSquare(move.getTo());
	char attackerType = Btrans::pieceToString(move.getAttackerType());
	char defenderType = Btrans::pieceToString(move.getDefenderType());
    uint8_t attackerSide = move.getAttackerSide();
	uint8_t defenderSide = move.getDefenderSide();
	uint8_t flag = move.getFlag();

    if (attackerSide == SIDE::White) attackerType = toupper(attackerType);
    if (defenderSide == SIDE::White) defenderType = toupper(defenderType);

    if (flag == Move::FLAG::WL_CASTLING || flag == Move::FLAG::BL_CASTLING)
        ostream << "0-0-0";
    else if (flag == Move::FLAG::WS_CASTLING || flag == Move::FLAG::BS_CASTLING)
        ostream << "0-0";
    else if (defenderType != '\0' || flag == Move::FLAG::EN_PASSANT_CAPTURE)
        ostream << attackerType << from << "x" << to;
    else
        ostream << attackerType << from << to;
    
    return ostream;
}

