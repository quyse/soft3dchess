#include "ModelLoader.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

bool operator==(const NormalVertex& a, const NormalVertex& b)
{
	return a.position == b.position && a.normal == b.normal && a.texcoord == b.texcoord;
}

Model<NormalVertex, int>* ModelLoader::LoadNormalModel(const wchar_t *fileName)
{
	std::ifstream file;
	file.open(fileName);
	if(!file.is_open()) return 0;

	std::vector<vector<3> > points;
	std::vector<vector<3> > normals;
	std::vector<vector<2> > texPoints;
	std::string geometryName;
	std::vector<NormalVertex> vertices;

	//цикл по строкам файла
	std::string ss;
	while(std::getline(file, ss, '\n'), !file.fail())
	{
		//если пустая, пропустить
		if(!ss[0]) continue;
		//если комментарий, пропустить
		if(isspace(ss[0]) || ss[0] == '#') continue;

		//считать первое слово (команду)
		std::stringstream s(ss);
		std::string command;
		s >> command;

		//вершина
		if(command == "v")
		{
			vector<3> v;
			s >> v.x >> v.y >> v.z;
			points.push_back(v);
		}
		//нормаль к вершине
		else if(command == "vn")
		{
			vector<3> v;
			s >> v.x >> v.y >> v.z;
			normals.push_back(v);
		}
		//текстурные координаты вершины
		else if(command == "vt")
		{
			vector<3> v;
			s >> v.x >> v.y >> v.z;
			//развернуть ось Y, так как в максе начало координат текстуры внизу
			v.y = 1.0f - v.y;
			texPoints.push_back(vector<2>(v.x, v.y));
		}
		//название геометрии (хз зачем)
		else if(command == "g")
			s >> geometryName;
		//треугольник
		else if(command == "f")
		{
			char c;
			int pointNumber, texPointNumber, normalNumber;
			int i;
			for(i = 0; i < 3; ++i)
			{
				s >> pointNumber >> c >> texPointNumber >> c >> normalNumber;
				NormalVertex vertex;
				vertex.position = points[pointNumber - 1];
				vertex.normal = normals[normalNumber - 1];
				vertex.texcoord = texPoints[texPointNumber - 1];
				vertices.push_back(vertex);
			}
			//поменять порядок вершин (надо, если будем добиваться строгого порядка)
			std::swap(vertices[vertices.size() - 1], vertices[vertices.size() - 3]);
		}
	}

	//оптимизировать модель
	std::vector<std::pair<int, NormalVertex> >  newVertices(vertices.size());
	int i, j;
	for(i = 0; i < (int)vertices.size(); ++i)
		newVertices[i] = std::make_pair(i, vertices[i]);
	struct Sorter
	{
		bool operator()(const std::pair<int, NormalVertex>& a, const std::pair<int, NormalVertex>& b) const
		{
			return a.second.position < b.second.position || a.second.position == b.second.position && (a.second.normal < b.second.normal || a.second.normal == b.second.normal && a.second.texcoord < b.second.texcoord);
		}
	};
	std::sort(newVertices.begin(), newVertices.end(), Sorter());

	std::vector<int> resultIndices(vertices.size());
	std::vector<NormalVertex> resultVertices;
	for(i = 0; i < (int)newVertices.size(); )
	{
		for(j = i; j < (int)newVertices.size(); ++j)
		{
			if(!(newVertices[j].second == newVertices[i].second)) break;
			resultIndices[newVertices[j].first] = resultVertices.size();
		}

		resultVertices.push_back(newVertices[i].second);

		i = j;
	}

	return Model<NormalVertex, int>::Create(&*resultVertices.begin(), resultVertices.size(), &*resultIndices.begin(), resultIndices.size());
}
