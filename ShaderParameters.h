#ifndef ___SHADER_PARAMETERS_H___
#define ___SHADER_PARAMETERS_H___

#include "math.h"
#include "Texture.h"

struct ShaderParameters
{
	matrix<4, 4> worldTransform;
	matrix<4, 4> worldViewProjTransform;
	vector<3> eyePosition;
	vector<3> lightPosition;
	vector<3> diffuseColor;
	vector<3> specularColor;
	vector<3> ambientColor;
	vector<3> lightColor;
	scalar materialDiffuseCoef;
	scalar materialSpecularCoef;
	Texture* colorTexture;
};

#endif
