#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	Camera(const glm::vec3 pos, const glm::vec3 direction, const float width, const float height);

	glm::mat4 const  GetViewMatrix();
	glm::mat4 const& GetProjectionMatrix();

	glm::vec3 const& GetPosition();
	glm::vec3 const& GetDirection();
	glm::vec3 const& GetUp();
	glm::vec3 const  GetRight();

	float const& GetFov();

	void SetFov(float const& fov);

	void MoveForward (const float amount);
	void MoveRight   (const float amount);
	void RotatePitch (const float amount);
	void RotateYaw   (const float amount);
private:
	void BuildProjectionMatrix();

	glm::vec3 mPosition  = glm::vec3(0);
	glm::vec3 mDirection = glm::vec3(0, 0, 1);
	glm::vec3 mUp        = glm::vec3(0, 1, 0);

	glm::mat4 mProjectionMatrix = glm::mat4(1.0f);

	float mFov = 80.0f;
	float mAspectRatio;
	float mWindowWidth  = 800.f;
	float mWindowHeight = 600.f;
	float mNearPlane = 0.1f;
	float mFarPlane = 100.f;
};

#endif
