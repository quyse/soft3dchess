#include "MainWindow.h"
#include "Connection.h"
#include "Resources.h"
#include <iostream>
#include <sstream>
#include <string>

ATOM MainWindow::wndClass = 0;

static BOOL CALLBACK ConnectDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static std::string* str;
	switch(msg)
	{
	case WM_INITDIALOG:
		str = (std::string*)lParam;
		return TRUE;
	case WM_CLOSE:
		EndDialog(hWnd, FALSE);
		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			{
				HWND hEdit = GetDlgItem(hWnd, IDC_EDIT_SERVER);
				int length = GetWindowTextLengthA(hEdit) + 1;
				char* buffer = new char[length];
				GetWindowTextA(hEdit, buffer, length);
				*str = buffer;
				EndDialog(hWnd, TRUE);
			}
			return TRUE;
		case IDCANCEL:
			EndDialog(hWnd, FALSE);
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}

static BOOL CALLBACK CreateServerDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool* isWhite;
	switch(msg)
	{
	case WM_INITDIALOG:
		isWhite = (bool*)lParam;
		CheckDlgButton(hWnd, IDC_RADIO_WHITE, BST_CHECKED);
		return TRUE;
	case WM_CLOSE:
		EndDialog(hWnd, FALSE);
		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			*isWhite = IsDlgButtonChecked(hWnd, IDC_RADIO_WHITE) == BST_CHECKED;
			EndDialog(hWnd, TRUE);
			return TRUE;
		case IDCANCEL:
			EndDialog(hWnd, FALSE);
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}

BOOL CALLBACK PromotionDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int* promotionFigure;
	switch(msg)
	{
	case WM_INITDIALOG:
		promotionFigure = (int*)lParam;
		{
			//заполнить список фигурами
			static const int figures[] = { Figure::typeQueen, Figure::typeKnight, Figure::typeCastle, Figure::typeBishop };
			std::wstring names = GetString(IDS_PROMOTION_FIGURES);
			HWND hList = GetDlgItem(hWnd, IDC_LIST_FIGURES);
			int k = 0;
			for(int i = 0; i < names.length(); )
			{
				int j;
				for(j = i; j < names.length(); ++j)
					if(names[j] == L'\t')
						break;
				ListBox_SetItemData(hList, ListBox_AddString(hList, names.substr(i, j - i).c_str()), figures[k++]);
				i = j + 1;
			}
		}
		return TRUE;
	case WM_CLOSE:
		EndDialog(hWnd, FALSE);
		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_LIST_FIGURES:
			if(HIWORD(wParam) == LBN_SELCHANGE)
			{
				EnableWindow(GetDlgItem(hWnd, IDOK), ListBox_GetCurSel(GetDlgItem(hWnd, IDC_LIST_FIGURES)) != LB_ERR);
				return TRUE;
			}
			return FALSE;
		case IDOK:
			{
				HWND hList = GetDlgItem(hWnd, IDC_LIST_FIGURES);
				*promotionFigure = ListBox_GetItemData(hList, ListBox_GetCurSel(hList));
			}
			EndDialog(hWnd, TRUE);
			return TRUE;
		case IDCANCEL:
			EndDialog(hWnd, FALSE);
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}

LRESULT CALLBACK MainWindow::StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(msg == WM_CREATE)
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)((CREATESTRUCT*)lParam)->lpCreateParams);
	MainWindow* window = (MainWindow*)GetWindowLong(hWnd, GWL_USERDATA);
	if(window)
		return window->WndProc(hWnd, msg, wParam, lParam);
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void MainWindow::OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify)
{
	switch(id)
	{
	case MID_CREATE_SERVER:
		{
			bool isWhite;
			if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_CREATE_SERVER), hWnd, CreateServerDlgProc, (LPARAM)&isWhite))
			{
				chessGame->LetsBeginBattle(isWhite ? 0 : 1);
				if(connection)
					delete connection;
				connection = new Connection(hWnd);
				connection->StartServer();
			}
		}
		break;
	case MID_CONNECT_GAME:
		{
			std::string serverName;
			if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_CONNECT), hWnd, ConnectDlgProc, (LPARAM)&serverName))
			{
				if(connection)
					delete connection;
				connection = new Connection(hWnd);
				connection->ConnectClient(serverName.c_str());
			}
		}
		break;
	case MID_BEGIN_GAME_SELF:
		if(connection)
		{
			delete connection;
			connection = 0;
		}
		chessGame->LetsBeginBattle(0, true);
		break;
	case MID_EXIT:
		DestroyWindow(hWnd);
		break;
	case MID_ABOUT:
		MessageBox(hWnd, GetString(IDS_ABOUT).c_str(), GetString(IDS_ABOUT_TITLE).c_str(), MB_ICONINFORMATION);
		break;
	}
}

