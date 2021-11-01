#include "../includes.h"
#include "object.h"
#include "buffer.h"
#include "console.h"
#include "generator.h"
#include "gui.h"

// ANCHOR MENUBAR.DEFINITION
void ImStudio::GUI::ShowMenubar()
{
    ImGui::SetNextWindowPos(mb_P);
    ImGui::SetNextWindowSize(mb_S);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.859f, 0.859f, 0.859f, 1.000f));
    ImGui::Begin("Menubar", NULL,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    // MENU
    if (ImGui::BeginMenuBar())
    {
        /// menu-file
        if (ImGui::BeginMenu("File"))
        {

            if (ImGui::MenuItem("Exit"))
            {
                state = false;
            };
            ImGui::EndMenu();
        }

        /// menu-edit
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::BeginMenu("Behavior"))
            {
                ImGui::MenuItem("Static Mode", NULL, &bw.staticlayout);
                ImGui::SameLine();
                utils::HelpMarker("Toggle between static/linear layout and fixed/manual layout");

                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Reset"))
            {
                if (bw.current_child)
                    bw.current_child = nullptr;
                bw.objects.clear();
            }

            ImGui::EndMenu();
        }

        /// menu-tools
        if (ImGui::BeginMenu("Tools"))
        {
            ImGui::MenuItem("Style Editor", NULL, &child_style);
            ImGui::MenuItem("Demo Window", NULL, &child_demo);
            ImGui::MenuItem("Console", NULL, &child_console);
            ImGui::MenuItem("Metrics", NULL, &child_metrics);
            ImGui::MenuItem("Stack Tool", NULL, &child_stack);
            ImGui::MenuItem("Color Export", NULL, &child_color);
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    // TAB
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

    ImGui::End();
    ImGui::PopStyleColor(1);
}

// ANCHOR SIDEBAR.DEFINITION
void ImStudio::GUI::ShowSidebar()
{
    ImGui::SetNextWindowPos(sb_P);
    ImGui::SetNextWindowSizeConstraints(ImVec2(0, -1), ImVec2(FLT_MAX, -1));
    ImGui::SetNextWindowSize(sb_S);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.10f, 0.10f, 0.10f, 1.00f));
    ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImVec4(0.67f, 0.67f, 0.67f, 1.00f));
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

        if (bw.current_child)// child exists
        {
            if (bw.current_child->child.open)// child open
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 1.000f, 0.110f, 1.000f));
                ImGui::Button("BeginChild"); // does nothing
                ImGui::PopStyleColor(1);
            }
            else // child closed
            {
                if (ImGui::Button("BeginChild"))
                {
                    bw.create("child");
                }
            }
        }
        else // no child
        {
            if (ImGui::Button("BeginChild"))
            {
                bw.create("child");
            }
        }
        ImGui::SameLine(); utils::HelpMarker
        ("Green = Open (Ready to add items). Calling EndChild will close it, "
        "and you can't add items to it unless you manually re-open it.");

        if (ImGui::Button("EndChild"))
        {
            if(bw.current_child) bw.current_child->child.open = false;
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
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_F9)))
        {
            child_console = not child_console;
        }
        
    }

    ImGui::End();
    ImGui::PopStyleVar(1);
    ImGui::PopStyleColor(2);
}

