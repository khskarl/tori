#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Context {

bool Initialize ();
void PollEvents ();
void SwapBuffers ();
void Destroy ();

bool IsKeyDown (const int keyCode);
bool ShouldClose ();

void SetCursorPositionCallback (void (*cursorPositionCallback)(GLFWwindow*, double, double));

}

#endif // CONTEXT_HPP
