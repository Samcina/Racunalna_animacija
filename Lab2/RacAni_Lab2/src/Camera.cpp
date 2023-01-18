#include "Camera.h"
#include <iostream>
Camera::Camera(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
	:
	eye(eye),
	oldEye(eye),
	center(center),
	up(up)
{
	viewMatrix = glm::lookAt(eye, center, up);
	projectionMatrix = glm::frustum(-0.5, 0.5, -0.5, 0.5, 1.0, 100.0);
}

void Camera::SetRotation(float l_r, float u_d)
{
	glm::vec4 temp;
	if(l_r != 0)
		temp = glm::rotate(glm::mat4(1.0f), glm::radians(l_r), glm::vec3(0, 1, 0)) * glm::vec4(eye, 1.0f);
	else
		temp = glm::rotate(glm::mat4(1.0f), glm::radians(u_d), glm::cross(eye, glm::vec3(0.0f, 1.0f, 0.0f))) * glm::vec4(eye, 1.0f);
	eye = glm::vec3(temp.x / temp[3], temp.y / temp[3], temp.z / temp[3]);
	viewMatrix = glm::lookAt(eye, center, up);
}

const glm::vec3& Camera::GetPosition() const
{
	return position;
}

const float Camera::GetRotation() const
{
	return rotation;
}

const glm::mat4& Camera::GetProjectionMatrix() const
{
	return projectionMatrix;
}

const glm::mat4& Camera::GetViewMatrix() const
{
	return viewMatrix;
}

void Camera::SetViewMatrix(const glm::mat4& viewMat)
{
	viewMatrix = viewMat;
}

void Camera::SetProjMatrix(const glm::mat4& projMat)
{
	projectionMatrix = projMat;
}

