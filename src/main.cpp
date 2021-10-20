#include "includes.h"
#include "sources/object.h"
#include "sources/buffer.h"
#include "sources/gui.h"

//-----------------------------------------------------------------------------
// ANCHOR GLFW STUFF
//-----------------------------------------------------------------------------

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

//-----------------------------------------------------------------------------
// SECTION MAIN FUNC()
// ANCHOR GLFW BOILERPLATE
//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    int  w_w      = 900;
    int  w_h      = 600;
    bool resizing = false;

    //---------------------------------------------------
    // ANCHOR ARGS
    //---------------------------------------------------
    std::vector<std::string> args(argv, argv + argc);

    for (size_t i = 1; i < args.size(); ++i)
    {
        if (args[i] == "-x")
        {
        }
    }
    //---------------------------------------------------
    //---------------------------------------------------

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

#if defined(IMGUI_IMPL_OPENGL_ES2)
    const char *glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    //-----------------------------------------------------------------------------
    // ANCHOR CREATE glwindow
    //-----------------------------------------------------------------------------

    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER,
                   GLFW_TRUE); // glwindow to transparent; handle color through
                               // (internal) ImGui Window;

    GLFWwindow *glwindow = glfwCreateWindow(w_w, w_h, "ImStudio", NULL, NULL);

    if (glwindow == NULL)
        return 1;

    glfwGetWindowSize(glwindow, &w_w, &w_h);

    glfwSetWindowUserPointer(glwindow, &resizing);

    glfwSetWindowSizeCallback(glwindow, [](GLFWwindow *window, int width, int height) {
        bool *resizing = static_cast<bool *>(glfwGetWindowUserPointer(window));
        *resizing      = true;
    });

    glfwMakeContextCurrent(glwindow);
    glfwSwapInterval(1); // Enable vsync
    // extra::glfwSetWindowCenter(glwindow);

    //-----------------------------------------------------------------------------
    // ANCHOR IMGUI
    //-----------------------------------------------------------------------------

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(glwindow, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    //-----------------------------------------------------------------------------
    // ANCHOR STYLES & SETTINGS
    //-----------------------------------------------------------------------------

    // io.Fonts->Build();
    io.IniFilename = NULL;
    ImVec4 bg      = ImVec4(0.123f, 0.123f, 0.123, 1.00f); // Main bg color

    ImGuiStyle &style = ImGui::GetStyle();

    style.WindowPadding  = ImVec2(12.00f, 8.00f);
    style.ItemSpacing    = ImVec2(15.00f, 4.00f);
    style.GrabMinSize    = 20.00f;
    style.WindowRounding = 8.00f;
    style.FrameRounding  = 12.00f;
    style.GrabRounding   = 12.00f;

    //ImVec4 *colors = style.Colors; // TODO Redo color scheme

    //-----------------------------------------------------------------------------
    // ANCHOR LAYOUT (size & pos) | Define relationships between windows
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // ANCHOR STATE (WORKSPACES & MAIN LAYOUT WINDOWS)
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // ANCHOR STATE (CHILDREN)
    //-----------------------------------------------------------------------------

    GUI gui;

    gui.bw.objects.reserve(250);

    //-----------------------------------------------------------------------------
    // ANCHOR VARS
    //-----------------------------------------------------------------------------

    std::mt19937 rng(time(NULL));

    //-----------------------------------------------------------------------------
    // SECTION MAIN LOOP
    //-----------------------------------------------------------------------------

    while ((!glfwWindowShouldClose(glwindow)) && (gui.state))
    {
        std::uniform_int_distribution<int> gen(999, 9999);

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
        gui.mb_P = ImVec2(0, 0);
        gui.mb_S = ImVec2(w_w, 46);
        if (gui.menubar)
        {
            gui.ShowMenubar();
        }

        // workspace-create
        if (gui.wksp_create)
        {

            {
                // create-sidebar
                gui.sb_P = ImVec2(0, gui.mb_S.y);
                gui.sb_S = ImVec2(w_w / 12, w_h - gui.mb_S.y);
                if (gui.sidebar)
                {
                    gui.ShowSidebar();
                }

                // create-properties
                gui.pt_P = ImVec2(w_w - 300, gui.mb_S.y);
                gui.pt_S = ImVec2(300, w_h - gui.mb_S.y);
                if (gui.properties)
                {
                    gui.ShowProperties();
                }

                // create-viewport
                gui.vp_P = ImVec2(gui.sb_S.x, gui.mb_S.y);
                gui.vp_S = ImVec2(gui.pt_P.x - gui.sb_S.x, w_h - gui.mb_S.y);
                if (gui.viewport)
                {
                    gui.ShowViewport(gen(rng));
                }

                if (gui.child_sty)
                {
                    if (ImGui::Begin("Style Editor", &gui.child_sty, ImGuiWindowFlags_AlwaysAutoResize))
                    {
                        ImGui::ShowStyleEditor();
                        ImGui::End();
                    }
                }

                if (gui.child_demo)
                {
                    ImGui::ShowDemoWindow(&gui.child_demo);
                }

                if (gui.child_metrics)
                {
                    ImGui::ShowMetricsWindow(&gui.child_metrics);
                }

                if (gui.child_stack)
                {
                    // ImGui::ShowStackToolWindow(); //Need update
                }

                if (gui.child_colexp)
                {
                    ImGui::SetNextWindowBgAlpha(0.35f);
                    if (ImGui::Begin("Color Export", &gui.child_colexp, ImGuiWindowFlags_AlwaysAutoResize))
                    {
                        ImGui::ColorEdit3("Your Color", (float *)&bg, ImGuiColorEditFlags_Float);
                        if (ImGui::Button("Export to Clipboard"))
                        {
                            std::string exp = "ImVec4 col = ImVec4(" + std::to_string(bg.x) + "f," +
                                              std::to_string(bg.y) + "f," + std::to_string(bg.z) + "f,1.00f);";
                            ImGui::LogToClipboard();
                            ImGui::LogText(exp.c_str());
                            ImGui::LogFinish();
                        }
                        ImGui::End();
                    }
                }
            }
        } //! SECTION wksp_create End
        //-----------------------------------------------------------------------------
        // ANCHOR wksp_output
        gui.ot_P = ImVec2(0, gui.mb_S.y);
        gui.ot_S = ImVec2(w_w, w_h - gui.mb_S.y);
        if (gui.wksp_output)
        {
            gui.ShowOutputWorkspace();
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
