#include "ParticleSystem.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <iostream>
#include <cmath>




ParticleSystem::ParticleSystem(unsigned int particleNum, const Texture& tex)
	:
	particleCount(particleNum),
	poolInd(0),
	particleVA(),
	particleVB(sizeof(Vertex) * 4 * particleNum),
	particleShader("res/shaders/particleVert.shader", "res/shaders/particleGeom.shader", "res/shaders/particleFrag.shader"),
	particleTexture(tex),
	transformMat(glm::mat4(1.0f))
{
	particlePool.resize(particleNum);

	particleVB.Bind();

	VertexBufferLayout layout;
	layout.Push<float>(3); //position
	layout.Push<float>(1); //lifetime
	particleVA.AddBuffer(particleVB, layout);

	particleVA.Unbind();

}



void ParticleSystem::Update(float timeStep)
{
	for (auto& particle : particlePool)
	{
		if (!particle.active)
			continue;
		if (particle.lifeRemaining <= 0)
		{
			activeParticles--;
			particle.active = false;			
			continue;
		}

		
		particle.lifeRemaining -= timeStep;
		particle.lifePercent = particle.lifeRemaining / particle.lifetime;
		particle.position += particle.velocity * timeStep;
	}
}

void ParticleSystem::Update(float timeStep, const glm::vec3& acceleration)
{
	for (auto& particle : particlePool)
	{
		if (!particle.active)
			continue;
		if (particle.lifeRemaining <= 0)
		{
			activeParticles--;
			particle.active = false;
			continue;
		}
		particle.lifeRemaining -= timeStep;
		particle.lifePercent = particle.lifeRemaining / particle.lifetime;
		particle.velocity += acceleration * timeStep;
		particle.position += particle.velocity * timeStep;
	}
}

void ParticleSystem::OnRender(Camera& camera)
{
	particleShader.Bind();
	particleTexture.Bind();
	particleShader.SetUniformMat4f("u_View", camera.GetViewMatrix());
	particleShader.SetUniformMat4f("u_Projection", camera.GetProjectionMatrix());
	particleShader.SetUniformMat4f("u_Transform", glm::mat4(1));
	particleShader.SetUniform1i("u_Texture", 0);
	
	std::vector<Vertex> batch;
	std::vector<unsigned int> batchIndex;
	unsigned int i = 0;
	for (auto& particle : particlePool)
	{
		if (!particle.active)
			continue;

		Vertex particleVertex;
		particleVertex.Position = particle.position;
		particleVertex.lifePercent = particle.lifePercent;

		batch.push_back( particleVertex );
		batchIndex.push_back( i );
		i += 1;
	}
	particleVA.Bind();
	particleVB.Bind();
	IndexBuffer particleIB((unsigned int*)batchIndex.data(), batchIndex.size());
	particleIB.Bind();

	
	glBufferSubData(GL_ARRAY_BUFFER, 0, batch.size() * sizeof(Vertex), batch.data());
	glDrawElements(GL_POINTS, batchIndex.size(), GL_UNSIGNED_INT, nullptr);

	particleIB.Unbind();
}

void ParticleSystem::Emit(const ParticleProps particleProps)
{



	Particle& particle = particlePool[poolInd];
	if (!particle.active) activeParticles++;
	particle.active = true;
	particle.position = particleProps.Position;
	particle.velocity = particleProps.Velocity;
	particle.color = particleProps.Color;
	particle.lifetime = particleProps.Lifetime;
	particle.lifeRemaining = particleProps.Lifetime;

	poolInd++;

	poolInd = poolInd % particlePool.size();
	
}

void ParticleSystem::SetTransform(const glm::mat4& transMat)
{
	transformMat = transMat;
}

const unsigned int ParticleSystem::GetParticleCount()
{
	return particleCount;
}

