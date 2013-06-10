#include "NormalShader.h"
#include <algorithm>

NormalShader::NormalShader(const ShaderParameters* shaderParameters) : shaderParameters(shaderParameters)
{
}

void NormalShader::SetShaderParameters(const ShaderParameters* shaderParameters)
{
	this->shaderParameters = shaderParameters;
}

NormalShader::VertexOutput NormalShader::ProcessVertex(const VertexInput& input)
{
	VertexOutput output;
	output.position = input.position * shaderParameters->worldViewProjTransform;
	output.normal = input.normal;
	output.texcoord = input.texcoord;
	vector<4> worldPosition = input.position * shaderParameters->worldTransform;
	output.worldPosition = vector<3>(worldPosition.x, worldPosition.y, worldPosition.z);
	return output;
}

NormalShader::PixelOutput NormalShader::ProcessPixel(const PixelInput& input)
{
	//Фонг
//	vector<3> toLight = normalize(shaderParameters->lightPosition - input.worldPosition);
	vector<3> normal = normalize(input.normal);
	vector<3> toLight = normalize(input.worldPosition - shaderParameters->lightPosition);
	vector<3> toEye = normalize(shaderParameters->eyePosition - input.worldPosition);
	vector<3> reflected = normalize(normal * dot(normal, toLight) * 2 - toLight);
//	vector<3> sample(1, 1, 1);
	vector<3> sample = shaderParameters->colorTexture->Sample(input.texcoord);
/*	return sample * (shaderParameters->ambientColor + shaderParameters->lightColor * (
		std::max(dot(input.normal, toLight), scalar()) * shaderParameters->materialDiffuseCoef +
		sqr(sqr(sqr(sqr(std::max(dot(toEye, reflected), scalar()))))) * shaderParameters->materialSpecularCoef));*/
	//вместо max - fabs
	return sample * (shaderParameters->ambientColor + shaderParameters->lightColor * (
		fabs(dot(normal, toLight)) * shaderParameters->materialDiffuseCoef +
		sqr(sqr(sqr(sqr(std::max(dot(toEye, reflected), scalar()))))) * shaderParameters->materialSpecularCoef));

/*	vector<3> normal = normalize(input.normal);
	vector<3> sample = shaderParameters->colorTexture->Sample(input.texcoord);
	return shaderParameters->lightColor * sample * (shaderParameters->diffuseColor * std::max(dot(toLight, normal), scalar()) + shaderParameters->specularColor * sqr(sqr(std::max(dot(normalize(toEye + toLight), normal), scalar()))));*/

/*	static const vector<3> warmColor = vector<3>(147, 181, 238) / 255;
	static const vector<3> coldColor = vector<3>(41, 109, 222) / 255;
//	return lerp(warmColor, coldColor, (dot(toLight, normalize(input.normal)) + 1) / 2);
	return lerp(shaderParameters->warmTexture->Sample(input.texcoord), shaderParameters->coldTexture->Sample(input.texcoord), (dot(toLight, normalize(input.normal)) + 1) / 2);*/

/*	//аппроксимация Шлика (взято из http://books.google.com/books?id=pCwwxlMuNycC&pg=PA132&dq=shader&ei=ULc9S6zLL4egzQTS9qi-DA&hl=ru&cd=2#v=onepage&q=shader&f=false)
	vector<3> toEye = normalize(shaderParameters->eyePosition - input.worldPosition);
	vector<3> toLight = normalize(shaderParameters->lightPosition - input.worldPosition);
	vector<3> reflected = normalize(input.normal * dot(input.normal, toLight) * 2 - toLight);
	vector<3> sample = shaderParameters->colorTexture->Sample(input.texcoord);
	scalar diffuse = std::max(dot(toLight, input.normal), 0.0f);
	scalar specular = std::max(sqr(sqr(dot(reflected, toEye))), 0.0f);
	return sample * (shaderParameters->ambientColor + shaderParameters->lightColor * (
		diffuse * shaderParameters->materialDiffuseCoef +
		specular * shaderParameters->materialSpecularCoef));*/
}

NormalShader::PixelOutput NormalShader::ProcessPixel(const PixelInput& input, const PixelOutput& inputPixel)
{
	return PixelOutput();
}
