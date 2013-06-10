#ifndef ___NORMAL_SHADER_H___
#define ___NORMAL_SHADER_H___

#include "ShaderParameters.h"
#include "VertexFormats.h"

//класс нормального шейдера выполняет функции сразу вершинного
//и пиксельного шейдеров
class NormalShader
{
	const ShaderParameters* shaderParameters;

public:
	enum
	{
		BlendingEnabled = 0,
		DepthWriteEnabled = 1
	};

	typedef NormalVertex VertexInput;
	typedef NormalOutputVertex VertexOutput;
	typedef VertexOutput PixelInput;
	typedef vector<3> PixelOutput;

	NormalShader(const ShaderParameters* shaderParameters = 0);
	void SetShaderParameters(const ShaderParameters* shaderParameters);
	VertexOutput ProcessVertex(const VertexInput& input);
	PixelOutput ProcessPixel(const PixelInput& input);
	PixelOutput ProcessPixel(const PixelInput& input, const PixelOutput& inputPixel);
};

#endif
