#include "../includes.h"
#include "buffer.h"
#include "object.h"
#include "gui.h"

void ImStudio::Menubar(ImVec2 &mb_P, ImVec2 &mb_S, bool &child_debug, bool &child_sty, bool &child_demo,
                       bool &child_metrics, bool &child_stack, bool &child_colexp, bool &main_state, bool &ly_save,
                       bool &wksp_logic, bool &wksp_interface)
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
                main_state = false;
            };
            ImGui::EndMenu();
        }

        /// menu-edit
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Save Layout"))
            {
                ly_save = true;
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
        // tab-interface
        if (ImGui::BeginTabItem("Interface"))
        {
            wksp_logic     = false;
            wksp_interface = true;
            ImGui::EndTabItem();
        }

        // tab-logic
        if (ImGui::BeginTabItem("Logic"))
        {
            wksp_interface = false;
            wksp_logic     = true;
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

void ImStudio::Sidebar(ImVec2 &sb_P, ImVec2 &sb_S, ImVec2 &sb_Sr, bool &ly_save, bool &resizing, int &w_w, int &w_h,
                       BufferWindow &bf, bool &main_state)
{
    ImGui::SetNextWindowPos(sb_P);
    ImGui::SetNextWindowSizeConstraints(ImVec2(0, -1), ImVec2(FLT_MAX, -1));
    ImGui::SetNextWindowSize(sb_S);
    ImGui::Begin("Sidebar", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
    sb_S = ImGui::GetWindowSize();
    if (ly_save)
    {
        sb_Sr = extra::GetWindowSRatio();
    }
    if (resizing)
    {
        sb_S = ImVec2(w_w / sb_Sr.x, w_h / sb_Sr.y);
    }

    /// content-sidebar
    {

        if (ImGui::Button("Window"))
        {
            bf.state = true;
        }
        if (ImGui::Button("Checkbox"))
        {
            bf.create("checkbox");
        }
        if (ImGui::Button("Button"))
        {
            std::cout << "creating button" << std::endl;
            bf.create("button");
        }
        if (ImGui::Button("Radio Button"))
        {
            bf.create("radio");
        }
        if (ImGui::Button("Combo"))
        {
            bf.create("combo");
        }
        if (ImGui::Button("Child"))
        {
            bf.create("child");
        }
        ImGui::SameLine();
        extra::HelpMarker("This is not an actual child window (ImGui::BeginChild) as "
                          "it's behavior is not desired here. However, ImStudio will "
                          "try its best to recreate the layout in the output. More "
                          "info at Github issue: ocornut/imgui #1496");
        if (ImGui::Button("Text"))
        {
            bf.create("text");
        }

        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
        {
            main_state = false;
        }
        
    }

    ImGui::End();
}

void ImStudio::Properties(ImVec2 &pt_P,ImVec2 &pt_Pr,ImVec2 &pt_S,ImVec2 &pt_Sr,ImVec2 &mb_S,int &w_w,int &w_h,bool &ly_save,bool &resizing,BufferWindow &bf,int &select,int &item_current,Object *selectobj,Object *selectobjprev)
{
    ImGui::SetNextWindowPos(pt_P);
                    ImGui::SetNextWindowSize(pt_S);
                    ImGui::Begin("Properties", NULL, ImGuiWindowFlags_NoMove);
                    pt_P = ImGui::GetWindowPos();
                    pt_S = ImGui::GetWindowSize();
                    if (ly_save)
                    {
                        pt_Sr   = extra::GetWindowSRatio();
                        pt_Pr   = extra::GetWindowPRatio();
                        ly_save = false;
                    }
                    if (resizing)
                    {
                        pt_P     = ImVec2(w_w / pt_Pr.x, mb_S.y);
                        pt_S     = ImVec2(w_w / pt_Sr.x, w_h / pt_Sr.y);
                        resizing = false;
                    }

                    /// content-properties
                    {
                        {
                            if (!bf.objects.empty())
                            {
                                const char *items[bf.objects.size()];
                                int         idarr[bf.objects.size()];
                                int         i = 0;
                                for (auto it = bf.objects.begin(); it != bf.objects.end(); ++it)
                                {
                                    Object &o = *it;
                                    items[i]  = o.identifier.c_str();
                                    idarr[i]  = o.id;
                                    if (o.id == select)
                                    {
                                        if (ImGui::IsMouseDown(0))
                                        {
                                            item_current = i;
                                        }

                                        std::cout << i << std::endl;
                                        std::cout << "prop-pre check" << std::endl;
                                    }
                                    else
                                    {
                                        std::cout << "prop-pre check FAIL" << std::endl;
                                    }
                                    i++;
                                }

                                ImGui::Combo("combo", &item_current, items, IM_ARRAYSIZE(items));

                                if (ImGui::IsMouseDown(0))
                                { // viewport select
                                    selectobj = bf.getobj(select);
                                }
                                else
                                { // combo select
                                    selectobj = bf.getobj(idarr[item_current]);
                                    select    = selectobj->id;
                                }

                                if (!selectobjprev)
                                {
                                    selectobjprev = selectobj;
                                }

                                if (selectobj->id != selectobjprev->id)
                                {
                                    bf.resetpropbuffer();
                                }

                                if (selectobj->type == "button")
                                {
                                    if (selectobj->propinit)
                                    {
                                        bf.prop_text1 = selectobj->value_s;
                                    }

                                    ImGui::InputText("Value", &bf.prop_text1);
                                    selectobj->value_s = bf.prop_text1;

                                    if ((ImGui::Button("Delete")) ||
                                        (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                                    {
                                        selectobj->del();
                                        if (item_current != 0)
                                        {
                                            item_current -= 1;
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

                                    if ((ImGui::Button("Delete")) ||
                                        (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                                    {
                                        selectobj->del();
                                        if (item_current != 0)
                                        {
                                            item_current -= 1;
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
                                    if ((ImGui::Button("Delete")) ||
                                        (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                                    {
                                        selectobj->del();
                                        if (item_current != 0)
                                        {
                                            item_current -= 1;
                                        }
                                    }
                                }
                                if (selectobj->type == "text")
                                {
                                    if (selectobj->propinit)
                                    {
                                        bf.prop_text1 = selectobj->value_s;
                                    }

                                    ImGui::InputText("Value", &bf.prop_text1);
                                    selectobj->value_s = bf.prop_text1;

                                    if ((ImGui::Button("Delete")) ||
                                        (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))))
                                    {
                                        selectobj->del();
                                        if (item_current != 0)
                                        {
                                            item_current -= 1;
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