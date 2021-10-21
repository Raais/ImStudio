#include "../includes.h"
#include "buffer.h"
#include "object.h"

void PropertyBuffer::resetpropbuffer()
{
    prop_text1 = "change me";
}

void BufferWindow::drawall(int *select, int gen_rand)
{
    if (state)
    {
        ImGui::SetNextWindowPos(pos, ImGuiCond_Once);
        ImGui::SetNextWindowSize(size, ImGuiCond_Once);
        ImGui::Begin(name.c_str(), &state);
        size = ImGui::GetWindowSize();
        pos  = ImGui::GetWindowPos();
        {

            extra::metrics();
            for (auto i = objects.begin(); i != objects.end(); ++i)
            {
                Object &o = *i;

                if (o.state == false)
                {
                    i = objects.erase(i);
                    break;
                }
                else
                {
                    o.draw(select, gen_rand, staticlayout);
                }
            }
        }
        ImGui::End();
    }
}

Object *BufferWindow::getobj(int id)
{
    for (Object &o : objects)
    {
        if (o.id == id)
        {
            return &o;
        }
    }
    return nullptr;
}

void BufferWindow::create(std::string type_)
{
    idvar++;
    Object widget(idvar, type_);
    objects.push_back(widget);
}
