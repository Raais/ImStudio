#include "main.h"

void MainWindowStyle()
{
    
    ImGuiIO &io = ImGui::GetIO();

    io.IniFilename              = NULL;

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
    
}

void MainWindowGUI(State & state)
{

    ImStudio::GUI &gui = state.gui;
    std::mt19937 &rng = state.rng;
    ImGuiIO &io = ImGui::GetIO();

    int &w_w = state.w_w;
    int &w_h = state.w_h;
    w_w = io.DisplaySize.x;
    w_h = io.DisplaySize.y;

    std::uniform_int_distribution<int> gen(999, 9999);

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

}