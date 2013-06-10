#include "FiguresField.h"

void FiguresField::AddPossibleMove(std::vector<ValidMove>& possibleMoves, int fromI, int fromJ, int toI, int toJ) const
{
	int party = field[fromI][fromJ].party;
	possibleMoves.push_back(ValidMove(fromI, fromJ, toI, toJ, field[toI][toJ].type && field[toI][toJ].party != party));
}

bool FiguresField::AddPossibleMoveWithCheck(std::vector<ValidMove>& possibleMoves, int fromI, int fromJ, int toI, int toJ) const
{
	//если целевая точка находится в пределах доски
	if(toI >= 0 && toI < 8 && toJ >= 0 && toJ < 8)
		//если она пуста или там находится враг
		if(!field[toI][toJ].type || field[fromI][fromJ].party != field[toI][toJ].party)
		{
			//добавить ход
			AddPossibleMove(possibleMoves, fromI, fromJ, toI, toJ);
			//если там был враг, то больше ходов не надо
			return !field[toI][toJ].type;
		}
	return false;
}

void FiguresField::AddRayPossibleMoves(std::vector<ValidMove>& possibleMoves, int fromI, int fromJ, int dirI, int dirJ) const
{
	for(int i = fromI + dirI, j = fromJ + dirJ; i >= 0 && i < 8 && j >= 0 && j < 8; i += dirI, j += dirJ)
		if(!AddPossibleMoveWithCheck(possibleMoves, fromI, fromJ, i, j))
			break;
}

std::vector<ValidMove> FiguresField::GetPossibleMoves(int i, int j, bool onlyTaking) const
{
	std::vector<ValidMove> possibleMoves;
	
	const Figure& figure = field[i][j];
	if(figure.type == Figure::typeNone)
		return possibleMoves;

	int party = field[i][j].party;
	//получить направление атаки (на противника)
	int attackDir = party ? -1 : 1;

	//выбор по типу фигуры
	switch(figure.type)
	{
	case Figure::typePawn://пешка
		{
			int ii = i + attackDir;
			//если есть куда ходить
			if(ii >= 0 && ii < 8)
			{
				//ход на 1, если там свободно (нельзя рубить)
				if(!field[ii][j].type)
					possibleMoves.push_back(ValidMove(i, j, ii, j, false));
				//ход по диагонали, если там есть враг (нельзя ходить просто)
				if(j - 1 >= 0 && field[ii][j - 1].type && field[ii][j - 1].party != party)
						possibleMoves.push_back(ValidMove(i, j, ii, j - 1, true));
				if(j + 1 < 8 && field[ii][j + 1].type && field[ii][j + 1].party != party)
						possibleMoves.push_back(ValidMove(i, j, ii, j + 1, true));
				//ход на 2, если пешка на исходной позиции
				if((party == 0 && i == 1 || party == 1 && i == 6) && !field[i + attackDir][j].type && !field[i + attackDir * 2][j].type)
					AddPossibleMove(possibleMoves, i, j, i + attackDir * 2, j);
				//взятие на проходе
				if(enPassant >= 0 && abs(enPassant - j) == 1 && (party == 0 && i == 4 || party == 1 && i == 3))
					possibleMoves.push_back(ValidMove(i, j, party == 0 ? 5 : 2, enPassant, true));
			}
		}
		break;
	case Figure::typeBishop://слон
		AddRayPossibleMoves(possibleMoves, i, j, -1, -1);
		AddRayPossibleMoves(possibleMoves, i, j, -1, 1);
		AddRayPossibleMoves(possibleMoves, i, j, 1, -1);
		AddRayPossibleMoves(possibleMoves, i, j, 1, 1);
		break;
	case Figure::typeKnight://конь
		AddPossibleMoveWithCheck(possibleMoves, i, j, i - 2, j - 1);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i - 2, j + 1);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i + 2, j - 1);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i + 2, j + 1);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i - 1, j - 2);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i - 1, j + 2);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i + 1, j - 2);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i + 1, j + 2);
		break;
	case Figure::typeCastle://ладья
		AddRayPossibleMoves(possibleMoves, i, j, -1, 0);
		AddRayPossibleMoves(possibleMoves, i, j, 1, 0);
		AddRayPossibleMoves(possibleMoves, i, j, 0, -1);
		AddRayPossibleMoves(possibleMoves, i, j, 0, 1);
		break;
	case Figure::typeQueen://ферзь
		AddRayPossibleMoves(possibleMoves, i, j, -1, 0);
		AddRayPossibleMoves(possibleMoves, i, j, 1, 0);
		AddRayPossibleMoves(possibleMoves, i, j, 0, -1);
		AddRayPossibleMoves(possibleMoves, i, j, 0, 1);
		AddRayPossibleMoves(possibleMoves, i, j, -1, -1);
		AddRayPossibleMoves(possibleMoves, i, j, -1, 1);
		AddRayPossibleMoves(possibleMoves, i, j, 1, -1);
		AddRayPossibleMoves(possibleMoves, i, j, 1, 1);
		break;
	case Figure::typeKing://король
		AddPossibleMoveWithCheck(possibleMoves, i, j, i - 1, j - 1);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i - 1, j);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i - 1, j + 1);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i, j - 1);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i, j + 1);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i + 1, j - 1);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i + 1, j);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i + 1, j + 1);

		//короткая рокировка
		if(!onlyTaking && parties[party].canCastlingShort && !field[i][j - 1].type && !field[i][j - 2].type)
		{
			//проверить, что ладья присутствует
			if(field[i][0].type != Figure::typeCastle || field[i][0].party != party) break;
			//проверить, что поля не находятся под боем
			if(IsCheck(party)) break;
			{
				FiguresField fieldCopy(*this);
				fieldCopy.MakeMove(i, j, i, j - 1);
				if(fieldCopy.IsCheck(party)) break;
			}
			{
				FiguresField fieldCopy(*this);
				fieldCopy.MakeMove(i, j, i, j - 2);
				if(fieldCopy.IsCheck(party)) break;
			}

			//проверки завершены, рокировка возможна
			AddPossibleMove(possibleMoves, i, j, i, j - 2);
		}

		//длинная рокировка
		if(!onlyTaking && parties[party].canCastlingLong && !field[i][j + 1].type && !field[i][j + 2].type && !field[i][j + 3].type)
		{
			//проверить, что ладья присутствует
			if(field[i][7].type != Figure::typeCastle || field[i][7].party != party) break;
			//проверить, что поля не находятся под боем
			if(IsCheck(party)) break;
			{
				FiguresField fieldCopy(*this);
				fieldCopy.MakeMove(i, j, i, j + 1);
				if(fieldCopy.IsCheck(party)) break;
			}
			{
				FiguresField fieldCopy(*this);
				fieldCopy.MakeMove(i, j, i, j + 2);
				if(fieldCopy.IsCheck(party)) break;
			}

			//проверки завершены, рокировка возможна
			AddPossibleMove(possibleMoves, i, j, i, j + 2);
		}
		break;
	}
	return possibleMoves;
}

