#include "BoardRenderer.h"


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

std::string getPieceName(uint8_t type) {
	switch (type) {
	case PIECE::PAWN: return "Pawn";
	case PIECE::KNIGHT: return "Knight";
	case PIECE::BISHOP: return "Bishop";
	case PIECE::ROOK: return "Rook";
	case PIECE::QUEEN: return "Queen";
	case PIECE::KING: return "King";
	default: return "";
	}
}

void BoardRenderer::drawBoard(const Position& position, uint8_t selectedSquare, 
								bool promotionOption, uint8_t promotionSquare,	
								uint8_t promotionSide) 
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
	if (highlight) highLightSquare();
    drawPieces(position);
	drawMask(selectedSquare, position);
    drawCoordinates();
    if (promotionOption) drawPromotionOptions(promotionSquare, promotionSide);
}

void BoardRenderer::drawBoard() const
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

void BoardRenderer::drawBoardBlurredAndDarkened() const
{
	

	static RenderTexture2D rt = LoadRenderTexture(squareSize * boardSize, squareSize * boardSize);

	BeginTextureMode(rt);
	ClearBackground(BLANK);  
	drawBoard();             
	EndTextureMode();

	

	const int blurPasses = 3;
	const float blurAlpha = 0.3f;

	
	for (int x = -blurPasses; x <= blurPasses; x++) {
		for (int y = -blurPasses; y <= blurPasses; y++) {
			DrawTextureRec(rt.texture,
				{ 0, 0, (float)rt.texture.width, -(float)rt.texture.height },
				{ (float)x, (float)y },
				Color{ 255, 255, 255, static_cast<unsigned char>(255 * blurAlpha) });
		}
	}

	drawBoard();

	DrawRectangle(0, 0, rt.texture.width, rt.texture.height, Color{ 0, 0, 0, 100 });
}

void BoardRenderer::drawWarning() const {
	if (!showWarning || warningLines.empty())
		return;

	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();
	int fontSize = static_cast<int>(warningFontSize);
	int lineHeight = fontSize + 5;

	int totalHeight = static_cast<int>(warningLines.size()) * lineHeight;
	float posY = (screenHeight - totalHeight) / 2.0f;

	for (size_t i = 0; i < warningLines.size(); ++i) {
		int lineWidth = MeasureText(warningLines[i].c_str(), fontSize);
		float posX = (screenWidth - lineWidth) / 2.0f;
		DrawText(warningLines[i].c_str(), static_cast<int>(posX), static_cast<int>(posY + i * lineHeight), fontSize, RED);
	}
}

void BoardRenderer::displayWarning(const std::string& text, float durationSeconds) {
	int screenWidth = GetScreenWidth();
	int fontSize = static_cast<int>(warningFontSize);

	warningLines = wrapTextToLines(text, static_cast<int>(screenWidth * 0.9f), fontSize);
	warningDuration = durationSeconds;
	warningStart = std::chrono::steady_clock::now();
	showWarning = true;

	std::thread([this]() {
		std::this_thread::sleep_for(std::chrono::duration<float>(warningDuration));
		showWarning = false;
		}).detach();
}

std::vector<std::string> BoardRenderer::wrapTextToLines(const std::string& text, int maxWidth, int fontSize) const {
	std::istringstream iss(text);
	std::vector<std::string> words;
	std::string word;
	while (iss >> word) {
		words.push_back(word);
	}

	std::vector<std::string> lines;
	std::string currentLine;

	for (size_t i = 0; i < words.size(); ++i) {
		std::string testLine = currentLine.empty() ? words[i] : currentLine + " " + words[i];
		int testWidth = MeasureText(testLine.c_str(), fontSize);

		if (testWidth > maxWidth) {
			if (!currentLine.empty()) {
				lines.push_back(currentLine);
			}
			currentLine = words[i];
		}
		else {
			currentLine = testLine;
		}
	}

	if (!currentLine.empty()) {
		lines.push_back(currentLine);
	}

	return lines;
}


void BoardRenderer::drawInfo(int x, int y, int fontSize, std::initializer_list<std::string> lines)
{
	const int lineSpacing = fontSize + 5;

	int currentY = y;
	for (const auto& line : lines)
	{
		DrawText(line.c_str(), x, currentY, fontSize, BLACK);
		currentY += lineSpacing;
	}
}

void BoardRenderer::drawPieceAtCursor(uint8_t type, uint8_t side,float scale)
{
	if (type == PIECE::NONE) return;

	std::string name = getPieceName(type);
	if (name.empty()) return;

	Vector2 mousePos = GetMousePosition();
	const PieceTextures* textures = allPieceTextures.at(theme);

	Texture2D texture = (side == SIDE::White) ? textures->white.at(name) : textures->black.at(name);
	float offsetX = texture.width / 2.0f*scale;
	float offsetY = texture.height / 2.0f * scale;
	Color aColor = { 255,255,255,128 };
	DrawTextureEx(texture, { mousePos.x - offsetX, mousePos.y - offsetY },0.0f, scale, aColor);
}

void BoardRenderer::drawPieceAtSquare(uint8_t type, uint8_t side, uint8_t square)
{
	if (type == PIECE::NONE) return;

	std::string name = getPieceName(type);
	if (name.empty()) return;

	const PieceTextures* textures = allPieceTextures[theme];
	Texture2D texture = (side == SIDE::White) ? textures->white.at(name) : textures->black.at(name);

	int row = square / boardSize;
	int col = square % boardSize;

	int x = col * squareSize;
	int y = (boardSize - 1 - row) * squareSize;

	DrawTexture(texture, x, y, WHITE);
}

void BoardRenderer::highLightSquare()
{
	
	for (auto square : squares)
	{
		std::pair<int, int> rowCol = Btrans::indexToRowCol(square);
		int rank = rowCol.first;
		int file = rowCol.second;
		int x = file * squareSize;
		int y = rank * squareSize;

		Rectangle rect = { (float)x, (float)y, (float)squareSize, (float)squareSize };
		DrawRectangleRec(rect, highlightColor);
	}
}

void BoardRenderer::drawTimer(const std::shared_ptr<Timer>& BlackTimer, const std::shared_ptr<Timer>& WhiteTimer)
{
	int fontSize = 24;
	int padding = 10;

	std::string blackTime = BlackTimer->GetRemainingTime();
	std::string whiteTime = WhiteTimer->GetRemainingTime();

	int boardWidth = boardSize * squareSize;
	int blackTextWidth = MeasureText(blackTime.c_str(), fontSize);
	DrawText(blackTime.c_str(), boardWidth - blackTextWidth - padding, padding + fontSize + 2, fontSize, BLACK);

	int whiteTextWidth = MeasureText(whiteTime.c_str(), fontSize);
	int yWhite = boardSize * squareSize - 2 * (fontSize + padding);
	DrawText(whiteTime.c_str(), boardWidth - whiteTextWidth - padding, yWhite + fontSize + 2, fontSize, BLACK);
}