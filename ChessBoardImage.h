#ifndef ___CHESS_BOARD_IMAGE_H___
#define ___CHESS_BOARD_IMAGE_H___

#include <vector>
#include <map>
#include "Fire.h"
#include "Camera.h"

/*
Файл содержит класс шахматной доски, представляющий изображение шахматной доски.
Шахматы обозначаются символами.
P - пешка
B - слон
K - конь
C - ладья
Q - ферзь
K - король
Большая буква - белый, маленькая - черный.
//спецсимволы:
Пробел - пустое поле
# - доска
*/

//структура, изображающая модель для рисования
struct ModelImage
{
	vector<3> position;
	char model;
	char colorTexture;
};

struct ChessBoardImage
{
	std::vector<ModelImage> modelImages;

	bool isBlueLight;
	vector<3> blueLight;
	std::vector<vector<3> > yellowLights;
	std::vector<vector<3> > greenLights;
	std::vector<vector<3> > redLights;

	Fire fire;

	ChessBoardImage() : isBlueLight(false) {}
};

#endif