void MainWindow::CheckFinish() const
{
	switch(chessGame->GetFinishStatus())
	{
	case GameFinish::Continued:
		break;
	case GameFinish::CheckMateForMe:
		MessageBox(hWnd, GetString(IDS_CHECKMATE_FOR_ME).c_str(), GetString(IDS_GAME_OVER).c_str(), MB_ICONINFORMATION);
		break;
	case GameFinish::CheckMateForHim:
		MessageBox(hWnd, GetString(IDS_CHECKMATE_FOR_HIM).c_str(), GetString(IDS_GAME_OVER).c_str(), MB_ICONINFORMATION);
		break;
	case GameFinish::StaleMate:
		MessageBox(hWnd, GetString(IDS_STALEMATE).c_str(), GetString(IDS_GAME_OVER).c_str(), MB_ICONINFORMATION);
		break;
	case GameFinish::Draw:
		MessageBox(hWnd, GetString(IDS_DRAW).c_str(), GetString(IDS_GAME_OVER).c_str(), MB_ICONINFORMATION);
		break;
	}
}

void MainWindow::OnConnection(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case MessageConnection_Accepted:
		{
			char s[10];
			sprintf(s, "s %d", !chessGame->GetOurParty());
			connection->Send(s);
		}
		MessageBox(hWnd, GetString(IDS_ACCEPT_SUCCEEDED).c_str(), GetString(IDS_GAME_BEGINS).c_str(), MB_ICONINFORMATION);
		break;
	case MessageConnection_AcceptFailed:
		MessageBox(hWnd, GetString(IDS_ACCEPT_FAILED).c_str(), GetString(IDS_ERROR).c_str(), MB_ICONSTOP);
		break;
	case MessageConnection_Connected:
		MessageBox(hWnd, GetString(IDS_CONNECT_SUCCEEDED).c_str(), GetString(IDS_GAME_BEGINS).c_str(), MB_ICONINFORMATION);
		break;
	case MessageConnection_ConnectFailed:
		MessageBox(hWnd, GetString(IDS_CONNECT_FAILED).c_str(), GetString(IDS_ERROR).c_str(), MB_ICONSTOP);
		break;
	case MessageConnection_IncomingMessage:
		{
			//расшифровать сообщение
			std::istringstream str((char*)lParam);
			char s[10];
			str >> s;
			if(s[0] == 'm')
			{
				int fromI, fromJ, toI, toJ, promotion;
				str >> fromI >> fromJ >> toI >> toJ >> promotion;
				//сделать ход
				if(chessGame->MakeMove(!chessGame->GetOurParty(), fromI, fromJ, toI, toJ, promotion))
					CheckFinish();
			}
			else if(s[0] == 's')
			{
				int ourParty;
				str >> ourParty;
				chessGame->LetsBeginBattle(ourParty);
			}
		}
		break;
	case MessageConnection_ReadFailed:
		MessageBox(hWnd, GetString(IDS_READ_FAILED).c_str(), GetString(IDS_ERROR).c_str(), MB_ICONSTOP);
		break;
	}
}

