#include "ims_gui.h"

void ImStudio::GUI::ShowOutputWorkspace()
{
    ImGui::SetNextWindowPos(ot_P);
    ImGui::SetNextWindowSizeConstraints(ImVec2(0, -1), ImVec2(FLT_MAX, -1));
    ImGui::SetNextWindowSize(ot_S);
    ImGui::Begin("wksp_output", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
    {
#ifdef __EMSCRIPTEN__
        if(ImGui::Button("Copy")){
            ImGui::LogToClipboard();
            ImGui::LogText(output.c_str());
            ImGui::LogFinish();
        };
        JsClipboard_SetClipboardText(ImGui::GetClipboardText());
#endif
        ImStudio::GenerateCode(&output, &bw);
    }
    ImGui::End();
}