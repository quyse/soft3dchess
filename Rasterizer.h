
#ifndef ___RENDERING_H___
#define ___RENDERING_H___

#include "math.h"

/*
����� ������������� ��������� ������������ ������������� � ������� ����������� ������� � �������� �����.
PixelShader::PixelInput
	typename Interpolator
		static PixelShader::PixelInput Inverpolate(PixelShader::PixelInput, PixelShader::PixelInput, scalar coef);
PixelShader - ���������� ������
		PixelShader::PixelOutput ProcessPixel(PixelShader::PixelInput);
		���
		PixelShader::PixelOutput ProcessPixel(PixelShader::PixelInput, PixelShader::PixelOutput);
		���� PixelShader::BlendingEnabled == true
	PixelShader::DepthWriteEnabled
	typename PixelInput;
	typename PixelOutput;
		vector<3> target;
OutputDevice - ���������� ������
	int left, top, right, bottom;//������� ������
	int width, height;//������� ������
	bool DepthTest(int x, int y, scalar z, bool write);//��������� depth-����; true, ���� ������ ������������
	PixelShader::PixelOutput GetPixel(int x, int y) const;//�������� ������
	void SetPixel(int x, int y, PixelShader::PixelOutput);//���������� ������
Vertex - ���������, ���������� �� ���������� �������, � ������������ �����������
*/
template <typename OutputDevice, typename PixelShader>
class Rasterizer
{
	OutputDevice& outputDevice;
	PixelShader& pixelShader;
	
	//�������������� ���� ��� ������� ������
	vector<4> ViewportTransform(const vector<4>& position)
	{
		vector<4> resultPosition(position);
		resultPosition.x = (1 + resultPosition.x) * outputDevice.width / 2;
		resultPosition.y = (1 - resultPosition.y) * outputDevice.height / 2;
		return resultPosition;
	}

	//����������, �������������� ��� ����������
	typedef typename PixelShader::PixelInput Vertex;
	typedef typename Vertex::Interpolator Interpolator;

	//��������������� ���������, �������������� ��� ���������
	struct Coord
	{
		scalar x, y;
		int ix, iy;
		Vertex vertex;
	};

	//��������������� �������, ��� ���������� �������� �� ����
	template <typename T, typename TT, TT T::* F>
	struct FieldSorter
	{
		inline bool operator()(const T& a, const T& b) const
		{
			return a.*F < b.*F;
		}
	};

	//���������� ���� �������� �� ����
	template <typename T, typename TT, TT T::* F>
	static inline void TwoSort(T& a, T& b)
	{
		if(b.*F < a.*F)
		{
			T c = a;
			a = b;
			b = c;
		}
	}

	//���������� ���� �������� �� ����
	template <typename T, typename TT, TT T::* F>
	static inline void ThreeSort(T* a)
	{
		TwoSort<T, TT, F>(a[0], a[1]);
		TwoSort<T, TT, F>(a[0], a[2]);
		TwoSort<T, TT, F>(a[1], a[2]);
	}

public:
	Rasterizer(OutputDevice& outputDevice, PixelShader& pixelShader)
		: outputDevice(outputDevice), pixelShader(pixelShader)
	{
	}

	inline void OutputPixel(int blending, const Vertex& vertex, int x, int y)
	{
		if(blending)
			outputDevice.SetPixel(x, y, pixelShader.ProcessPixel(vertex, outputDevice.GetPixel(x, y)));
		else
			outputDevice.SetPixel(x, y, pixelShader.ProcessPixel(vertex));
	}

	//������������� ������������
	template <typename Index>
	void DrawIndexed(const Vertex* vertices, const Index* indices, int indicesCount)
	{
		//���� �� �������������
		for(int i = 0; i < indicesCount; i += 3)
		{
			//����������� �������� �����
			if(crossxy(
				vertices[indices[i + 1]].position - vertices[indices[i]].position,
				vertices[indices[i + 2]].position - vertices[indices[i]].position) > 0)
				continue;

			//������������ ��� ������� ���������, � ����������� �� ������ �����������
			Coord coordsX[3], coordsY[3];
			for(int j = 0; j < 3; ++j)
			{
				const Vertex& vertex = vertices[indices[i + j]];
				vector<4> position = ViewportTransform(vertex.position);
				coordsX[j].x = position.x;
				coordsX[j].y = position.y;
				coordsX[j].ix = (int)std::ceil(position.x);
				coordsX[j].iy = (int)std::ceil(position.y);
				coordsX[j].vertex = vertex;
				coordsY[j] = coordsX[j];
			}

			//������������� ������� ��������� �� X � �� Y
			ThreeSort<Coord, scalar, &Coord::x>(coordsX);
			ThreeSort<Coord, scalar, &Coord::y>(coordsY);

			//�������� ������ � ������� ����������� ��� ������������
			int beginY = std::max(coordsY[0].iy, outputDevice.top);
			int endY = std::min(coordsY[2].iy, outputDevice.bottom);

			//���� ������������ �� Y
			for(int y = beginY; y < endY; ++y)
			{
				//������� �������
				scalar longCoef = (y - coordsY[0].y) / (coordsY[2].y - coordsY[0].y);
				scalar beginX = coordsY[2].x * longCoef + coordsY[0].x * (1 - longCoef);
				Vertex beginVertex = Interpolator::Interpolate(coordsY[0].vertex, coordsY[2].vertex, longCoef);
				//�������� �������
				scalar shortCoef;
				scalar endX;
				Vertex endVertex;
				if(y < coordsY[1].y)
				{
					shortCoef = (y - coordsY[0].y) / (coordsY[1].y - coordsY[0].y);
					endX = coordsY[1].x * shortCoef + coordsY[0].x * (1 - shortCoef);
					endVertex = Interpolator::Interpolate(coordsY[0].vertex, coordsY[1].vertex, shortCoef);
				}
				else
				{
					shortCoef = (y - coordsY[1].y) / (coordsY[2].y - coordsY[1].y);
					endX = coordsY[2].x * shortCoef + coordsY[1].x * (1 - shortCoef);
					endVertex = Interpolator::Interpolate(coordsY[1].vertex, coordsY[2].vertex, shortCoef);
				}

				//���� ���, �������� ��, ����� ���� ���������
				if(beginX > endX)
				{
					std::swap(beginX, endX);
					std::swap(beginVertex, endVertex);
				}

				//���������� ����� ��������� ������
				int ibeginX = std::max((int)beginX, outputDevice.left);
				int iendX = std::min((int)endX, outputDevice.right);

				//���� ������������ �� X
				for(int x = ibeginX; x < iendX; ++x)
				{
					//��������� �����������
					scalar coef = (x - beginX) / (endX - beginX);
					//��������������� �������
					Vertex vertex = Interpolator::Interpolate(beginVertex, endVertex, coef);
					//��������� ���� �������
					if(outputDevice.DepthTest(x, y, vertex.position.z, PixelShader::DepthWriteEnabled))
						//��������� ���������� ������
						OutputPixel(PixelShader::BlendingEnabled, vertex, x, y);
				}
			}
		}
	}
};

#endif
