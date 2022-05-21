#include "ims_gui.h"

void ImStudio::GUI::ShowProperties()
{
    ImGui::SetNextWindowPos(pt_P);
    ImGui::SetNextWindowSize(pt_S);
    ImGui::Begin("Properties", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    {
        {
            if (!bw.objects.empty())
            {
                // array for Properties "object" drop down combo
                std::vector<const char*> prop_objects_arr; // contains identifiers ex: child1::button2
                std::vector<int>         idof_prop_objects_arr; // contains id associated with ^
                int                      curr_index = 0;
                static int               selected_index = 0;

                static BaseObject* selected_obj_ptr = nullptr;
                static int         prev_selected_obj_id = 0;
                // static = persistent in loop / between calls
                
                for (Object &obj : bw.objects) // Fill both arrays with contents from bw.objects
                {
                    prop_objects_arr.push_back(const_cast<char *>(obj.identifier.c_str()));
                    idof_prop_objects_arr.push_back(obj.id);

                    if (obj.id == bw.selected_obj_id) // if o is last selected item
                    {
                        selected_index = curr_index; // select it from prop_objects_arr
                    }
                    curr_index++;

                    // now do the same if obj is a child window and has children
                    // we cant use recursion because different types Object vs BaseObject
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

                // Cycle selection through array elements using left/right arrow keys
                //HOTKEY: RIGHT ARROW - Cycle selected item forward
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
                //HOTKEY: LEFT ARROW - Cycle selected item backward
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

                ImGui::Combo("Object", &selected_index,  prop_objects_arr.data(), prop_objects_arr.size());

                selected_obj_ptr = bw.getbaseobj(idof_prop_objects_arr[selected_index]);
                bw.selected_obj_id  = selected_obj_ptr->id;
                // selecting object from properties
                // this will be overridden from viewport bufferwindow drag (BaseObject.draw() assigns to active item)
                // and vice versa

                ////////PROP BUFFERS/////////
                static std::string prop_inputbuf_value   = "change me";
                static std::string prop_inputbuf_label   = "##";
                /////////////////////////////

                if (bw.selected_obj_id != prev_selected_obj_id)
                {
                    //reset prop buffers so text input value is not transferred from object to object
                    prop_inputbuf_value = "change me";
                    prop_inputbuf_label = "##";
                }

                // Per object property fields
                if (selected_obj_ptr->type == "button")
                {
                    if (selected_obj_ptr->propinit) prop_inputbuf_value = selected_obj_ptr->value_s; // run after first time

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    //HOTKEY: CTRL + E - Focus on property field
                    if (ImGui::IsKeyDown(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("Value", &prop_inputbuf_value);
                    selected_obj_ptr->value_s = prop_inputbuf_value;
                    ImGui::NewLine();

                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);

                    if (selected_obj_ptr->center_h) ImGui::BeginDisabled(true);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    if (selected_obj_ptr->center_h) ImGui::EndDisabled();

                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");

                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    ImGui::NewLine();

                    ImGui::Checkbox("Auto Resize", &selected_obj_ptr->autoresize);
                    
                    if (selected_obj_ptr->autoresize) ImGui::BeginDisabled(true);
                    ImGui::InputFloat("Size X", &selected_obj_ptr->size.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Size Y", &selected_obj_ptr->size.y, 1.0f, 10.0f, "%.3f");
                    if (selected_obj_ptr->autoresize) ImGui::EndDisabled();

                    //HOTKEY: DELETE - Delete selected object
                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
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

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("Label", &prop_inputbuf_label);
                    selected_obj_ptr->label   = prop_inputbuf_label;
                    ImGui::Combo("Value", &cur, items, IM_ARRAYSIZE(items));
                    if (cur == 0) selected_obj_ptr->value_b = false;
                    else selected_obj_ptr->value_b = true;
                    
                    ImGui::NewLine();

                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
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

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("Label", &prop_inputbuf_label);
                    selected_obj_ptr->label   = prop_inputbuf_label;
                    ImGui::Combo("Value", &cur, items, IM_ARRAYSIZE(items));
                    if (cur == 0) selected_obj_ptr->value_b = false;
                    else selected_obj_ptr->value_b = true;
                    
                    ImGui::NewLine();

                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
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

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("Value", &prop_inputbuf_value);
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->value_s = prop_inputbuf_value;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "bullet")
                {
                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "arrow")
                {
                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();
                    
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "combo")
                {
                    if (selected_obj_ptr->propinit) prop_inputbuf_label = selected_obj_ptr->label;

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("Label", &prop_inputbuf_label);
                    selected_obj_ptr->label = prop_inputbuf_label;
                    ImGui::NewLine();

                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();

                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "listbox")
                {
                    if (selected_obj_ptr->propinit) prop_inputbuf_label = selected_obj_ptr->label;

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("Label", &prop_inputbuf_label);
                    selected_obj_ptr->label = prop_inputbuf_label;
                    ImGui::NewLine();

                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();

                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
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

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::InputText("Value", &prop_inputbuf_value);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label   = prop_inputbuf_label;
                    selected_obj_ptr->value_s = prop_inputbuf_value;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
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

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
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

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
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

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
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

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
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

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
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

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
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

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
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

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
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

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
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

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
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

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
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

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
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

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
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

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
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

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
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

                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if (ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && (ImGui::IsKeyPressed(ImGuiKey_E)))
                    {
                        ImGui::SetKeyboardFocusHere();
                    }
                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }

                if (selected_obj_ptr->type == "child")
                {
                    if (bw.getobj(selected_obj_ptr->id)->child.open) ImGui::Text("OPEN");
                    else ImGui::Text("CLOSED");
                    ImGui::NewLine();

                    ImGui::Checkbox("Border", &bw.getobj(selected_obj_ptr->id)->child.border);
                    ImGui::NewLine();

                    ImGui::InputFloat("Min X", &bw.getobj(selected_obj_ptr->id)->child.grab1.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Min Y", &bw.getobj(selected_obj_ptr->id)->child.grab1.y, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Max X", &bw.getobj(selected_obj_ptr->id)->child.grab2.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Max Y", &bw.getobj(selected_obj_ptr->id)->child.grab2.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &bw.getobj(selected_obj_ptr->id)->child.locked);

                    if (ImGui::Button("Open"))
                    {
                        bw.getobj(selected_obj_ptr->id)->child.open = true;
                        bw.open_child = true;
                        bw.open_child_id = selected_obj_ptr->id;
                    }
                    if (ImGui::Button("Close"))
                    {
                        bw.getobj(selected_obj_ptr->id)->child.open = false;
                        bw.open_child = false;
                        bw.open_child_id = -1;
                    }
                    ImGui::NewLine();

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        if (bw.open_child_id == selected_obj_ptr->id)
                        {
                            bw.open_child = false;
                            bw.open_child_id = -1;
                        }
                        bw.getobj(selected_obj_ptr->id)->child.open = false;
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "sameline")
                {
                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();
                    
                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "newline")
                {
                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "separator")
                {
                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "progressbar")
                {
                    
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();
                    
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGuiKey_Delete)))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                selected_obj_ptr->propinit = true;
                prev_selected_obj_id = selected_obj_ptr->id;
            }
        }
    }

    ImGui::End();
}