// ANCHOR PROPERTIES.DEFINITION
void ImStudio::GUI::ShowProperties()
{
    ImGui::SetNextWindowPos(pt_P);
    ImGui::SetNextWindowSize(pt_S);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.859f, 0.859f, 0.859f, 1.000f));
    ImGui::Begin("Properties", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    {
        {
            if (!bw.objects.empty())
            {
                //SECTION CREATE PROPARRAY
                int allvecsize = 0;
                for (Object &o : bw.objects) // Calc total objects created in all vectors [bw.objects+all(child.objects)]
                {
                    allvecsize++;
                    if (!o.child.objects.empty())
                    {
                        for (BaseObject &cw : o.child.objects)
                        {
                            (void)cw;//for compiler warning
                            allvecsize++;
                        }
                    }
                }

                const char *items[allvecsize]; // contains identifiers ex: child1::button2
                int         idarr[allvecsize]; // contains id associated with ^
                int         i = 0;
                for (Object &o : bw.objects) // Fill both arrays with contents from bw.objects [Object]
                {
                    items[i] = o.identifier.c_str();
                    idarr[i] = o.id;
                    if (o.id == selectid) // 1. if last select/drag in bw
                    {
                        if (ImGui::IsMouseDown(0))
                        {
                            selectproparray = i; // 2. select prop from bw
                        }
                    }
                    if (o.id == bw.idvar) // 1. o is last created item
                    {
                        if (o.selectinit == true)
                        {
                            selectproparray = i; // select last created item
                            o.selectinit    = false;
                        }
                    }
                    i++;
                }
                for (Object &o : bw.objects) // cpy-grp
                {
                    if (!o.child.objects.empty())
                    {
                        for (BaseObject &cw :
                             o.child.objects) // Fill both arrays with contents from child.objects [BaseObject]
                        {
                            items[i] = cw.identifier.c_str();
                            idarr[i] = cw.id;
                            if (cw.id == selectid) // 1. if last select/drag in bw
                            {
                                if (ImGui::IsMouseDown(0))
                                {
                                    selectproparray = i; // 2. select prop from bw
                                }
                            }
                            if (cw.id == bw.idvar) // 1. o is last created item
                            {
                                if (cw.selectinit == true)
                                {
                                    selectproparray = i; // select last created item
                                    cw.selectinit   = false;
                                }
                            }
                            i++;
                        }
                    }
                }
                //!SECTION CREATE PROPARRAY

                ImGui::Combo("combo", &selectproparray, items, IM_ARRAYSIZE(items));

                if (ImGui::IsMouseDown(0))
                { // bw select
                    selectobj = bw.getbaseobj(selectid);
                }
                else
                { // combo select if (newest) else {selectid = newest}
                    selectobj = bw.getbaseobj(idarr[selectproparray]);
                    selectid  = selectobj->id;
                }

                /////////PROP BUFFER/////////
                static std::string prop_text1   = "change me";
                static std::string prop_text2   = "";
                /////////////////////////////

                if (selectobj->id != previd)
                {
                    //reset prop buffer so text input value is not transferred from object to object
                    prop_text1 = "change me";
                    prop_text2 = "";
                    //bw.resetpropbuffer();
                }

                if (selectobj->type == "button")
                {
                    if (selectobj->propinit) prop_text1 = selectobj->value_s; // run after first time

                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Value", &prop_text1);
                    selectobj->value_s = prop_text1;
                    ImGui::NewLine();

                    ImGui::Checkbox("Center Horizontally", &selectobj->center_h);

                    if (selectobj->center_h) ImGui::BeginDisabled(true);
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    if (selectobj->center_h) ImGui::EndDisabled();

                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");

                    ImGui::Checkbox("Drag Locked", &selectobj->locked);
                    ImGui::NewLine();

                    ImGui::Checkbox("Auto Resize", &selectobj->autoresize);
                    
                    if (selectobj->autoresize) ImGui::BeginDisabled(true);
                    ImGui::InputFloat("Size X", &selectobj->size.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Size Y", &selectobj->size.y, 1.0f, 10.0f, "%.3f");
                    if (selectobj->autoresize) ImGui::EndDisabled();

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }

                if (selectobj->type == "checkbox")
                {
                    const char *items[] = {"False", "True"};
                    static int  cur     = 0;
                    if (selectobj->propinit)
                    {
                        prop_text2 = selectobj->label;
                        cur = selectobj->value_b;
                    }

                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_text2);
                    selectobj->label   = prop_text2;
                    ImGui::Combo("Value", &cur, items, IM_ARRAYSIZE(items));
                    if (cur == 0) selectobj->value_b = false;
                    else selectobj->value_b = true;
                    
                    ImGui::NewLine();

                    ImGui::Checkbox("Center Horizontally", &selectobj->center_h);
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selectobj->locked);

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }

                if (selectobj->type == "radio")
                {
                    const char *items[] = {"False", "True"};
                    static int  cur     = 0;
                    if (selectobj->propinit)
                    {
                        prop_text2 = selectobj->label;
                        cur = selectobj->value_b;
                    }

                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_text2);
                    selectobj->label   = prop_text2;
                    ImGui::Combo("Value", &cur, items, IM_ARRAYSIZE(items));
                    if (cur == 0) selectobj->value_b = false;
                    else selectobj->value_b = true;
                    
                    ImGui::NewLine();

                    ImGui::Checkbox("Center Horizontally", &selectobj->center_h);
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selectobj->locked);

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }
                if (selectobj->type == "text")
                {
                    if (selectobj->propinit)
                    {
                        prop_text1 = selectobj->value_s;
                    }

                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Value", &prop_text1);
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selectobj->center_h);
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selectobj->locked);
                    selectobj->value_s = prop_text1;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }
                if (selectobj->type == "bullet")
                {
                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::Checkbox("Center Horizontally", &selectobj->center_h);
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selectobj->locked);

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }
                if (selectobj->type == "arrow")
                {
                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();
                    
                    ImGui::Checkbox("Center Horizontally", &selectobj->center_h);
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selectobj->locked);

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }
                if (selectobj->type == "combo")
                {
                    if (selectobj->propinit) prop_text2 = selectobj->label;

                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_text2);
                    selectobj->label = prop_text2;
                    ImGui::NewLine();

                    ImGui::InputFloat("Width", &selectobj->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();

                    ImGui::Checkbox("Center Horizontally", &selectobj->center_h);
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selectobj->locked);

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }
                if (selectobj->type == "listbox")
                {
                    if (selectobj->propinit) prop_text2 = selectobj->label;

                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_text2);
                    selectobj->label = prop_text2;
                    ImGui::NewLine();

                    ImGui::InputFloat("Width", &selectobj->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();

                    ImGui::Checkbox("Center Horizontally", &selectobj->center_h);
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selectobj->locked);

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }
                if (selectobj->type == "textinput")
                {
                    if (selectobj->propinit)
                    {
                        prop_text1 = selectobj->value_s;
                        prop_text2 = selectobj->label;
                    }

                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_text2);
                    ImGui::InputText("Value", &prop_text1);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selectobj->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selectobj->center_h);
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selectobj->locked);
                    selectobj->label   = prop_text2;
                    selectobj->value_s = prop_text1;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }
                if (selectobj->type == "inputint")
                {
                    if (selectobj->propinit)
                    {
                        prop_text2 = selectobj->label;
                    }

                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_text2);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selectobj->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selectobj->center_h);
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selectobj->locked);
                    selectobj->label = prop_text2;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }
                if (selectobj->type == "inputfloat")
                {
                    if (selectobj->propinit)
                    {
                        prop_text2 = selectobj->label;
                    }

                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_text2);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selectobj->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selectobj->center_h);
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selectobj->locked);
                    selectobj->label = prop_text2;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }
                if (selectobj->type == "inputdouble")
                {
                    if (selectobj->propinit)
                    {
                        prop_text2 = selectobj->label;
                    }

                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_text2);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selectobj->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selectobj->center_h);
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selectobj->locked);
                    selectobj->label = prop_text2;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }
                if (selectobj->type == "inputscientific")
                {
                    if (selectobj->propinit)
                    {
                        prop_text2 = selectobj->label;
                    }

                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_text2);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selectobj->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selectobj->center_h);
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selectobj->locked);
                    selectobj->label = prop_text2;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }
                if (selectobj->type == "inputfloat3")
                {
                    if (selectobj->propinit)
                    {
                        prop_text2 = selectobj->label;
                    }

                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_text2);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selectobj->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selectobj->center_h);
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selectobj->locked);
                    selectobj->label = prop_text2;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }
                if (selectobj->type == "dragint")
                {
                    if (selectobj->propinit)
                    {
                        prop_text2 = selectobj->label;
                    }

                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_text2);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selectobj->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selectobj->center_h);
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selectobj->locked);
                    selectobj->label = prop_text2;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }
                if (selectobj->type == "dragint100")
                {
                    if (selectobj->propinit)
                    {
                        prop_text2 = selectobj->label;
                    }

                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_text2);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selectobj->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selectobj->center_h);
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selectobj->locked);
                    selectobj->label = prop_text2;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }
                if (selectobj->type == "dragfloat")
                {
                    if (selectobj->propinit)
                    {
                        prop_text2 = selectobj->label;
                    }

                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_text2);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selectobj->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selectobj->center_h);
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selectobj->locked);
                    selectobj->label = prop_text2;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }
                if (selectobj->type == "dragfloatsmall")
                {
                    if (selectobj->propinit)
                    {
                        prop_text2 = selectobj->label;
                    }

                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_text2);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selectobj->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selectobj->center_h);
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selectobj->locked);
                    selectobj->label = prop_text2;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }
                if (selectobj->type == "sliderint")
                {
                    if (selectobj->propinit)
                    {
                        prop_text2 = selectobj->label;
                    }

                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_text2);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selectobj->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selectobj->center_h);
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selectobj->locked);
                    selectobj->label = prop_text2;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }
                if (selectobj->type == "sliderfloat")
                {
                    if (selectobj->propinit)
                    {
                        prop_text2 = selectobj->label;
                    }

                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_text2);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selectobj->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selectobj->center_h);
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selectobj->locked);
                    selectobj->label = prop_text2;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }
                if (selectobj->type == "sliderfloatlog")
                {
                    if (selectobj->propinit)
                    {
                        prop_text2 = selectobj->label;
                    }

                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_text2);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selectobj->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selectobj->center_h);
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selectobj->locked);
                    selectobj->label = prop_text2;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }
                if (selectobj->type == "sliderangle")
                {
                    if (selectobj->propinit)
                    {
                        prop_text2 = selectobj->label;
                    }

                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_text2);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selectobj->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selectobj->center_h);
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selectobj->locked);
                    selectobj->label = prop_text2;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }
                if (selectobj->type == "color1")
                {
                    if (selectobj->propinit)
                    {
                        prop_text2 = selectobj->label;
                    }

                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_text2);
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selectobj->center_h);
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selectobj->locked);
                    selectobj->label = prop_text2;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }
                if (selectobj->type == "color2")
                {
                    if (selectobj->propinit)
                    {
                        prop_text2 = selectobj->label;
                    }

                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_text2);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selectobj->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selectobj->center_h);
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selectobj->locked);
                    selectobj->label = prop_text2;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }
                if (selectobj->type == "color3")
                {
                    if (selectobj->propinit)
                    {
                        prop_text2 = selectobj->label;
                    }

                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    ImGui::InputText("Label", &prop_text2);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selectobj->width, 1.0f, 10.0f, "%.3f");
                    ImGui::NewLine();
                    ImGui::Checkbox("Center Horizontally", &selectobj->center_h);
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selectobj->locked);
                    selectobj->label = prop_text2;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }

                if (selectobj->type == "child")
                {
                    if (bw.getobj(selectobj->id)->child.open) ImGui::Text("OPEN");
                    else ImGui::Text("CLOSED");
                    ImGui::NewLine();

                    ImGui::Checkbox("Border", &bw.getobj(selectobj->id)->child.border);
                    ImGui::NewLine();

                    ImGui::InputFloat("Min X", &bw.getobj(selectobj->id)->child.grab1.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Min Y", &bw.getobj(selectobj->id)->child.grab1.y, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Max X", &bw.getobj(selectobj->id)->child.grab2.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Max Y", &bw.getobj(selectobj->id)->child.grab2.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &bw.getobj(selectobj->id)->child.locked);

                    if ((ImGui::Button("Open")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_O))))
                    {
                        bw.current_child             = bw.getobj(selectobj->id);
                        bw.current_child->child.open = true;
                    }
                    if (ImGui::Button("Close"))
                    {
                        bw.current_child             = bw.getobj(selectobj->id);
                        bw.current_child->child.open = false;
                    }
                    ImGui::NewLine();

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        bw.getobj(selectobj->id)->child.open = false;
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }
                if (selectobj->type == "sameline")
                {
                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();
                    
                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }
                if (selectobj->type == "newline")
                {
                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }
                if (selectobj->type == "separator")
                {
                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }
                if (selectobj->type == "progressbar")
                {
                    //Stats
                    if (selectobj->ischildwidget) ImGui::Text("Child Widget: True");
                    else ImGui::Text("Child Widget: False");
                    ImGui::NewLine();
                    
                    ImGui::Checkbox("Center Horizontally", &selectobj->center_h);
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Drag Locked", &selectobj->locked);

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0) selectproparray -= 1;
                    }
                }
                selectobj->propinit = true;
                previd              = selectobj->id;
            }
        }
    }

    ImGui::End();
    ImGui::PopStyleColor(1);
}

