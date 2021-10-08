#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>

#include <iostream>
#include <string>
#include <vector>

#include "font/opensans.cpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "extra.h"

//-----------------------------------------------------------------------------
// ANCHOR OBJECTS
//-----------------------------------------------------------------------------

void metrics(int w_w, int w_h) {  // ugly debug stuff
  ImGuiIO& io = ImGui::GetIO();
  ImGuiWindow* window = ImGui::GetCurrentWindowRead();
  ImVec2 padding = ImGui::GetStyle().WindowPadding;
  ImGui::NewLine();
  ImGui::NewLine();
  ImGui::NewLine();
  ImGui::Text("%dx%d", w_w, w_h);
  ImGui::Text("name: %s", window->Name);
  ImGui::Text("IMsize: %g,%g", window->Size.x, window->Size.y);
  ImGui::Text("region: %g,%g", ImGui::GetWindowContentRegionMax().x,
              ImGui::GetWindowContentRegionMax().y);
  ImGui::Text("region: %g,%g", ImGui::GetContentRegionMax().x,
              ImGui::GetContentRegionMax().y);
  ImGui::Text("padding: %gx%g", padding.x, padding.y);
  ImGui::Text("mousepos: (%g,%g)", io.MousePos.x, io.MousePos.y);
  ImGui::Text("global cursor: (%g,%g)", ImGui::GetCursorPosX,
              ImGui::GetCursorPosY);
}

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
  int w_w = 900;
  int w_h = 600;
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
        *resizing = true;
      });

  glfwMakeContextCurrent(glwindow);
  glfwSwapInterval(1);  // Enable vsync
  extra::glfwSetWindowCenter(glwindow);

  //-----------------------------------------------------------------------------
  // ANCHOR CONTEXT
  //-----------------------------------------------------------------------------

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(glwindow, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  //-----------------------------------------------------------------------------
  // ANCHOR STYLES & SETTINGS
  //-----------------------------------------------------------------------------

  // io.Fonts->Build();
  io.IniFilename = NULL;
  ImVec4 bg = ImVec4(0.123f, 0.123f, 0.123, 1.00f);  // Main bg color

  ImGuiStyle& style = ImGui::GetStyle();

  style.WindowPadding = ImVec2(12.00f, 8.00f);
  style.ItemSpacing = ImVec2(15.00f, 4.00f);
  style.GrabMinSize = 20.00f;
  style.WindowRounding = 8.00f;
  style.FrameRounding = 12.00f;
  style.GrabRounding = 12.00f;

  ImVec4* colors = style.Colors;
  colors[ImGuiCol_Border] = ImVec4(0.11f, 0.11f, 0.11f, 0.50f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.23f, 0.23f, 0.23f, 0.54f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.19f, 0.56f, 0.98f, 0.67f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(1.00f, 0.36f, 0.00f, 1.00f);
  colors[ImGuiCol_CheckMark] = ImVec4(0.97f, 0.29f, 0.00f, 1.00f);
  colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.16f, 0.00f, 1.00f);
  colors[ImGuiCol_Button] = ImVec4(1.00f, 0.23f, 0.00f, 0.84f);
  colors[ImGuiCol_ButtonHovered] = ImVec4(0.98f, 0.43f, 0.26f, 1.00f);
  colors[ImGuiCol_ButtonActive] = ImVec4(0.62f, 0.12f, 0.00f, 1.00f);
  colors[ImGuiCol_ResizeGrip] = ImVec4(0.64f, 0.64f, 0.64f, 0.20f);
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.98f, 0.50f, 0.26f, 0.67f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 0.33f, 0.00f, 0.95f);
  colors[ImGuiCol_Tab] = ImVec4(0.81f, 0.24f, 0.04f, 0.86f);
  colors[ImGuiCol_TabHovered] = ImVec4(1.00f, 0.34f, 0.14f, 0.80f);
  colors[ImGuiCol_TabActive] = ImVec4(1.00f, 0.34f, 0.10f, 1.00f);
  colors[ImGuiCol_TabUnfocused] = ImVec4(0.91f, 0.09f, 0.00f, 0.97f);

  //-----------------------------------------------------------------------------
  // ANCHOR LAYOUT (size & pos) | Define relationships between windows
  //-----------------------------------------------------------------------------

  float mb_Sx = w_w;
  float mb_Sy = w_h / 19;
  ImVec2 mb_S = ImVec2(mb_Sx, mb_Sy);

  float sb_Sx = w_w / 8;
  float sb_Sy = w_h - mb_Sy;
  ImVec2 sb_S = ImVec2(sb_Sx, sb_Sy);

  float vp_Sx = w_w - sb_Sx;
  float vp_Sy = w_h - mb_Sy;
  ImVec2 vp_S = ImVec2(vp_Sx, vp_Sy);
  //-----------------------------------------------------------------------------
  float mb_Px = 0;
  float mb_Py = 0;
  ImVec2 mb_P = ImVec2(mb_Px, mb_Py);

  float sb_Px = 0;
  float sb_Py = mb_Sy;
  ImVec2 sb_P = ImVec2(sb_Px, sb_Py);

  float vp_Px = sb_Sx;
  float vp_Py = sb_Py;
  ImVec2 vp_P = ImVec2(vp_Px, vp_Py);

  //-----------------------------------------------------------------------------
  // ANCHOR IMGUI WINDOWS (MAIN)
  //-----------------------------------------------------------------------------

  bool menubar = true;
  bool sidebar = true;
  bool viewport = true;

  //-----------------------------------------------------------------------------
  // ANCHOR STATES (CHILDREN)
  //-----------------------------------------------------------------------------

  bool dbg = false;
  bool sty = false;
  bool met = false;
  bool exit = false;
  bool colexp = false;

  //-----------------------------------------------------------------------------
  // ANCHOR VARS
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // SECTION MAIN LOOP >>>>
  //-----------------------------------------------------------------------------

  while (!glfwWindowShouldClose(glwindow)) {
    if (exit) {
      break;
    }
    glfwPollEvents();
    glfwGetWindowSize(glwindow, &w_w, &w_h);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::SetNextWindowBgAlpha(0.00f);

    //-----------------------------------------------------------------------------
    // SECTION GUI
    //-----------------------------------------------------------------------------

    // ANCHOR MENUBAR
    if (menubar) {
      ImGui::SetNextWindowPos(mb_P);
      ImGui::SetNextWindowSize(mb_S);
      ImGui::Begin("Menubar", NULL,
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar |
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar);

      if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Debug")) {
          ImGui::MenuItem("Settings", NULL, &dbg);
          ImGui::MenuItem("Style Editor", NULL, &sty);
          ImGui::MenuItem("Metrics", NULL, &met);
          ImGui::MenuItem("Exit", NULL, &exit);
          ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tools")) {
          ImGui::MenuItem("Color Export", NULL, &colexp);
          ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
      }

      if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None)) {
        if (ImGui::BeginTabItem("Description")) {
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Details")) {
          ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
      }

      ImGui::End();
    }

    // ANCHOR SIDEBAR
    if (sidebar) {
      ImGui::SetNextWindowPos(sb_P);
      ImGui::SetNextWindowSizeConstraints(ImVec2(0, -1), ImVec2(FLT_MAX, -1));
      ImGui::SetNextWindowSize(sb_S);
      ImGui::Begin("Sidebar", NULL, ImGuiWindowFlags_NoTitleBar);
      sb_S = ImGui::GetWindowSize();

      if (resizing) {
        sb_S = ImVec2(sb_Sx, sb_Sy);
        resizing = false;
      }

      ImGui::ColorEdit3("Color", (float*)&bg, ImGuiColorEditFlags_Float);
      if (ImGui::Button("Export")) {
        std::string exp = "ImVec4 col = ImVec4(" + std::to_string(bg.x) + "f," +
                          std::to_string(bg.y) + "f," + std::to_string(bg.z) +
                          "f,1.00f);";
        ImGui::LogToClipboard();
        ImGui::LogText(exp.c_str());
        ImGui::LogFinish();
      }

      if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape))) {
        break;
      }

      ImGui::End();
    }

    // ANCHOR VIEWPORT
    if (viewport) {
      ImGui::SetNextWindowPos(vp_P);
      ImGui::SetNextWindowSize(vp_S);
      ImGui::Begin(
          "Viewport", NULL,
          ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);
      vp_P = ImVec2(sb_S.x, sb_P.y);
      vp_S = ImVec2(w_w - sb_S.x, w_h - mb_S.y);

      metrics(w_w, w_h);

      ImGui::SetCursorPos(ImVec2(64, 70));
      ImGui::Text("X");
      ImGui::SetCursorPos(ImVec2(0, 0));

      ImGui::End();
    }

    // ANCHOR DEBUG
    if (dbg) {
      ImGui::SetNextWindowBgAlpha(0.35f);
      if (ImGui::Begin("dbg", &dbg, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("hello");
        ImGui::End();
      }
    }

    if (sty) {
      ImGui::ShowStyleEditor();
    }

    if (met) {
      ImGui::ShowMetricsWindow(&met);
    }

    if (colexp) {
      ImGui::SetNextWindowBgAlpha(0.35f);
      if (ImGui::Begin("Color Export", &colexp,
                       ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::ColorEdit3("Your Color", (float*)&bg, ImGuiColorEditFlags_Float);
        if (ImGui::Button("Export to Clipboard")) {
          std::string exp = "ImVec4 col = ImVec4(" + std::to_string(bg.x) +
                            "f," + std::to_string(bg.y) + "f," +
                            std::to_string(bg.z) + "f,1.00f);";
          ImGui::LogToClipboard();
          ImGui::LogText(exp.c_str());
          ImGui::LogFinish();
        }
        ImGui::End();
      }
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

  //! SECTION MAIN LOOP End <<<<
  //-----------------------------------------------------------------------------

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(glwindow);
  glfwTerminate();

  return 0;

  //! SECTION MAIN FUNC End()
  //-----------------------------------------------------------------------------
}