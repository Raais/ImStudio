#pragma once

#include "../includes.h"

namespace extra
{

void glfwSetWindowCenter(GLFWwindow *window);

ImVec2 GetLocalCursor();

ImVec2 GetStaticCursor();

ImVec2 GetWindowSRatio();

ImVec2 GetWindowPRatio();

ImVec2 GetLastItemPos();

void GrabButton(ImVec2 pos, int random_int);

void HelpMarker(const char *desc);

void metrics();

} // namespace extra
