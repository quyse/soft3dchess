#ifndef ___CHESS_GAME_H___
#define ___CHESS_GAME_H___

/*
Файл содержит класс шахматной игры. Он содержит все необходимые данные о состоянии игры.

Соглашения:
Номера сторон: 0 - белые, 1 - черные
Система координат: ось J - вдоль фронтов начального построения слева направо, ось I - от белых к черным (снизу вверх)
То есть у белых координата I меньше, чем у черных.
Проще говоря, J - буквы от A до H, I - цифры от 1 до 8. (но координаты все от 0 до 7)

Рокировка обозначается ходом ладьи.
*/

#include <vector>
#include <string>
#include "ChessResources.h"
#include "ChessBoardImage.h"
#include "FiguresField.h"

//варианты завершения игры
struct GameFinish
{
	enum
	{
		//еще не завершилась
		Continued,
		//мат нам
		CheckMateForMe,
		//мат им
		CheckMateForHim,
		//пат
		StaleMate,
		//ничья
		Draw
	};
};

class ChessPlayer;
class ChessGame
{
private:
	//у кого текущий ход
	int currentMove;
	//"наша" сторона
	int ourParty;
	//режим игры с самим собой - когда наша сторона меняется с каждым ходом
	int selfMode;

public:
	FiguresField figures;
private:
	const ChessResources& chessResources;

	//координаты клеток подсветки
	std::pair<int, int> blueLight;
	//координаты, откуда и куда идет анимированная фигура
	std::pair<int, int> animatedFrom, animatedTo;
	//время начала анимации (тики)
	int animatedTime;

	//изображение поля боя
	ChessBoardImage boardImage;

	//мат для стороны?
	bool IsCheckMate(int party) const;
	//пат для стороны?
	bool IsStaleMate(int party) const;

	//получить символ модели для типа фигуры
	char GetFigureModel(int figureType) const;

	//подготовить огонь для хода со взятием
	void PrepareFire(int toI, int toJ, int figureType);

public:
	ChessGame(const ChessResources& chessResources);

	//указать игрока
	void SetPlayer(int party, ChessPlayer* player);

	//установить начальное положение фигур (и нашу сторону)
	void LetsBeginBattle(int ourParty, bool selfMode = false);

	int GetCurrentMove() const;
	int GetOurParty() const;

	//получить фигуру
	const Figure& GetFigure(int i, int j) const;

	//получить положение клетки
	vector<3> GetCellPosition(int i, int j) const;
	//выбрать ближайшую клетку по экранным координатам
	std::pair<int, int> PickCell(const vector<2>& coord, const Camera& camera) const;

	//обновить изображение доски
	void UpdateBoardImage();
	//получить изображение доски
	const ChessBoardImage& GetBoardImage() const;

	//сделать ход (возвращает true, если ход принят)
	bool MakeMove(int party, int fromI, int fromJ, int toI, int toJ, int promotion = Figure::typeNone);

	//указать клетку, с которой должны показываться ходы
	void SetCellMovesFromLight(int i, int j);

	//получить состояние игры
	int GetFinishStatus() const;
};

#endif
