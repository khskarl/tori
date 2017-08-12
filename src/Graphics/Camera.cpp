#include "Camera.hpp"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera () {
	this->BuildProjectionMatrix();
}

Camera::Camera (const glm::vec3 pos, const glm::vec3 direction, const float width, const float height) {
	m_position  = pos;
	m_direction = direction;
	m_width  = width;
	m_height = height;

	this->BuildProjectionMatrix();
}

void Camera::BuildProjectionMatrix () {
	mProjectionMatrix = glm::perspective(glm::radians(m_fov),
	                                     GetAspectRatio(),
	                                     m_nearPlane,
	                                     m_farPlane);
}

glm::mat4 const Camera::GetViewMatrix () {
	return glm::lookAt(m_position, m_position + m_direction, m_up);
}

glm::mat4 const& Camera::GetProjectionMatrix () {
	return mProjectionMatrix;
}

glm::vec3 const& Camera::GetPosition () {
	return m_position;
}

glm::vec3 const& Camera::GetDirection () {
	return m_direction;
}

glm::vec3 const& Camera::GetUp () {
	return m_up;
}

glm::vec3 const Camera::GetRight () {
	return glm::normalize(glm::cross(m_direction, m_up));
}

float const Camera::GetAspectRatio () {
	return m_width / m_height;
}

void Camera::MoveForward (const float amount) {
	m_position += m_direction * amount;
}

void Camera::MoveRight (const float amount) {
	m_position += GetRight() * amount;
}

void Camera::RotatePitch (const float amount) {
	m_pitch += amount;
	if (m_pitch >  90.f) m_pitch =  90.f - 0.001f;
	if (m_pitch < -90.f) m_pitch = -90.f + 0.001f;
	RecomputeDirection();
}

void Camera::RotateYaw (const float amount) {
	m_yaw += amount;
	if (m_yaw > 360.0f) m_yaw = 0.0f;
	if (m_yaw < 0.0f)   m_yaw = 360.0f;
	RecomputeDirection();
}

void Camera::RecomputeDirection () {
	float pitch = glm::radians(m_pitch);
	float yaw   = glm::radians(m_yaw);
	m_direction = (glm::vec3(glm::cos(pitch) * glm::cos(yaw),
	                         glm::sin(pitch),
	                         glm::cos(pitch) * glm::sin(yaw)));
	m_direction = glm::normalize(m_direction);
}
