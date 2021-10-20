#include "../includes.h"
#include "object.h"

Object::Object(int idvar_, std::string type_)
{ // class-object-constr
    id         = idvar_;
    type       = type_;
    identifier = type_ + std::to_string(idvar_);
    value_s    = type_ + std::to_string(idvar_);
}

void Object::draw(int *select, int gen_rand)
{
    if (state)
    {
        if (type == "button")
        {
            ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            ImGui::Button(value_s.c_str());

            ImGui::PopID();
            if (ImGui::IsItemActive())
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "checkbox")
        {
            ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            ImGui::Checkbox(value_s.c_str(), &value_b);

            ImGui::PopID();
            if (ImGui::IsItemActive())
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "radio")
        {
            ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            ImGui::RadioButton(value_s.c_str(), &value_b);

            ImGui::PopID();
            if (ImGui::IsItemActive())
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "combo")
        {
            ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            const char *items[]      = {"Never", "Gonna", "Give", "You", "Up"};
            static int  item_current = 0;
            ImGui::Combo(value_s.c_str(), &item_current, items, IM_ARRAYSIZE(items));

            ImGui::PopID();
            if (ImGui::IsItemActive())
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "child")
        {
            auto fg = ImGui::GetForegroundDrawList();

            if (!init)
            {
                child_id1 = gen_rand;
                child_id2 = gen_rand + 1;
            }
            extra::GrabButton(child_grab1, child_id1);
            if (ImGui::IsItemActive())
            {
                child_grab1 = extra::GetLocalCursor();
                *select     = id;
            }

            extra::GrabButton(child_grab2, child_id2);
            if (ImGui::IsItemActive())
            {
                child_grab2 = extra::GetLocalCursor();
                *select     = id;
            }
            child.Min.x = child_grab1.x + ImGui::GetWindowPos().x;
            child.Min.y = child_grab1.y + ImGui::GetWindowPos().y;
            child.Max.x = child_grab2.x + ImGui::GetWindowPos().x + 15;
            child.Max.y = child_grab2.y + ImGui::GetWindowPos().y + 14;
            // child.Min = child_grab1;
            // child.Max = child_grab2;
            if (id == *select)
            {
                fg->AddRect(child.Min, child.Max, IM_COL32(255, 255, 0, 255));
            }
            else
            {
                fg->AddRect(child.Min, child.Max, IM_COL32(170, 170, 170, 255));
            }

            init = true;
        }
        if (type == "text")
        {
            ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            ImGui::Text(value_s.c_str());
            ImGui::SetCursorPos(pos);
            ImGui::InvisibleButton("", ImGui::CalcTextSize(value_s.c_str()));

            ImGui::PopID();
            if (ImGui::IsItemActive())
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
    }
}

void Object::del()
{
    state = false;
}

void Object::highlight(int *select)
{
    if (id == *select)
    {
        ImGui::GetForegroundDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(),
                                                IM_COL32(255, 255, 0, 255));
    }
}
