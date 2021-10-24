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
                    if(!o.isChild)
                    {
                        o.draw(select, gen_rand, staticlayout);
                    }
                    else
                    {
                        std::cout << "adding to child" << std::endl;
                        addingtochild = true;
                        cur_child = &o;
                        o.child_.drawall(select, gen_rand, staticlayout);
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
        if (!o.child_.objects.empty())
        {
            for (BaseObject &cw : o.child_.objects)
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
    if(!addingtochild)
    {
        Object widget(idvar, type_);
        widget.parent = &widget;
        if (type_ == "child"){widget.child_.objects.reserve(250);}
        objects.push_back(widget);
    }
    else
    {
        BaseObject childwidget;
        childwidget.parent = cur_child;
        childwidget.ischildwidget = true;
        childwidget.id         = idvar;
        childwidget.type = type_;
        childwidget.identifier = type_ + std::to_string(idvar);
        childwidget.value_s = type_ + std::to_string(idvar);
        cur_child->child_.objects.push_back(childwidget);
        std::cout << "CREATED childwidget" << std::endl;
    }
}
