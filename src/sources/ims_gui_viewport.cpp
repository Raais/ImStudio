#include "ims_gui.h"

void ImStudio::GUI::ShowViewport()
{
    ImGui::SetNextWindowPos(vp_P);
    ImGui::SetNextWindowSize(vp_S);
    ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);

    /// content-viewport
    {
        utils::DrawGrid();
        
        ImGui::Text("Buffer Window: %gx%g", bw.size.x, bw.size.y);
        ImGui::SameLine();
        utils::TextCentered("Make sure to lock widgets before interacting with them.", 1);
        ImGui::Text("Cursor: %gx%g", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
        ImGui::Text("Objects: %d", static_cast<int>(bw.objects.size()));
        if (!bw.objects.empty()) ImGui::Text("Selected: %s", bw.getbaseobj(bw.selected_obj_id)->identifier.c_str());
        ImGui::Text("Performance: %.1f FPS", ImGui::GetIO().Framerate);
        utils::HelpMarker("Hotkeys:\nAlt + E - \"Add Item\" context menu\n"\
                          "Left/Right Arrow - Cycle object selection\n"\
                          "Ctrl + E - Focus on property field\nDelete - Delete selected object");
        
        bw.drawall();
    }

    ImGui::End();
}