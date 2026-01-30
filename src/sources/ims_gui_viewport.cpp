#include "ims_gui.h"

void ImStudio::GUI::ShowViewport()
{
    ImGui::SetNextWindowPos(vp_P);
    ImGui::SetNextWindowSize(vp_S);
    ImGui::Begin("视口", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);

    /// 视口内容
    {
        utils::DrawGrid();
        
        ImGui::Text("缓冲区窗口: %gx%g", bw.size.x, bw.size.y);
        ImGui::SameLine();
        utils::TextCentered("在与控件交互之前，请确保锁定它们。", 1);
        ImGui::Text("光标: %gx%g", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
        ImGui::Text("对象: %d", static_cast<int>(bw.objects.size()));
        if (!bw.objects.empty()) ImGui::Text("选中: %s", bw.getbaseobj(bw.selected_obj_id)->identifier.c_str());
        ImGui::Text("性能: %.1f FPS", ImGui::GetIO().Framerate);
        utils::HelpMarker("热键:\nAlt + M - \"添加\" 上下文菜单\n"
                          "左右箭头 - 循环选择对象\n"
                          "Ctrl + E - 聚焦到属性字段\nDelete - 删除选中对象");
        
        bw.drawall();
    }

    ImGui::End();
}
