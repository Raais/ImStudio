#include "../includes.h"
#include "object.h"
#include "buffer.h"
#include "generator.h"

void ImStudio::Recreate(BaseObject obj, std::string* output, bool staticlayout)
{
    std::string bfs;

    if (obj.type == "button")
    {
        if (!staticlayout) {
        bfs += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",obj.pos.x,obj.pos.y);
        }
        bfs += fmt::format("\tImGui::Button(\"{}\", ImVec2({},{})); ",obj.value_s, obj.size.x, obj.size.y);
        bfs += "//remove size argument (ImVec2) to auto-resize\n\n";
        
    }

    if (obj.type == "radio")
    {
        if (!staticlayout) {
        bfs += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",obj.pos.x,obj.pos.y);
        }
        bfs += fmt::format("\tstatic bool r1{} = false;\n",obj.id);
        bfs += fmt::format("\tImGui::RadioButton(\"{}\", r1{});\n\n",obj.label, obj.id);
    }

    if (obj.type == "checkbox")
    {
        if (!staticlayout) {
        bfs += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",obj.pos.x,obj.pos.y);
        }
        bfs += fmt::format("\tstatic bool c1{} = false;\n",obj.id);
        bfs += fmt::format("\tImGui::Checkbox(\"{}\", &c1{});\n\n",obj.label, obj.id);
    }

    if (obj.type == "text")
    {
        if (!staticlayout) {
        bfs += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",obj.pos.x,obj.pos.y);
        }
        bfs += fmt::format("\tImGui::Text(\"{}\");\n\n",obj.value_s);
    }

    if (obj.type == "bullet")
    {
        if (!staticlayout) {
        bfs += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",obj.pos.x,obj.pos.y);
        }
        bfs += "\tImGui::Bullet();\n\n";
        
    }

    if (obj.type == "arrow")
    {
        if (!staticlayout) {
        bfs += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",obj.pos.x,obj.pos.y);
        }
        bfs += "\tImGui::ArrowButton(\"##left\", ImGuiDir_Left);\n";
        bfs += "\tImGui::SameLine();\n";
        bfs += "\tImGui::ArrowButton(\"##right\", ImGuiDir_Right);\n\n";
    }

    if (obj.type == "combo")
    {
        if (!staticlayout) {
        bfs += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",obj.pos.x,obj.pos.y);
        }
        bfs += fmt::format("\tImGui::PushItemWidth({});\n",obj.width);
        bfs += fmt::format("\tstatic int item_current{} = 0;\n",obj.id);
        bfs += fmt::format("\tconst char *items{}[] = {{\"Never\", \"Gonna\", \"Give\", \"You\", \"Up\"}};\n",obj.id);
        bfs += fmt::format("\tImGui::Combo(\"{0}\", &item_current{1}, items{1}, IM_ARRAYSIZE(items{1}));\n",obj.label,obj.id);
        bfs += "\tImGui::PopItemWidth();\n\n";
    }

    if (obj.type == "listbox")
    {
        if (!staticlayout) {
        bfs += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",obj.pos.x,obj.pos.y);
        }
        bfs += fmt::format("\tImGui::PushItemWidth({});\n",obj.width);
        bfs += fmt::format("\tstatic int item_current{} = 0;\n",obj.id);
        bfs += fmt::format("\tconst char *items{}[] = {{\"Never\", \"Gonna\", \"Give\", \"You\", \"Up\"}};\n",obj.id);
        bfs += fmt::format("\tImGui::ListBox(\"{0}\", &item_current{1}, items{1}, IM_ARRAYSIZE(items{1}));\n",obj.label,obj.id);
        bfs += "\tImGui::PopItemWidth();\n\n";
    }

    if (obj.type == "textinput")
    {
        if (!staticlayout) {
        bfs += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",obj.pos.x,obj.pos.y);
        }
        bfs += fmt::format("\tImGui::PushItemWidth({}); ",obj.width);
        bfs += "//NOTE: (Push/Pop)ItemWidth is optional\n";
        //static char str0[128] = "Hello, world!";
        //ImGui::InputText("input text", str0, IM_ARRAYSIZE(str0));
        bfs += fmt::format("\tstatic char str{}[128] = \"{}\";\n",obj.id,obj.value_s);
        bfs += fmt::format("\tImGui::InputText(\"{0}\", str{1}, IM_ARRAYSIZE(str{1}));\n",obj.label,obj.id);
        bfs += "\tImGui::PopItemWidth();\n\n";
    }

    if (obj.type == "inputint")
    {
        if (!staticlayout) {
        bfs += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",obj.pos.x,obj.pos.y);
        }
        bfs += fmt::format("\tImGui::PushItemWidth({});\n",obj.width);
        //static int i0 = 123;
        //ImGui::InputInt("input int", &i0);
        bfs += fmt::format("\tstatic int i{} = 123;\n",obj.id);
        bfs += fmt::format("\tImGui::InputInt(\"{}\", &i{});\n",obj.label,obj.id);
        bfs += "\tImGui::PopItemWidth();\n\n";
    }

    if (obj.type == "inputfloat")
    {
        if (!staticlayout) {
        bfs += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",obj.pos.x,obj.pos.y);
        }
        bfs += fmt::format("\tImGui::PushItemWidth({});\n",obj.width);
        //static float f0 = 0.001f;
        //ImGui::InputFloat("input float", &f0, 0.01f, 1.0f, "%.3f");
        bfs += fmt::format("\tstatic float f{} = 0.001f;\n",obj.id);
        bfs += fmt::format("\tImGui::InputFloat(\"{}\", &f{}, 0.01f, 1.0f, \"%.3f\");\n",obj.label,obj.id);
        bfs += "\tImGui::PopItemWidth();\n\n";
    }

    if (obj.type == "inputdouble")
    {
        if (!staticlayout) {
        bfs += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",obj.pos.x,obj.pos.y);
        }
        bfs += fmt::format("\tImGui::PushItemWidth({});\n",obj.width);
        //static double d0 = 999999.00000001;
        //ImGui::InputDouble("input double", &d0, 0.01f, 1.0f, "%.8f");
        bfs += fmt::format("\tstatic double d{} = 999999.00000001;\n",obj.id);
        bfs += fmt::format("\tImGui::InputDouble(\"{}\", &d{}, 0.01f, 1.0f, \"%.8f\");\n",obj.label,obj.id);
        bfs += "\tImGui::PopItemWidth();\n\n";
    }

    if (obj.type == "inputscientific")
    {
        if (!staticlayout) {
        bfs += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",obj.pos.x,obj.pos.y);
        }
        bfs += fmt::format("\tImGui::PushItemWidth({});\n",obj.width);
        //static float f1 = 1.e10f;
        //ImGui::InputFloat("input scientific", &f1, 0.0f, 0.0f, "%e");
        bfs += fmt::format("\tstatic float f{} = 1.e10f;\n",obj.id);
        bfs += fmt::format("\tImGui::InputFloat(\"{}\", &f{}, 0.0f, 0.0f, \"%e\");\n",obj.label,obj.id);
        bfs += "\tImGui::PopItemWidth();\n\n";
    }

    if (obj.type == "inputfloat3")
    {
        if (!staticlayout) {
        bfs += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",obj.pos.x,obj.pos.y);
        }
        bfs += fmt::format("\tImGui::PushItemWidth({});\n",obj.width);
        //static float vec4a[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
        //ImGui::InputFloat3("input float3", vec4a);
        bfs += fmt::format("\tstatic float vec4a{}[4] = {{ 0.10f, 0.20f, 0.30f, 0.44f }};\n",obj.id);
        bfs += fmt::format("\tImGui::InputFloat3(\"{}\", vec4a{});\n",obj.label,obj.id);
        bfs += "\tImGui::PopItemWidth();\n\n";
    }

    if (obj.type == "dragint")
    {
        if (!staticlayout) {
        bfs += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",obj.pos.x,obj.pos.y);
        }
        bfs += fmt::format("\tImGui::PushItemWidth({});\n",obj.width);
        //static int i1 = 50;
        //ImGui::DragInt("drag int", &i1, 1);
        bfs += fmt::format("\tstatic int i1{0} = 50;\n",obj.id);
        bfs += fmt::format("\tImGui::DragInt(\"{}\", &i1{}, 1);\n",obj.label,obj.id);
        bfs += "\tImGui::PopItemWidth();\n\n";
    }

    if (obj.type == "dragint100")
    {
        if (!staticlayout) {
        bfs += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",obj.pos.x,obj.pos.y);
        }
        bfs += fmt::format("\tImGui::PushItemWidth({});\n",obj.width);
        //static int i2 = 42;
        //ImGui::DragInt("drag int 0..100", &i2, 1, 0, 100, "%d%%", ImGuiSliderFlags_AlwaysClamp);
        bfs += fmt::format("\tstatic int i2{0} = 42;\n",obj.id);
        bfs += fmt::format("\tImGui::DragInt(\"{}\", &i2{}, 1, 0, 100, \"%d%%\", ImGuiSliderFlags_AlwaysClamp);\n",obj.label,obj.id);
        bfs += "\tImGui::PopItemWidth();\n\n";
    }

    if (obj.type == "dragfloat")
    {
        if (!staticlayout) {
        bfs += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",obj.pos.x,obj.pos.y);
        }
        bfs += fmt::format("\tImGui::PushItemWidth({});\n",obj.width);
        //static float f1 = 1.00f;
        //ImGui::DragFloat("drag float", &f1, 0.005f);
        bfs += fmt::format("\tstatic float f1{0} = 1.00f;\n",obj.id);
        bfs += fmt::format("\tImGui::DragFloat(\"{}\", &f1{}, 0.005f);\n",obj.label,obj.id);
        bfs += "\tImGui::PopItemWidth();\n\n";
    }

    if (obj.type == "dragfloatsmall")
    {
        if (!staticlayout) {
        bfs += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",obj.pos.x,obj.pos.y);
        }
        bfs += fmt::format("\tImGui::PushItemWidth({});\n",obj.width);
        //static float f2 = 0.0067f;
        //ImGui::DragFloat("drag small float", &f2, 0.0001f, 0.0f, 0.0f, "%.06f ns");
        bfs += fmt::format("\tstatic float f2{0} = 0.0067f;\n",obj.id);
        bfs += fmt::format("\tImGui::DragFloat(\"{}\", &f2{}, 0.0001f, 0.0f, 0.0f, \"%.06f ns\");\n",obj.label,obj.id);
        bfs += "\tImGui::PopItemWidth();\n\n";
    }

    if (obj.type == "sliderint")
    {
        if (!staticlayout) {
        bfs += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",obj.pos.x,obj.pos.y);
        }
        bfs += fmt::format("\tImGui::PushItemWidth({});\n",obj.width);
        //static int i1 = 0;
        //ImGui::SliderInt("slider int", &i1, -1, 3);
        bfs += fmt::format("\tstatic int i1{0} = 0;\n",obj.id);
        bfs += fmt::format("\tImGui::SliderInt(\"{}\", &i1{}, -1, 3);\n",obj.label,obj.id);
        bfs += "\tImGui::PopItemWidth();\n\n";
    }

    if (obj.type == "sliderfloat")
    {
        if (!staticlayout) {
        bfs += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",obj.pos.x,obj.pos.y);
        }
        bfs += fmt::format("\tImGui::PushItemWidth({});\n",obj.width);
        //static float f1 = 0.123f;
        //ImGui::SliderFloat("slider float", &f1, 0.0f, 1.0f, "ratio = %.3f");
        bfs += fmt::format("\tstatic float f1{0} = 0.123f;\n",obj.id);
        bfs += fmt::format("\tImGui::SliderFloat(\"{}\", &f1{}, 0.0f, 1.0f, \"ratio = %.3f\");\n",obj.label,obj.id);
        bfs += "\tImGui::PopItemWidth();\n\n";
    }

    if (obj.type == "sliderfloatlog")
    {
        if (!staticlayout) {
        bfs += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",obj.pos.x,obj.pos.y);
        }
        bfs += fmt::format("\tImGui::PushItemWidth({});\n",obj.width);
        //static float f2 = 0.0f;
        //ImGui::SliderFloat("slider float (log)", &f2, -10.0f, 10.0f, "%.4f", ImGuiSliderFlags_Logarithmic);
        bfs += fmt::format("\tstatic float f2{0} = 0.0f;\n",obj.id);
        bfs += fmt::format("\tImGui::SliderFloat(\"{}\", &f2{}, -10.0f, 10.0f, \"%.4f\", ImGuiSliderFlags_Logarithmic);\n",obj.label,obj.id);
        bfs += "\tImGui::PopItemWidth();\n\n";
    }

    if (obj.type == "sliderangle")
    {
        if (!staticlayout) {
        bfs += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",obj.pos.x,obj.pos.y);
        }
        bfs += fmt::format("\tImGui::PushItemWidth({});\n",obj.width);
        //static float angle = 0.0f;
        //ImGui::SliderAngle("slider angle", &angle);
        bfs += fmt::format("\tstatic float angle{0} = 0.0f;\n",obj.id);
        bfs += fmt::format("\tImGui::SliderAngle(\"{}\", &angle{});\n",obj.label,obj.id);
        bfs += "\tImGui::PopItemWidth();\n\n";
    }

    if (obj.type == "color1")
    {
        if (!staticlayout) {
        bfs += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",obj.pos.x,obj.pos.y);
        }
        //static float col1[3] = {1.0f, 0.0f, 0.2f};
        //ImGui::ColorEdit3(label.c_str(), col1, ImGuiColorEditFlags_NoInputs);
        bfs += fmt::format("\tstatic float col1{0}[3] = {{1.0f, 0.0f, 0.2f}};\n",obj.id);
        bfs += fmt::format("\tImGui::ColorEdit3(\"{}\", col1{}, ImGuiColorEditFlags_NoInputs);\n\n",obj.label,obj.id);
    }

    if (obj.type == "color2")
    {
        if (!staticlayout) {
        bfs += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",obj.pos.x,obj.pos.y);
        }
        bfs += fmt::format("\tImGui::PushItemWidth({});\n",obj.width);
        //static float col2[3] = {1.0f, 0.0f, 0.2f};
        //ImGui::ColorEdit3(label.c_str(), col2);
        bfs += fmt::format("\tstatic float col2{0}[3] = {{1.0f, 0.0f, 0.2f}};\n",obj.id);
        bfs += fmt::format("\tImGui::ColorEdit3(\"{}\", col2{});\n\n",obj.label,obj.id);
        bfs += "\tImGui::PopItemWidth();\n\n";
    }

    if (obj.type == "color3")
    {
        if (!staticlayout) {
        bfs += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",obj.pos.x,obj.pos.y);
        }
        bfs += fmt::format("\tImGui::PushItemWidth({});\n",obj.width);
        //static float col3[4] = {0.4f, 0.7f, 0.0f, 0.5f};
        //ImGui::ColorEdit4(label.c_str(), col3);
        bfs += fmt::format("\tstatic float col3{0}[4] = {{0.4f, 0.7f, 0.0f, 0.5f}};\n",obj.id);
        bfs += fmt::format("\tImGui::ColorEdit4(\"{}\", col3{});\n\n",obj.label,obj.id);
        bfs += "\tImGui::PopItemWidth();\n\n";
    }

    if (obj.type == "sameline")
    {
        if (staticlayout) {
        bfs += "\tImGui::SameLine();\n\n";
        }
    }

    if (obj.type == "newline")
    {
        if (staticlayout) {
        bfs += "\tImGui::NewLine();\n\n";
        }
    }

    if (obj.type == "separator")
    {
        if (staticlayout) {
        bfs += "\tImGui::Separator();\n\n";
        }
    }

    if (obj.type == "progressbar")
    {
        if (!staticlayout) {
        bfs += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",obj.pos.x,obj.pos.y);
        }
        bfs += fmt::format("\tImGui::PushItemWidth({});\n",obj.width);
        //static float progress = 0.0f;
        //ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f));
        bfs += fmt::format("\tstatic float progress{} = 0.0f;\n",obj.id);
        bfs += fmt::format("\tImGui::ProgressBar(progress{}, ImVec2(0.0f, 0.0f));\n",obj.id);
        bfs += "\tImGui::PopItemWidth();\n\n";
    }
    
    output->append(bfs);
    
}

