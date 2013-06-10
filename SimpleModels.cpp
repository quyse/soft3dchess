#include "Model.h"
#include "VertexFormats.h"

template <int alphaCount, int betaCount>
Model<TexVertex, int>* CreateSphere(scalar radius)
{
	const scalar pi = 3.1415926535897932;
	TexVertex vertices[alphaCount * betaCount];
	for(int i = 0; i < alphaCount; ++i)
	{
		scalar alpha = i * pi * 2 / alphaCount;
		scalar x = cos(alpha);
		scalar y = sin(alpha);
		for(int j = 0; j < betaCount; ++j)
		{
			scalar beta = j * pi / betaCount;
			scalar z = cos(beta);
			scalar betaSin = sin(beta);
			vertices[i * betaCount + j].position = vector<3>(x * betaSin * radius, y * betaSin * radius, z * radius);
			vertices[i * betaCount + j].texCoord = vector<2>(scalar(j) / betaCount, scalar(i) / alphaCount);
		}
	}
	const int betaCount2 = betaCount - 1;
	int indices[alphaCount * betaCount2 * 6];
	for(int i = 0; i < alphaCount; ++i)
		for(int j = 0; j < betaCount2; ++j)
		{
			int ii = i < alphaCount - 1 ? (i + 1) : 0;
			int jj = j + 1;
			int p = (i * betaCount2 + j) * 6;
			indices[p] = i * betaCount + j;
			indices[p + 1] = i * betaCount + jj;
			indices[p + 2] = ii * betaCount + jj;
			indices[p + 3] = ii * betaCount + jj;
			indices[p + 4] = ii * betaCount + j;
			indices[p + 5] = i * betaCount + j;
		}
	return Model<TexVertex, int>::Create(vertices, alphaCount * betaCount, indices, alphaCount * betaCount2 * 6);
}

template <int alphaCount, int betaCount>
Model<TexVertex, int>* CreateTorus(scalar radius1, scalar radius2)
{
	const scalar pi = 3.1415926535897932;
	TexVertex vertices[alphaCount * betaCount];
	for(int i = 0; i < alphaCount; ++i)
	{
		scalar alpha = i * pi * 2 / alphaCount;
		scalar alphaCos = cos(alpha);
		scalar alphaSin = sin(alpha);
		for(int j = 0; j < betaCount; ++j)
		{
			scalar beta = j * pi * 2 / betaCount;
			scalar len = radius1 + radius2 * cos(beta);
			vertices[i * betaCount + j].position = vector<3>(len * alphaCos, len * alphaSin, radius2 * sin(beta));
			vertices[i * betaCount + j].texcoord = vector<2>(scalar(j) / betaCount, scalar(i) / alphaCount);
		}
	}
	int indices[alphaCount * betaCount * 6];
	for(int i = 0; i < alphaCount; ++i)
		for(int j = 0; j < betaCount; ++j)
		{
			int ii = i < alphaCount - 1 ? (i + 1) : 0;
			int jj = j < betaCount - 1 ? (j + 1) : 0;
			int p = (i * betaCount + j) * 6;
			indices[p] = i * betaCount + j;
			indices[p + 1] = i * betaCount + jj;
			indices[p + 2] = ii * betaCount + jj;
			indices[p + 3] = ii * betaCount + jj;
			indices[p + 4] = ii * betaCount + j;
			indices[p + 5] = i * betaCount + j;
		}
	return Model<TexVertex, int>::Create(vertices, alphaCount * betaCount, indices, alphaCount * betaCount * 6);
}
