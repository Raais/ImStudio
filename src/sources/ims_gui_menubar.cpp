#include "ims_gui.h"

void ImStudio::GUI::ShowMenubar()
{
    ImGui::SetNextWindowPos(mb_P);
    ImGui::SetNextWindowSize(mb_S);
    ImGui::Begin("菜单栏", NULL,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    // 菜单
    if (ImGui::BeginMenuBar())
    {
        /// 菜单-文件
        if (ImGui::BeginMenu("文件"))
        {
            #ifndef __EMSCRIPTEN__
            if (ImGui::MenuItem("导出到剪贴板"))
            {
                ImGui::LogToClipboard();
                ImGui::LogText("%s", output.c_str());
                ImGui::LogFinish();
            };
            #endif

            if (ImGui::MenuItem("退出"))
            {
                state = false;
            };
            ImGui::EndMenu();
        }

        /// 菜单-编辑
        if (ImGui::BeginMenu("编辑"))
        {
            if (ImGui::BeginMenu("布局"))
            {
                ImGui::MenuItem("紧凑", NULL, &compact);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("行为"))
            {
                ImGui::MenuItem("静态模式", NULL, &bw.staticlayout);
                ImGui::SameLine();
                utils::HelpMarker("在静态/线性布局和固定/手动布局之间切换");

                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("重置"))
            {
                bw.objects.clear();
                bw.selected_obj_id = -1;
                bw.open_child_id = -1;
                bw.open_child = false;
                bw.idgen = 0;
            }

            ImGui::EndMenu();
        }

        /// 菜单-工具
        if (ImGui::BeginMenu("工具"))
        {
            ImGui::MenuItem("样式编辑器", NULL, &child_style);
            ImGui::MenuItem("演示窗口", NULL, &child_demo);
            ImGui::MenuItem("指标", NULL, &child_metrics);
            ImGui::MenuItem("堆栈工具", NULL, &child_stack);
            ImGui::MenuItem("颜色导出", NULL, &child_color);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("帮助"))
        {
            if (ImGui::MenuItem("资源")) child_resources = true;
            if (ImGui::MenuItem("关于 ImStudio")) child_about = true;
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    // 标签
    if (!compact)
    {
        if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
        {
            // 标签-创建
            if (ImGui::BeginTabItem("创建"))
            {
                wksp_output = false;
                wksp_create = true;
                ImGui::EndTabItem();
            }

            // 标签-输出
            if (ImGui::BeginTabItem("输出"))
            {
                wksp_create = false;
                wksp_output = true;
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }
    

    ImGui::End();
}
