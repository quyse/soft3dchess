#include "FiguresField.h"

void FiguresField::AddPossibleMove(std::vector<ValidMove>& possibleMoves, int fromI, int fromJ, int toI, int toJ) const
{
	int party = field[fromI][fromJ].party;
	possibleMoves.push_back(ValidMove(fromI, fromJ, toI, toJ, field[toI][toJ].type && field[toI][toJ].party != party));
}

bool FiguresField::AddPossibleMoveWithCheck(std::vector<ValidMove>& possibleMoves, int fromI, int fromJ, int toI, int toJ) const
{
	//���� ������� ����� ��������� � �������� �����
	if(toI >= 0 && toI < 8 && toJ >= 0 && toJ < 8)
		//���� ��� ����� ��� ��� ��������� ����
		if(!field[toI][toJ].type || field[fromI][fromJ].party != field[toI][toJ].party)
		{
			//�������� ���
			AddPossibleMove(possibleMoves, fromI, fromJ, toI, toJ);
			//���� ��� ��� ����, �� ������ ����� �� ����
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
	//�������� ����������� ����� (�� ����������)
	int attackDir = party ? -1 : 1;

	//����� �� ���� ������
	switch(figure.type)
	{
	case Figure::typePawn://�����
		{
			int ii = i + attackDir;
			//���� ���� ���� ������
			if(ii >= 0 && ii < 8)
			{
				//��� �� 1, ���� ��� �������� (������ ������)
				if(!field[ii][j].type)
					possibleMoves.push_back(ValidMove(i, j, ii, j, false));
				//��� �� ���������, ���� ��� ���� ���� (������ ������ ������)
				if(j - 1 >= 0 && field[ii][j - 1].type && field[ii][j - 1].party != party)
						possibleMoves.push_back(ValidMove(i, j, ii, j - 1, true));
				if(j + 1 < 8 && field[ii][j + 1].type && field[ii][j + 1].party != party)
						possibleMoves.push_back(ValidMove(i, j, ii, j + 1, true));
				//��� �� 2, ���� ����� �� �������� �������
				if((party == 0 && i == 1 || party == 1 && i == 6) && !field[i + attackDir][j].type && !field[i + attackDir * 2][j].type)
					AddPossibleMove(possibleMoves, i, j, i + attackDir * 2, j);
				//������ �� �������
				if(enPassant >= 0 && abs(enPassant - j) == 1 && (party == 0 && i == 4 || party == 1 && i == 3))
					possibleMoves.push_back(ValidMove(i, j, party == 0 ? 5 : 2, enPassant, true));
			}
		}
		break;
	case Figure::typeBishop://����
		AddRayPossibleMoves(possibleMoves, i, j, -1, -1);
		AddRayPossibleMoves(possibleMoves, i, j, -1, 1);
		AddRayPossibleMoves(possibleMoves, i, j, 1, -1);
		AddRayPossibleMoves(possibleMoves, i, j, 1, 1);
		break;
	case Figure::typeKnight://����
		AddPossibleMoveWithCheck(possibleMoves, i, j, i - 2, j - 1);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i - 2, j + 1);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i + 2, j - 1);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i + 2, j + 1);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i - 1, j - 2);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i - 1, j + 2);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i + 1, j - 2);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i + 1, j + 2);
		break;
	case Figure::typeCastle://�����
		AddRayPossibleMoves(possibleMoves, i, j, -1, 0);
		AddRayPossibleMoves(possibleMoves, i, j, 1, 0);
		AddRayPossibleMoves(possibleMoves, i, j, 0, -1);
		AddRayPossibleMoves(possibleMoves, i, j, 0, 1);
		break;
	case Figure::typeQueen://�����
		AddRayPossibleMoves(possibleMoves, i, j, -1, 0);
		AddRayPossibleMoves(possibleMoves, i, j, 1, 0);
		AddRayPossibleMoves(possibleMoves, i, j, 0, -1);
		AddRayPossibleMoves(possibleMoves, i, j, 0, 1);
		AddRayPossibleMoves(possibleMoves, i, j, -1, -1);
		AddRayPossibleMoves(possibleMoves, i, j, -1, 1);
		AddRayPossibleMoves(possibleMoves, i, j, 1, -1);
		AddRayPossibleMoves(possibleMoves, i, j, 1, 1);
		break;
	case Figure::typeKing://������
		AddPossibleMoveWithCheck(possibleMoves, i, j, i - 1, j - 1);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i - 1, j);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i - 1, j + 1);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i, j - 1);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i, j + 1);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i + 1, j - 1);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i + 1, j);
		AddPossibleMoveWithCheck(possibleMoves, i, j, i + 1, j + 1);

		//�������� ���������
		if(!onlyTaking && parties[party].canCastlingShort && !field[i][j - 1].type && !field[i][j - 2].type)
		{
			//���������, ��� ����� ������������
			if(field[i][0].type != Figure::typeCastle || field[i][0].party != party) break;
			//���������, ��� ���� �� ��������� ��� ����
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

			//�������� ���������, ��������� ��������
			AddPossibleMove(possibleMoves, i, j, i, j - 2);
		}

		//������� ���������
		if(!onlyTaking && parties[party].canCastlingLong && !field[i][j + 1].type && !field[i][j + 2].type && !field[i][j + 3].type)
		{
			//���������, ��� ����� ������������
			if(field[i][7].type != Figure::typeCastle || field[i][7].party != party) break;
			//���������, ��� ���� �� ��������� ��� ����
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

			//�������� ���������, ��������� ��������
			AddPossibleMove(possibleMoves, i, j, i, j + 2);
		}
		break;
	}
	return possibleMoves;
}

