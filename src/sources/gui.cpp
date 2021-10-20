#include "../includes.h"
#include "buffer.h"
#include "object.h"
#include "gui.h"

//ANCHOR MENUBAR.DEFINITION
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
            if (ImGui::MenuItem("Exit"))
            {
                state = false;
            };
            ImGui::EndMenu();
        }

        /// menu-edit
        if (ImGui::BeginMenu("Edit"))
        {
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

//ANCHOR SIDEBAR.DEFINITION
void GUI::ShowSidebar()
{
    ImGui::SetNextWindowPos(sb_P);
    ImGui::SetNextWindowSizeConstraints(ImVec2(0, -1), ImVec2(FLT_MAX, -1));
    ImGui::SetNextWindowSize(sb_S);
    ImGui::Begin("Sidebar", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);

    /// content-sidebar
    {
        {
            if (ImGui::Button("Window"))
            {
                bw.state = true;
            }
            if (ImGui::Button("Checkbox"))
            {
                bw.create("checkbox");
            }
            if (ImGui::Button("Button"))
            {
                bw.create("button");
            }
            if (ImGui::Button("Radio Button"))
            {
                bw.create("radio");
            }
            if (ImGui::Button("Combo"))
            {
                bw.create("combo");
            }
            if (ImGui::Button("Child"))
            {
                bw.create("child");
            }
            ImGui::SameLine();
            extra::HelpMarker("This is not an actual child window (ImGui::BeginChild) as "
                              "it's behavior is not desired here. However, ImStudio will "
                              "try its best to recreate the layout in the output. More "
                              "info at Github issue: ocornut/imgui #1496");
            if (ImGui::Button("Text"))
            {
                bw.create("text");
            }

            if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
            {
                state = false;
            }
        }
    }

    ImGui::End();
}

//ANCHOR PROPERTIES.DEFINITION
void GUI::ShowProperties()
{
    ImGui::SetNextWindowPos(pt_P);
    ImGui::SetNextWindowSize(pt_S);
    ImGui::Begin("Properties", NULL, ImGuiWindowFlags_NoMove);
    // pt_P = ImGui::GetWindowPos();
    // pt_S = ImGui::GetWindowSize();
    /// content-properties
    {
        {
            if (!bw.objects.empty())
            {
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

                    if ((ImGui::Button("Delete")) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                    {
                        selectobj->del();
                        if (selectproparray != 0)
                        {
                            selectproparray -= 1;
                        }
                    }
                }

                if (selectobj->type == "radio")
                {
                }
                if (selectobj->type == "combo")
                {
                }
                if (selectobj->type == "child")
                {
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
                selectobj->propinit = true;
                selectobjprev       = selectobj;
            }
        }
    }

    ImGui::End();
}

//ANCHOR VIEWPORT.DEFINITION
void GUI::ShowViewport(int gen_rand)
{
    ImGui::SetNextWindowPos(vp_P);
    ImGui::SetNextWindowSize(vp_S);
    ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);

    /// content-viewport
    {
        ImGui::Text("objects.size: %d", bw.objects.size());
        ImGui::Text("itemcur: %d", selectproparray);
        if (!bw.objects.empty())
        {
            ImGui::Text("Selected = %s", selectobj->identifier.c_str());
        }
        bw.drawall(&selectid, gen_rand);
        // ImGui::Text("%d", bw.win.size());

        extra::metrics();
    }

    ImGui::End();
}

//ANCHOR OUTPUTWKSP.DEFINITION
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
                                          ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 64),
                                          ImGuiInputTextFlags_ReadOnly);
            }
            ImGui::End();
}