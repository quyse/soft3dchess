#include "ChessGame.h"
#include "windows.h"

ChessGame::ChessGame(const ChessResources& chessResources)
: currentMove(-1), ourParty(0), blueLight(-1, -1), animatedFrom(-1, -1), animatedTo(-1, -1), selfMode(false),
	chessResources(chessResources)
{
}

void ChessGame::LetsBeginBattle(int ourParty, bool selfMode)
{
	this->ourParty = ourParty;
	this->selfMode = selfMode;
	currentMove = 0;

	figures = FiguresField();
	for(int i = 0; i < 8; ++i)
		for(int j = 0; j < 8; ++j)
			figures[i][j].type = Figure::typeNone;

	for(int i = 0; i < 2; ++i)
		for(int j = 0; j < 8; ++j)
			figures[i][j].party = 0;
	for(int i = 6; i < 8; ++i)
		for(int j = 0; j < 8; ++j)
			figures[i][j].party = 1;

	figures[0][0].type = Figure::typeCastle;
	figures[0][1].type = Figure::typeKnight;
	figures[0][2].type = Figure::typeBishop;
	figures[0][3].type = Figure::typeKing;
	figures[0][4].type = Figure::typeQueen;
	figures[0][5].type = Figure::typeBishop;
	figures[0][6].type = Figure::typeKnight;
	figures[0][7].type = Figure::typeCastle;
	for(int j = 0; j < 8; ++j)
		figures[1][j].type = Figure::typePawn;

	figures[7][0].type = Figure::typeCastle;
	figures[7][1].type = Figure::typeKnight;
	figures[7][2].type = Figure::typeBishop;
	figures[7][3].type = Figure::typeKing;
	figures[7][4].type = Figure::typeQueen;
	figures[7][5].type = Figure::typeBishop;
	figures[7][6].type = Figure::typeKnight;
	figures[7][7].type = Figure::typeCastle;
	for(int j = 0; j < 8; ++j)
		figures[6][j].type = Figure::typePawn;
}

int ChessGame::GetCurrentMove() const
{
	return currentMove;
}

int ChessGame::GetOurParty() const
{
	return ourParty;
}

const Figure& ChessGame::GetFigure(int i, int j) const
{
	return figures[i][j];
}

vector<3> ChessGame::GetCellPosition(int i, int j) const
{
	return vector<3>(i * 2 + 1 - 8, j * 2 + 1 - 8, 0);
}

char ChessGame::GetFigureModel(int figureType) const
{
	switch(figureType)
	{
	case Figure::typePawn:
		return 'P';
	case Figure::typeBishop:
		return 'B';
	case Figure::typeKnight:
		return 'N';
	case Figure::typeCastle:
		return 'C';
	case Figure::typeQueen:
		return 'Q';
	case Figure::typeKing:
		return 'K';
	}
	return '#';
}

std::pair<int, int> ChessGame::PickCell(const vector<2>& coord, const Camera& camera) const
{
	std::pair<int, int> bestCell(0, 0);
	scalar bestDistance = 1e10;
	matrix<4, 4> viewProjTransform = camera.GetViewProjTransform();
	for(int i = 0; i < 8; ++i)
		for(int j = 0; j < 8; ++j)
		{
			vector<4> onScreen = GetCellPosition(i, j) * viewProjTransform;
			scalar len = length2(coord - vector<2>(onScreen.x, onScreen.y) / onScreen.w);
			if(len < bestDistance)
			{
				bestCell = std::make_pair(i, j);
				bestDistance = len;
			}
		}
	return bestCell;
}

