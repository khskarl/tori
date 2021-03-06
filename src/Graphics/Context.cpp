#include "Context.hpp"

#include <Log.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

#include "../Settings.hpp"

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	int state = glfwGetKey(window, key);

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

namespace Context {

GLFWwindow* mWindow = nullptr;

bool Initialize () {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
	glfwWindowHint(GLFW_RESIZABLE, true);
	glfwWindowHint(GLFW_SAMPLES, 8);
	mWindow = glfwCreateWindow(Settings::ScreenWidth, Settings::ScreenHeight,
	                           "I am a happy window :D", nullptr, nullptr);

	if (mWindow == nullptr) {
		LOG_ERROR("Failed to Create OpenGL Context");
		return false;
	}

	glfwSetKeyCallback(mWindow, KeyCallback);
	glfwSetMouseButtonCallback(mWindow, MouseButtonCallback);

	// Create Context and Load OpenGL
	glfwMakeContextCurrent(mWindow);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		LOG_ERROR("Failed to initialize glew");
		return false;
	}

	// Setup ImGui binding
	ImGui_ImplGlfwGL3_Init(mWindow, true);
	std::string version = std::string((const char *)glGetString(GL_VERSION));
	LOG_INFO("OpenGL " + version);
	return true;
}

void PollEvents () {
	glfwPollEvents();
	ImGui_ImplGlfwGL3_NewFrame();
}

void SwapBuffers () {
	glfwSwapBuffers(mWindow);
}

void Destroy () {
	ImGui_ImplGlfwGL3_Shutdown();
	glfwTerminate();
}

bool IsKeyDown (const int keyCode) {
	return glfwGetKey(mWindow, keyCode) == GLFW_PRESS;
}

bool ShouldClose () {
	return glfwWindowShouldClose(mWindow);
}

void SetCursorPositionCallback (void (*cursorPositionCallback)(GLFWwindow*, double, double)) {
	glfwSetCursorPosCallback(mWindow, cursorPositionCallback);
}

}
