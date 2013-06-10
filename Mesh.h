#ifndef ___MODEL_H___
#define ___MODEL_H___

#include <Verticer.h>
#include <algorithm>

/*
Файл содержит класс полигональной сетки, или проще говоря, совокупности вершин и индексов
*/

template <typename InputVertex, typename OutputVertex, typename Index = short int>
class Mesh
{
	std::vector<InputVertex> inputVertices;
	std::vector<OutputVertex> outputVertices;
	std::vector<Index> indices;

public:
	Mesh(const std::vector<InputVertex>& inputVertices, const std::vector<Index>& indices)
		: inputVertices(inputVertices), outputVertices(inputVertices.size()), indices(indices)
	{
	}

	template <typename VertexShader>
	void Prepare()
	{
		Verticer<VertexShader>::ProcessVertices((int)inputVertices.size(), &*inputVertices.begin(), &*outputVertices.begin());
	}

	template <typename PixelShader, typename OutputDevice>
	void Paint()
	{
		Rasterizer<PixelShader, OutputDevice>::DrawIndexed(&*outputVertices.begin(), (int)outputVertices.size(), &*indices.begin(), (int)indices.size());
	}
};

#endif
