#ifndef ___CHESS_BOARD_IMAGE_H___
#define ___CHESS_BOARD_IMAGE_H___

#include <vector>
#include <map>
#include "Fire.h"
#include "Camera.h"

/*
���� �������� ����� ��������� �����, �������������� ����������� ��������� �����.
������� ������������ ���������.
P - �����
B - ����
K - ����
C - �����
Q - �����
K - ������
������� ����� - �����, ��������� - ������.
//�����������:
������ - ������ ����
# - �����
*/

//���������, ������������ ������ ��� ���������
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
