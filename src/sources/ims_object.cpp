#include "ims_object.h"

ImStudio::BaseObject::BaseObject(int idgen_, std::string type_, int parent_id_) // for child widgets
{
    ischildwidget = true;
    id            = idgen_;
    type          = type_;
    identifier    = "child" + std::to_string(parent_id_) + "::" + type_ + std::to_string(idgen_);
    value_s       = type_ + std::to_string(idgen_);
}

ImStudio::Object::Object(int idgen_, std::string type_) : BaseObject()
{
    if (type_ == "child")
    {
        child.objects.reserve(250);
        child.open = true;
        child.id   = idgen_;
    }
    id         = idgen_;
    type       = type_;
    identifier = type_ + std::to_string(idgen_);
    value_s    = type_ + std::to_string(idgen_);
}

//void ImStudio::BaseObject::draw(int *select, bool staticlayout = false)
// moved to ims_object_draw.cpp

void ImStudio::BaseObject::del()
{
    state = false;
}

void ImStudio::BaseObject::highlight(int *select)
{
    if (id == *select)
    {
        ImRect itemrect = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
        itemrect.Min.x -= 5;
        itemrect.Min.y -= 5;
        itemrect.Max.x += 5;
        itemrect.Max.y += 5;
        ImGui::GetForegroundDrawList()->AddRect(itemrect.Min, itemrect.Max, IM_COL32(255, 255, 0, 255));
    }
}

void ImStudio::ContainerChild::drawall(int *select, bool staticlayout)
{
    static auto dl = ImGui::GetWindowDrawList();

    if (!grabinit)
    {
        srand(time(NULL));
        grab1_id = rand() % 100;
        grab2_id = grab1_id + 1;
    }

    freerect.Min.x = grab1.x;
    freerect.Min.y = grab1.y;
    freerect.Max.x = grab2.x + 15;
    freerect.Max.y = grab2.y + 14;

    if (!staticlayout)
        ImGui::SetCursorPos(freerect.Min);
    ImGui::BeginChild(id, freerect.GetSize(), border,
                      ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);
    for (auto i = objects.begin(); i != objects.end(); ++i)
    {
        BaseObject &o = *i;
        if (o.state == false)
        {
            i = objects.erase(i);
            break;
        }
        else
        {
            o.draw(select, staticlayout);
        }
    }
    ImGui::EndChild();
    windowrect = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());

    if (open)
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.000f, 1.000f, 0.110f, 1.000f));
    if (!staticlayout)
    {
        if(!locked){
        if (utils::GrabButton(grab1, grab1_id))
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
            grab1   = utils::GetLocalCursor();
            *select = id;
        }

        if (utils::GrabButton(grab2, grab2_id))
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);
            grab2   = utils::GetLocalCursor();
            *select = id;
        }}
    }
    if (open)
        ImGui::PopStyleColor(1);

    windowrect.Min.x -= 5;
    windowrect.Min.y -= 5;
    windowrect.Max.x += 5;
    windowrect.Max.y += 5;

    if (id == *select)
    {
        if (open)
        {
            dl->AddRect(windowrect.Min, windowrect.Max, IM_COL32(0, 255, 28, 255));
        }
        else
        {
            dl->AddRect(windowrect.Min, windowrect.Max, IM_COL32(255, 255, 0, 255));
        }
    }

    grabinit = true;
}