std::vector<ValidMove> FiguresField::GetValidMoves(int i, int j) const
{
	//получить нашу сторону
	int party = field[i][j].party;

	//получить возможные ходы
	std::vector<ValidMove> possibleMoves = GetPossibleMoves(i, j);

	//вектор результирующих допустимых ходов
	std::vector<ValidMove> validMoves;

	//проверить все возможные ходы на допустимость
	for(int i = 0; i < possibleMoves.size(); ++i)
	{
		//получить ход
		const ValidMove& move = possibleMoves[i];
		//скопировать поле
		FiguresField fieldCopy(*this);
		//выполнить там ход
		fieldCopy.MakeMove(move.fromI, move.fromJ, move.toI, move.toJ);
		//если там не наблюдается шах нам
		if(!fieldCopy.IsCheck(party))
			//то ход допустимый
			validMoves.push_back(move);
	}

	return validMoves;
}

void FiguresField::MakeMove(int fromI, int fromJ, int toI, int toJ)
{
	//если фигура - король
	if(field[fromI][fromJ].type == Figure::typeKing)
	{
		//если ход - короткая рокировка
		if(toJ == fromJ - 2)
		{
			//передвинуть ладью
			field[fromI][fromJ - 1] = field[fromI][0];
			field[fromI][0].type = Figure::typeNone;
		}
		//если ход - длинная рокировка
		if(toJ == fromJ + 2)
		{
			//передвинуть ладью
			field[fromI][fromJ + 1] = field[fromI][7];
			field[fromI][7].type = Figure::typeNone;
		}
	}

	//если это пешка, шагающая на 2 шага вперед
	if(field[fromI][fromJ].type == Figure::typePawn && abs(toI - fromI) == 2)
		//установить указатель для возможного взятия на проходе
		enPassant = fromJ;
	else
		enPassant = -1;

	//если это пешка, которая рубит (наискосок), и при этом идет на пустое поле
	if(field[fromI][fromJ].type == Figure::typePawn && fromI != toI && fromJ != toJ && !field[toI][toJ].type)
		//то это означает, что она выполняет взятие на проходе
		//снять пешку с прохода
		field[fromI][toJ].type = Figure::typeNone;

	//переместить фигуру
	field[toI][toJ] = field[fromI][fromJ];
	field[fromI][fromJ].type = Figure::typeNone;
}

bool FiguresField::IsCheck(int party) const
{
	//просто проверить, не угрожает ли кто-нибудь королю
	for(int i = 0; i < 8; ++i)
		for(int j = 0; j < 8; ++j)
		{
			const Figure& figure = field[i][j];
			if(figure.type && figure.party != party)
			{
				std::vector<ValidMove> moves = GetPossibleMoves(i, j, true);
				for(int k = 0; k < moves.size(); ++k)
					if(moves[k].taking)
					{
						const Figure& ourFigure = field[moves[k].toI][moves[k].toJ];
						if(ourFigure.type == Figure::typeKing)
							return true;
					}
			}
		}
	return false;
}

bool FiguresField::IsCanMove(int party) const
{
	//сторона может сходить, если у нее есть хотя бы один допустимый ход
	for(int i = 0; i < 8; ++i)
		for(int j = 0; j < 8; ++j)
		{
			const Figure& figure = field[i][j];
			if(figure.type && figure.party == party)
				if(!GetValidMoves(i, j).empty())
					return true;
		}
	return false;
}