void ImStudio::GenerateCode(std::string* output, BufferWindow* bw)
{
    *output  = "/*\nGENERATED CODE | READ-ONLY\nYou can directly copy from here, or from File > Export to clipboard\n*/\n\n";
    *output += "static bool window = true;\n";
    *output += fmt::format("ImGui::SetNextWindowSize(ImVec2({},{}));\n", bw->size.x, bw->size.y);
    *output += "//!! You might want to use these ^^ values in the OS window instead, and add the ImGuiWindowFlags_NoTitleBar flag in the ImGui window !!\n\n";
    *output += "if (ImGui::Begin(\"window_name\", &window))\n{\n\n";
    for (auto i = bw->objects.begin(); i != bw->objects.end(); ++i)
    {
        Object &o = *i;

        if (o.type != "child")
        {
            Recreate(o, output, bw->staticlayout);
        }
        else
        {
            if (!bw->staticlayout) {
            *output += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",o.child.freerect.Min.x,o.child.freerect.Min.y);
            }
            *output += fmt::format("\tImGui::BeginChild({}, ImVec2({},{}), {});\n\n", o.child.id, o.child.freerect.GetSize().x, o.child.freerect.GetSize().y, o.child.border);
            for (auto i = o.child.objects.begin(); i != o.child.objects.end(); ++i)
            {
                BaseObject &cw = *i;// child widget

                Recreate(cw, output, bw->staticlayout);

            }
            *output += "\tImGui::EndChild();\n\n";
        }
    }
    *output += "\n\tImGui::End();\n}\n";
    ImGui::InputTextMultiline("##source", output,
                              ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 64), ImGuiInputTextFlags_ReadOnly);

}