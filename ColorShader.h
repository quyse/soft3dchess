#ifndef ___COLOR_SHADER_H___
#define ___COLOR_SHADER_H___

#include "ShaderParameters.h"
#include "VertexFormats.h"

//класс цветового шейдера выполняет функции сразу вершинного
//и пиксельного шейдеров
class ColorShader
{
	const ShaderParameters* shaderParameters;

public:
	enum
	{
		BlendingEnabled = 1,
		DepthWriteEnabled = 0
	};

	typedef NormalVertex VertexInput;
	typedef SimpleOutputVertex VertexOutput;
	typedef VertexOutput PixelInput;
	typedef vector<3> PixelOutput;

	ColorShader(const ShaderParameters* shaderParameters = 0);
	void SetShaderParameters(const ShaderParameters* shaderParameters);
	VertexOutput ProcessVertex(const VertexInput& input);
	PixelOutput ProcessPixel(const PixelInput& input, const PixelOutput& inputPixel);
};

#endif
