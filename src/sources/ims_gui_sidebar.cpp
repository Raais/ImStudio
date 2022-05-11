#include "ims_gui.h"

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
            bw.create("button");

        if (ImGui::Button("Radio Button"))
            bw.create("radio");

        if (ImGui::Button("Checkbox"))
            bw.create("checkbox");

        if (ImGui::Button("Text"))
            bw.create("text");

        if (ImGui::Button("Bullet"))
            bw.create("bullet");

        if (ImGui::Button("Arrow"))
            bw.create("arrow");

        if (ImGui::Button("Combo"))
            bw.create("combo");

        if (ImGui::Button("Listbox"))
            bw.create("listbox");

        ImGui::Separator();

        //ANCHOR SIDEBAR.DATAINPUTS
        ImGui::Text("Data Inputs");
        ImGui::Separator();

        if (ImGui::Button("Input Text"))
            bw.create("textinput");

        if (ImGui::Button("Input Int"))
            bw.create("inputint");

        ImGui::SameLine(); utils::HelpMarker
        ("You can apply arithmetic operators +,*,/ on numerical values.\n"
        "  e.g. [ 100 ], input \'*2\', result becomes [ 200 ]\n"
        "Use +- to subtract.");

        if (ImGui::Button("Input Float"))
            bw.create("inputfloat");

        if (ImGui::Button("Input Double"))
            bw.create("inputdouble");

        if (ImGui::Button("Input Scientific"))
            bw.create("inputscientific");

        ImGui::SameLine(); utils::HelpMarker
        ("You can input value using the scientific notation,\n"
        "  e.g. \"1e+8\" becomes \"100000000\".");

        if (ImGui::Button("Input Float3"))
            bw.create("inputfloat3");

        if (ImGui::Button("Drag Int"))
            bw.create("dragint");

        ImGui::SameLine(); utils::HelpMarker
        ("Click and drag to edit value.\n"
        "Hold SHIFT/ALT for faster/slower edit.\n"
        "Double-click or CTRL+click to input value.");

        if (ImGui::Button("Drag Int %"))
            bw.create("dragint100");

        if (ImGui::Button("Drag Float"))
            bw.create("dragfloat");

        if (ImGui::Button("Drag Float Small"))
            bw.create("dragfloatsmall");

        if (ImGui::Button("Slider Int"))
            bw.create("sliderint");

        ImGui::SameLine(); utils::HelpMarker("CTRL+click to input value.");

        if (ImGui::Button("Slider Float"))
            bw.create("sliderfloat");

        if (ImGui::Button("Slider Float Log"))
            bw.create("sliderfloatlog");

        if (ImGui::Button("Slider Angle"))
            bw.create("sliderangle");

        ImGui::Separator();
        
        ImGui::Text("Color Pickers");
        ImGui::Separator();

        if (ImGui::Button("Color 1"))
            bw.create("color1");

        if (ImGui::Button("Color 2"))
            bw.create("color2");

        if (ImGui::Button("Color 3"))
            bw.create("color3");

        ImGui::Separator();

        //ANCHOR SIDEBAR.OTHERS
        ImGui::Text("Misc");
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