LRESULT MainWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_LBUTTONDOWN, OnLButtonDown);
		HANDLE_MSG(hWnd, WM_LBUTTONUP, OnLButtonUp);
		HANDLE_MSG(hWnd, WM_RBUTTONDOWN, OnRButtonDown);
		HANDLE_MSG(hWnd, WM_RBUTTONUP, OnRButtonUp);
		HANDLE_MSG(hWnd, WM_MOUSEMOVE, OnMouseMove);
		HANDLE_MSG(hWnd, WM_SIZE, OnSize);
	case WM_CONNECTION:
		OnConnection(hWnd, wParam, lParam);
		return 0;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

void MainWindow::OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	surface->Paint(hdc, 0, 0);
/*	SelectPen(hdc, GetStockPen(WHITE_PEN));
	for(int i = 0; i < testLines.size(); ++i)
	{
		MoveToEx(hdc, 0, testLines[i], NULL);
		LineTo(hdc, surface->GetWidth(), testLines[i]);
	}*/
	EndPaint(hWnd, &ps);
}

void MainWindow::OnDestroy(HWND hWnd)
{
	SetWindowLong(hWnd, GWL_USERDATA, 0);
	delete this;
	PostQuitMessage(0);
}

void MainWindow::OnLButtonDown(HWND hWnd, BOOL doubleClick, int x, int y, UINT keyFlags)
{
	leftButtonDown = true;
	mouse = lastMouse = ConvertCoords(x, y);
	if(chessGame->GetCurrentMove() != chessGame->GetOurParty())
		return;
	std::pair<int, int> cell = chessGame->PickCell(mouse, *camera);
	const Figure& figure = chessGame->GetFigure(cell.first, cell.second);
	//если мышь попала в нашу фигуру
	if(figure.type && figure.party == chessGame->GetOurParty())
	{
		//выделить ее
		selectedFigureI = cell.first;
		selectedFigureJ = cell.second;
		chessGame->SetCellMovesFromLight(cell.first, cell.second);
	}
	//иначе если наша фигура уже выделена
	else if(selectedFigureI >= 0 && selectedFigureJ >= 0)
	{
		const Figure& selectedFigure = chessGame->GetFigure(selectedFigureI, selectedFigureJ);
		int promotion = Figure::typeNone;
		//если это ход - прохождение пешки
		if(selectedFigure.type == Figure::typePawn
			&& (selectedFigure.party == 0 && selectedFigureI == 6 && cell.first == 7
			|| selectedFigure.party == 1 && selectedFigureI == 1 && cell.first == 0)
			&& abs(selectedFigureJ - cell.second) <= 1)
		{
			//проверить, что ход - допустимый
			const std::vector<ValidMove> validMoves = chessGame->figures.GetValidMoves(selectedFigureI, selectedFigureJ);
			int i;
			for(i = 0; i < validMoves.size(); ++i)
				if(validMoves[i].toI == cell.first && validMoves[i].toJ == cell.second)
					break;
			if(i >= validMoves.size())
				return;
			//запросить, какую фигуру ставить
			if(!DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_PROMOTION), hWnd, PromotionDlgProc, (LPARAM)&promotion))
				return;
		}
		//попробовать сделать ход; если он принят
		if(chessGame->MakeMove(chessGame->GetOurParty(), selectedFigureI, selectedFigureJ, cell.first, cell.second, promotion))
		{
			//отправить сообщение, если есть соединение (соединения нет, если игра идет сама с собой)
			if(connection)
			{
				char message[Connection::messageSize];
				sprintf(message, "m %d %d %d %d %d", selectedFigureI, selectedFigureJ, cell.first, cell.second, promotion);
				connection->Send(message);
			}

			//сбросить выделение
			selectedFigureI = selectedFigureJ = -1;
			chessGame->SetCellMovesFromLight(-1, -1);

			//проверить завершенность
			CheckFinish();
		}
	}
	CaptureMouse(true);
}

void MainWindow::OnLButtonUp(HWND hWnd, int x, int y, UINT keyFlags)
{
	leftButtonDown = false;
	CaptureMouse(false);
}

void MainWindow::OnRButtonDown(HWND hWnd, BOOL doubleClick, int x, int y, UINT keyFlags)
{
	rightButtonDown = true;
	mouse = lastMouse = ConvertCoords(x, y);
	CaptureMouse(true);
}

