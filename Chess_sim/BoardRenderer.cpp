#include "BoardRenderer.h"
// для debug (опционально)

BoardRenderer::BoardRenderer(int squareSize)
{
    this->squareSize = squareSize;
	this->OffsetXForRows = squareSize - 15;
	this->OffsetYForRows = 5;
	this->OffsetXForFiles = 5;
	this->OffsetYForFiles = squareSize - 20;
	allPieceTextures[Theme::Theme1] = &Init::getPieceTextures(Theme::Theme1);
	allPieceTextures[Theme::Theme2] = &Init::getPieceTextures(Theme::Theme2);
	allPieceTextures[Theme::Theme3] = &Init::getPieceTextures(Theme::Theme3);
	this->squareWhiteTex = &Init::getSquareWhite();
	this->squareBlackTex = &Init::getSquareBlack();
	theme = Theme::Theme1;
}



void BoardRenderer::drawBoard(const Position& position, uint8_t selectedSquare, 
								bool promotionOption, uint8_t promotionSquare,	
								uint8_t promotionSide) const
{

    for (int rank = 0; rank < boardSize; ++rank)
    {
        for (int file = 0; file < boardSize; ++file)
        {
            int x = file * squareSize;
            int y = rank * squareSize;

            bool isWhiteSquare = (rank + file) % 2 == 0;
            const Texture2D* squareTex = isWhiteSquare ? squareWhiteTex : squareBlackTex;
            Rectangle source = { 0, 0, (float)squareTex->width, (float)squareTex->height };
            Rectangle dest = { (float)x, (float)y, (float)squareSize, (float)squareSize };
            Vector2 origin = { 0, 0 };

            DrawTexturePro(*squareTex, source, dest, origin, 0.0f, WHITE);
        }

    }

    drawPieces(position);
	drawMask(selectedSquare, position);
    drawCoordinates();
    if (promotionOption) drawPromotionOptions(promotionSquare, promotionSide);
}

void BoardRenderer::drawPieces(const Position& position) const
{
	std::string fen = position.toFEN();
	int row = 0, col = 0;
	const PieceTextures* textures = allPieceTextures.at(theme);
	float pieceSize = squareSize * 1.5f;

	for (char c : fen)
	{
		if (c == '/')
		{
			row++;
			col = 0;
		}
		else if (isdigit(c))
		{
			col += c - '0';
		}
		else
		{
			bool isWhite = isupper(c);
			std::string pieceType;

			switch (tolower(c))
			{
			case 'k': pieceType = "King"; break;
			case 'q': pieceType = "Queen"; break;
			case 'r': pieceType = "Rook"; break;
			case 'b': pieceType = "Bishop"; break;
			case 'n': pieceType = "Knight"; break;
			case 'p': pieceType = "Pawn"; break;
			}

			std::map<std::string, Texture2D> texMap = isWhite ? textures->white : textures->black;
			float texAspectRatio = static_cast<float>(texMap[pieceType].width) / static_cast<float>(texMap[pieceType].height);

			float newWidth = pieceSize;
			float newHeight = pieceSize / texAspectRatio;

			if (newHeight > squareSize)
			{
				newHeight = squareSize;
				newWidth = squareSize * texAspectRatio;
			}
			Vector2 offset = centerPiece(pieceSize, texMap[pieceType].width, texMap[pieceType].height);
			float xOffset = offset.x;
			float yOffset = offset.y;

			Rectangle source = { 0, 0, texMap[pieceType].width, texMap[pieceType].height };
			Rectangle dest = { col * squareSize + xOffset, row * squareSize + yOffset, newWidth, newHeight };

			Vector2 origin = { newWidth / 2.0f, newHeight / 2.0f };
			float rotation = 0.0f;
			Color tint = WHITE;

			DrawTexturePro(texMap[pieceType], source, dest, origin, rotation, tint);
			col++;

		}
	}
}

