#ifndef ___CHESS_RESOURCES_H___
#define ___CHESS_RESOURCES_H___

#include "Model.h"
#include "Texture.h"
#include "VertexFormats.h"
#include <map>

class ChessResources
{
private:
	std::map<char, Model<NormalVertex, int>*> models;
	std::map<char, Texture*> textures;

	void LoadModel(char symbol, const wchar_t* fileName);
	void LoadTexture(char symbol, const wchar_t* fileName);

public:
	ChessResources();

	Model<NormalVertex, int>* GetModel(char c) const;
	Texture* GetTexture(char c) const;
};

#endif
