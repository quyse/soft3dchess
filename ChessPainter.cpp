#include "ChessPainter.h"

ChessPainter::ChessPainter(const ChessResources* chessResources, const Surface::OutputDevice& outputDevice, const Environment* environment)
: chessResources(chessResources), outputDevice(outputDevice), environment(environment)
{
}

Surface::OutputDevice& ChessPainter::GetOutputDevice()
{
	return outputDevice;
}

void ChessPainter::Paint(const Camera& camera, const ChessBoardImage& chessBoardImage)
{
	//задать параметры шейдера, одинаковые для всех фигур и доски
	shaderParameters.ambientColor = vector<3>(1, 1, 1) / 10;
	shaderParameters.lightColor = vector<3>(1, 1, 1) * 0.45;
	shaderParameters.lightPosition = environment->lightPosition;
	shaderParameters.eyePosition = camera.GetPosition();
	shaderParameters.materialDiffuseCoef = 0.8;
	shaderParameters.materialSpecularCoef = 0.2;

	shaderParameters.specularColor = vector<3>(1, 1, 1);

	normalShader.SetShaderParameters(&shaderParameters);
	colorShader.SetShaderParameters(&shaderParameters);

	const scalar pi = 3.1415926535897932;
	matrix<4, 4> rotation180 = CreateRotationZMatrix(pi);

	//нарисовать фигуры
	const std::vector<ModelImage>& modelImages = chessBoardImage.modelImages;
	for(int i = 0; i < modelImages.size(); ++i)
	{
		const ModelImage& modelImage = modelImages[i];
		vector<3> position = modelImage.position;
		if(modelImage.colorTexture == 'W')
			shaderParameters.worldTransform = rotation180 * CreateTranslationMatrix(position.x, position.y, position.z);
		else
			shaderParameters.worldTransform = CreateTranslationMatrix(position.x, position.y, position.z);
		shaderParameters.worldViewProjTransform = shaderParameters.worldTransform * camera.GetViewProjTransform();
		shaderParameters.colorTexture = chessResources->GetTexture(modelImage.colorTexture);
		renderer.Render(outputDevice, *chessResources->GetModel(modelImage.model), normalShader, normalShader);
	}

	//нарисовать доску
	{
		SetMatrixIdentity(shaderParameters.worldTransform);
		shaderParameters.worldViewProjTransform = shaderParameters.worldTransform * camera.GetViewProjTransform();
		shaderParameters.colorTexture = chessResources->GetTexture('W');
		shaderParameters.diffuseColor = vector<3>(1, 1, 1);
		renderer.Render(outputDevice, *chessResources->GetModel('w'), normalShader, normalShader);
		shaderParameters.colorTexture = chessResources->GetTexture('B');
		shaderParameters.diffuseColor = vector<3>(0, 0, 0);
		renderer.Render(outputDevice, *chessResources->GetModel('b'), normalShader, normalShader);
		shaderParameters.worldTransform = CreateTranslationMatrix(0, 0, 0.1f);
		shaderParameters.worldViewProjTransform = shaderParameters.worldTransform * camera.GetViewProjTransform();
		renderer.Render(outputDevice, *chessResources->GetModel('#'), normalShader, normalShader);
	}

	//нарисовать синий маркер
	if(chessBoardImage.isBlueLight)
	{
		shaderParameters.worldTransform = CreateTranslationMatrix(chessBoardImage.blueLight.x, chessBoardImage.blueLight.y, chessBoardImage.blueLight.z);
		shaderParameters.worldViewProjTransform = shaderParameters.worldTransform * camera.GetViewProjTransform();
		shaderParameters.diffuseColor = vector<3>(0, 0, 1);
		renderer.Render(outputDevice, *chessResources->GetModel(L'%'), colorShader, colorShader);
	}
	//нарисовать желтые маркеры
	{
		const std::vector<vector<3> >& markers = chessBoardImage.yellowLights;
		shaderParameters.diffuseColor = vector<3>(0.5, 0.5, 1);
		for(int i = 0; i < markers.size(); ++i)
		{
			shaderParameters.worldTransform = CreateTranslationMatrix(markers[i].x, markers[i].y, markers[i].z);
			shaderParameters.worldViewProjTransform = shaderParameters.worldTransform * camera.GetViewProjTransform();
			renderer.Render(outputDevice, *chessResources->GetModel(L'%'), colorShader, colorShader);
		}
	}
	//нарисовать зеленые маркеры
	{
		const std::vector<vector<3> >& markers = chessBoardImage.greenLights;
		shaderParameters.diffuseColor = vector<3>(0, 1, 0);
		for(int i = 0; i < markers.size(); ++i)
		{
			shaderParameters.worldTransform = CreateTranslationMatrix(markers[i].x, markers[i].y, markers[i].z);
			shaderParameters.worldViewProjTransform = shaderParameters.worldTransform * camera.GetViewProjTransform();
			renderer.Render(outputDevice, *chessResources->GetModel(L'%'), colorShader, colorShader);
		}
	}
	//нарисовать красные маркеры
	{
		const std::vector<vector<3> >& markers = chessBoardImage.redLights;
		shaderParameters.diffuseColor = vector<3>(1, 0, 0);
		for(int i = 0; i < markers.size(); ++i)
		{
			shaderParameters.worldTransform = CreateTranslationMatrix(markers[i].x, markers[i].y, markers[i].z);
			shaderParameters.worldViewProjTransform = shaderParameters.worldTransform * camera.GetViewProjTransform();
			renderer.Render(outputDevice, *chessResources->GetModel(L'%'), colorShader, colorShader);
		}
	}

	//нарисовать огонь
	chessBoardImage.fire.Render(camera, renderer, outputDevice);
}
