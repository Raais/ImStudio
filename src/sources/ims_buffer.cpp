#include "ims_buffer.h"

void ImStudio::BufferWindow::drawall()
{
    if (state)
    {
        ImVec2 parent_size = ImGui::GetWindowSize();
        ImVec2 init_size   = ImVec2(parent_size.x * 0.8, parent_size.y * 0.7);
        ImVec2 center      = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(size);
        ImGui::SetNextWindowSize(init_size, ImGuiCond_Once);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.08f, 0.09f, 0.09f, 1.00f));
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.08f, 0.09f, 0.09f, 1.00f));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.26f, 0.59f, 0.98f, 0.40f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.86f, 0.86f, 0.86f, 0.50f));
        ImGui::Begin("缓冲区", &state);
        size = ImGui::GetWindowSize();
        pos  = ImGui::GetWindowPos();
        {
            // 拖动时绘制光标
            if (ImGui::IsMouseDown(0))
            {
                ImGui::GetIO().MouseDrawCursor = true;
            }
            else
            {
                ImGui::GetIO().MouseDrawCursor = false;
            }

            // 热键: ALT + M - "添加" 上下文菜单
            if ((ImGui::IsWindowHovered()) &&
                ((ImGui::IsKeyDown(ImGuiKey_ModAlt) && (ImGui::IsKeyPressed(ImGuiKey_M))) ||
                (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(1))))
            {
                ImGui::OpenPopup("bwcontextmenu");

            }
            if (ImGui::BeginPopupContextWindow("bwcontextmenu"))
            {
                if (ImGui::BeginMenu("添加"))
                {
                    if (ImGui::BeginMenu("基础组件"))
                    {
                        if (ImGui::MenuItem("按钮"))
                            create("button",1);
                        if (ImGui::MenuItem("单选按钮"))
                            create("radio",1);
                        if (ImGui::MenuItem("复选框"))
                            create("checkbox",1);
                        if (ImGui::MenuItem("文本"))
                            create("text",1);
                        if (ImGui::MenuItem("项目符号"))
                            create("bullet",1);
                        if (ImGui::MenuItem("箭头"))
                            create("arrow",1);
                        if (ImGui::MenuItem("下拉菜单"))
                            create("combo",1);
                        if (ImGui::MenuItem("列表框"))
                            create("listbox",1);
                        ImGui::EndMenu();
                    }
                    if (ImGui::BeginMenu("数据输入"))
                    {
                        if (ImGui::MenuItem("文本输入"))
                            create("textinput",1);
                        if (ImGui::MenuItem("整数输入"))
                            create("inputint",1);
                        if (ImGui::MenuItem("浮点数输入"))
                            create("inputfloat",1);
                        if (ImGui::MenuItem("双精度输入"))
                            create("inputdouble",1);
                        if (ImGui::MenuItem("科学计数法输入"))
                            create("inputscientific",1);
                        if (ImGui::MenuItem("三维向量输入"))
                            create("inputfloat3",1);
                        if (ImGui::MenuItem("拖动整数"))
                            create("dragint",1);
                        if (ImGui::MenuItem("拖动整数百分比"))
                            create("dragint100",1);
                        if (ImGui::MenuItem("拖动浮点数"))
                            create("dragfloat",1);
                        if (ImGui::MenuItem("拖动小浮点数"))
                            create("dragfloatsmall",1);
                        if (ImGui::MenuItem("滑块整数"))
                            create("sliderint",1);
                        if (ImGui::MenuItem("滑块浮点数"))
                            create("sliderfloat",1);
                        if (ImGui::MenuItem("对数滑块"))
                            create("sliderfloatlog",1);
                        if (ImGui::MenuItem("角度滑块"))
                            create("sliderangle",1);
                        ImGui::EndMenu();
                    }
                    if (ImGui::BeginMenu("颜色选择器"))
                    {
                        if (ImGui::MenuItem("颜色 1"))
                            create("color1",1);
                        if (ImGui::MenuItem("颜色 2"))
                            create("color2",1);
                        if (ImGui::MenuItem("颜色 3"))
                            create("color3",1);
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndPopup();
            }

            for (auto i = objects.begin(); i != objects.end(); ++i)
            {
                Object &o = *i;

                if (o.state == false)
                {
                    i = objects.erase(i);
                    break;
                }
                else
                {
                    if (o.type != "child")
                    {
                        o.draw(&selected_obj_id, staticlayout);
                    }
                    else
                    {
                        o.child.drawall(&selected_obj_id, staticlayout);
                    }
                }
            }
        }
        ImGui::End();
        ImGui::PopStyleColor(4);
    }
}

ImStudio::Object *ImStudio::BufferWindow::getobj(int id)
{
    for (Object &o : objects)
    {
        if (o.id == id)
        {
            return &o;
        }
    }
    return nullptr;
}

ImStudio::BaseObject *ImStudio::BufferWindow::getbaseobj(int id)
{
    for (Object &o : objects)
    {
        if (o.id == id)
        {
            return &o;
        }
        if (!o.child.objects.empty())
        {
            for (BaseObject &cw : o.child.objects)
            {
                if (cw.id == id)
                {
                    return &cw;
                }
            }
        }
    }
    return nullptr;
}

void ImStudio::BufferWindow::create(std::string type_, bool atcursor)
{
    idgen++;
    if (!((open_child) &&
          (getbaseobj(open_child_id)) &&
          (getobj(open_child_id)->child.open)))
    {
        // 没有打开子窗口
        Object widget(idgen, type_);
        if (atcursor)
        {
            widget.pos = ImVec2(ImGui::GetMousePos().x - pos.x, ImGui::GetMousePos().y - pos.y);
        }
        objects.push_back(widget);
    }
    else
    {
        // 打开子窗口
        BaseObject childwidget(idgen, type_, open_child_id);
        getobj(open_child_id)->child.objects.push_back(childwidget);
    }
    selected_obj_id = idgen; // 选择新对象
}
