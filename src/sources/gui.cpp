#include "../includes.h"
#include "object.h"
#include "buffer.h"
#include "console.h"
#include "gui.h"

// ANCHOR MENUBAR.DEFINITION
void GUI::ShowMenubar()
{
    ImGui::SetNextWindowPos(mb_P);
    ImGui::SetNextWindowSize(mb_S);
    ImGui::Begin("Menubar", NULL,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    // MENU
    if (ImGui::BeginMenuBar())
    {
        /// menu-debug
        if (ImGui::BeginMenu("Debug"))
        {
            ImGui::MenuItem("Settings", NULL, &child_debug);
            ImGui::MenuItem("Style Editor", NULL, &child_sty);
            ImGui::MenuItem("Demo Window", NULL, &child_demo);
            ImGui::MenuItem("Metrics", NULL, &child_metrics);
            ImGui::MenuItem("Stack Tool", NULL, &child_stack);
            ImGui::MenuItem("Console", NULL, &child_console);
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
                extra::HelpMarker("Toggle between static/linear layout and fixed/manual layout");

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        /// menu-tools
        if (ImGui::BeginMenu("Tools"))
        {
            ImGui::MenuItem("Color Export", NULL, &child_colexp);
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
}

// ANCHOR SIDEBAR.DEFINITION
void GUI::ShowSidebar()
{
    ImGui::SetNextWindowPos(sb_P);
    ImGui::SetNextWindowSizeConstraints(ImVec2(0, -1), ImVec2(FLT_MAX, -1));
    ImGui::SetNextWindowSize(sb_S);
    ImGui::Begin("Sidebar", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    /// content-sidebar
    {
        {
            ImGui::TextDisabled("NOTE");
            ImGui::SameLine();
            extra::HelpMarker("THESE ARE NOT THE ONLY WIDGETS IMGUI HAS!\nYou can find out more in the Dear ImGui "
                              "Demo (Tools > Demo Window) and imgui/imgui_demo.cpp");
            ImGui::Separator();
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
            ImGui::Text("Plain Text");
            ImGui::Separator();

            if (ImGui::Button("Text"))
            {
                bw.create("text");
            }
            if (ImGui::Button("Input Text"))
            {
                bw.create("textinput");
            }

            ImGui::Separator();
            ImGui::Text("Data Inputs");
            ImGui::Separator();

            if (ImGui::Button("Input Int"))
            {
                bw.create("inputint");
            }
            ImGui::SameLine();
            extra::HelpMarker("You can apply arithmetic operators +,*,/ on numerical values.\n"
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
            ImGui::SameLine();
            extra::HelpMarker("You can input value using the scientific notation,\n"
                              "  e.g. \"1e+8\" becomes \"100000000\".");
            if (ImGui::Button("Input Float3"))
            {
                bw.create("inputfloat3");
            }
            if (ImGui::Button("Drag Int"))
            {
                bw.create("dragint");
            }
            ImGui::SameLine();
            extra::HelpMarker("Click and drag to edit value.\n"
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
            ImGui::SameLine();
            extra::HelpMarker("CTRL+click to input value.");
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
            ImGui::Text("Others");
            ImGui::Separator();

            if (ImGui::Button("Child"))
            {
                bw.create("child");
            }
            ImGui::SameLine();
            extra::HelpMarker("This is not an actual child window (ImGui::BeginChild) as "
                              "its behavior is not desired here.");
            if (ImGui::Button("EndChild"))
            {
                bw.addingtochild = false;
            }
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
    }

    ImGui::End();
}

// ANCHOR PROPERTIES.DEFINITION
void GUI::ShowProperties()
{
    ImGui::SetNextWindowPos(pt_P);
    ImGui::SetNextWindowSize(pt_S);
    ImGui::Begin("Properties", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    // pt_P = ImGui::GetWindowPos();
    // pt_S = ImGui::GetWindowSize();
    /// content-properties
    {
        {
            if (!bw.objects.empty())
            {
                int allvecsize = 0;

                const char *items[bw.objects.size()];
                int         idarr[bw.objects.size()];
                int         i = 0;
                for (auto it = bw.objects.begin(); it != bw.objects.end(); ++it)
                {
                    Object &o = *it;
                    items[i]  = o.identifier.c_str();
                    idarr[i]  = o.id;
                    if (o.id == selectid)
                    {
                        if (ImGui::IsMouseDown(0))
                        {
                            selectproparray = i;
                        }
                    }
                    i++;
                }

                ImGui::Combo("combo", &selectproparray, items, IM_ARRAYSIZE(items));

                if (ImGui::IsMouseDown(0))
                { // viewport select
                    selectobj = bw.getobj(selectid);
                }
                else
                { // combo select
                    selectobj = bw.getobj(idarr[selectproparray]);
                    selectid  = selectobj->id;
                }

                if (!selectobjprev)
                {
                    selectobjprev = selectobj;
                }

                if (selectobj->id != selectobjprev->id)
                {
                    bw.resetpropbuffer();
                }

                if (selectobj->type == "button")
                {
                    if (selectobj->propinit)
                    {
                        bw.prop_text1 = selectobj->value_s;
                    }

                    ImGui::InputText("Value", &bw.prop_text1);
                    ImGui::NewLine();
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Locked", &selectobj->locked);
                    selectobj->value_s = bw.prop_text1;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0)
                        {
                            selectproparray -= 1;
                        }
                    }
                }

                if (selectobj->type == "checkbox")
                {
                    const char *items[] = {"False", "True"};
                    static int  cur     = 0;
                    if (selectobj->propinit)
                    {
                        cur = selectobj->value_b;
                    }

                    ImGui::Combo("Value", &cur, items, IM_ARRAYSIZE(items));
                    if (cur == 0)
                    {
                        selectobj->value_b = false;
                    }
                    else
                    {
                        selectobj->value_b = true;
                    }
                    ImGui::NewLine();
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Locked", &selectobj->locked);

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))) ||
                        (globaldelete))
                    {
                        selectobj->del();
                        if (selectproparray != 0)
                        {
                            selectproparray -= 1;
                        }
                        globaldelete = false;
                    }
                }

                if (selectobj->type == "radio")
                {
                }
                if (selectobj->type == "combo")
                {
                    if (selectobj->propinit)
                    {
                        bw.prop_text1 = selectobj->value_s;
                    }

                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selectobj->width, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Locked", &selectobj->locked);
                    selectobj->value_s = bw.prop_text1;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0)
                        {
                            selectproparray -= 1;
                        }
                    }
                }
                if (selectobj->type == "child")
                {

                    /*if (selectobj->child_.objects.empty()))
                    {
                        ImGui::Text("child.objects.size() = %d",static_cast<Object*>(selectobj->parent)->child_.objects.size());
                    }*/

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0)
                        {
                            selectproparray -= 1;
                        }
                    }
                }
                if (selectobj->type == "text")
                {
                    if (selectobj->propinit)
                    {
                        bw.prop_text1 = selectobj->value_s;
                    }

                    ImGui::InputText("Value", &bw.prop_text1);
                    ImGui::NewLine();
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Locked", &selectobj->locked);
                    selectobj->value_s = bw.prop_text1;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0)
                        {
                            selectproparray -= 1;
                        }
                    }
                }
                if (selectobj->type == "textinput")
                {
                    if (selectobj->propinit)
                    {
                        bw.prop_text1 = selectobj->value_s;
                        bw.prop_text2 = selectobj->label;
                    }

                    ImGui::InputText("Label", &bw.prop_text2);
                    ImGui::InputText("Value", &bw.prop_text1);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selectobj->width, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Locked", &selectobj->locked);
                    selectobj->label   = bw.prop_text2;
                    selectobj->value_s = bw.prop_text1;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0)
                        {
                            selectproparray -= 1;
                        }
                    }
                }
                if (selectobj->type == "inputint")
                {
                    if (selectobj->propinit)
                    {
                        bw.prop_text2 = selectobj->label;
                    }

                    ImGui::InputText("Label", &bw.prop_text2);
                    ImGui::NewLine();
                    ImGui::InputFloat("Width", &selectobj->width, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position X", &selectobj->pos.x, 1.0f, 10.0f, "%.3f");
                    ImGui::InputFloat("Position Y", &selectobj->pos.y, 1.0f, 10.0f, "%.3f");
                    ImGui::Checkbox("Locked", &selectobj->locked);
                    selectobj->label = bw.prop_text2;

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0)
                        {
                            selectproparray -= 1;
                        }
                    }
                }
                selectobj->propinit = true;
                selectobjprev       = selectobj;
            }
        }
    }

    ImGui::End();
}

