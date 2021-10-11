#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>

#include <iostream>
#include <string>
#include <vector>

#include "font/opensans.cpp"
#include "../../imgui.h"
#include "../../backends/imgui_impl_glfw.h"
#include "../../backends/imgui_impl_opengl3.h"
#include "../../imgui_internal.h"
#include "utils/extra.h"

//-----------------------------------------------------------------------------
// ANCHOR OBJECTS
//-----------------------------------------------------------------------------

class Win {
 public:
  int         id    = 0;
  bool        state = true;
  std::string name  = "window_";
  ImVec2      size  = ImVec2(1070, 680);
  ImVec2      pos   = ImVec2(280, 120);
  void        draw() {
    if (state) {
      ImGui::SetNextWindowPos(pos, ImGuiCond_Once);
      ImGui::SetNextWindowSize(size, ImGuiCond_Once);
      ImGui::Begin(name.c_str(), &state);
      ImGui::Text("hello");
      ImGui::End();
    }
  }
  Win(int newid) {
    id = newid;
    name += std::to_string(id);
  };
};

class Cbx {
 public:
  int count;
};

class Btn {
 public:
  int         id    = 0;
  bool        state = true;
  std::string text  = "button_";
  void        draw() {
    if (state) {
      ImGui::Button(text.c_str());
    }
  }
  Btn(int newid) {
    id = newid;
    text += std::to_string(id);
  };
};

class Rdo {
 public:
  int count;
};

class Txt {
 public:
  int count;
};

class DrawBuffer {
 public:
  int              idvar = 0;
  std::vector<Win> win   = {};
  std::vector<Btn> btn   = {};
  void             drawall() {
    if (!win.empty()) {
      for (auto i = win.begin(); i != win.end(); ++i) {
        Win& w = *i;
        w.draw();

        if (w.state == false) {
          i = win.erase(i);
          break;
        }
      }
    }
    if (!btn.empty()) {
      for (auto i = btn.begin(); i != btn.end(); ++i) {
        Btn& b = *i;
        b.draw();

        if (b.state == false) {
          i = btn.erase(i);
          break;
        }
      }
    }
    // std::cout << "hello3" << std::endl;
  }
  void createwin() {
    idvar++;
    Win newin(idvar);
    win.push_back(newin);
  }
  void createbtn() {
    idvar++;
    Btn newbtn(idvar);
    btn.push_back(newbtn);
  }
};