void ChessGame::UpdateBoardImage()
{
	const int animationLength = 1000;
	int time = GetTickCount();
	if(time - animatedTime > animationLength)
		animatedFrom = animatedTo = std::make_pair(-1, -1);

	std::vector<ModelImage>& modelImages = boardImage.modelImages;
	modelImages.clear();
	for(int i = 0; i < 8; ++i)
		for(int j = 0; j < 8; ++j)
			if(figures[i][j].type)
			{
				ModelImage modelImage;
				modelImage.model = GetFigureModel(figures[i][j].type);
				modelImage.colorTexture = figures[i][j].party ? 'B' : 'W';
				if(animatedTo.first == i && animatedTo.second == j)
					modelImage.position = lerp(GetCellPosition(animatedFrom.first, animatedFrom.second), GetCellPosition(i, j), scalar(time - animatedTime) / animationLength);
				else
					modelImage.position = GetCellPosition(i, j);
				modelImages.push_back(modelImage);
			}

	boardImage.yellowLights.clear();
	boardImage.greenLights.clear();
	boardImage.redLights.clear();

	if(currentMove == ourParty)
		for(int i = 0; i < 8; ++i)
			for(int j = 0; j < 8; ++j)
			{
				const Figure& figure = figures[i][j];
				if(figure.type && figure.party == ourParty)
					if((i != blueLight.first || j != blueLight.second) && !figures.GetValidMoves(i, j).empty())
						boardImage.yellowLights.push_back(GetCellPosition(i, j));
			}

	if(blueLight.first >= 0 && blueLight.second >= 0)
	{
		boardImage.isBlueLight = true;
		boardImage.blueLight = GetCellPosition(blueLight.first, blueLight.second);

		std::vector<ValidMove> validMoves = figures.GetValidMoves(blueLight.first, blueLight.second);
		for(int i = 0; i < validMoves.size(); ++i)
			(validMoves[i].taking ? boardImage.redLights : boardImage.greenLights).push_back(GetCellPosition(validMoves[i].toI, validMoves[i].toJ));
	}
	else
		boardImage.isBlueLight = false;

	boardImage.fire.CheckFinish();
}

const ChessBoardImage& ChessGame::GetBoardImage() const
{
	return boardImage;
}

bool ChessGame::MakeMove(int party, int fromI, int fromJ, int toI, int toJ, int promotion)
{
	//��������, ��� ����� �� �������, ��� ������ ���
	if(party != currentMove)
		return false;
	//��������, ��� ������� ������ ����, � ��� ������ �������
	if(!figures[fromI][fromJ].type || figures[fromI][fromJ].party != party)
		return false;
	//�������� ���������� ����
	const std::vector<ValidMove> validMoves = figures.GetValidMoves(fromI, fromJ);
	for(int i = 0; i < validMoves.size(); ++i)
		//���� ���
		if(validMoves[i].toI == toI && validMoves[i].toJ == toJ)
		{
			//���� ������ - �����
			if(figures[fromI][fromJ].type == Figure::typeCastle)
			{
				//�� �������� ��������������� ���� ���������� ���������
				if(party == 0 && fromI == 0 || party == 1 && fromI == 7)
					if(fromJ == 0)
						figures.parties[party].canCastlingShort = false;
					else if(fromJ == 7)
						figures.parties[party].canCastlingLong = false;
			}
			//����� ���� ������ - ������
			if(figures[fromI][fromJ].type == Figure::typeKing)
			{
				//�� �������� ��� ����� ���������� ���������
				figures.parties[party].canCastlingShort = false;
				figures.parties[party].canCastlingLong = false;

				//���� ��� - �������� ���������
				if(toJ == fromJ - 2)
				{
					//����������� �����
					figures[fromI][fromJ - 1] = figures[fromI][0];
					figures[fromI][0].type = Figure::typeNone;
				}
				//���� ��� - ������� ���������
				if(toJ == fromJ + 2)
				{
					//����������� �����
					figures[fromI][fromJ + 1] = figures[fromI][7];
					figures[fromI][7].type = Figure::typeNone;
				}
			}

			//���� ��� �����, �������� �� 2 ���� ������
			if(figures[fromI][fromJ].type == Figure::typePawn && abs(toI - fromI) == 2)
				//���������� ��������� ��� ���������� ������ �� �������
				figures.enPassant = fromJ;
			else
				figures.enPassant = -1;

			//���������� ��������� ��������� ��������� ������, � ��� ��������� ������
			int fireI = toI, fireJ = toJ, fireFigureType = figures[toI][toJ].type;

			//���� ��� �����, ������� ����� (���������), � ��� ���� ���� �� ������ ����
			if(figures[fromI][fromJ].type == Figure::typePawn && validMoves[i].taking && !figures[toI][toJ].type)
			{
				//�� ��� ��������, ��� ��� ��������� ������ �� �������
				//������� ����������
				fireI = fromI;
				fireFigureType = figures[fromI][toJ].type;
				//����� ����� � �������
				figures[fromI][toJ].type = Figure::typeNone;
			}

			//����������� ������; ���� ������ "�����������", �� �������� �� ����� ������
			figures[toI][toJ] = figures[fromI][fromJ];
			figures[fromI][fromJ].type = Figure::typeNone;
			if(promotion != Figure::typeNone)
				figures[toI][toJ].type = promotion;

			//������� ������� ���
			currentMove = !currentMove;
			//������� "����" �������, ���� ����� ���� ��� � �����
			if(selfMode)
				ourParty = !ourParty;

			//������ ��������
			animatedFrom = std::make_pair(fromI, fromJ);
			animatedTo = std::make_pair(toI, toJ);
			animatedTime = GetTickCount();
			//���� ��� ��� �� �������, �� ��������� �����
			if(validMoves[i].taking)
				PrepareFire(fireI, fireJ, fireFigureType);
			return true;
		}
	return false;
}

