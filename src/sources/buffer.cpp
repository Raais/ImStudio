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
                        if(!o.child.init) {current_child = &o; o.child.init = true;}

                        o.child.drawall(select, gen_rand, staticlayout);
                    }
                    
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

BaseObject *BufferWindow::getbaseobj(int id)
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
    if(!current_child)
    {
        Object widget(idvar, type_);
        objects.push_back(widget);
    }
    else
    {
        if(!current_child->child.open)
        {
            Object widget(idvar, type_);
            objects.push_back(widget);
        }
        else
        {
            BaseObject childwidget(idvar, type_, current_child->id);
            childwidget.parent = current_child;
            current_child->child.objects.push_back(childwidget);
        }
    }
}
