#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>
#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"
#include <array>
#include <Texture.h>



struct ParticleProps
{
	glm::vec3 Position;
	glm::vec3 Velocity;
	glm::vec3 Color;
	float Lifetime = 10.0f;
};

class ParticleSystem
{
public:
	ParticleSystem(unsigned int particleCount, const Texture& texture);
	void Update(float timeStep);
	void Update(float timeStep, const glm::vec3& acceleration);
	void OnRender(Camera& camera);
	void Emit(const ParticleProps particleProps);
	void SetTransform(const glm::mat4& transMat);
	const unsigned int GetParticleCount();
	friend class ParticleHandler;
public:
	struct Particle 
	{
		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec3 color;
		float lifePercent = 1.0f;
		float size;
		float lifetime = 1.0f;
		float lifeRemaining = 1.0f;
		bool active = false;
		
		int row;
		int col;
	};
	std::vector<Particle> particlePool;
	unsigned int activeParticles;
private:
	struct Vertex
	{
		glm::vec3 Position;
		float lifePercent;
	};
private:
	size_t poolInd;
	unsigned int particleCount;
	VertexArray particleVA;
	VertexBuffer particleVB;
	Shader particleShader;
	Texture particleTexture;
	glm::mat4 transformMat;
	
};

