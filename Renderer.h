#ifndef ___RENDERER_H___
#define ___RENDERER_H___

#include "Verticer.h"
#include "Rasterizer.h"
#include "Model.h"
#include "MemoryBuffer.h"

template <typename OutputDevice>
class Renderer
{
private:
	MemoryBuffer vertexBuffer;

public:
	template <typename Vertex, typename Index, typename VertexShader, typename PixelShader>
	void Render(OutputDevice& outputDevice, const Model<Vertex, Index>& model, VertexShader& vertexShader, PixelShader& pixelShader)
	{
		vertexBuffer.Resize<VertexShader::VertexOutput>(model.GetVerticesCount());
		VertexShader::VertexOutput* outputVertices = vertexBuffer.GetPointer<VertexShader::VertexOutput>();
		Verticer<VertexShader>::ProcessVertices(vertexShader, model.GetVerticesCount(), model.GetVertices(), outputVertices);
		Rasterizer<OutputDevice, PixelShader>(outputDevice, pixelShader).DrawIndexed(outputVertices, model.GetIndices(), model.GetIndicesCount());
	}
};

#endif
