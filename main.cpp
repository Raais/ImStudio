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

//-----------------------------------------------------------------------------
// ANCHOR OBJECTS
//-----------------------------------------------------------------------------

// ANCHOR Object
class Object {//class-object
 public:
  int         id;
  std::string identifier;
  std::string type;
  bool        state   = true;
  bool        value_b = true;
  bool        moving  = false;
  std::string value_s;
  ImVec2      pos = ImVec2(100, 100);
  ImVec2      size;
  Object(int idvar_, std::string type_) {//class-object-constr
    id         = idvar_;
    type       = type_;
    identifier = type_ + std::to_string(idvar_);
    value_s    = type_ + std::to_string(idvar_);
    std::cout << "Object constr: " << value_s << "::" << this << std::endl;
  }
  void draw(int* select) {
    if (state) {
      if (type == "button") {
        std::cout << "drawing button: " << this << std::endl;
        ImGui::SetCursorPos(pos);

        ImGui::PushID(id);
        ImGui::Button(value_s.c_str());
        ImGui::PopID();

        if (ImGui::IsItemActive()) {
          pos.x   = extra::GetLocalCursor().x;
          pos.y   = extra::GetLocalCursor().y;
          *select = id;
          // selected_ = this;
          std::cout << "obj-active assign" << std::endl;
        } else {
          std::cout << "drawing button: not active" << std::endl;
        }

        highlight(select);
        // ImGui::Text("%g,%g",extra::GetLastItemPos().x,
        // extra::GetLastItemPos().y);
      }
      if (type == "checkbox") {
        ImGui::Checkbox(value_s.c_str(), &value_b);
        // highlight();
      }
      if (type == "radio") {
        ImGui::RadioButton(value_s.c_str(), &value_b);
        // highlight();
      }
      if (type == "combo") {
        const char* items[]      = {"Never", "Gonna", "Give", "You", "Up"};
        static int  item_current = 0;
        ImGui::Combo(value_s.c_str(), &item_current, items,
                     IM_ARRAYSIZE(items));
        // highlight();
      }
    }
  }
  void del() { state = false; }

 private:
  void highlight(int* select) {
    if (id == *select) {
      std::cout << "highlight" << std::endl;
      ImGui::GetForegroundDrawList()->AddRect(
          ImGui::GetCurrentContext()->LastItemData.Rect.Min,
          ImGui::GetCurrentContext()->LastItemData.Rect.Max,
          IM_COL32(255, 255, 0, 255));
    }
  }
};

class PropertyBuffer {
  public:
    //static char prop_text1[128] = "Text";
    void resetpropbuffer() {
      //prop_text1 = "Text";
    }
}

// ANCHOR BufferWindow
class BufferWindow : public PropertyBuffer {//class-object
 public:
  int         id    = 0;
  bool        state = false;
  std::string name  = "window_0";
  ImVec2      size  = ImVec2(1070, 680);
  ImVec2      pos   = ImVec2(280, 120);
  int         idvar = 0;

  // std::vector<Win> win = {};
  std::vector<Object> objects = {};
  void                drawall(int* select) {//class-object-drawall
    if (state) {
      ImGui::SetNextWindowPos(pos, ImGuiCond_Once);
      ImGui::SetNextWindowSize(size, ImGuiCond_Once);
      ImGui::Begin(name.c_str(), &state);
      {
        // std::cout << "drawall [" << selected_ << "]" << std::endl;
        extra::metrics();
        for (auto i = objects.begin(); i != objects.end(); ++i) {
          Object& o = *i;
          o.draw(select);

          if (o.state == false) {
            i = objects.erase(i);
            break;
          }
        }
      }
      ImGui::End();
    }
  }
  Object* getobj(int id) {
    for (Object& o : objects) {
      if (o.id == id) {
        return &o;
      }
    }
  }
  bool AnySelected(Object* selected_) {
    for (Object& o : objects) {
      if (&o == selected_) {
        return true;
      }
    }
    return false;
  }
  std::string gettype(int id) {
    for (Object& o : objects) {
      if (o.id == id) {
        return o.type;
      }
    }
  }
  void create(std::string type_) {
    idvar++;
    Object widget(idvar, type_);
    std::cout << "created button: " << &widget << std::endl;
    objects.push_back(widget);
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

  bool         child_debug   = false;
  bool         child_sty     = false;
  bool         child_metrics = false;
  bool         child_colexp  = false;
  bool         child_stack   = false;
  bool         ly_save       = false;
  BufferWindow bf;
  bf.objects.reserve(250);

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
          ImGui::MenuItem("Stack Tool", NULL, &child_stack);
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
      static Object* selectobj    = nullptr;
      static Object* selectobjprev    = nullptr;
      static int     select       = 0;
      static int     item_current = 0;

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
                bf.state = true;
              }
              if (ImGui::Button("Checkbox")) {
                bf.create("checkbox");
              }
              if (ImGui::Button("Button")) {
                std::cout << "creating button" << std::endl;
                bf.create("button");
              }
              if (ImGui::Button("Radio Button")) {
                bf.create("radio");
              }
              if (ImGui::Button("Combo")) {
                bf.create("combo");
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
        static bool   is    = false;
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
              if (!bf.objects.empty()) {
                const char* items[bf.objects.size()];
                int         idarr[bf.objects.size()];
                int         i = 0;
                for (auto it = bf.objects.begin(); it != bf.objects.end();
                     ++it) {
                  Object& o = *it;
                  items[i]  = o.identifier.c_str();
                  idarr[i]  = o.id;
                  if (o.id == select) {
                    if (!is) {
                      item_current = i;
                    }

                    std::cout << i << std::endl;
                    std::cout << "prop-pre check" << std::endl;
                  } else {
                    std::cout << "prop-pre check FAIL" << std::endl;
                  }
                  i++;
                }

                ImGui::Combo("combo", &item_current, items,
                             IM_ARRAYSIZE(items));

                if (ImGui::IsMouseDown(0)) {
                  is = false;
                } else {
                  is = true;
                }

                if (!is) {//viewport select
                  selectobj = bf.getobj(select);
                } else {//combo select
                  selectobj = bf.getobj(idarr[item_current]);
                  select    = selectobj->id;
                }

                /*if(!selectobjprev){
                  selectobjprev = selectobj;
                }*/

                if (selectobj->type == "button") {
                  //if(selectobj->id != selectobjprev->id){bf.resetpropbuffer();}
                  static char prop_text1[128] = "Text";
                  ImGui::InputText("Value", prop_text1, IM_ARRAYSIZE(prop_text1));
                  selectobj->value_s = prop_text1;
                  std::cout << "button properties" << std::endl;
                }
                if (selectobj->type == "checkbox") {
                }
                if (selectobj->type == "radio") {
                }
                if (selectobj->type == "combo") {
                }
                selectobjprev = selectobj;
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
            std::cout << "drawing viewport" << std::endl;
            ImGui::Text("%d", bf.objects.size());
            if (!bf.objects.empty()) {
              std::stringstream ss;
              ImGui::Text("Selected = %s", selectobj->identifier.c_str());
            }
            bf.drawall(&select);
            // ImGui::Text("%d", bf.win.size());

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

        if (child_stack) {
          //ImGui::ShowStackToolWindow(); //Need update
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

        ImGui::InputTextMultiline(
            "##source", text, IM_ARRAYSIZE(text),
            ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 64),
            ImGuiInputTextFlags_ReadOnly);
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
