#include "Init.h"

PieceTextures Init::loadPieceTextures(Theme theme)
{
	PieceTextures textures;
	std::string piecesPath;
	std::string themeSuffix;

	switch (theme) {
	case Theme::Theme1: themeSuffix = ""; break;
	case Theme::Theme2: themeSuffix = "2"; break;
	case Theme::Theme3: themeSuffix = "3"; break;
	default: themeSuffix = "1"; break;
	}

	for (const std::string& name : Init::names)
	{
		std::string whiteTexturePath = Init::whitePiecesPath + name + "W" + themeSuffix + ".png";
		std::string blackTexturePath = Init::blackPiecesPath + name + "B" + themeSuffix + ".png";

		Texture2D whiteTexture = LoadTexture(whiteTexturePath.c_str());
		if (whiteTexture.id == 0) {
			std::cout << "Failed to load white piece texture: " << whiteTexturePath << std::endl;
		}
		else {
			textures.white[name] = whiteTexture;
		}

		Texture2D blackTexture = LoadTexture(blackTexturePath.c_str());
		if (blackTexture.id == 0) {
			std::cout << "Failed to load black piece texture: " << blackTexturePath << std::endl;
		}
		else {
			textures.black[name] = blackTexture;
		}
	}
	currentTheme = theme;
	return textures;
}

// Init.cpp
void Init::loadSquareTextures()
{
	if (squaresLoaded) return;

	std::string squareWPath = squarePath + "squareW.png";
	std::string squareBPath = squarePath + "squareB.png";

	squareWhiteTex = LoadTexture(squareWPath.c_str());
	if (squareWhiteTex.id == 0)
		std::cout << "Failed to load white square texture: " << squareWPath << std::endl;

	squareBlackTex = LoadTexture(squareBPath.c_str());
	if (squareBlackTex.id == 0)
		std::cout << "Failed to load black square texture: " << squareBPath << std::endl;

	squaresLoaded = true;
}

void Init::loadAllTextures()
{
	if (texturesLoaded) return;
	loadSquareTextures();
	
	cachedThemes[Theme::Theme1] = loadPieceTextures(Theme::Theme1);
	cachedThemes[Theme::Theme2] = loadPieceTextures(Theme::Theme2);
	cachedThemes[Theme::Theme3] = loadPieceTextures(Theme::Theme3);

	currentTheme = Theme::Theme1;
	texturesLoaded = true;
}

void Init::unloadAllTextures()
{
	for (auto& [theme, textures] : cachedThemes) {
		for (auto& [_, tex] : textures.white) UnloadTexture(tex);
		for (auto& [_, tex] : textures.black) UnloadTexture(tex);
	}
	cachedThemes.clear();
}

const PieceTextures& Init::getPieceTextures(Theme theme)
{
	loadAllTextures();
	return cachedThemes.at(theme);
}

const Texture2D& Init::getPawnIcon(Theme theme, bool isWhite)
{
	const auto& tex = getPieceTextures(theme);
	return isWhite ? tex.white.at("Pawn") : tex.black.at("Pawn");
}

const Texture2D& Init::getSquareWhite()
{
	return squareWhiteTex;
}

const Texture2D& Init::getSquareBlack() 
{
	return squareBlackTex;
}