void MainWindow::OnRButtonUp(HWND hWnd, int x, int y, UINT keyFlags)
{
	rightButtonDown = false;
	CaptureMouse(false);
}

void MainWindow::OnMouseMove(HWND hWnd, int x, int y, UINT keyFlags)
{
	mouse = ConvertCoords(x, y);
}

void MainWindow::OnSize(HWND hWnd, UINT state, int cx, int cy)
{
	if(state == SIZE_MINIMIZED)
		active = false;
	else if(state == SIZE_RESTORED || state == SIZE_MAXIMIZED)
		active = true;
	if(state != SIZE_MINIMIZED)
		surface->Resize(cx, cy);
}

MainWindow::MainWindow()
: surface(0), active(true), leftButtonDown(false), rightButtonDown(false),
	mouseCaptureCount(0), chessGame(0), connection(0), selectedFigureI(-1), selectedFigureJ(-1)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	if(!wndClass)
	{
		WNDCLASS wc;
		ZeroMemory(&wc, sizeof(wc));
		wc.hInstance = hInstance;
		wc.lpszClassName = TEXT("mwc");
		wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_APP));
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = GetStockBrush(WHITE_BRUSH);
		wc.lpfnWndProc = StaticWndProc;
		wndClass = RegisterClass(&wc);
	}

	surface = new Surface(0, 0);

//#define MAXIMIZED
#ifdef MAXIMIZED
	const int width = GetSystemMetrics(SM_CXSCREEN);
	const int height = GetSystemMetrics(SM_CYSCREEN);
#else
	const int width = 512;
	const int height = 512;
#endif
	hWnd = CreateWindow((LPCTSTR)wndClass, GetString(IDS_MAIN_TITLE).c_str(),
		WS_OVERLAPPEDWINDOW | WS_OVERLAPPED | WS_CAPTION |WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
		(GetSystemMetrics(SM_CXSCREEN) - width) / 2, (GetSystemMetrics(SM_CYSCREEN) - height) / 2, width, height, NULL, LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU_MAIN)), hInstance, this);

#ifdef MAXIMIZED
	ShowWindow(hWnd, SW_MAXIMIZE);
#endif
}

MainWindow::~MainWindow()
{
	delete surface;
}

vector<2> MainWindow::ConvertCoords(int x, int y) const
{
	return vector<2>(x, -y) * 2 / vector<2>(surface->GetWidth(), surface->GetHeight()) + vector<2>(-1, 1);
}

void MainWindow::CaptureMouse(bool capture)
{
	if(capture && mouseCaptureCount == 0)
		SetCapture(hWnd);
	if(!capture && mouseCaptureCount == 1)
		ReleaseCapture();
	if(capture)
		mouseCaptureCount++;
	else
		mouseCaptureCount--;
}

bool MainWindow::IsActive() const
{
	return active;
}

vector<2> MainWindow::GetMouse() const
{
	return mouse;
}

vector<2> MainWindow::GetMouseDifference()
{
	vector<2> diff = mouse - lastMouse;
	lastMouse = mouse;
	return diff;
}

bool MainWindow::IsLeftButtonDown() const
{
	return leftButtonDown;
}

bool MainWindow::IsRightButtonDown() const
{
	return rightButtonDown;
}

Surface* MainWindow::GetSurface() const
{
	return surface;
}

void MainWindow::SetCamera(Camera* camera)
{
	this->camera = camera;
}

void MainWindow::SetChessGame(ChessGame* chessGame)
{
	this->chessGame = chessGame;
}

void MainWindow::SetTitle(const wchar_t* title)
{
	SetWindowText(hWnd, title);
}

void MainWindow::Invalidate()
{
	InvalidateRect(hWnd, NULL, FALSE);
}

bool MainWindow::ProcessMessages()
{
	MSG msg;
	while(active ? PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) : GetMessage(&msg, NULL, 0, 0))
	{
		if(msg.message == WM_QUIT)
			return false;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return active;
}

void MainWindow::Run()
{
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