void ChessGame::SetCellMovesFromLight(int i, int j)
{
	blueLight = std::make_pair(i, j);
}

int ChessGame::GetFinishStatus() const
{
	//����������, ���� �� �����-������ ������, ����� �������
	bool draw = true;
	for(int i = 0; draw && i < 8; ++i)
		for(int j = 0; draw && j < 8; ++j)
			if(figures[i][j].type && figures[i][j].type!= Figure::typeKing)
				draw = false;
	if(draw)
		return GameFinish::Draw;

	//����������, ���� �� ������ ���� � ����, ��� �����
	if(figures.IsCanMove(currentMove))
		//���� ����, �� ���� ��� �� �����������
		return GameFinish::Continued;
	//����� ����
	//����������, ��������� �� ����, ��� ������ �����, ���
	if(figures.IsCheck(currentMove))
		//���� ���������, �� ��� ���
		return GetOurParty() == currentMove ? GameFinish::CheckMateForMe : GameFinish::CheckMateForHim;
	//����� ��� ���
	return GameFinish::StaleMate;
}

void ChessGame::PrepareFire(int toI, int toJ, int figureType)
{
	//�������� ������ ��������� ������
	const Model<NormalVertex, int>* model = chessResources.GetModel(GetFigureModel(figureType));

	//�������� �������� ������������ ������� ������
	static MemoryBuffer buffer;
	int verticesCount = model->GetVerticesCount();
	const NormalVertex* vertices = model->GetVertices();
	buffer.Resize<vector<3> >(verticesCount);
	vector<3>* verticesBuffer = buffer.GetPointer<vector<3> >();
	for(int i = 0; i < verticesCount; ++i)
		verticesBuffer[i] = vertices[i].position;
	std::random_shuffle(verticesBuffer, verticesBuffer + verticesCount);

	//���������� ������
	const int particlesCount = std::min(100, verticesCount);
	//����� ������ ��������� ���������� ������, � �� ��� ������� �������
	vector<3> position = GetCellPosition(toI, toJ);
	boardImage.fire.particles.resize(particlesCount);
	for(int i = 0; i < particlesCount; ++i)
		boardImage.fire.particles[i] = position + verticesBuffer[i];
	boardImage.fire.Initialize();
}