// ANCHOR VIEWPORT.DEFINITION
void ImStudio::GUI::ShowViewport(int gen_rand)
{
    ImGui::SetNextWindowPos(vp_P);
    ImGui::SetNextWindowSize(vp_S);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.224f, 0.224f, 0.224f, 1.000f));
    ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);

    /// content-viewport
    {
        utils::TextCentered("Make sure to lock widgets before interacting with them.", 1);
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 70);
        ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
        ImGui::SetCursorPos(ImVec2(0, 5));
        if (bw.current_child)
        {
            ImGui::Text("cur child name = %d", bw.current_child->id);
            ImGui::Text("child.open = %d", bw.current_child->child.open);
        }
        ImGui::Text("objects.size: %d", static_cast<int>(bw.objects.size()));
        ImGui::Text("itemcur: %d", selectproparray);
        if (!bw.objects.empty())
        {
            ImGui::Text("Selected = %s", selectobj->identifier.c_str());
            ImGui::Text("ischild = %d", selectobj->ischild);
        }
        bw.drawall(&selectid, gen_rand);
        // ImGui::Text("%d", bw.win.size());

        // utils::metrics();
    }

    ImGui::End();
    ImGui::PopStyleColor(1);
}

// ANCHOR OUTPUTWKSP.DEFINITION
void ImStudio::GUI::ShowOutputWorkspace()
{
    ImGui::SetNextWindowPos(ot_P);
    ImGui::SetNextWindowSizeConstraints(ImVec2(0, -1), ImVec2(FLT_MAX, -1));
    ImGui::SetNextWindowSize(ot_S);
    ImGui::Begin("wksp_output", NULL, ImGuiWindowFlags_NoTitleBar);
    {
        ImStudio::GenerateCode(&bw);
    }
    ImGui::End();
}

void ImStudio::GUI::ShowConsole(bool *p_open, GUI *gui_)
{
    static Console console(gui_);
    console.Draw("Console", p_open);
}