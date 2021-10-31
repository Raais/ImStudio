#include "../includes.h"
#include "object.h"
#include "buffer.h"
#include "generator.h"

void ImStudio::Recreate(BaseObject obj, std::string* str, bool staticlayout)
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
        bfs += fmt::format("\tImGui::RadioButton(\"{}\", {});\n\n",obj.label, obj.value_b);
    }

    if (obj.type == "checkbox")
    {
        if (!staticlayout) {
        bfs += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",obj.pos.x,obj.pos.y);
        }
        bfs += fmt::format("\tImGui::Checkbox(\"{}\", {});\n\n",obj.label, obj.value_b);
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

    str->append(bfs);
    
}

void ImStudio::GenerateCode(BufferWindow* bw)
{
    std::string output = "/*\nGENERATED CODE\n*/\n\n";
    output += "static bool window = true;\n";
    output += fmt::format("ImGui::SetNextWindowSize(ImVec2({},{}));\n", bw->size.x, bw->size.y);
    output += "if (ImGui::Begin(\"window_name\", &window))\n{\n\n";
    for (auto i = bw->objects.begin(); i != bw->objects.end(); ++i)
    {
        Object &o = *i;

        if (o.type != "child")
        {
            Recreate(o, &output, bw->staticlayout);
        }
        else
        {
            if (!bw->staticlayout) {
            output += fmt::format("\tImGui::SetCursorPos(ImVec2({},{}));\n",o.child.freerect.Min.x,o.child.freerect.Min.y);
            }
            output += fmt::format("\tImGui::BeginChild({}, ImVec2({},{}), {});\n\n", o.child.id, o.child.freerect.GetSize().x, o.child.freerect.GetSize().y, o.child.border);
            for (auto i = o.child.objects.begin(); i != o.child.objects.end(); ++i)
            {
                BaseObject &cw = *i;// child widget

                Recreate(cw, &output, bw->staticlayout);

            }
            output += "\tImGui::EndChild();\n\n";
        }
    }
    output += "\n\tImGui::End();\n}\n";
    ImGui::InputTextMultiline("##source", &output,
                              ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 64), ImGuiInputTextFlags_ReadOnly);

}