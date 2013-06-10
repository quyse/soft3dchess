#ifndef ___VERTICER_H___
#define ___VERTICER_H___

/*
Класс обработчика вершин выполняет обработку вершин вершинным шейдером.
VertexShader - вершинный шейдер
	VertexShader::Output ProcessVertex(VertexShader::Input);
	typename VertexInput;
	typename VertexOutput;
		vector<4> position;
*/

template <typename VertexShader>
class Verticer
{
public:
	static void ProcessVertices(VertexShader& vertexShader, int verticesCount, const typename VertexShader::VertexInput* vertices, typename VertexShader::VertexOutput* resultVertices)
	{
		//выполнить обработку вершин
		for(int i = 0; i < verticesCount; ++i)
		{
			VertexShader::VertexOutput& resultVertex = resultVertices[i];
			//применить вершинный шейдер
			resultVertex = vertexShader.ProcessVertex(vertices[i]);
			//перевести в пространство экрана
			resultVertex.position = resultVertex.position / resultVertex.position.w;
		}
	}
};

#endif
