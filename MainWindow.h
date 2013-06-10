#ifndef ___MAIN_WINDOW_H___
#define ___MAIN_WINDOW_H___

#include "Surface.h"
#include "Camera.h"
#include "Connection.h"
#include "ChessGame.h"
#include <vector>//TEST

class MainWindow
{
private:
	HWND hWnd;
	Surface* surface;
	bool active;
	vector<2> mouse;
	bool leftButtonDown, rightButtonDown;
	vector<2> lastMouse;
	int mouseCaptureCount;
	//координаты выбранной фигуры
	int selectedFigureI, selectedFigureJ;

	Camera* camera;
	ChessGame* chessGame;
	Connection* connection;

	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	vector<2> ConvertCoords(int x, int y) const;
	void CaptureMouse(bool capture);

	void CheckFinish() const;

	void OnPaint(HWND hWnd);
	void OnDestroy(HWND hWnd);
	void OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify);
	void OnLButtonDown(HWND hWnd, BOOL doubleClick, int x, int y, UINT keyFlags);
	void OnRButtonDown(HWND hWnd, BOOL doubleClick, int x, int y, UINT keyFlags);
	void OnLButtonUp(HWND hWnd, int x, int y, UINT keyFlags);
	void OnRButtonUp(HWND hWnd, int x, int y, UINT keyFlags);
	void OnMouseMove(HWND hWnd, int x, int y, UINT keyFlags);
	void OnSize(HWND hWnd, UINT state, int cx, int cy);
	void OnConnection(HWND hWnd, WPARAM wParam, LPARAM lParam);

	static ATOM wndClass;

public:
	std::vector<int> testLines;

	MainWindow();
	~MainWindow();

	bool IsActive() const;
	vector<2> GetMouse() const;
	vector<2> GetMouseDifference();
	bool IsLeftButtonDown() const;
	bool IsRightButtonDown() const;

	Surface* GetSurface() const;
	void SetCamera(Camera* camera);
	void SetChessGame(ChessGame* chessGame);

	void SetTitle(const wchar_t* title);

	void Invalidate();
	bool ProcessMessages();
	void Run();
};

#endif
