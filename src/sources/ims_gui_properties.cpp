#include "ims_gui.h"

void ImStudio::GUI::ShowProperties()
{
    ImGui::SetNextWindowPos(pt_P);
    ImGui::SetNextWindowSize(pt_S);
    ImGui::Begin("属性", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    {
        {
            if (!bw.objects.empty())
            {
                // 属性"对象"下拉组合框的数组
                std::vector<const char*> prop_objects_arr; // 包含标识符，例如：child1::button2
                std::vector<int>         idof_prop_objects_arr; // 包含与^关联的ID
                int                      curr_index = 0;
                static int               selected_index = 0;

                static BaseObject* selected_obj_ptr = nullptr;
                static int         prev_selected_obj_id = 0;
                // static = 在循环/调用之间持久化
                
                for (Object &obj : bw.objects) // 用bw.objects的内容填充两个数组
                {
                    prop_objects_arr.push_back(const_cast<char *>(obj.identifier.c_str()));
                    idof_prop_objects_arr.push_back(obj.id);

                    if (obj.id == bw.selected_obj_id) // 如果o是最后选择的项目
                    {
                        selected_index = curr_index; // 从prop_objects_arr中选择它
                    }
                    curr_index++;

                    // 现在如果obj是子窗口并具有子项，则执行相同操作
                    // 我们不能使用递归，因为不同类型的Object与BaseObject
                    ////////////////////////////////////////////////////////////////////////////////////////////
                    if (!obj.child.objects.empty()){
                        for (BaseObject &childobj : obj.child.objects)
                        {
                            prop_objects_arr.push_back(const_cast<char *>(childobj.identifier.c_str()));
                            idof_prop_objects_arr.push_back(childobj.id);
                            if (childobj.id == bw.selected_obj_id)
                            {
                                selected_index = curr_index;
                            }
                            curr_index++;
                        }
                    }
                    ////////////////////////////////////////////////////////////////////////////////////////////
                }

                // 使用左右箭头键循环选择数组元素
                // 热键: 右箭头 - 向前循环选择项目
                if (ImGui::IsKeyPressed(ImGuiKey_RightArrow))
                {
                    if (selected_index < prop_objects_arr.size() - 1)
                    {
                        selected_index++;
                    }
                    else
                    {
                        selected_index = 0;
                    }
                }
                // 热键: 左箭头 - 向后循环选择项目
                if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow))
                {
                    if (selected_index > 0)
                    {
                        selected_index--;
                    }
                    else
                    {
                        selected_index = prop_objects_arr.size() - 1;
                    }
                }

                ImGui::Combo("对象", &selected_index,  prop_objects_arr.data(), prop_objects_arr.size());

                selected_obj_ptr = bw.getbaseobj(idof_prop_objects_arr[selected_index]);
                bw.selected_obj_id  = selected_obj_ptr->id;
                // 从属性中选择对象
                // 这将被视口缓冲区窗口拖动覆盖（BaseObject.draw()分配给活动项目）
                // 反之亦然

                ////////属性缓冲区/////////
                static std::string prop_inputbuf_value   = "change me";
                static std::string prop_inputbuf_label   = "##";
                /////////////////////////////

                if (bw.selected_obj_id != prev_selected_obj_id)
                {
                    // 重置属性缓冲区，以便文本输入值不会从一个对象传递到另一个对象
                    prop_inputbuf_value = "change me";
                    prop_inputbuf_label = "##";
                }

                // 每个对象的属性字段
                if (selected_obj_ptr->type == "button")
                {
                    if (selected_obj_ptr->propinit) prop_inputbuf_value = selected_obj_ptr->value_s; // 第一次运行后执行

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("子控件: True");
                    else ImGui::Text("子控件: False");
                    ImGui::NewLine();

                    // 热键: CTRL + E - 聚焦到属性字段
                    if (ImGui::IsKeyDown(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("值", &prop_inputbuf_value);
                    selected_obj_ptr->value_s = prop_inputbuf_value;
                    ImGui::NewLine();

                    ImGui::Checkbox("水平居中", &selected_obj_ptr->center_h);

                    if (selected_obj_ptr->center_h) ImGui::BeginDisabled(true);
                    ImGui::InputFloat("位置 X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    if (selected_obj_ptr->center_h) ImGui::EndDisabled();

                    ImGui::InputFloat("位置 Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");

                    ImGui::Checkbox("拖动锁定", &selected_obj_ptr->locked);
                    ImGui::NewLine();

                    ImGui::Checkbox("自动调整大小", &selected_obj_ptr->autoresize);
                    
                    if (selected_obj_ptr->autoresize) ImGui::BeginDisabled(true);
                    ImGui::InputFloat("大小 X", &selected_obj_ptr->size.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("大小 Y", &selected_obj_ptr->size.y, 1.0f, 10.0f, "%.3f");
                    if (selected_obj_ptr->autoresize) ImGui::EndDisabled();

                    // 热键: DELETE - 删除选中的对象
                    if ((ImGui::Button("删除")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }

                if (selected_obj_ptr->type == "checkbox")
                {
                    const char *items[] = {"False", "True"};
                    static int  cur     = 0;
                    if (selected_obj_ptr->propinit)
                    {
                        prop_inputbuf_label = selected_obj_ptr->label;
                        cur = selected_obj_ptr->value_b;
                    }

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("子控件: True");
                    else ImGui::Text("子控件: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("标签", &prop_inputbuf_label);
                    selected_obj_ptr->label   = prop_inputbuf_label;
                    ImGui::Combo("值", &cur, items, IM_ARRAYSIZE(items));
                    if (cur == 0) selected_obj_ptr->value_b = false;
                    else selected_obj_ptr->value_b = true;
                    
                    ImGui::NewLine();

                    ImGui::Checkbox("水平居中", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("位置 X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("位置 Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("拖动锁定", &selected_obj_ptr->locked);

                    if ((ImGui::Button("删除")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }

                if (selected_obj_ptr->type == "radio")
                {
                    const char *items[] = {"False", "True"};
                    static int  cur     = 0;
                    if (selected_obj_ptr->propinit)
                    {
                        prop_inputbuf_label = selected_obj_ptr->label;
                        cur = selected_obj_ptr->value_b;
                    }

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("子控件: True");
                    else ImGui::Text("子控件: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("标签", &prop_inputbuf_label);
                    selected_obj_ptr->label   = prop_inputbuf_label;
                    ImGui::Combo("值", &cur, items, IM_ARRAYSIZE(items));
                    if (cur == 0) selected_obj_ptr->value_b = false;
                    else selected_obj_ptr->value_b = true;
                    
                    ImGui::NewLine();

                    ImGui::Checkbox("水平居中", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("位置 X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("位置 Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("拖动锁定", &selected_obj_ptr->locked);

                    if ((ImGui::Button("删除")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "text")
                {
                    if (selected_obj_ptr->propinit)
                    {
                        prop_inputbuf_value = selected_obj_ptr->value_s;
                    }

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("子控件: True");
                    else ImGui::Text("子控件: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("值", &prop_inputbuf_value);
                    ImGui::NewLine();
                    ImGui::Checkbox("水平居中", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("位置 X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("位置 Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("拖动锁定", &selected_obj_ptr->locked);
                    selected_obj_ptr->value_s = prop_inputbuf_value;

                    if ((ImGui::Button("删除")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "bullet")
                {
                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("子控件: True");
                    else ImGui::Text("子控件: False");
                    ImGui::NewLine();

                    ImGui::Checkbox("水平居中", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("位置 X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("位置 Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("拖动锁定", &selected_obj_ptr->locked);

                    if ((ImGui::Button("删除")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "arrow")
                {
                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("子控件: True");
                    else ImGui::Text("子控件: False");
                    ImGui::NewLine();
                    
                    ImGui::Checkbox("水平居中", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("位置 X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("位置 Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("拖动锁定", &selected_obj_ptr->locked);

                    if ((ImGui::Button("删除")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "combo")
                {
                    if (selected_obj_ptr->propinit) prop_inputbuf_label = selected_obj_ptr->label;

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("子控件: True");
                    else ImGui::Text("子控件: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("标签", &prop_inputbuf_label);
                    selected_obj_ptr->label = prop_inputbuf_label;
                    ImGui::NewLine();

                    ImGui::InputFloat("宽度", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();

                    ImGui::Checkbox("水平居中", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("位置 X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("位置 Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("拖动锁定", &selected_obj_ptr->locked);

                    if ((ImGui::Button("删除")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "listbox")
                {
                    if (selected_obj_ptr->propinit) prop_inputbuf_label = selected_obj_ptr->label;

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("子控件: True");
                    else ImGui::Text("子控件: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("标签", &prop_inputbuf_label);
                    selected_obj_ptr->label = prop_inputbuf_label;
                    ImGui::NewLine();

                    ImGui::InputFloat("宽度", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();

                    ImGui::Checkbox("水平居中", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("位置 X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("位置 Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("拖动锁定", &selected_obj_ptr->locked);

                    if ((ImGui::Button("删除")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "textinput")
                {
                    if (selected_obj_ptr->propinit)
                    {
                        prop_inputbuf_value = selected_obj_ptr->value_s;
                        prop_inputbuf_label = selected_obj_ptr->label;
                    }

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("子控件: True");
                    else ImGui::Text("子控件: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("标签", &prop_inputbuf_label);
                    ImGui::InputText("值", &prop_inputbuf_value);
                    ImGui::NewLine();
                    ImGui::InputFloat("宽度", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("水平居中", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("位置 X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("位置 Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("拖动锁定", &selected_obj_ptr->locked);
                    selected_obj_ptr->label   = prop_inputbuf_label;
                    selected_obj_ptr->value_s = prop_inputbuf_value;

                    if ((ImGui::Button("删除")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "inputint")
                {
                    if (selected_obj_ptr->propinit)
                    {
                        prop_inputbuf_label = selected_obj_ptr->label;
                    }

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("子控件: True");
                    else ImGui::Text("子控件: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("标签", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("宽度", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("水平居中", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("位置 X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("位置 Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("拖动锁定", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("删除")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "inputfloat")
                {
                    if (selected_obj_ptr->propinit)
                    {
                        prop_inputbuf_label = selected_obj_ptr->label;
                    }

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("子控件: True");
                    else ImGui::Text("子控件: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("标签", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("宽度", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("水平居中", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("位置 X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("位置 Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("拖动锁定", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("删除")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "inputdouble")
                {
                    if (selected_obj_ptr->propinit)
                    {
                        prop_inputbuf_label = selected_obj_ptr->label;
                    }

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("子控件: True");
                    else ImGui::Text("子控件: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("标签", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("宽度", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("水平居中", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("位置 X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("位置 Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("拖动锁定", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("删除")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "inputscientific")
                {
                    if (selected_obj_ptr->propinit)
                    {
                        prop_inputbuf_label = selected_obj_ptr->label;
                    }

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("子控件: True");
                    else ImGui::Text("子控件: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("标签", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("宽度", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("水平居中", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("位置 X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("位置 Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("拖动锁定", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("删除")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "inputfloat3")
                {
                    if (selected_obj_ptr->propinit)
                    {
                        prop_inputbuf_label = selected_obj_ptr->label;
                    }

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("子控件: True");
                    else ImGui::Text("子控件: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("标签", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("宽度", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("水平居中", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("位置 X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("位置 Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("拖动锁定", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("删除")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "dragint")
                {
                    if (selected_obj_ptr->propinit)
                    {
                        prop_inputbuf_label = selected_obj_ptr->label;
                    }

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("子控件: True");
                    else ImGui::Text("子控件: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("标签", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("宽度", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("水平居中", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("位置 X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("位置 Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("拖动锁定", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("删除")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "dragint100")
                {
                    if (selected_obj_ptr->propinit)
                    {
                        prop_inputbuf_label = selected_obj_ptr->label;
                    }

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("子控件: True");
                    else ImGui::Text("子控件: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("标签", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("宽度", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("水平居中", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("位置 X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("位置 Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("拖动锁定", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("删除")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "dragfloat")
                {
                    if (selected_obj_ptr->propinit)
                    {
                        prop_inputbuf_label = selected_obj_ptr->label;
                    }

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("子控件: True");
                    else ImGui::Text("子控件: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("标签", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("宽度", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("水平居中", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("位置 X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("位置 Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("拖动锁定", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("删除")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "dragfloatsmall")
                {
                    if (selected_obj_ptr->propinit)
                    {
                        prop_inputbuf_label = selected_obj_ptr->label;
                    }

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("子控件: True");
                    else ImGui::Text("子控件: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("标签", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("宽度", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("水平居中", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("位置 X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("位置 Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("拖动锁定", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("删除")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "sliderint")
                {
                    if (selected_obj_ptr->propinit)
                    {
                        prop_inputbuf_label = selected_obj_ptr->label;
                    }

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("子控件: True");
                    else ImGui::Text("子控件: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("标签", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("宽度", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("水平居中", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("位置 X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("位置 Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("拖动锁定", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("删除")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "sliderfloat")
                {
                    if (selected_obj_ptr->propinit)
                    {
                        prop_inputbuf_label = selected_obj_ptr->label;
                    }

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("子控件: True");
                    else ImGui::Text("子控件: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("标签", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("宽度", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("水平居中", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("位置 X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("位置 Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("拖动锁定", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("删除")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "sliderfloatlog")
                {
                    if (selected_obj_ptr->propinit)
                    {
                        prop_inputbuf_label = selected_obj_ptr->label;
                    }

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("子控件: True");
                    else ImGui::Text("子控件: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("标签", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("宽度", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("水平居中", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("位置 X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("位置 Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("拖动锁定", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("删除")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "sliderangle")
                {
                    if (selected_obj_ptr->propinit)
                    {
                        prop_inputbuf_label = selected_obj_ptr->label;
                    }

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("子控件: True");
                    else ImGui::Text("子控件: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("标签", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("宽度", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("水平居中", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("位置 X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("位置 Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("拖动锁定", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("删除")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "color1")
                {
                    if (selected_obj_ptr->propinit)
                    {
                        prop_inputbuf_label = selected_obj_ptr->label;
                    }

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("子控件: True");
                    else ImGui::Text("子控件: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("标签", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::Checkbox("水平居中", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("位置 X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("位置 Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("拖动锁定", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("删除")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "color2")
                {
                    if (selected_obj_ptr->propinit)
                    {
                        prop_inputbuf_label = selected_obj_ptr->label;
                    }

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("子控件: True");
                    else ImGui::Text("子控件: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("标签", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("宽度", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("水平居中", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("位置 X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("位置 Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("拖动锁定", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("删除")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "color3")
                {
                    if (selected_obj_ptr->propinit)
                    {
                        prop_inputbuf_label = selected_obj_ptr->label;
                    }

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("子控件: True");
                    else ImGui::Text("子控件: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("标签", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("宽度", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("水平居中", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("位置 X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("位置 Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("拖动锁定", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("删除")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
            }
        }
    }
    ImGui::End();
}