std::vector<ValidMove> FiguresField::GetValidMoves(int i, int j) const
{
	//�������� ���� �������
	int party = field[i][j].party;

	//�������� ��������� ����
	std::vector<ValidMove> possibleMoves = GetPossibleMoves(i, j);

	//������ �������������� ���������� �����
	std::vector<ValidMove> validMoves;

	//��������� ��� ��������� ���� �� ������������
	for(int i = 0; i < possibleMoves.size(); ++i)
	{
		//�������� ���
		const ValidMove& move = possibleMoves[i];
		//����������� ����
		FiguresField fieldCopy(*this);
		//��������� ��� ���
		fieldCopy.MakeMove(move.fromI, move.fromJ, move.toI, move.toJ);
		//���� ��� �� ����������� ��� ���
		if(!fieldCopy.IsCheck(party))
			//�� ��� ����������
			validMoves.push_back(move);
	}

	return validMoves;
}

void FiguresField::MakeMove(int fromI, int fromJ, int toI, int toJ)
{
	//���� ������ - ������
	if(field[fromI][fromJ].type == Figure::typeKing)
	{
		//���� ��� - �������� ���������
		if(toJ == fromJ - 2)
		{
			//����������� �����
			field[fromI][fromJ - 1] = field[fromI][0];
			field[fromI][0].type = Figure::typeNone;
		}
		//���� ��� - ������� ���������
		if(toJ == fromJ + 2)
		{
			//����������� �����
			field[fromI][fromJ + 1] = field[fromI][7];
			field[fromI][7].type = Figure::typeNone;
		}
	}

	//���� ��� �����, �������� �� 2 ���� ������
	if(field[fromI][fromJ].type == Figure::typePawn && abs(toI - fromI) == 2)
		//���������� ��������� ��� ���������� ������ �� �������
		enPassant = fromJ;
	else
		enPassant = -1;

	//���� ��� �����, ������� ����� (���������), � ��� ���� ���� �� ������ ����
	if(field[fromI][fromJ].type == Figure::typePawn && fromI != toI && fromJ != toJ && !field[toI][toJ].type)
		//�� ��� ��������, ��� ��� ��������� ������ �� �������
		//����� ����� � �������
		field[fromI][toJ].type = Figure::typeNone;

	//����������� ������
	field[toI][toJ] = field[fromI][fromJ];
	field[fromI][fromJ].type = Figure::typeNone;
}

bool FiguresField::IsCheck(int party) const
{
	//������ ���������, �� �������� �� ���-������ ������
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
	//������� ����� �������, ���� � ��� ���� ���� �� ���� ���������� ���
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
