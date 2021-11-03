#include "includes.h"
#include "sources/object.h"
#include "sources/buffer.h"
#include "sources/console.h"
#include "sources/gui.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int argc, char *argv[])
{
    int  w_w      = 900;
    int  w_h      = 600;
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

    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER,
                   GLFW_TRUE);

    GLFWwindow *glwindow = glfwCreateWindow(w_w, w_h, "ImStudio", NULL, NULL);

    if (glwindow == NULL)
        return 1;

    glfwGetWindowSize(glwindow, &w_w, &w_h);

    glfwMakeContextCurrent(glwindow);
    glfwSwapInterval(1); // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(glwindow, true);
    ImGui_ImplOpenGL3_Init(glsl_version);


    // io.Fonts->Build();
    io.IniFilename              = NULL;
    ImVec4 bg                   = ImVec4(0.123f, 0.123f, 0.123, 1.00f);
    //GLFW_TRANSPARENT_FRAMEBUFFER + 0 alpha value ^ works if you have a compositor

    ImGuiStyle &style = ImGui::GetStyle();

    style.WindowPadding         = ImVec2(12.00f, 8.00f);
    style.ItemSpacing           = ImVec2(15.00f, 4.00f);
    style.GrabMinSize           = 20.00f;
    style.WindowRounding        = 8.00f;
    style.FrameBorderSize       = 1.00f;
    style.FrameRounding         = 4.00f;
    style.GrabRounding          = 12.00f;

    ImVec4* colors = ImGui::GetStyle().Colors;
    
    colors[ImGuiCol_Text]       = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
    colors[ImGuiCol_WindowBg]   = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
    colors[ImGuiCol_Border]     = ImVec4(0.12f, 0.12f, 0.12f, 0.50f);
    colors[ImGuiCol_Button]     = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);

    ImStudio::GUI gui;
    gui.bw.objects.reserve(2048);

    std::mt19937 rng(time(NULL)); //MT-PRNG

    while ((!glfwWindowShouldClose(glwindow)) && (gui.state))
    {
        causefail
        std::uniform_int_distribution<int> gen(999, 9999);

        glfwPollEvents();
        glfwGetWindowSize(glwindow, &w_w, &w_h);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::SetNextWindowBgAlpha(0.00f);

#define MAIN

#ifdef MAIN

        // window-menubar
        gui.mb_P = ImVec2(0, 0);
        gui.mb_S = ImVec2(w_w, 46);
        if (gui.menubar) gui.ShowMenubar();

        // workspace-create
        if (gui.wksp_create)
        {
            { // create-main
                // create-sidebar
                gui.sb_P = ImVec2(0, gui.mb_S.y);
                gui.sb_S = ImVec2(170, w_h - gui.mb_S.y);
                if (gui.sidebar) gui.ShowSidebar();

                // create-properties
                gui.pt_P = ImVec2(w_w - 300, gui.mb_S.y);
                gui.pt_S = ImVec2(300, w_h - gui.mb_S.y);
                if (gui.properties) gui.ShowProperties();

                // create-viewport
                gui.vp_P = ImVec2(gui.sb_S.x, gui.mb_S.y);
                gui.vp_S = ImVec2(gui.pt_P.x - gui.sb_S.x, w_h - gui.mb_S.y);
                if (gui.viewport) gui.ShowViewport(gen(rng));

            }
            { // create-children
                if (gui.child_style) utils::ShowStyleEditorWindow(&gui.child_style);

                if (gui.child_demo) ImGui::ShowDemoWindow(&gui.child_demo);

                if (gui.child_metrics) ImGui::ShowMetricsWindow(&gui.child_metrics);

                if (gui.child_stack) ImGui::ShowStackToolWindow(&gui.child_stack);

                if (gui.child_color) utils::ShowColorExportWindow(&gui.child_color);

                if (gui.child_console) gui.ShowConsole(&gui.child_console,&gui);

                if (gui.child_resources) utils::ShowResourcesWindow(&gui.child_resources);

                if (gui.child_about) utils::ShowAboutWindow(&gui.child_about);
            }
        }

        // workspace-output
        gui.ot_P = ImVec2(0, gui.mb_S.y);
        gui.ot_S = ImVec2(w_w, w_h - gui.mb_S.y);
        if (gui.wksp_output) gui.ShowOutputWorkspace();

#endif

#ifdef TEST

#endif

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(glwindow, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(bg.x * bg.w, bg.y * bg.w, bg.z * bg.w, bg.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(glwindow);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(glwindow);
    glfwTerminate();

    return 0;
}