// ANCHOR VIEWPORT.DEFINITION
void GUI::ShowViewport(int gen_rand)
{
    ImGui::SetNextWindowPos(vp_P);
    ImGui::SetNextWindowSize(vp_S);
    ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);

    /// content-viewport
    {

        ImGui::Text("addingtochild = %d", bw.addingtochild);
        ImGui::TextDisabled("Make sure to lock widgets before interacting with them.");
        ImGui::Text("objects.size: %d", static_cast<int>(bw.objects.size()));
        ImGui::Text("itemcur: %d", selectproparray);
        if (!bw.objects.empty())
        {
            ImGui::Text("Selected = %s", selectobj->identifier.c_str());
            ImGui::Text("ischild = %d", selectobj->isChild);
        }
        bw.drawall(&selectid, gen_rand);
        // ImGui::Text("%d", bw.win.size());

        extra::metrics();
    }

    ImGui::End();
}

// ANCHOR OUTPUTWKSP.DEFINITION
void GUI::ShowOutputWorkspace()
{
    ImGui::SetNextWindowPos(ot_P);
    ImGui::SetNextWindowSizeConstraints(ImVec2(0, -1), ImVec2(FLT_MAX, -1));
    ImGui::SetNextWindowSize(ot_S);
    ImGui::Begin("wksp_output", NULL, ImGuiWindowFlags_NoTitleBar);
    {
        static char text[1024 * 16] = "/*\n"
                                      " GENERATED CODE\n"
                                      " READ-ONLY | IMSTUDIO IS NOT A COMPILER FOR C++!\n"
                                      "*/\n\n"
                                      "auto layout = You.DesignSomethingFunky();\n"
                                      "ImStudio.GenerateCode(layout);";

        ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text),
                                  ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 64), ImGuiInputTextFlags_ReadOnly);
    }
    ImGui::End();
}

void GUI::ShowConsole(bool *p_open, GUI *gui_)
{
    static Console console(gui_);
    console.Draw("Console", p_open);
}