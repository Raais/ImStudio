#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <emscripten.h>
#include <SDL.h>
#include <SDL_opengles2.h>

#include "includes.h"
#include "sources/object.h"
#include "sources/buffer.h"
#include "sources/gui.h"

SDL_Window *g_Window = NULL;
SDL_GLContext g_GLContext = NULL;
struct State
{
    ImStudio::GUI gui;
    std::mt19937 rng;
    int w_w = 900;
    int w_h = 600;
};

static void main_loop(void *);

int main(int, char **)
{
    State state;
    state.gui.bw.objects.reserve(2048);
    state.rng.seed(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    const char *glsl_version = "#version 100";
    // const char* glsl_version = "#version 300 es";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    SDL_WindowFlags window_flags =
        (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    g_Window = SDL_CreateWindow("ImStudio", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    g_GLContext = SDL_GL_CreateContext(g_Window);
    if (!g_GLContext)
    {
        fprintf(stderr, "Failed to initialize WebGL context!\n");
        return 1;
    }
    SDL_GL_SetSwapInterval(1); // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    io.IniFilename = NULL;

    ImGuiStyle &style = ImGui::GetStyle();

    style.WindowPadding = ImVec2(12.00f, 8.00f);
    style.ItemSpacing = ImVec2(15.00f, 4.00f);
    style.GrabMinSize = 20.00f;
    style.WindowRounding = 8.00f;
    style.FrameBorderSize = 1.00f;
    style.FrameRounding = 4.00f;
    style.GrabRounding = 12.00f;

    ImVec4 *colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_Text] = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(0.12f, 0.12f, 0.12f, 0.50f);
    colors[ImGuiCol_Button] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);

    ImGui_ImplSDL2_InitForOpenGL(g_Window, g_GLContext);
    ImGui_ImplOpenGL3_Init(glsl_version);

    /////////////////////////////////////////////////////////
    emscripten_set_main_loop_arg(main_loop, &state, 0, true);
    /////////////////////////////////////////////////////////
}

static void main_loop(void *arg)
{
    State &state = *(State *)arg;
    ImStudio::GUI &gui = state.gui;
    std::mt19937 &rng = state.rng;
    int &w_w = state.w_w;
    int &w_h = state.w_h;
    
    std::uniform_int_distribution<int> gen(999, 9999);

    ImGuiIO &io = ImGui::GetIO();

    static ImVec4 bg = ImVec4(0.123f, 0.123f, 0.123, 1.00f);

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
    }
    w_w = io.DisplaySize.x;
    w_h = io.DisplaySize.y;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::SetNextWindowBgAlpha(0.00f);

    // window-menubar
    gui.mb_P = ImVec2(0, 0);
    gui.mb_S = ImVec2(w_w, 46);
    if (gui.menubar) gui.ShowMenubar();

    // workspace-create
    if (!gui.compact){
        if (gui.wksp_create)
        {// create-main
            {// create-sidebar
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
        }
        // workspace-output
        gui.ot_P = ImVec2(0, gui.mb_S.y);
        gui.ot_S = ImVec2(w_w, w_h - gui.mb_S.y);
        if (gui.wksp_output) gui.ShowOutputWorkspace();
    }
    else
    {
        gui.wksp_output = true;
            
        // create-sidebar
        gui.sb_P = ImVec2(0, gui.mb_S.y);
        gui.sb_S = ImVec2(170, w_h - gui.mb_S.y);
        if (gui.sidebar) gui.ShowSidebar();

        // create-properties
        gui.pt_P = ImVec2(w_w - 300, gui.mb_S.y);
        gui.pt_S = ImVec2(300, w_h - gui.mb_S.y);
        if (gui.properties) gui.ShowProperties();

        // workspace-output
        gui.ot_P = ImVec2(gui.sb_S.x, w_h - 300);
        gui.ot_S = ImVec2(gui.pt_P.x - gui.sb_S.x, 300);
        if (gui.wksp_output) gui.ShowOutputWorkspace();

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

        if (gui.child_resources) utils::ShowResourcesWindow(&gui.child_resources);

        if (gui.child_about) utils::ShowAboutWindow(&gui.child_about);
    }

    // Rendering
    ImGui::Render();
    SDL_GL_MakeCurrent(g_Window, g_GLContext);
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(bg.x * bg.w, bg.y * bg.w, bg.z * bg.w, bg.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(g_Window);
}