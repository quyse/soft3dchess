#ifndef ___CHESS_PAINTER_H___
#define ___CHESS_PAINTER_H___

#include "ShaderParameters.h"
#include "NormalShader.h"
#include "ColorShader.h"
#include "Renderer.h"
#include "ChessResources.h"
#include "ChessBoardImage.h"
#include "Camera.h"
#include "Surface.h"
#include "Environment.h"

class ChessPainter
{
private:
	const Environment* environment;
	ShaderParameters shaderParameters;
	const ChessResources* chessResources;
	Surface::OutputDevice outputDevice;
	NormalShader normalShader;
	ColorShader colorShader;
	Renderer<Surface::OutputDevice> renderer;

public:
	ChessPainter(const ChessResources* chessResources, const Surface::OutputDevice& outputDevice, const Environment* environment);

	Surface::OutputDevice& GetOutputDevice();
	void Paint(const Camera& camera, const ChessBoardImage& chessBoardImage);
};

#endif
