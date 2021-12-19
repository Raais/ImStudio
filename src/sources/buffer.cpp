#include "../includes.h"
#include "buffer.h"
#include "object.h"

void ImStudio::BufferWindow::drawall(int *select, int gen_rand)
{
    if (state)
    {
        ImVec2 parent_size = ImGui::GetWindowSize();
        ImVec2 init_size   = ImVec2(parent_size.x * 0.8, parent_size.y * 0.7);
        ImVec2 center      = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(size);
        ImGui::SetNextWindowSize(init_size, ImGuiCond_Once);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.08f, 0.09f, 0.09f, 1.00f));
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.08f, 0.09f, 0.09f, 1.00f));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.26f, 0.59f, 0.98f, 0.40f));
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.86f, 0.86f, 0.86f, 0.50f));
        ImGui::Begin("buffer", &state);
        size = ImGui::GetWindowSize();
        pos  = ImGui::GetWindowPos();
        {
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
                    if (o.type != "child")
                    {
                        o.draw(select, gen_rand, staticlayout);
                    }
                    else
                    {
                        if (!o.child.init)
                        {
                            current_child = &o;
                            o.child.init  = true;
                        }

                        o.child.drawall(select, gen_rand, staticlayout);
                    }
                }
            }
        }
        ImGui::End();
        ImGui::PopStyleColor(4);
    }
}

ImStudio::Object *ImStudio::BufferWindow::getobj(int id)
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

ImStudio::BaseObject *ImStudio::BufferWindow::getbaseobj(int id)
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

void ImStudio::BufferWindow::create(std::string type_)
{
    idvar++;
    if (!current_child)
    {
        Object widget(idvar, type_);
        objects.push_back(widget);
    }
    else
    {
        if (!current_child->child.open)
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
