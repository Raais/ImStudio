#include "../includes.h"
#include "object.h"
#include "buffer.h"
#include "generator.h"

void ImStudio::Recreate(BaseObject obj, std::string* str)
{
    std::string bfs;

    if (obj.type == "button")
    {
        bfs = fmt::format("ImGui::Button({});\n",obj.value_s);
        
        str->append(bfs);
    }

}

void ImStudio::GenerateCode(BufferWindow* bw)
{
    std::string output = "/*\nGENERATED CODE\n*/\n\n";
    for (auto i = bw->objects.begin(); i != bw->objects.end(); ++i)
    {
        Object &o = *i;

        if (o.type != "child")
        {
            Recreate(o,&output);
        }
    }
    ImGui::InputTextMultiline("##source", &output,
                              ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 64), ImGuiInputTextFlags_ReadOnly);

}