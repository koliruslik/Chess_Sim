

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

