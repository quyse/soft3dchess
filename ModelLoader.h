#ifndef ___MODEL_LOADER_H___
#define ___MODEL_LOADER_H___

#include "Model.h"
#include "VertexFormats.h"

/*
���� �������� ����� ���������� ������� �� ������ ������� Wavefront OBJ.
*/

class ModelLoader
{
private:

public:
	Model<NormalVertex, int>* LoadNormalModel(const wchar_t* fileName);
};

#endif
