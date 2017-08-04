#include "Camera.hpp"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera () {
	mAspectRatio = mWindowWidth / mWindowHeight;
	this->BuildProjectionMatrix();
}

Camera::Camera (const glm::vec3 pos, const glm::vec3 direction, const float width, const float height) {
	mPosition = pos;
	mDirection = direction;
	mWindowWidth = width;
	mWindowHeight = height;
	mAspectRatio = width / height;

	this->BuildProjectionMatrix();
}

void Camera::BuildProjectionMatrix () {
	mProjectionMatrix = glm::perspective(glm::radians(mFov),
	                                     mAspectRatio,
	                                     mNearPlane,
	                                     mFarPlane);
}

glm::mat4 const Camera::GetViewMatrix () {
	return glm::lookAt(mPosition, mPosition + mDirection, mUp);
}

glm::mat4 const& Camera::GetProjectionMatrix () {
	return mProjectionMatrix;
}

glm::vec3 const& Camera::GetPosition () {
	return mPosition;
}

glm::vec3 const& Camera::GetDirection () {
	return mDirection;
}

glm::vec3 const& Camera::GetUp () {
	return mUp;
}

glm::vec3 const Camera::GetRight () {
	return glm::cross(mDirection, mUp);
}

void Camera::MoveForward (const float amount) {
	mPosition += mDirection * amount;
}

void Camera::MoveRight (const float amount) {
	glm::vec3 right = GetRight();
	mPosition += right * amount;
}

void Camera::RotatePitch (const float amount) {
	glm::vec3 right = GetRight();
	mDirection = glm::rotate(mDirection, amount, right);
}

void Camera::RotateYaw (const float amount) {
	mDirection = glm::rotate(mDirection, amount, mUp);
}