void BoardRenderer::drawMask(uint8_t selectedSquare, const Position& position) const
{
	if (selectedSquare == 255) return;
    //std::cout << "selected square: " << selectedSquare << std::endl;
    MoveList moves = LegalMoveGen::generate(position, position.getPieceSideAt(selectedSquare), false);
    std::vector<std::string> squares;
    for (int i = 0; i < moves.getSize(); ++i)
    {
        int from = moves[i].getFrom();
        if (from != selectedSquare || position.getPieceSideAt(from) != position.getSideToMove())
        {
            continue;
        }
        int to = moves[i].getTo();
        //std::cout << "to " << to << std::endl;
        int col = to % 8;
        int row = 7 - (to / 8);
        //std::cout << "possible move: " << indexToSquare(to) << std::endl
        DrawRectangle(col * squareSize, row * squareSize, squareSize, squareSize, maskColor);
    }
}

void BoardRenderer::drawPromotionOptions(uint8_t promotionSquare, uint8_t promotionSide) const
{
	float pieceSize = squareSize * 1.5f;
	int col = Btrans::indexToRowCol(promotionSquare).second;
	int row = Btrans::indexToRowCol(promotionSquare).first;
	bool isWhiteSquare = (row + col) % 2 == 0;
	const Texture2D* squareTex = isWhiteSquare ? squareWhiteTex : squareBlackTex;
	const PieceTextures* currentTextures = allPieceTextures.at(theme);
	std::map<std::string, Texture2D> texMap = (promotionSide == SIDE::White) ? currentTextures->white : currentTextures->black;

	for (int i = 0; i < 4; ++i)
	{
		const std::string& pieceName = promotionNames[i];
		float texAspectRatio = static_cast<float>(texMap[pieceName].width) / static_cast<float>(texMap[pieceName].height);

		float newWidth = pieceSize;
		float newHeight = pieceSize / texAspectRatio;

		if (newHeight > squareSize)
		{
			newHeight = squareSize;
			newWidth = squareSize * texAspectRatio;
		}
		Vector2 offset = centerPiece(pieceSize, texMap[pieceName].width, texMap[pieceName].height);
		float xOffset = offset.x;
		float yOffset = offset.y;

		Rectangle source = { 0, 0, texMap[pieceName].width, texMap[pieceName].height };
		Rectangle dest = { col * squareSize + xOffset, row * squareSize + yOffset, newWidth, newHeight };

		Vector2 origin = { newWidth / 2.0f, newHeight / 2.0f };
		float rotation = 0.0f;
		Color tint = WHITE;
		DrawRectangle(col * squareSize, row * squareSize, squareSize, squareSize, promoteColor);
		DrawTexturePro(*squareTex, source, dest, origin, 0.0f, WHITE);
		DrawTexturePro(texMap[pieceName], source, dest, origin, rotation, tint);
		if (row >= 0 && row <= 3) row++;
		else if (row <= 7 && row >= 4) row--;
	}
}

void BoardRenderer::drawCoordinates() const
{
	const char* files[8] = { "a", "b", "c", "d", "e", "f", "g", "h" };
	Color textColor = BLACK;

	for (int rank = 0; rank < boardSize; ++rank)
	{
		std::string rowIndexStr = std::to_string(8 - rank);
		if (boardSize - 1 >= 0)
		{
			DrawText(rowIndexStr.c_str(), (boardSize - 1) * squareSize + OffsetXForRows,
				rank * squareSize + OffsetYForRows, 20, textColor);
		}
	}

	for (int file = 0; file < boardSize; ++file)
	{
		std::string fileIndex = files[file];
		DrawText(fileIndex.c_str(), file * squareSize + OffsetXForFiles,
			(boardSize - 1) * squareSize + OffsetYForFiles, 20, textColor);
	}
}


Vector2 BoardRenderer::centerPiece(float targetSize, float texWidth, float texHeight) const
{
	const float xOffset = squareSize / 2;
	const float yOffset = squareSize / 2 - constOffset;

	return { xOffset, yOffset };
}

void BoardRenderer::setTheme(Theme newTheme)
{
	theme = newTheme;
}