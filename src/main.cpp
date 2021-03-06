// 3rdparty Headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>

// Local Headers
#include <ImGui.hpp>
#include <Assets/AssetManager.hpp>
#include "Graphics/Renderer.hpp"
#include "Graphics/Context.hpp"
#include "Graphics/Program.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Material.hpp"
#include "GameObject.hpp"

// General variables
const float dt = 1.f / 60.f;
glm::vec2 mLastCursorPosition(0, 0);
glm::ivec2 mWindowSize(1280, 800);

// Camera
Camera mCamera(glm::vec3(0, 5, -5), glm::vec3(0, -1, 1), mWindowSize.x, mWindowSize.y);

static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	if (state == GLFW_PRESS && !ImGui::IsMouseHoveringAnyWindow()) {
		float dx = xpos - mLastCursorPosition.x;
		float dy = ypos - mLastCursorPosition.y;
		mCamera.RotatePitch(-dy * 10.f * dt);
		mCamera.RotateYaw  ( dx * 10.f * dt);
		glfwSetCursorPos(window, mWindowSize.x / 2, mWindowSize.y / 2);
	}
	glfwGetCursorPos(window, &xpos, &ypos);
	mLastCursorPosition = { xpos, ypos };
}

int main(int argc, char * argv[]) {
	Context::Initialize();
	Context::SetCursorPositionCallback(CursorPositionCallback);

	// AssetManager::Get().Setup();
	Renderer renderer;
	renderer.SetActiveCamera(&mCamera);
	renderer.Setup();

	GameObject* sphere = new GameObject();
	sphere->m_model = Model("sphere.obj", "woodfloor.mat");
	sphere->m_position = glm::vec3(0, 0, 0);
	renderer.Submit(sphere);

	// GameObject* bamboo = new GameObject();
	// bamboo->m_model = Model("sphere.obj",
	//                         "rusted_albedo.png",
	//                         "rusted_normal.png",
	//                         "rusted_roughness.png",
	//                         "rusted_metalness.png",
	//                         "rusted_ao.png");
	// bamboo->m_position = glm::vec3(5, 0, 5);
	// renderer.Submit(bamboo);

	// Game Loop
	while ((Context::ShouldClose() || Context::IsKeyDown(GLFW_KEY_ESCAPE)) == false) {
		Context::PollEvents();

		{
			static bool show_renderer_window = false;
			static bool show_texture_window  = false;
			static bool show_material_window = false;
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("Renderer"))
				{
					ImGui::MenuItem("Settings", NULL, &show_renderer_window);
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Assets"))
				{
					ImGui::MenuItem("Textures", NULL, &show_texture_window);
					ImGui::MenuItem("Materials", NULL, &show_material_window);
					ImGui::EndMenu();
				}

				ImGui::EndMainMenuBar();
			}

			if (show_renderer_window) {
				renderer.RendererWindow(&show_renderer_window);
			}

			if (show_texture_window) {
				AssetManager::Get().TexturesWindow(&show_texture_window);
			}

			if (show_material_window) {
				AssetManager::Get().MaterialsWindow(&show_material_window);
			}
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
