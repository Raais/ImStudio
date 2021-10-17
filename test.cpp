#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

#include "font/opensans.cpp"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"
#include "utils/extra.h"
#include "imgui/misc/cpp/imgui_stdlib.cpp"

//-----------------------------------------------------------------------------
// ANCHOR GLFW STUFF
//-----------------------------------------------------------------------------

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && \
    !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

//-----------------------------------------------------------------------------
// SECTION MAIN FUNC()
// ANCHOR GLFW BOILERPLATE
//-----------------------------------------------------------------------------

int main(int argc, char* argv[]) {
  int  w_w      = 900;
  int  w_h      = 600;
  bool resizing = false;

  //---------------------------------------------------
  // ANCHOR ARGS
  //---------------------------------------------------
  std::vector<std::string> args(argv, argv + argc);

  for (size_t i = 1; i < args.size(); ++i) {
    if (args[i] == "-x") {
    }
  }
  //---------------------------------------------------
  //---------------------------------------------------

  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) return 1;

#if defined(IMGUI_IMPL_OPENGL_ES2)
  const char* glsl_version = "#version 100";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
  const char* glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
  const char* glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

  //-----------------------------------------------------------------------------
  // ANCHOR CREATE glwindow
  //-----------------------------------------------------------------------------

  glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER,
                 GLFW_TRUE);  // glwindow to transparent; handle color through
                              // (internal) ImGui Window;

  GLFWwindow* glwindow = glfwCreateWindow(w_w, w_h, "ImStudio", NULL, NULL);

  if (glwindow == NULL) return 1;

  glfwGetWindowSize(glwindow, &w_w, &w_h);

  glfwSetWindowUserPointer(glwindow, &resizing);

  glfwSetWindowSizeCallback(
      glwindow, [](GLFWwindow* window, int width, int height) {
        bool* resizing = static_cast<bool*>(glfwGetWindowUserPointer(window));
        *resizing      = true;
      });

  glfwMakeContextCurrent(glwindow);
  glfwSwapInterval(1);  // Enable vsync
  // extra::glfwSetWindowCenter(glwindow);

  //-----------------------------------------------------------------------------
  // ANCHOR IMGUI
  //-----------------------------------------------------------------------------

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();

  ImGui_ImplGlfw_InitForOpenGL(glwindow, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  ImVec4      bg = ImVec4(0.123f, 0.123f, 0.123, 1.00f);  // Main bg color
  std::string s  = "hey";
  bool        is = false;
  bool a = false;

  while (!glfwWindowShouldClose(glwindow)) {
    glfwPollEvents();
    glfwGetWindowSize(glwindow, &w_w, &w_h);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::SetNextWindowBgAlpha(0.00f);

    if (is) {
      ImGui::Text("Active");
    } else {
      ImGui::Text("Disabled");
    }

    //ImGui::Button("hello");
    static char str0[128] = "Hello, world!";
            ImGui::InputText("input text", str0, IM_ARRAYSIZE(str0));

    if (ImGui::IsItemActive()) {
      is = true;
    } else {
      is = false;
    }

    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape))) {
      break;
    }

    //! SECTION GUI End
    //-----------------------------------------------------------------------------
    // ANCHOR RENDER
    //-----------------------------------------------------------------------------

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(glwindow, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(bg.x * bg.w, bg.y * bg.w, bg.z * bg.w, bg.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(glwindow);
  }

  //! SECTION MAIN LOOP End
  //-----------------------------------------------------------------------------

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(glwindow);
  glfwTerminate();

  return 0;

  //! SECTION MAIN FUNC() End
  //-----------------------------------------------------------------------------
}
