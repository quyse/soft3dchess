#ifndef ___FIGURES_FIELD_H___
#define ___FIGURES_FIELD_H___

/*
���� �������� ����� ���� �����.
*/

#include <vector>

//���������, �������������� ������
struct Figure
{
	enum
	{
		typeNone,//�����
		typePawn,//�����
		typeBishop,//����
		typeKnight,//����
		typeCastle,//�����
		typeQueen,//�����
		typeKing//������
	};
	int type;

	//�������
	int party;

	Figure() : type(typeNone), party(-1) {}
};

//��������� ����������� ����
struct ValidMove
{
	//������
	int fromI, fromJ;
	//����
	int toI, toJ;
	//�� �������
	bool taking;

	ValidMove(int fromI, int fromJ, int toI, int toJ, bool taking)
		: fromI(fromI), fromJ(fromJ), toI(toI), toJ(toJ), taking(taking)
	{
	}
};

//�������
struct Party
{
	//�������� ��������� ����� �������
	bool canCastlingShort;
	//������� ��������� ����� �������
	bool canCastlingLong;

	Party() : canCastlingShort(true), canCastlingLong(true) {}
};

struct FiguresField
{
	Party parties[2];
	Figure field[8][8];
	int enPassant;

	FiguresField() : enPassant(-1) {}

	inline Figure* operator[](int i)
	{
		return field[i];
	}

	inline const Figure* operator[](int i) const
	{
		return field[i];
	}

	void AddPossibleMove(std::vector<ValidMove>& possibleMoves, int fromI, int fromJ, int toI, int toJ) const;
	bool AddPossibleMoveWithCheck(std::vector<ValidMove>& possibleMoves, int fromI, int fromJ, int toI, int toJ) const;
	void AddRayPossibleMoves(std::vector<ValidMove>& possibleMoves, int startI, int startJ, int dirI, int dirJ) const;

	//�������� ��������� ���� ������ (�� ���� ��� ����� �����)
	std::vector<ValidMove> GetPossibleMoves(int i, int j, bool onlyTaking = false) const;
	//�������� ���������� ���� ������ (�� ���� � ������ �����)
	std::vector<ValidMove> GetValidMoves(int i, int j) const;

	//������� ���
	void MakeMove(int fromI, int fromJ, int toI, int toJ);

	//��� ��� �������?
	bool IsCheck(int party) const;
	//������ ������ ��� �������?
	bool IsCanMove(int party) const;
};

#endif
