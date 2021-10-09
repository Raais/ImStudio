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

ImVec2 GetWindowRatio() {
  ImGuiWindow*   w     = ImGui::GetCurrentWindow();
  ImGuiViewport* v     = ImGui::GetMainViewport();
  ImVec2         ratio = ImVec2(v->Size.x / w->Size.x, v->Size.y / w->Size.y);
  return ratio;
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

  //-----------------------------------------------------------------------------
  // ANCHOR STYLES & SETTINGS
  //-----------------------------------------------------------------------------

  // io.Fonts->Build();
  io.IniFilename = NULL;
  ImVec4 bg      = ImVec4(0.123f, 0.123f, 0.123, 1.00f);  // Main bg color

  ImGuiStyle& style = ImGui::GetStyle();

  style.WindowPadding  = ImVec2(12.00f, 8.00f);
  style.ItemSpacing    = ImVec2(15.00f, 4.00f);
  style.GrabMinSize    = 20.00f;
  style.WindowRounding = 8.00f;
  style.FrameRounding  = 12.00f;
  style.GrabRounding   = 12.00f;

  ImVec4* colors                     = style.Colors;  // TODO Redo color scheme
  colors[ImGuiCol_Border]            = ImVec4(0.11f, 0.11f, 0.11f, 0.50f);
  colors[ImGuiCol_FrameBg]           = ImVec4(0.23f, 0.23f, 0.23f, 0.54f);
  colors[ImGuiCol_FrameBgActive]     = ImVec4(0.19f, 0.56f, 0.98f, 0.67f);
  colors[ImGuiCol_TitleBgActive]     = ImVec4(1.00f, 0.36f, 0.00f, 1.00f);
  colors[ImGuiCol_CheckMark]         = ImVec4(0.97f, 0.29f, 0.00f, 1.00f);
  colors[ImGuiCol_SliderGrab]        = ImVec4(0.80f, 0.16f, 0.00f, 1.00f);
  colors[ImGuiCol_Button]            = ImVec4(1.00f, 0.23f, 0.00f, 0.84f);
  colors[ImGuiCol_ButtonHovered]     = ImVec4(0.98f, 0.43f, 0.26f, 1.00f);
  colors[ImGuiCol_ButtonActive]      = ImVec4(0.62f, 0.12f, 0.00f, 1.00f);
  colors[ImGuiCol_ResizeGrip]        = ImVec4(0.64f, 0.64f, 0.64f, 0.20f);
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.98f, 0.50f, 0.26f, 0.67f);
  colors[ImGuiCol_ResizeGripActive]  = ImVec4(1.00f, 0.33f, 0.00f, 0.95f);
  colors[ImGuiCol_Tab]               = ImVec4(0.81f, 0.24f, 0.04f, 0.86f);
  colors[ImGuiCol_TabHovered]        = ImVec4(1.00f, 0.34f, 0.14f, 0.80f);
  colors[ImGuiCol_TabActive]         = ImVec4(1.00f, 0.34f, 0.10f, 1.00f);
  colors[ImGuiCol_TabUnfocused]      = ImVec4(0.91f, 0.09f, 0.00f, 0.97f);

  //-----------------------------------------------------------------------------
  // ANCHOR LAYOUT (size & pos) | Define relationships between windows
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // ANCHOR STATE (WORKSPACES & MAIN LAYOUT WINDOWS)
  //-----------------------------------------------------------------------------

  bool wksp_interface = true;
  bool wksp_logic     = false;

  bool menubar  = true;
  bool sidebar  = true;
  bool viewport = true;

  //-----------------------------------------------------------------------------
  // ANCHOR STATE (CHILDREN)
  //-----------------------------------------------------------------------------

  bool child_debug   = false;
  bool child_sty     = false;
  bool child_metrics = false;
  bool child_colexp  = false;
  bool ly_save       = false;

  //-----------------------------------------------------------------------------
  // ANCHOR VARS
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // SECTION MAIN LOOP
  //-----------------------------------------------------------------------------

  while (!glfwWindowShouldClose(glwindow)) {
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
    ImVec2 mb_P = ImVec2(0, 0);
    ImVec2 mb_S = ImVec2(w_w, 46);
    if (menubar) {
      ImGui::SetNextWindowPos(mb_P);
      ImGui::SetNextWindowSize(mb_S);
      ImGui::Begin("Menubar", NULL,
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar |
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar |
                       ImGuiWindowFlags_NoScrollbar |
                       ImGuiWindowFlags_NoScrollWithMouse);

      if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Debug")) {
          ImGui::MenuItem("Settings", NULL, &child_debug);
          ImGui::MenuItem("Style Editor", NULL, &child_sty);
          ImGui::MenuItem("Metrics", NULL, &child_metrics);
          if (ImGui::MenuItem("Exit")) {
            break;
          };
          ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
          if (ImGui::MenuItem("Save Layout")) {
            ly_save = true;
          }
          ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tools")) {
          ImGui::MenuItem("Color Export", NULL, &child_colexp);
          ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
      }

      if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None)) {
        if (ImGui::BeginTabItem("Interface")) {
          wksp_logic     = false;
          wksp_interface = true;
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Logic")) {
          wksp_interface = false;
          wksp_logic     = true;
          ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
      }

      ImGui::End();
    }

    //-----------------------------------------------------------------------------
    // SECTION wksp_interface
    //-----------------------------------------------------------------------------
    if (wksp_interface) {
      // ANCHOR SIDEBAR
      ImVec2 sb_P  = ImVec2(0, mb_S.y);
      static ImVec2 sb_S  = ImVec2(w_w / 8, w_h - mb_S.y);
      static ImVec2 sb_Sr = ImVec2(8, 1.015444);
      if (sidebar) {
        ImGui::SetNextWindowPos(sb_P);
        ImGui::SetNextWindowSizeConstraints(ImVec2(0, -1), ImVec2(FLT_MAX, -1));
        ImGui::SetNextWindowSize(sb_S);
        ImGui::Begin("Sidebar", NULL, ImGuiWindowFlags_NoTitleBar);
        sb_S = ImGui::GetWindowSize();
        if (ly_save) {
          sb_Sr   = extra::GetWindowRatio();
          ly_save = false;
        }
        if (resizing) {
          sb_S     = ImVec2(w_w / sb_Sr.x, w_h / sb_Sr.y);
          resizing = false;
        }

        extra::metrics();

        ImGui::ColorEdit3("Color", (float*)&bg, ImGuiColorEditFlags_Float);
        if (ImGui::Button("Export")) {
          std::string exp = extra::string_format(
              "ImVec4 col = ImVec4(%ff,%ff,%ff,1.00f;", bg.x, bg.y, bg.z);
          ImGui::LogToClipboard();
          ImGui::LogText(exp.c_str());
          ImGui::LogFinish();
        }

        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape))) {
          break;
        }

        ;  // For viewport
        ImGui::End();
      }

      //-----------------------------------------------------------------------------
      // ANCHOR VIEWPORT
      ImVec2 vp_P = ImVec2(sb_S.x, mb_S.y);
      ImVec2 vp_S = ImVec2(w_w - sb_S.x, w_h - mb_S.y);
      if (viewport) {
        ImGui::SetNextWindowPos(vp_P);
        ImGui::SetNextWindowSize(vp_S);
        ImGui::Begin(
            "Viewport", NULL,
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);

        extra::metrics();

        ImGui::End();
      }

      //-----------------------------------------------------------------------------
      //-----------------------------------------------------------------------------
      // ANCHOR CHILDREN
      if (child_debug) {
        ImGui::SetNextWindowBgAlpha(0.35f);
        if (ImGui::Begin("child_debug", &child_debug,
                         ImGuiWindowFlags_AlwaysAutoResize)) {
          ImGui::Text("hello");
          ImGui::End();
        }
      }

      if (child_sty) {
        ImGui::ShowStyleEditor();
      }

      if (child_metrics) {
        ImGui::ShowMetricsWindow(&child_metrics);
      }

      if (child_colexp) {
        ImGui::SetNextWindowBgAlpha(0.35f);
        if (ImGui::Begin("Color Export", &child_colexp,
                         ImGuiWindowFlags_AlwaysAutoResize)) {
          ImGui::ColorEdit3("Your Color", (float*)&bg,
                            ImGuiColorEditFlags_Float);
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
    }  //! SECTION wksp_interface End
    //-----------------------------------------------------------------------------
    // ANCHOR wksp_logic
    ImVec2 lg_P = ImVec2(0, mb_S.y);
    ImVec2 lg_S = ImVec2(w_w, w_h - mb_S.y);
    if (wksp_logic) {
      ImGui::SetNextWindowPos(lg_P);
      ImGui::SetNextWindowSizeConstraints(ImVec2(0, -1), ImVec2(FLT_MAX, -1));
      ImGui::SetNextWindowSize(lg_S);
      ImGui::Begin("wksp_logic", NULL, ImGuiWindowFlags_NoTitleBar);
      ImGui::Text("Im not going to help you with that!! :/");
      ImGui::End();
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