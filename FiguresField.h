#ifndef ___FIGURES_FIELD_H___
#define ___FIGURES_FIELD_H___

/*
Файл содержит класс поля фигур.
*/

#include <vector>

//структура, олицетворяющая фигуру
struct Figure
{
	enum
	{
		typeNone,//пусто
		typePawn,//пешка
		typeBishop,//слон
		typeKnight,//конь
		typeCastle,//ладья
		typeQueen,//ферзь
		typeKing//король
	};
	int type;

	//сторона
	int party;

	Figure() : type(typeNone), party(-1) {}
};

//структура допустимого хода
struct ValidMove
{
	//откуда
	int fromI, fromJ;
	//куда
	int toI, toJ;
	//со взятием
	bool taking;

	ValidMove(int fromI, int fromJ, int toI, int toJ, bool taking)
		: fromI(fromI), fromJ(fromJ), toI(toI), toJ(toJ), taking(taking)
	{
	}
};

//сторона
struct Party
{
	//короткую рокировку можно сделать
	bool canCastlingShort;
	//длинную рокировку можно сделать
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

	//получить возможные ходы фигуры (то есть без учета шахов)
	std::vector<ValidMove> GetPossibleMoves(int i, int j, bool onlyTaking = false) const;
	//получить допустимые ходы фигуры (то есть с учетом шахов)
	std::vector<ValidMove> GetValidMoves(int i, int j) const;

	//сделать ход
	void MakeMove(int fromI, int fromJ, int toI, int toJ);

	//шах для стороны?
	bool IsCheck(int party) const;
	//нельзя ходить для стороны?
	bool IsCanMove(int party) const;
};

#endif
