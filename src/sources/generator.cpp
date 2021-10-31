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