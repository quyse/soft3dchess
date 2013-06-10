#include "ColorShader.h"
#include <algorithm>

ColorShader::ColorShader(const ShaderParameters* shaderParameters) : shaderParameters(shaderParameters)
{
}

void ColorShader::SetShaderParameters(const ShaderParameters* shaderParameters)
{
	this->shaderParameters = shaderParameters;
}

ColorShader::VertexOutput ColorShader::ProcessVertex(const VertexInput& input)
{
	VertexOutput output;
	output.position = input.position * shaderParameters->worldViewProjTransform;
	output.sourcePosition = input.position;
	return output;
}

ColorShader::PixelOutput ColorShader::ProcessPixel(const PixelInput& input)
{
	return PixelOutput();
}

ColorShader::PixelOutput ColorShader::ProcessPixel(const PixelInput& input, const PixelOutput& inputPixel)
{
	return lerp(inputPixel, shaderParameters->diffuseColor, std::max(1.0f - fabs(0.625f - length(input.sourcePosition)) * 1.6f, 0.0f));
}
