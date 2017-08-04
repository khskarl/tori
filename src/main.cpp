// 3rdparty Headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>

// Local Headers
#include "Graphics/Renderer.hpp"
#include "Graphics/Context.hpp"
#include "Graphics/Program.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Camera.hpp"

#include "GameObject.hpp"

// General variables
const float dt = 1.f / 60.f;
glm::vec2 mLastCursorPosition(0, 0);
glm::ivec2 mWindowSize(1280, 800);

// Camera
Camera mCamera(glm::vec3(0, 5, 0), glm::vec3(0, -1, 1), mWindowSize.x, mWindowSize.y);

static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	if (state == GLFW_PRESS && !ImGui::IsMouseHoveringAnyWindow()) {
		float dx = xpos - mLastCursorPosition.x;
		float dy = ypos - mLastCursorPosition.y;
		mCamera.RotatePitch(-dy * 0.2f * dt);
		mCamera.RotateYaw  (-dx * 0.2f * dt);
		glfwSetCursorPos(window, mWindowSize.x / 2, mWindowSize.y / 2);
	}
	glfwGetCursorPos(window, &xpos, &ypos);
	mLastCursorPosition = { xpos, ypos };
}

int main(int argc, char * argv[]) {
	Context::Initialize();
	Context::SetCursorPositionCallback(CursorPositionCallback);

	Renderer renderer;
	renderer.SetActiveCamera(&mCamera);
	renderer.Setup();

	GameObject* sphere = new GameObject();
	sphere->m_model = Model("sphere.obj",
	                        "woodfloor_albedo.png",
	                        "woodfloor_normal.png",
	                        "woodfloor_roughness.png",
	                        "woodfloor_metalness.png",
	                        "woodfloor_ao.png");
	sphere->m_position = glm::vec3(0, 0, 5);

	GameObject* bamboo = new GameObject();
	bamboo->m_model = Model("sphere.obj",
	                        "bamboo_albedo.png",
	                        "bamboo_normal.png",
	                        "bamboo_roughness.png",
	                        "bamboo_metalness.png",
	                        "bamboo_ao.png");
	bamboo->m_position = glm::vec3(5, 0, 5);

	renderer.Submit(sphere);
	renderer.Submit(bamboo);

	// Game Loop
	while ((Context::ShouldClose() || Context::IsKeyDown(GLFW_KEY_ESCAPE)) == false) {
		Context::PollEvents();

		{
			if (ImGui::Button("Reload shaders")) renderer.m_mainProgram->Reload();
			ImGui::Checkbox("Wireframe", &renderer.m_bRenderWireframe);
			ImGui::Text("%.3f ms/frame (%.1f FPS)",
			            1000.0f / ImGui::GetIO().Framerate,
			            ImGui::GetIO().Framerate);
			ImGui::Text(ImGui::IsMouseHoveringAnyWindow() ? "Yes" : "No");
			ImGui::Image((void*)sphere->m_model.m_albedo->m_id, ImVec2(128, 128));
			ImGui::Image((void*)sphere->m_model.m_normal->m_id, ImVec2(128, 128));
			ImGui::Image((void*)sphere->m_model.m_roughness->m_id, ImVec2(128, 128));
			ImGui::Image((void*)sphere->m_model.m_metalness->m_id, ImVec2(128, 128));
			ImGui::Image((void*)sphere->m_model.m_ao->m_id, ImVec2(128, 128));
		}

		if (Context::IsKeyDown(GLFW_KEY_W))	mCamera.MoveForward( 10 * dt);
		if (Context::IsKeyDown(GLFW_KEY_A))	mCamera.MoveRight  (-10 * dt);
		if (Context::IsKeyDown(GLFW_KEY_S))	mCamera.MoveForward(-10 * dt);
		if (Context::IsKeyDown(GLFW_KEY_D))	mCamera.MoveRight  ( 10 * dt);
		if (Context::IsKeyDown(GLFW_KEY_R))	renderer.m_mainProgram->Reload();

		renderer.RenderFrame();
		Context::SwapBuffers();
	}

	Context::Destroy();
	return 0;
}
