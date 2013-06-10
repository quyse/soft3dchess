#ifndef ___VERTICER_H___
#define ___VERTICER_H___

/*
����� ����������� ������ ��������� ��������� ������ ��������� ��������.
VertexShader - ��������� ������
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
		//��������� ��������� ������
		for(int i = 0; i < verticesCount; ++i)
		{
			VertexShader::VertexOutput& resultVertex = resultVertices[i];
			//��������� ��������� ������
			resultVertex = vertexShader.ProcessVertex(vertices[i]);
			//��������� � ������������ ������
			resultVertex.position = resultVertex.position / resultVertex.position.w;
		}
	}
};

#endif
