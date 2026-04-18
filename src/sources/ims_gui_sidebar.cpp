#include "ims_gui.h"

void ImStudio::GUI::ShowSidebar()
{
    ImGui::SetNextWindowPos(sb_P);
    ImGui::SetNextWindowSizeConstraints(ImVec2(0, -1), ImVec2(FLT_MAX, -1));
    ImGui::SetNextWindowSize(sb_S);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.00f, 5.00f));
    ImGui::Begin("侧边栏", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    /// 侧边栏内容
    {

        ImGui::TextDisabled("注意");
        ImGui::SameLine(); utils::HelpMarker
        ("这些不是 ImGui 提供的唯一控件！\n"
        "你可以在 Dear ImGui 演示中找到更多内容 "
        "（工具 > 演示窗口）和 imgui/imgui_demo.cpp");
        ImGui::Separator();

        // 锚点 SIDEBAR.PRIMITIVES
        ImGui::Text("基础组件");
        ImGui::Separator();

        if (ImGui::Button("窗口"))
        {
            bw.state = true;
        }

        if (ImGui::Button("按钮"))
            bw.create("button");

        if (ImGui::Button("单选按钮"))
            bw.create("radio");

        if (ImGui::Button("复选框"))
            bw.create("checkbox");

        if (ImGui::Button("文本"))
            bw.create("text");

        if (ImGui::Button("项目符号"))
            bw.create("bullet");

        if (ImGui::Button("箭头"))
            bw.create("arrow");

        if (ImGui::Button("下拉菜单"))
            bw.create("combo");

        if (ImGui::Button("列表框"))
            bw.create("listbox");

        ImGui::Separator();

        // 锚点 SIDEBAR.DATAINPUTS
        ImGui::Text("数据输入");
        ImGui::Separator();

        if (ImGui::Button("文本输入"))
            bw.create("textinput");

        if (ImGui::Button("整数输入"))
            bw.create("inputint");

        ImGui::SameLine(); utils::HelpMarker
        ("你可以对数值应用算术运算符 +,*,/。\n"
        " 例如：[ 100 ]，输入 '*2'，结果变为 [ 200 ]\n"
        "使用 +- 进行减法。");

        if (ImGui::Button("浮点数输入"))
            bw.create("inputfloat");

        if (ImGui::Button("双精度输入"))
            bw.create("inputdouble");

        if (ImGui::Button("科学计数法输入"))
            bw.create("inputscientific");

        ImGui::SameLine(); utils::HelpMarker
        ("你可以使用科学计数法输入值，\n"
        " 例如：\"1e+8\" 变为 \"100000000\"。");

        if (ImGui::Button("三维向量输入"))
            bw.create("inputfloat3");

        if (ImGui::Button("拖动整数"))
            bw.create("dragint");

        ImGui::SameLine(); utils::HelpMarker
        ("点击并拖动以编辑值。\n"
        "按住 SHIFT/ALT 可加快/减慢编辑速度。\n"
        "双击或 CTRL+点击以输入值。");

        if (ImGui::Button("拖动整数百分比"))
            bw.create("dragint100");

        if (ImGui::Button("拖动浮点数"))
            bw.create("dragfloat");

        if (ImGui::Button("拖动小浮点数"))
            bw.create("dragfloatsmall");

        if (ImGui::Button("滑块整数"))
            bw.create("sliderint");

        ImGui::SameLine(); utils::HelpMarker("CTRL+点击以输入值。");

        if (ImGui::Button("滑块浮点数"))
            bw.create("sliderfloat");

        if (ImGui::Button("对数滑块"))
            bw.create("sliderfloatlog");

        if (ImGui::Button("角度滑块"))
            bw.create("sliderangle");

        ImGui::Separator();
        
        ImGui::Text("颜色选择器");
        ImGui::Separator();

        if (ImGui::Button("颜色 1"))
            bw.create("color1");

        if (ImGui::Button("颜色 2"))
            bw.create("color2");

        if (ImGui::Button("颜色 3"))
            bw.create("color3");

        ImGui::Separator();

        // 锚点 SIDEBAR.OTHERS
        ImGui::Text("其他");
        ImGui::Separator();

        if (bw.open_child)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 1.000f, 0.110f, 1.000f));
            ImGui::Button("BeginChild");
            ImGui::PopStyleColor(1);
        }
        else
        {
            if (ImGui::Button("BeginChild"))
            {
                bw.create("child");
                bw.open_child_id = bw.idgen;
                bw.open_child = true;
            }
        }
        ImGui::SameLine(); utils::HelpMarker
        ("绿色 = 打开（准备添加项目）。调用 EndChild 将关闭它，"
        " 除非你手动重新打开，否则无法向其添加项目。");

        if (ImGui::Button("EndChild"))
        {
            if ((bw.getbaseobj(bw.open_child_id)))
            {
                bw.getobj(bw.open_child_id)->child.open = false;
                bw.open_child_id = -1;
            }
            bw.open_child = false;
        }

        ImGui::BeginDisabled(true);
        if (ImGui::Button("BeginGroup"))
        {
            //
        }
        ImGui::EndDisabled();
        ImGui::SameLine(); utils::HelpMarker
        ("组不是 ImStudio 的功能，但你可以使用"
         " 子项（无边框）来重现类似行为。");

        if (ImGui::Button("<< 同一行"))
        {
            bw.create("sameline");
        }

        if (ImGui::Button("新行"))
        {
            bw.create("newline");
        }

        if (ImGui::Button("分隔线"))
        {
            bw.create("separator");
        }

        if (ImGui::Button("进度条"))
        {
            bw.create("progressbar");
        }
        ImGui::Separator();

        ImGui::Checkbox("静态模式", &bw.staticlayout);

        if ((ImGui::GetIO().KeyAlt) && (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_F4))))
        {
            state = false;
        }
        
    }

    ImGui::End();
    ImGui::PopStyleVar(1);
}
