#ifndef ___CHESS_GAME_H___
#define ___CHESS_GAME_H___

/*
���� �������� ����� ��������� ����. �� �������� ��� ����������� ������ � ��������� ����.

����������:
������ ������: 0 - �����, 1 - ������
������� ���������: ��� J - ����� ������� ���������� ���������� ����� �������, ��� I - �� ����� � ������ (����� �����)
�� ���� � ����� ���������� I ������, ��� � ������.
����� ������, J - ����� �� A �� H, I - ����� �� 1 �� 8. (�� ���������� ��� �� 0 �� 7)

��������� ������������ ����� �����.
*/

#include <vector>
#include <string>
#include "ChessResources.h"
#include "ChessBoardImage.h"
#include "FiguresField.h"

//�������� ���������� ����
struct GameFinish
{
	enum
	{
		//��� �� �����������
		Continued,
		//��� ���
		CheckMateForMe,
		//��� ��
		CheckMateForHim,
		//���
		StaleMate,
		//�����
		Draw
	};
};

class ChessPlayer;
class ChessGame
{
private:
	//� ���� ������� ���
	int currentMove;
	//"����" �������
	int ourParty;
	//����� ���� � ����� ����� - ����� ���� ������� �������� � ������ �����
	int selfMode;

public:
	FiguresField figures;
private:
	const ChessResources& chessResources;

	//���������� ������ ���������
	std::pair<int, int> blueLight;
	//����������, ������ � ���� ���� ������������� ������
	std::pair<int, int> animatedFrom, animatedTo;
	//����� ������ �������� (����)
	int animatedTime;

	//����������� ���� ���
	ChessBoardImage boardImage;

	//��� ��� �������?
	bool IsCheckMate(int party) const;
	//��� ��� �������?
	bool IsStaleMate(int party) const;

	//�������� ������ ������ ��� ���� ������
	char GetFigureModel(int figureType) const;

	//����������� ����� ��� ���� �� �������
	void PrepareFire(int toI, int toJ, int figureType);

public:
	ChessGame(const ChessResources& chessResources);

	//������� ������
	void SetPlayer(int party, ChessPlayer* player);

	//���������� ��������� ��������� ����� (� ���� �������)
	void LetsBeginBattle(int ourParty, bool selfMode = false);

	int GetCurrentMove() const;
	int GetOurParty() const;

	//�������� ������
	const Figure& GetFigure(int i, int j) const;

	//�������� ��������� ������
	vector<3> GetCellPosition(int i, int j) const;
	//������� ��������� ������ �� �������� �����������
	std::pair<int, int> PickCell(const vector<2>& coord, const Camera& camera) const;

	//�������� ����������� �����
	void UpdateBoardImage();
	//�������� ����������� �����
	const ChessBoardImage& GetBoardImage() const;

	//������� ��� (���������� true, ���� ��� ������)
	bool MakeMove(int party, int fromI, int fromJ, int toI, int toJ, int promotion = Figure::typeNone);

	//������� ������, � ������� ������ ������������ ����
	void SetCellMovesFromLight(int i, int j);

	//�������� ��������� ����
	int GetFinishStatus() const;
};

#endif
