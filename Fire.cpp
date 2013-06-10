#include "Fire.h"
#include "windows.h"

struct FireStatic
{
	Model<ParticleVertex, int>* particleModel;

	FireStatic()
	{
		const ParticleVertex vertices[4] =
		{
			{ vector<2>(-1, -1) },
			{ vector<2>(-1, 1) },
			{ vector<2>(1, 1) },
			{ vector<2>(1, -1) }
		};
		const int indices[6] = { 0, 1, 2, 0, 2, 3 };
		particleModel = Model<ParticleVertex, int>::Create(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
	}
} fireStatic;


struct FireShader
{
	enum
	{
		BlendingEnabled = 1,
		DepthWriteEnabled = 0
	};

	typedef ParticleVertex VertexInput;
	typedef ParticleOutputVertex VertexOutput;
	typedef VertexOutput PixelInput;
	typedef vector<3> PixelOutput;

	vector<4> currentParticlePosition;
	scalar particleSize;
	vector<3> color;
	scalar alpha;
	matrix<4, 4> projTransform;

	VertexOutput ProcessVertex(const VertexInput& input)
	{
		VertexOutput output;
		output.position = currentParticlePosition + vector<4>(input.position.x, input.position.y, 0, 0) * projTransform * particleSize;
		output.sourcePosition = input.position;
		return output;
	}

	PixelOutput ProcessPixel(const PixelInput& input) { return vector<3>(); }

	PixelOutput ProcessPixel(const PixelInput& input, const PixelOutput& inputPixel)
	{
		return lerp(inputPixel, color, alpha * (1.0f - std::min(sqr(input.sourcePosition.x) + sqr(input.sourcePosition.y), 1.0f)));
	}
};

void Fire::Initialize()
{
	//задать случайные направления
	directions.resize(particles.size());
	for(int i = 0; i < particles.size(); ++i)
		directions[i] = vector<3>(
			scalar(rand()) * 2 / RAND_MAX - 1,
			scalar(rand()) * 2 / RAND_MAX - 1,
			scalar(rand()) * 2 / RAND_MAX - 1);

	//включить эффект
	enabled = true;
	//установить начальное время
	beginTime = GetTickCount();
}

static const scalar maxTime = 1000;

void Fire::CheckFinish()
{
	time = GetTickCount() - beginTime;
	if(time > maxTime)
		enabled = false;
}

void Fire::Render(const Camera& camera, Renderer<Surface::OutputDevice>& renderer, Surface::OutputDevice& outputDevice) const
{
	if(!enabled)
		return;

	scalar time2 = sqr(time);

	FireShader shader;
	shader.color = vector<3>(1, 0, 0);
	shader.alpha = 1.0f - time / maxTime;
	shader.projTransform = camera.GetProjTransform();
	shader.particleSize = 0.25f;

	for(int i = 0; i < particles.size(); ++i)
	{
		//вычислить текущее положение частицы
		vector<3> position = particles[i] + directions[i] * time2 * 0.000001;

		shader.currentParticlePosition = position * camera.GetViewProjTransform();
		renderer.Render(outputDevice, *fireStatic.particleModel, shader, shader);
	}
}