static void HelpMarker(const char* desc) {
  ImGui::TextDisabled("(?)");
  if (ImGui::IsItemHovered()) {
    ImGui::BeginTooltip();
    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
    ImGui::TextUnformatted(desc);
    ImGui::PopTextWrapPos();
    ImGui::EndTooltip();
  }
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

  ImVec4* colors = style.Colors;  // TODO Redo color scheme

  //-----------------------------------------------------------------------------
  // ANCHOR LAYOUT (size & pos) | Define relationships between windows
  //-----------------------------------------------------------------------------

  //-----------------------------------------------------------------------------
  // ANCHOR STATE (WORKSPACES & MAIN LAYOUT WINDOWS)
  //-----------------------------------------------------------------------------

  bool wksp_interface = true;
  bool wksp_logic     = false;

  bool menubar    = true;
  bool sidebar    = true;
  bool viewport   = true;
  bool properties = true;

  //-----------------------------------------------------------------------------
  // ANCHOR STATE (CHILDREN)
  //-----------------------------------------------------------------------------

  bool       child_debug   = false;
  bool       child_sty     = false;
  bool       child_metrics = false;
  bool       child_colexp  = false;
  bool       ly_save       = false;
  DrawBuffer bf;

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

      // MENU
      if (ImGui::BeginMenuBar()) {
        /// menu-debug
        if (ImGui::BeginMenu("Debug")) {
          ImGui::MenuItem("Settings", NULL, &child_debug);
          ImGui::MenuItem("Style Editor", NULL, &child_sty);
          ImGui::MenuItem("Metrics", NULL, &child_metrics);
          if (ImGui::MenuItem("Exit")) {
            break;
          };
          ImGui::EndMenu();
        }

        /// menu-edit
        if (ImGui::BeginMenu("Edit")) {
          if (ImGui::MenuItem("Save Layout")) {
            ly_save = true;
          }
          ImGui::EndMenu();
        }

        /// menu-tools
        if (ImGui::BeginMenu("Tools")) {
          ImGui::MenuItem("Color Export", NULL, &child_colexp);
          ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
      }

      // TAB
      if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None)) {
        // tab-interface
        if (ImGui::BeginTabItem("Interface")) {
          wksp_logic     = false;
          wksp_interface = true;
          ImGui::EndTabItem();
        }

        // tab-logic
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
      static ImVec2 sb_P = ImVec2(0, mb_S.y);
      static ImVec2 sb_S = ImVec2(w_w / 12, w_h - mb_S.y);
      static ImVec2 sb_Sr =
          ImVec2(12, 1.015444);  // sb_S expressed as ratio to make
                                 // scaling/resizing simpler

      {
        if (sidebar) {
          ImGui::SetNextWindowPos(sb_P);
          ImGui::SetNextWindowSizeConstraints(ImVec2(0, -1),
                                              ImVec2(FLT_MAX, -1));
          ImGui::SetNextWindowSize(sb_S);
          ImGui::Begin("Sidebar", NULL,
                       ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
          sb_S = ImGui::GetWindowSize();
          if (ly_save) {
            sb_Sr = extra::GetWindowSRatio();
          }
          if (resizing) {
            sb_S = ImVec2(w_w / sb_Sr.x, w_h / sb_Sr.y);
          }

          /// content-sidebar
          {
            {
              if (ImGui::Button("Window")) {
                bf.createwin();
              }
              if (ImGui::Button("Checkbox")) {
              }
              if (ImGui::Button("Button")) {
                bf.createbtn();
              }
              if (ImGui::Button("Radio Button")) {
              }
              if (ImGui::Button("Text")) {
              }

              if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape))) {
                break;
              }
            }
          }

          ImGui::End();
        }

        //-----------------------------------------------------------------------------
        // ANCHOR PROPERTIES
        static ImVec2 pt_P  = ImVec2(w_w - 300, mb_S.y);
        static ImVec2 pt_S  = ImVec2(300, w_h - mb_S.y);
        static ImVec2 pt_Sr = ImVec2(6.40000, 1.046766);
        static ImVec2 pt_Pr = ImVec2(1.175750, 22.869566);
        if (properties) {
          ImGui::SetNextWindowPos(pt_P);
          ImGui::SetNextWindowSize(pt_S);
          ImGui::Begin("Properties", NULL, ImGuiWindowFlags_NoMove);
          pt_P = ImGui::GetWindowPos();
          pt_S = ImGui::GetWindowSize();
          if (ly_save) {
            pt_Sr   = extra::GetWindowSRatio();
            pt_Pr   = extra::GetWindowPRatio();
            ly_save = false;
          }
          if (resizing) {
            pt_P     = ImVec2(w_w / pt_Pr.x, mb_S.y);
            pt_S     = ImVec2(w_w / pt_Sr.x, w_h / pt_Sr.y);
            resizing = false;
          }

          /// content-properties
          {
            {
              if (!bf.win.empty()) {
                const char* items[bf.win.size()];
                for (auto it = bf.win.begin(); it != bf.win.end(); ++it) {
                  Win& w   = *it;
                  int  i   = std::distance(bf.win.begin(), it);
                  items[i] = w.name.c_str();
                }
                static int item_current = 0;
                ImGui::Combo("combo", &item_current, items,
                             IM_ARRAYSIZE(items));
                ImGui::SameLine();
                HelpMarker(
                    "Using the simplified one-liner Combo API here.\nRefer to "
                    "the \"Combo\" section below for an explanation of how to "
                    "use the more flexible and general BeginCombo/EndCombo "
                    "API.");
              }
            }
          }

          ImGui::End();
        }

        //-----------------------------------------------------------------------------
        // ANCHOR VIEWPORT
        ImVec2 vp_P = ImVec2(sb_S.x, mb_S.y);
        ImVec2 vp_S = ImVec2(pt_P.x - sb_S.x, w_h - mb_S.y);
        if (viewport) {
          ImGui::SetNextWindowPos(vp_P);
          ImGui::SetNextWindowSize(vp_S);
          ImGui::Begin("Viewport", NULL,
                       ImGuiWindowFlags_NoResize |
                           ImGuiWindowFlags_NoBringToFrontOnFocus);

          /// content-viewport
          {
            ImGui::Text("%d", bf.win.size());
            bf.drawall();
            ImGui::Text("%d", bf.win.size());
            extra::metrics();
          }

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
            std::cout << "hello4" << std::endl;
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
      {
        static char text[1024 * 16] =
            "/*\n"
            " GENERATED CODE\n"
            " READ-ONLY | IMSTUDIO IS NOT A COMPILER FOR C++!\n"
            "*/\n\n"
            "auto layout = You.DesignSomethingFancy();\n"
            "ImStudio.GenerateCode(layout);";
            
            

        ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 64), ImGuiInputTextFlags_ReadOnly);
      }
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