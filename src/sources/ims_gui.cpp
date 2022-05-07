#include "ims_gui.h"

// ANCHOR MENUBAR.DEFINITION
void ImStudio::GUI::ShowMenubar()
{
    ImGui::SetNextWindowPos(mb_P);
    ImGui::SetNextWindowSize(mb_S);
    ImGui::Begin("Menubar", NULL,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    // MENU
    if (ImGui::BeginMenuBar())
    {
        /// menu-file
        if (ImGui::BeginMenu("File"))
        {
            #ifndef __EMSCRIPTEN__
            if (ImGui::MenuItem("Export to clipboard"))
            {
                ImGui::LogToClipboard();
                ImGui::LogText("%s", output.c_str());
                ImGui::LogFinish();
            };
            #endif

            if (ImGui::MenuItem("Exit"))
            {
                state = false;
            };
            ImGui::EndMenu();
        }

        /// menu-edit
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::BeginMenu("Layout"))
            {
                ImGui::MenuItem("Compact", NULL, &compact);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Behavior"))
            {
                ImGui::MenuItem("Static Mode", NULL, &bw.staticlayout);
                ImGui::SameLine();
                utils::HelpMarker("Toggle between static/linear layout and fixed/manual layout");

                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Reset"))
            {
                bw.objects.clear();
                bw.selected_obj_id = -1;
                bw.open_child_id = -1;
                bw.open_child = false;
                bw.idgen = 0;
            }

            ImGui::EndMenu();
        }

        /// menu-tools
        if (ImGui::BeginMenu("Tools"))
        {
            ImGui::MenuItem("Style Editor", NULL, &child_style);
            ImGui::MenuItem("Demo Window", NULL, &child_demo);
            ImGui::MenuItem("Metrics", NULL, &child_metrics);
            ImGui::MenuItem("Stack Tool", NULL, &child_stack);
            ImGui::MenuItem("Color Export", NULL, &child_color);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("Resources")) child_resources = true;
            if (ImGui::MenuItem("About ImStudio")) child_about = true;
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    // TAB
    if (!compact)
    {
        if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
        {
            // tab-create
            if (ImGui::BeginTabItem("Create"))
            {
                wksp_output = false;
                wksp_create = true;
                ImGui::EndTabItem();
            }

            // tab-output
            if (ImGui::BeginTabItem("Output"))
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

// ANCHOR SIDEBAR.DEFINITION
void ImStudio::GUI::ShowSidebar()
{
    ImGui::SetNextWindowPos(sb_P);
    ImGui::SetNextWindowSizeConstraints(ImVec2(0, -1), ImVec2(FLT_MAX, -1));
    ImGui::SetNextWindowSize(sb_S);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.00f, 5.00f));
    ImGui::Begin("Sidebar", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    /// content-sidebar
    {

        ImGui::TextDisabled("NOTE");
        ImGui::SameLine(); utils::HelpMarker
        ("THESE ARE NOT THE ONLY WIDGETS IMGUI PROVIDES!\n"
        "You can find out more in the Dear ImGui Demo "
        "(Tools > Demo Window) and imgui/imgui_demo.cpp");
        ImGui::Separator();

        //ANCHOR SIDEBAR.PRIMITIVES
        ImGui::Text("Primitives");
        ImGui::Separator();

        if (ImGui::Button("Window"))
        {
            bw.state = true;
        }

        if (ImGui::Button("Button"))
        {
            bw.create("button");
        }

        if (ImGui::Button("Radio Button"))
        {
            bw.create("radio");
        }

        if (ImGui::Button("Checkbox"))
        {
            bw.create("checkbox");
        }

        if (ImGui::Button("Text"))
        {
            bw.create("text");
        }

        if (ImGui::Button("Bullet"))
        {
            bw.create("bullet");
        }

        if (ImGui::Button("Arrow"))
        {
            bw.create("arrow");
        }

        if (ImGui::Button("Combo"))
        {
            bw.create("combo");
        }

        if (ImGui::Button("Listbox"))
        {
            bw.create("listbox");
        }
        ImGui::Separator();

        //ANCHOR SIDEBAR.DATAINPUTS
        ImGui::Text("Data Inputs");
        ImGui::Separator();

        if (ImGui::Button("Input Text"))
        {
            bw.create("textinput");
        }

        if (ImGui::Button("Input Int"))
        {
            bw.create("inputint");
        }
        ImGui::SameLine(); utils::HelpMarker
        ("You can apply arithmetic operators +,*,/ on numerical values.\n"
        "  e.g. [ 100 ], input \'*2\', result becomes [ 200 ]\n"
        "Use +- to subtract.");

        if (ImGui::Button("Input Float"))
        {
            bw.create("inputfloat");
        }

        if (ImGui::Button("Input Double"))
        {
            bw.create("inputdouble");
        }

        if (ImGui::Button("Input Scientific"))
        {
            bw.create("inputscientific");
        }
        ImGui::SameLine(); utils::HelpMarker
        ("You can input value using the scientific notation,\n"
        "  e.g. \"1e+8\" becomes \"100000000\".");

        if (ImGui::Button("Input Float3"))
        {
            bw.create("inputfloat3");
        }

        if (ImGui::Button("Drag Int"))
        {
            bw.create("dragint");
        }
        ImGui::SameLine(); utils::HelpMarker
        ("Click and drag to edit value.\n"
        "Hold SHIFT/ALT for faster/slower edit.\n"
        "Double-click or CTRL+click to input value.");

        if (ImGui::Button("Drag Int %"))
        {
            bw.create("dragint100");
        }

        if (ImGui::Button("Drag Float"))
        {
            bw.create("dragfloat");
        }

        if (ImGui::Button("Drag Float Small"))
        {
            bw.create("dragfloatsmall");
        }

        if (ImGui::Button("Slider Int"))
        {
            bw.create("sliderint");
        }
        ImGui::SameLine(); utils::HelpMarker("CTRL+click to input value.");

        if (ImGui::Button("Slider Float"))
        {
            bw.create("sliderfloat");
        }

        if (ImGui::Button("Slider Float Log"))
        {
            bw.create("sliderfloatlog");
        }

        if (ImGui::Button("Slider Angle"))
        {
            bw.create("sliderangle");
        }
        ImGui::Separator();
        
        ImGui::Text("Color Pickers");
        ImGui::Separator();

        if (ImGui::Button("Color 1"))
        {
            bw.create("color1");
        }

        if (ImGui::Button("Color 2"))
        {
            bw.create("color2");
        }

        if (ImGui::Button("Color 3"))
        {
            bw.create("color3");
        }
        ImGui::Separator();

        //ANCHOR SIDEBAR.OTHERS
        ImGui::Text("Others");
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
        ("Green = Open (Ready to add items). Calling EndChild will close it, "
        "and you can't add items to it unless you manually re-open it.");

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
        ("Groups are not a feature of ImStudio, but you can probably use "
         "a child (without borders) to reproduce similar behavior.");

        if (ImGui::Button("<< Same Line"))
        {
            bw.create("sameline");
        }

        if (ImGui::Button("New Line"))
        {
            bw.create("newline");
        }

        if (ImGui::Button("Separator"))
        {
            bw.create("separator");
        }

        if (ImGui::Button("Progress Bar"))
        {
            bw.create("progressbar");
        }
        ImGui::Separator();

        ImGui::Checkbox("Static Mode", &bw.staticlayout);

        if ((ImGui::GetIO().KeyAlt) && (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_F4))))
        {
            state = false;
        }
        
    }

    ImGui::End();
    ImGui::PopStyleVar(1);
}

// ANCHOR PROPERTIES.DEFINITION
void ImStudio::GUI::ShowProperties()
{
    ImGui::SetNextWindowPos(pt_P);
    ImGui::SetNextWindowSize(pt_S);
    ImGui::Begin("Properties", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    {
        {
            /*
            Apologies for this convoluted mess. Will probably rewrite all this sometime.
            TODO: Break this up into multiple files.
            */
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

                if (selected_obj_ptr->type == "button")
                {
                    if (selected_obj_ptr->propinit) prop_inputbuf_value = selected_obj_ptr->value_s; // run after first time

                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

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

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
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

                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

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

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
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

                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

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

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
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

                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Value", &prop_inputbuf_value);
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->value_s = prop_inputbuf_value;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "bullet")
                {
                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "arrow")
                {
                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();
                    
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "combo")
                {
                    if (selected_obj_ptr->propinit) prop_inputbuf_label = selected_obj_ptr->label;

                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_inputbuf_label);
                    selected_obj_ptr->label = prop_inputbuf_label;
                    ImGui::NewLine();

                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();

                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "listbox")
                {
                    if (selected_obj_ptr->propinit) prop_inputbuf_label = selected_obj_ptr->label;

                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_inputbuf_label);
                    selected_obj_ptr->label = prop_inputbuf_label;
                    ImGui::NewLine();

                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();

                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
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

                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

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

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
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

                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
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

                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
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

                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
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

                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
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

                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
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

                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
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

                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
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

                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
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

                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
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

                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
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

                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
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

                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
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

                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
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

                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
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

                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
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

                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_inputbuf_label);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selected_obj_ptr->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);
                    selected_obj_ptr->label = prop_inputbuf_label;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
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

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
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
                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();
                    
                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "newline")
                {
                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "separator")
                {
                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selected_obj_ptr->del();
                        if (selected_index != 0) selected_index -= 1;
                    }
                }
                if (selected_obj_ptr->type == "progressbar")
                {
                    //Stats
                    if (selected_obj_ptr->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();
                    
                    ImGui::Checkbox("Center Horizontally", &selected_obj_ptr->center_h);
                    ImGui::InputFloat("Position X", &selected_obj_ptr->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selected_obj_ptr->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selected_obj_ptr->locked);

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
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

// ANCHOR VIEWPORT.DEFINITION
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
        ImGui::Text("Objects: %d", static_cast<int>(bw.objects.size()));
        if (!bw.objects.empty()) ImGui::Text("Selected: %s", bw.getbaseobj(bw.selected_obj_id)->identifier.c_str());
        ImGui::Text("Performance: %.1f FPS", ImGui::GetIO().Framerate);
        
        bw.drawall();
    }

    ImGui::End();
}

// ANCHOR OUTPUTWKSP.DEFINITION
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
