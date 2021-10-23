#include "../includes.h"
#include "object.h"

Object::Object(int idvar_, std::string type_)
{ // class-object-constr
    id         = idvar_;
    type       = type_;
    identifier = type_ + std::to_string(idvar_);
    value_s    = type_ + std::to_string(idvar_);
}

void Object::draw(int *select, int gen_rand, bool staticlayout = false)
{
    if (state)
    {
        if (type == "button")
        {
            if(!staticlayout)ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            ImGui::Button(value_s.c_str());

            ImGui::PopID();
            if ((!locked)&&(ImGui::IsItemActive()))
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "checkbox")
        {
            if(!staticlayout)ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            ImGui::Checkbox(label.c_str(), &value_b);

            ImGui::PopID();
            if ((!locked)&&(ImGui::IsItemActive()))
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "radio")
        {
            if(!staticlayout)ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            ImGui::RadioButton(label.c_str(), &value_b);

            ImGui::PopID();
            if ((!locked)&&(ImGui::IsItemActive()))
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "text")
        {

            ImGui::PushID(id);
            if(!staticlayout) ImGui::SetCursorPos(pos);
            ImGui::Text(value_s.c_str());
            if(!staticlayout)
            {
                ImVec2 textsize = ImGui::CalcTextSize(value_s.c_str());
                if(textsize.x <= 0){textsize.x = 1;}
                ImGui::SetCursorPos(pos);
                ImGui::InvisibleButton("", textsize);
            }
            
            ImGui::PopID();
            if ((!locked)&&(ImGui::IsItemActive()))
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "textinput")
        {
            ImGui::PushID(id);
            ImGui::PushItemWidth(width);
            if(!staticlayout) ImGui::SetCursorPos(pos);
            ImGui::InputText(label.c_str(),&value_s);

            ImGui::PopItemWidth();
            ImGui::PopID();
            if((!locked)&&((ImGui::IsItemActive())&&(ImGui::IsMouseDown(0))))
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "arrow")
        {
            ImGui::PushItemWidth(width);
            if(!staticlayout)ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            ImGui::ArrowButton("##left", ImGuiDir_Left);
            ImGui::SameLine();
            ImGui::ArrowButton("##right", ImGuiDir_Right);

            ImGui::PopID();
            ImGui::PopItemWidth();
            if ((!locked)&&(ImGui::IsItemActive()))
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "combo")
        {
            ImGui::PushItemWidth(width);
            if(!staticlayout)ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            const char *items[]      = {"Never", "Gonna", "Give", "You", "Up"};
            ImGui::Combo(label.c_str(), &item_current, items, IM_ARRAYSIZE(items));

            ImGui::PopID();
            ImGui::PopItemWidth();
            if ((!locked)&&(ImGui::IsItemActive()))
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "listbox")
        {
            ImGui::PushItemWidth(width);
            if(!staticlayout)ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            const char *items[]      = {"Never", "Gonna", "Give", "You", "Up"};
            ImGui::ListBox(label.c_str(), &item_current, items, IM_ARRAYSIZE(items));

            ImGui::PopID();
            ImGui::PopItemWidth();
            if ((!locked)&&(ImGui::IsItemActive()))
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "inputint")
        {
            ImGui::PushItemWidth(width);
            if(!staticlayout)ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            ImGui::InputInt(label.c_str(),&ii0);

            ImGui::PopID();
            ImGui::PopItemWidth();
            if((!locked)&&((ImGui::IsItemActive())&&(ImGui::IsMouseDown(0))))
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "inputfloat")
        {
            ImGui::PushItemWidth(width);
            if(!staticlayout)ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            ImGui::InputFloat(label.c_str(),&fi0,0.01f, 1.0f, "%.3f");

            ImGui::PopID();
            ImGui::PopItemWidth();
            if((!locked)&&((ImGui::IsItemActive())&&(ImGui::IsMouseDown(0))))
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "inputdouble")
        {
            ImGui::PushItemWidth(width);
            if(!staticlayout)ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            ImGui::InputDouble(label.c_str(),&di0,0.01f, 1.0f, "%.8f");

            ImGui::PopID();
            ImGui::PopItemWidth();
            if((!locked)&&((ImGui::IsItemActive())&&(ImGui::IsMouseDown(0))))
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "inputscientific")
        {
            ImGui::PushItemWidth(width);
            if(!staticlayout)ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            ImGui::InputFloat(label.c_str(), &fi1, 0.0f, 0.0f, "%e");

            ImGui::PopID();
            ImGui::PopItemWidth();
            if((!locked)&&((ImGui::IsItemActive())&&(ImGui::IsMouseDown(0))))
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "inputfloat3")
        {
            ImGui::PushItemWidth(width);
            if(!staticlayout)ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            ImGui::InputFloat3(label.c_str(), vec4a);

            ImGui::PopID();
            ImGui::PopItemWidth();
            if((!locked)&&((ImGui::IsItemActive())&&(ImGui::IsMouseDown(0))))
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "dragint")
        {
            ImGui::PushItemWidth(width);
            if(!staticlayout)ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            ImGui::DragInt(label.c_str(), &id1, 1);

            ImGui::PopID();
            ImGui::PopItemWidth();
            if ((!locked)&&(ImGui::IsItemActive()))
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "dragint100")
        {
            ImGui::PushItemWidth(width);
            if(!staticlayout)ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            ImGui::DragInt(label.c_str(), &id2, 1, 0, 100, "%d%%", ImGuiSliderFlags_AlwaysClamp);

            ImGui::PopID();
            ImGui::PopItemWidth();
            if ((!locked)&&(ImGui::IsItemActive()))
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "dragfloat")
        {
            ImGui::PushItemWidth(width);
            if(!staticlayout)ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            ImGui::DragFloat(label.c_str(), &fd1, 0.005f);

            ImGui::PopID();
            ImGui::PopItemWidth();
            if ((!locked)&&(ImGui::IsItemActive()))
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "dragfloatsmall")
        {
            ImGui::PushItemWidth(width);
            if(!staticlayout)ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            ImGui::DragFloat(label.c_str(), &fd2, 0.0001f, 0.0f, 0.0f, "%.06f ns");

            ImGui::PopID();
            ImGui::PopItemWidth();
            if ((!locked)&&(ImGui::IsItemActive()))
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "sliderint")
        {
            ImGui::PushItemWidth(width);
            if(!staticlayout)ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            ImGui::SliderInt(label.c_str(), &is1, -1, 3);

            ImGui::PopID();
            ImGui::PopItemWidth();
            if ((!locked)&&(ImGui::IsItemActive()))
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "sliderfloat")
        {
            ImGui::PushItemWidth(width);
            if(!staticlayout)ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            ImGui::SliderFloat(label.c_str(), &fs1, 0.0f, 1.0f, "ratio = %.3f");

            ImGui::PopID();
            ImGui::PopItemWidth();
            if ((!locked)&&(ImGui::IsItemActive()))
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "sliderfloatlog")
        {
            ImGui::PushItemWidth(width);
            if(!staticlayout)ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            ImGui::SliderFloat(label.c_str(), &fs2, -10.0f, 10.0f, "%.4f", ImGuiSliderFlags_Logarithmic);

            ImGui::PopID();
            ImGui::PopItemWidth();
            if ((!locked)&&(ImGui::IsItemActive()))
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "sliderangle")
        {
            ImGui::PushItemWidth(width);
            if(!staticlayout)ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            ImGui::SliderAngle(label.c_str(), &angle);

            ImGui::PopID();
            ImGui::PopItemWidth();
            if ((!locked)&&(ImGui::IsItemActive()))
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "color1")
        {
            ImGui::PushItemWidth(width);
            if(!staticlayout)ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            ImGui::ColorEdit3(label.c_str(), col1,ImGuiColorEditFlags_NoInputs);

            ImGui::PopID();
            ImGui::PopItemWidth();
            if ((!locked)&&(ImGui::IsItemActive()))
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "color2")
        {
            ImGui::PushItemWidth(width);
            if(!staticlayout)ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            ImGui::ColorEdit3(label.c_str(), col2);

            ImGui::PopID();
            ImGui::PopItemWidth();
            if ((!locked)&&(ImGui::IsItemActive()))
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "color3")
        {
            ImGui::PushItemWidth(width);
            if(!staticlayout)ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            ImGui::ColorEdit4(label.c_str(), col3);

            ImGui::PopID();
            ImGui::PopItemWidth();
            if ((!locked)&&(ImGui::IsItemActive()))
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "child")
        {//TODO: Child behaviour switch
            auto fg = ImGui::GetForegroundDrawList();

            if (!init)
            {
                child_id1 = gen_rand;
                child_id2 = gen_rand + 1;
            }
            extra::GrabButton(child_grab1, child_id1);
            if ((!locked)&&(ImGui::IsItemActive()))
            {
                child_grab1 = extra::GetLocalCursor();
                *select     = id;
            }

            extra::GrabButton(child_grab2, child_id2);
            if ((!locked)&&(ImGui::IsItemActive()))
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
        if (type == "bullet")
        {
            if(!staticlayout) ImGui::SetCursorPos(pos);
            ImGui::PushID(id);

            ImGui::Bullet();

            ImGui::PopID();
            if((!locked)&&((ImGui::IsItemActive())&&(ImGui::IsMouseDown(0))))
            {
                pos     = extra::GetLocalCursor();
                *select = id;
            }
            highlight(select);
        }
        if (type == "sameline")
        {
            if(staticlayout) ImGui::SameLine();
        }
        if (type == "newline")
        {
            if(staticlayout) ImGui::NewLine();
        }
        if (type == "separator")
        {
            if(staticlayout) ImGui::Separator();
        }
        if (type == "begingroup")
        {
            //if(staticlayout) ImGui::BeginGroup();
        }
        if (type == "endgroup")
        {
            //if(staticlayout) ImGui::EndGroup();
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
