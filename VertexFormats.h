#ifndef ___VERTEX_FORMATS_H___
#define ___VERTEX_FORMATS_H___

#include "math.h"

struct TexVertex
{
	vector<3> position;
	vector<2> texcoord;
};

struct TexOutputVertex
{
	vector<4> position;
	vector<2> texcoord;

	struct Interpolator
	{
		static inline TexOutputVertex Interpolate(const TexOutputVertex& a, const TexOutputVertex& b, scalar coef)
		{
			TexOutputVertex r;
			r.position = a.position * (1 - coef) + b.position * coef;
			r.texcoord = a.texcoord * (1 - coef) + b.texcoord * coef;
			return r;
		}
	};
};

struct NormalVertex
{
	vector<3> position;
	vector<3> normal;
	vector<2> texcoord;
};

struct NormalOutputVertex
{
	vector<4> position;
	vector<3> normal;
	vector<2> texcoord;
	vector<3> worldPosition;

	struct Interpolator
	{
		static inline NormalOutputVertex Interpolate(const NormalOutputVertex& a, const NormalOutputVertex& b, scalar coef)
		{
			NormalOutputVertex r;
			r.position = a.position * (1 - coef) + b.position * coef;
			r.normal = a.normal * (1 - coef) + b.normal * coef;
			r.texcoord = a.texcoord * (1 - coef) + b.texcoord * coef;
			r.worldPosition = a.worldPosition * (1 - coef) + b.worldPosition * coef;
			return r;
		}
	};
};

struct SimpleOutputVertex
{
	vector<4> position;
	vector<3> sourcePosition;

	struct Interpolator
	{
		static inline SimpleOutputVertex Interpolate(const SimpleOutputVertex& a, const SimpleOutputVertex& b, scalar coef)
		{
			SimpleOutputVertex r;
			r.position = lerp(a.position, b.position, coef);
			r.sourcePosition = lerp(a.sourcePosition, b.sourcePosition, coef);
			return r;
		}
	};
};

struct ParticleVertex
{
	vector<2> position;
};

struct ParticleOutputVertex
{
	vector<4> position;
	vector<2> sourcePosition;

	struct Interpolator
	{
		static inline ParticleOutputVertex Interpolate(const ParticleOutputVertex& a, const ParticleOutputVertex& b, scalar coef)
		{
			ParticleOutputVertex r;
			r.position = lerp(a.position, b.position, coef);
			r.sourcePosition = lerp(a.sourcePosition, b.sourcePosition, coef);
			return r;
		}
	};
};

#endif
