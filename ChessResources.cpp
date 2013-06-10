#include "ChessResources.h"
#include "ModelLoader.h"

ChessResources::ChessResources()
{
	LoadModel('P', L"pawn.obj");
	LoadModel('B', L"bishop.obj");
	LoadModel('N', L"knight.obj");
	LoadModel('C', L"castle.obj");
	LoadModel('Q', L"queen.obj");
	LoadModel('K', L"king.obj");
	LoadModel('w', L"whitecells.obj");
	LoadModel('b', L"blackcells.obj");
	LoadModel('#', L"board.obj");
	LoadModel('!', L"beam.obj");
	LoadModel('%', L"circle.obj");
	LoadTexture('W', L"white.bmp");
	LoadTexture('B', L"black.bmp");
	LoadTexture('R', L"red.bmp");
}

void ChessResources::LoadModel(char symbol, const wchar_t *fileName)
{
	models[symbol] = ModelLoader().LoadNormalModel(fileName);
}

void ChessResources::LoadTexture(char symbol, const wchar_t* fileName)
{
	textures[symbol] = Texture::LoadFromFile(fileName);
}

Model<NormalVertex, int>* ChessResources::GetModel(char c) const
{
	return models.find(c)->second;
}

Texture* ChessResources::GetTexture(char c) const
{
	return textures.find(c)->second;
}
