#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


class Camera
{
public:

	Camera(glm::vec3 eye, glm::vec3 center, glm::vec3 up);

	void SetRotation(float l_r, float u_d);

	const glm::vec3& GetPosition() const;
	const float GetRotation() const;

	const glm::mat4& GetProjectionMatrix() const;
	const glm::mat4& GetViewMatrix() const;
	void SetViewMatrix(const glm::mat4& viewMat);
	void SetProjMatrix(const glm::mat4& projMat);
private:
	glm::vec3 position;
	float rotation;
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
private:
	glm::vec3 eye;
	glm::vec3 oldEye;
	glm::vec3 center;
	glm::vec3 up;
};

