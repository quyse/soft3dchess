#ifndef ___MODEL_H___
#define ___MODEL_H___

#include <vector>
#include <algorithm>

template <typename ModelVertex, typename ModelIndex>
class Model
{
	typedef ModelVertex Vertex;

	const ModelVertex* vertices;
	int verticesCount;
	const ModelIndex* indices;
	int indicesCount;

	Model(const ModelVertex* vertices, int verticesCount, const ModelIndex* indices, int indicesCount)
		: vertices(vertices), verticesCount(verticesCount), indices(indices), indicesCount(indicesCount)
	{
	}

public:
	~Model()
	{
		delete [] vertices;
		delete [] indices;
	}

	const ModelVertex* GetVertices() const
	{
		return vertices;
	}

	const ModelIndex* GetIndices() const
	{
		return indices;
	}

	int GetVerticesCount() const
	{
		return verticesCount;
	}

	int GetIndicesCount() const
	{
		return indicesCount;
	}

	static Model<ModelVertex, ModelIndex>* Create(const ModelVertex* vertices, int verticesCount, const ModelIndex* indices, int indicesCount)
	{
		ModelVertex* verticesCopy = new ModelVertex[verticesCount];
		std::copy(vertices, vertices + verticesCount, verticesCopy);
		ModelIndex* indicesCopy = new ModelIndex[indicesCount];
		std::copy(indices, indices + indicesCount, indicesCopy);
		return new Model<ModelVertex, ModelIndex>(verticesCopy, verticesCount, indicesCopy, indicesCount);
	}

	template <typename Rasterizer>
	void Draw(Rasterizer& rasterizer)
	{
		Rasterizer::DrawIndexed(&*vertices.begin(), vertices.size(), &*indices.begin(), indices.size());
	}
};

#endif
