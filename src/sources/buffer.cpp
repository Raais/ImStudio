#include "../includes.h"
#include "buffer.h"
#include "object.h"

void PropertyBuffer::resetpropbuffer()
{
    prop_text1 = "change me";
    prop_text2 = "change me";
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

                if (o.open) childopen = true;
                else childopen = false;

                if (o.state == false)
                {
                    i = objects.erase(i);
                    break;
                }
                else
                {
                    if(o.type != "child")
                    {
                        o.draw(select, gen_rand, staticlayout);
                    }
                    else
                    {
                        cur_child = &o;
                        o.child.drawall(select, gen_rand, staticlayout);
                    }
                    
                }
            }
        }
        ImGui::End();
    }
}

BaseObject *BufferWindow::getobj(int id)
{
    for (Object &o : objects)
    {
        if (o.id == id)
        {
            return &o;
        }
        if (!o.child.objects.empty())
        {
            for (BaseObject &cw : o.child.objects)
            {
                if (cw.id == id)
                {
                    return &cw;
                }
            }
        }
    }
    return nullptr;
}

void BufferWindow::create(std::string type_)
{
    idvar++;
    if(!childopen)
    {
        Object widget(idvar, type_);
        objects.push_back(widget);
    }
    else
    {
        BaseObject childwidget(idvar, type_);
        childwidget.parent = cur_child;
        cur_child->child.objects.push_back(childwidget);
    }
}
