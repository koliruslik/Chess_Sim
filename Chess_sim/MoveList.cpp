
#include "MoveList.hpp"


MoveList::MoveList() {
    this->size = 0;
}
Move &MoveList::operator[](uint8_t index) 
{
    return this->moves[index];
}
Move MoveList::operator[](uint8_t index) const 
{
    return this->moves[index];
}

bool MoveList::operator==(const MoveList& other) const
{
    if (size != other.size) return false;
    for (uint8_t i = 0; i < size; ++i)
    {
        if (!(moves[i] == other.moves[i])) return false;
    }
    return true;
}

void MoveList::push(Move move) {

    this->moves[this->size] = move;
    this->size = this->size + 1;
}

void MoveList::clear() {
    size = 0; 
    moves.fill(Move()); 
}

uint8_t MoveList::getSize() const
{
    return this->size;
}

bool MoveList::hasMoves() const 
{
	return this->size > 0;
}

void MoveList::printMoves() const
{
    float moveCtr = 0;
    if (hasMoves())
    {
        std::cout << "\n No moves available.\n";
        return;
    }
    for (uint8_t i = 0; i < size; ++i)
    {
        std::cout << moves[i];
        moveCtr += 0.5;
    }
}

void MoveList::readMovesFromFile(std::string filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Unable to open file for reading: " << filePath << '\n';
        return;
    }

    size = 0;

    std::string line;
    while (std::getline(file, line))
    {
        std::cout << "Reading line: " << line << '\n';

        std::istringstream iss(line);

        iss.ignore(iss.str().find_first_of(' ') + 1);

        std::string moveStr;
        int flag;

        if (iss >> moveStr) {
            std::cout << "Move: " << moveStr << '\n';
        }
        else {
            std::cerr << "Failed to read moveStr\n";
            continue;
        }

        if (iss >> flag) {
            std::cout << "Flag: " << flag << '\n';
        }
        else {
            std::cerr << "Failed to read flag\n";
            continue;
        }

        Move move = Move::strToMove(moveStr, flag);
        push(move);
    }

    file.close();
}
void MoveList::saveToFile(std::string& annotation, float moveCtr, std::string filePath)
{
    for (size_t i = 0; i < size; ++i)
    {
        moves[i].ToFile(annotation, moveCtr, filePath);
    }
}

