#include "MainWindow.h"
#include "ChessPainter.h"
#include "ChessGame.h"
#include "MultiCore.h"
#include "Resources.h"
#include <commctrl.h>

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, INT)
{
	InitCommonControls();

	MainWindow* window = new MainWindow;

	Surface& surface = *window->GetSurface();

	struct PaintHelper
	{
		ChessPainter* chessPainter;
		const Camera* camera;
		const ChessBoardImage* chessBoardImage;

		PaintHelper(ChessPainter* chessPainter, const Camera* camera, const ChessBoardImage* chessBoardImage)
			: chessPainter(chessPainter), camera(camera), chessBoardImage(chessBoardImage)
		{
		}

		void Paint()
		{
			chessPainter->Paint(*camera, *chessBoardImage);
		}

		static void RunTestPainter(void* paintHelper)
		{
			((PaintHelper*)paintHelper)->Paint();
		}
	};

	MultiCore multiCore;

	std::wstring title;
	{
		int id;
		switch(multiCore.GetCoreCount())
		{
		case 1:
			id = IDS_TITLE_SINGLE_CORE;
			break;
		case 2:
			id = IDS_TITLE_DUAL_CORE;
			break;
		case 4:
			id = IDS_TITLE_QUAD_CORE;
			break;
		default:
			id = IDS_TITLE_MULTI_CORE;
			break;
		}
		title = GetString(id);
	}

	Camera camera;
	const vector<3> target(0, 0, 0);
	camera.SetTarget(target);

	ChessResources chessResources;

	ChessGame chessGame(chessResources);

	window->SetCamera(&camera);
	window->SetChessGame(&chessGame);

	Environment environment;

	int coreCount = multiCore.GetCoreCount();

	std::vector<ChessPainter> chessPainters;
	chessPainters.reserve(coreCount);
	for(int i = 0; i < coreCount; ++i)
		chessPainters.push_back(ChessPainter(&chessResources, surface.GetOutputDevice(0, 0, 0, 0), &environment));
	std::vector<PaintHelper> paintHelpers;
	paintHelpers.reserve(coreCount);
	for(int i = 0; i < coreCount; ++i)
	{
		paintHelpers.push_back(PaintHelper(&chessPainters[i], &camera, &chessGame.GetBoardImage()));
		multiCore.SetWork(i, PaintHelper::RunTestPainter, &paintHelpers[i]);
	}

	unsigned beginTick = GetTickCount();

	int frame = 0;

	//инициализировать разделительные линии
	std::vector<scalar> coreLines(coreCount - 1);
	for(int i = 0; i < coreLines.size(); ++i)
		coreLines[i] = scalar(i + 1) / coreCount;

	while(window->ProcessMessages())
	{
		frame++;
		const scalar viewDistance = 20;
		const scalar lightDistance = 2;
		const scalar pi = 3.1415926535897932;

		//угол поворота камеры вокруг оси Z
		static scalar alpha = pi / 8;
		//угол поворота камеры относительно горизонта
		static scalar beta = pi / 4;

		if(window->IsRightButtonDown())
		{
			vector<2> mouseDiff = window->GetMouseDifference();
			alpha -= mouseDiff.x / 2;
			beta -= mouseDiff.y / 2;
		}

		//обновить изображение доски
		chessGame.UpdateBoardImage();

		//получить текущие размеры поверхности
		int width = surface.GetWidth();
		int height = surface.GetHeight();
		camera.SetScreenSize(width, height);

		camera.SetPosition(target + vector<3>(cos(alpha) * cos(beta), sin(alpha) * cos(beta), sin(beta)) * viewDistance);
		camera.Update();

		const scalar lightAlpha = scalar(frame) / 10;
//		environment.lightPosition = target + vector<3>(0, 0, 20) + vector<3>(cos(lightAlpha * 10), sin(-lightAlpha * 10), 0) * lightDistance;
		environment.lightPosition = target + vector<3>(0, 0, 10);

		//перенастроить выходные устройства
		{
			scalar lastLine = 0;
			for(int i = 0; i < coreCount; ++i)
			{
				Surface::OutputDevice& outputDevice = chessPainters[i].GetOutputDevice();
				outputDevice.width = width;
				outputDevice.height = height;
				outputDevice.left = 0;
				outputDevice.right = width;
			}
			for(int i = 0; i < coreLines.size(); ++i)
			{
				int h = coreLines[i] * height;
				chessPainters[i].GetOutputDevice().bottom = h;
				chessPainters[i + 1].GetOutputDevice().top = h;
			}
			chessPainters[0].GetOutputDevice().top = 0;
			chessPainters[coreCount - 1].GetOutputDevice().bottom = height;
		}

		window->testLines.resize(coreLines.size());
		for(int i = 0; i < coreLines.size(); ++i)
			window->testLines[i] = (int)(coreLines[i] * height);

		environment.selectedCell = chessGame.PickCell(window->GetMouse(), camera);

		surface.Clear(vector<3>(0, 0, 0));
		multiCore.DoWork();
		surface.CalculateCoreLines(coreLines);
		window->Invalidate();

		static const int frameCount = 32;
		if(frame % frameCount == 0)
		{
			wchar_t buf[1000];
			int tick = GetTickCount();
			_swprintf(buf, title.c_str(), double(frameCount) * 1000 / (tick - beginTick));
			beginTick = tick;
			window->SetTitle(buf);
		}
	}

	return 0;
}
