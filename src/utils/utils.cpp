#include "../includes.h"
#include "utils.h"

void extra::glfwSetWindowCenter(GLFWwindow *window)
{
    // Get window position and size
    int window_x, window_y;
    glfwGetWindowPos(window, &window_x, &window_y);

    int window_width, window_height;
    glfwGetWindowSize(window, &window_width, &window_height);

    // Halve the window size and use it to adjust the window position to the
    // center of the window
    window_width *= 0.5;
    window_height *= 0.5;

    window_x += window_width;
    window_y += window_height;

    // Get the list of monitors
    int           monitors_length;
    GLFWmonitor **monitors = glfwGetMonitors(&monitors_length);

    if (monitors == NULL)
    {
        // Got no monitors back
        return;
    }

    // Figure out which monitor the window is in
    GLFWmonitor *owner = NULL;
    int          owner_x, owner_y, owner_width, owner_height;

    for (int i = 0; i < monitors_length; i++)
    {
        // Get the monitor position
        int monitor_x, monitor_y;
        glfwGetMonitorPos(monitors[i], &monitor_x, &monitor_y);

        // Get the monitor size from its video mode
        int          monitor_width, monitor_height;
        GLFWvidmode *monitor_vidmode = (GLFWvidmode *)glfwGetVideoMode(monitors[i]);

        if (monitor_vidmode == NULL)
        {
            // Video mode is required for width and height, so skip this monitor
            continue;
        }
        else
        {
            monitor_width  = monitor_vidmode->width;
            monitor_height = monitor_vidmode->height;
        }

        // Set the owner to this monitor if the center of the window is within its
        // bounding box
        if ((window_x > monitor_x && window_x < (monitor_x + monitor_width)) &&
            (window_y > monitor_y && window_y < (monitor_y + monitor_height)))
        {
            owner = monitors[i];

            owner_x = monitor_x;
            owner_y = monitor_y;

            owner_width  = monitor_width;
            owner_height = monitor_height;
        }
    }

    if (owner != NULL)
    {
        // Set the window position to the center of the owner monitor
        glfwSetWindowPos(window, owner_x + (owner_width * 0.5) - window_width,
                         owner_y + (owner_height * 0.5) - window_height);
    }
}

ImVec2 extra::GetLocalCursor()
{
    ImGuiIO &     io         = ImGui::GetIO();
    ImGuiContext &g          = *ImGui::GetCurrentContext();
    ImGuiWindow * w          = g.CurrentWindow;
    ImVec2        cursor     = ImVec2(io.MousePos.x - w->Pos.x, io.MousePos.y - w->Pos.y);
    ImRect        itemrect   = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
    float         itemwidth  = itemrect.Max.x - itemrect.Min.x;
    float         itemheight = itemrect.Max.y - itemrect.Min.y;
    cursor.x                -= itemwidth / 2;
    cursor.y                -= itemheight / 2;
    return cursor;
}

ImVec2 extra::GetStaticCursor()
{
    ImGuiContext &g      = *ImGui::GetCurrentContext();
    ImGuiWindow * w      = g.CurrentWindow;
    ImVec2        cursor = ImVec2(ImGui::GetCursorPosX() - w->Pos.x, ImGui::GetCursorPosY() - w->Pos.y);
    return cursor;
}

ImVec2 extra::GetWindowSRatio()
{ // AKA scale factor
    ImGuiWindow *  w     = ImGui::GetCurrentWindow();
    ImGuiViewport *v     = ImGui::GetMainViewport();
    ImVec2         ratio = ImVec2(v->Size.x / w->Size.x, v->Size.y / w->Size.y);
    return ratio;
}

ImVec2 extra::GetWindowPRatio()
{ // AKA scale factor
    ImGuiWindow *  w     = ImGui::GetCurrentWindow();
    ImGuiViewport *v     = ImGui::GetMainViewport();
    ImVec2         ratio = ImVec2(v->Size.x / w->Pos.x, v->Size.y / w->Pos.y);
    return ratio;
}

ImVec2 extra::GetLastItemPos()
{ // Dangerous. FramePadding has to be 4x3
    ImGuiContext &g            = *ImGui::GetCurrentContext();
    ImGuiWindow * w            = ImGui::GetCurrentWindow();
    ImRect        r            = g.LastItemData.Rect;
    ImVec2        FramePadding = ImGui::GetStyle().FramePadding;
    ImVec2        pos          = ImVec2(r.Max.x - w->Pos.x - 71, r.Max.y - w->Pos.y - 19);
    return pos;
}

void extra::ShowStyleEditorWindow(bool *child_sty)
{
    ImGui::Begin("Style Editor", child_sty, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::ShowStyleEditor();
    ImGui::End();
}

void extra::ShowColorExportWindow(bool *child_colexp)
{
    if (ImGui::Begin("Color Export", child_colexp, ImGuiWindowFlags_AlwaysAutoResize))
    {
        static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);

        static bool         alpha_preview      = true;
        static bool         alpha_half_preview = true;
        static bool         drag_and_drop      = true;
        static bool         options_menu       = true;
        static bool         hdr                = false;
        ImGuiColorEditFlags misc_flags         = (hdr ? ImGuiColorEditFlags_HDR : 0) |
                                         (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) |
                                         (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf
                                                             : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) |
                                         (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

        ImGui::Text("Color widget:");
        ImGui::SameLine();
        HelpMarker("Click on the color square to open a color picker.\n"
                   "CTRL+click on individual component to input value.\n");
        ImGui::ColorEdit3("MyColor##1", (float *)&color, misc_flags);
        ImGui::SameLine();
        HelpMarker("Right click me to export colors with your preferred format.");

        ImGui::Text("Color widget HSV with Alpha:");
        ImGui::ColorEdit4("MyColor##2", (float *)&color, ImGuiColorEditFlags_DisplayHSV | misc_flags);

        ImGui::Text("Color widget with Float Display:");
        ImGui::ColorEdit4("MyColor##2f", (float *)&color, ImGuiColorEditFlags_Float | misc_flags);

        ImGui::Text("Color picker:");
        static bool   alpha        = true;
        static bool   alpha_bar    = true;
        static bool   side_preview = true;
        static bool   ref_color    = false;
        static ImVec4 ref_color_v(1.0f, 0.0f, 1.0f, 0.5f);
        static int    display_mode = 0;
        static int    picker_mode  = 2;
        ImGui::Combo("Display Mode", &display_mode, "Auto/Current\0None\0RGB Only\0HSV Only\0Hex Only\0");
        ImGui::SameLine();
        HelpMarker("ColorEdit defaults to displaying RGB inputs if you don't specify a display mode, "
                   "but the user can change it with a right-click.\n\nColorPicker defaults to displaying RGB+HSV+Hex "
                   "if you don't specify a display mode.\n\nYou can change the defaults using SetColorEditOptions().");
        ImGui::Combo("Picker Mode", &picker_mode, "Auto/Current\0Hue bar + SV rect\0Hue wheel + SV triangle\0");
        ImGui::SameLine();
        HelpMarker("User can right-click the picker to change mode.");
        ImGuiColorEditFlags flags = misc_flags;
        if (!alpha)
            flags |=
                ImGuiColorEditFlags_NoAlpha; // This is by default if you call ColorPicker3() instead of ColorPicker4()
        if (alpha_bar)
            flags |= ImGuiColorEditFlags_AlphaBar;
        if (!side_preview)
            flags |= ImGuiColorEditFlags_NoSidePreview;
        if (picker_mode == 1)
            flags |= ImGuiColorEditFlags_PickerHueBar;
        if (picker_mode == 2)
            flags |= ImGuiColorEditFlags_PickerHueWheel;
        if (display_mode == 1)
            flags |= ImGuiColorEditFlags_NoInputs; // Disable all RGB/HSV/Hex displays
        if (display_mode == 2)
            flags |= ImGuiColorEditFlags_DisplayRGB; // Override display mode
        if (display_mode == 3)
            flags |= ImGuiColorEditFlags_DisplayHSV;
        if (display_mode == 4)
            flags |= ImGuiColorEditFlags_DisplayHex;
        ImGui::ColorPicker4("MyColor##4", (float *)&color, flags, ref_color ? &ref_color_v.x : NULL);

        ImGui::Text("Set defaults in code:");
        ImGui::SameLine();
        HelpMarker("SetColorEditOptions() is designed to allow you to set boot-time default.\n"
                   "We don't have Push/Pop functions because you can force options on a per-widget basis if needed,"
                   "and the user can change non-forced ones with the options menu.\nWe don't have a getter to avoid"
                   "encouraging you to persistently save values that aren't forward-compatible.");
        if (ImGui::Button("Default: Uint8 + HSV + Hue Bar"))
            ImGui::SetColorEditOptions(ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_DisplayHSV |
                                       ImGuiColorEditFlags_PickerHueBar);
        if (ImGui::Button("Default: Float + HDR + Hue Wheel"))
            ImGui::SetColorEditOptions(ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR |
                                       ImGuiColorEditFlags_PickerHueWheel);

        // HSV encoded support (to avoid RGB<>HSV round trips and singularities when S==0 or V==0)
        static ImVec4 color_hsv(0.23f, 1.0f, 1.0f, 1.0f); // Stored as HSV!
        ImGui::Spacing();
        ImGui::Text("HSV encoded colors");
        ImGui::SameLine();
        HelpMarker(
            "By default, colors are given to ColorEdit and ColorPicker in RGB, but ImGuiColorEditFlags_InputHSV"
            "allows you to store colors as HSV and pass them to ColorEdit and ColorPicker as HSV. This comes with the"
            "added benefit that you can manipulate hue values with the picker even when saturation or value are zero.");
        ImGui::Text("Color widget with InputHSV:");
        ImGui::ColorEdit4("HSV shown as RGB##1", (float *)&color_hsv,
                          ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputHSV | ImGuiColorEditFlags_Float);
        ImGui::ColorEdit4("HSV shown as HSV##1", (float *)&color_hsv,
                          ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_InputHSV | ImGuiColorEditFlags_Float);
        ImGui::DragFloat4("Raw HSV values", (float *)&color_hsv, 0.01f, 0.0f, 1.0f);
        ImGui::End();
    }
}

bool extra::IsItemActiveAlt(ImVec2 pos, int id)
{
    bool active = false;
    ImGui::SetCursorPos(pos);
    ImGui::PushID(id);
    ImGui::InvisibleButton(" ", ImGui::GetItemRectSize());
    ImGui::PopID();
    active = ImGui::IsItemActive();
    return active;
}

bool extra::GrabButton(ImVec2 pos, int random_int)
{
    bool active = false;
    ImGui::SetCursorPos(pos);
    ImGui::PushID(random_int);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.00f, 0.00f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
    ImGui::BeginChild(random_int + 9, ImVec2(20, 20));
    ImGui::Button("  ");
    active = ImGui::IsItemActive();
    ImGui::EndChild();
    ImGui::PopStyleVar(2);
    ImGui::PopID();
    return active;
}

void extra::HelpMarker(const char *desc)
{
    ImGui::PushStyleColor(ImGuiCol_Text,ImVec4(0.92f, 0.92f, 0.92f, 1.00f));
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
    ImGui::PopStyleColor(1);
}

std::string extra::QueryLastItem()
{
    char ret[999];
    std::sprintf(ret,
        "IsItemFocused() = %d\n"
        "IsItemHovered() = %d\n"
        "IsItemHovered(_AllowWhenBlockedByPopup) = %d\n"
        "IsItemHovered(_AllowWhenBlockedByActiveItem) = %d\n"
        "IsItemHovered(_AllowWhenOverlapped) = %d\n"
        "IsItemHovered(_AllowWhenDisabled) = %d\n"
        "IsItemHovered(_RectOnly) = %d\n"
        "IsItemActive() = %d\n"
        "IsItemEdited() = %d\n"
        "IsItemActivated() = %d\n"
        "IsItemDeactivated() = %d\n"
        "IsItemDeactivatedAfterEdit() = %d\n"
        "IsItemVisible() = %d\n"
        "IsItemClicked() = %d\n"
        "IsItemToggledOpen() = %d\n"
        "GetItemRectMin() = (%.1f, %.1f)\n"
        "GetItemRectMax() = (%.1f, %.1f)\n"
        "GetItemRectSize() = (%.1f, %.1f)",
        ImGui::IsItemFocused(),
        ImGui::IsItemHovered(),
        ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup),
        ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem),
        ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped),
        ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled),
        ImGui::IsItemHovered(ImGuiHoveredFlags_RectOnly),
        ImGui::IsItemActive(),
        ImGui::IsItemEdited(),
        ImGui::IsItemActivated(),
        ImGui::IsItemDeactivated(),
        ImGui::IsItemDeactivatedAfterEdit(),
        ImGui::IsItemVisible(),
        ImGui::IsItemClicked(),
        ImGui::IsItemToggledOpen(),
        ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y,
        ImGui::GetItemRectMax().x, ImGui::GetItemRectMax().y,
        ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y
    );
    std::string rets(ret);
    return rets;
}

void extra::TextCentered(std::string text, int type = 0)
{
    auto windowWidth = ImGui::GetWindowSize().x;
    auto textWidth   = ImGui::CalcTextSize(text.c_str()).x;

    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    switch (type)
    {
    case 0:
        ImGui::Text(text.c_str());
        break;
    
    case 1:
        ImGui::TextDisabled(text.c_str());
        break;
    }
}

void extra::metrics()
{ // ugly debug stuff

    ImGuiIO &      io      = ImGui::GetIO();
    ImGuiWindow *  window  = ImGui::GetCurrentWindowRead();
    ImGuiViewport *vw      = ImGui::GetMainViewport();
    ImGuiContext & g       = *ImGui::GetCurrentContext();
    ImVec2         padding = ImGui::GetStyle().WindowPadding;
    ImGui::Text("%gx%g", vw->Size.x, vw->Size.y);
    // ImGui::Text("%f%",(window->Size.x/vw->Size.x)*100);
    ImGui::Text("name: %s", window->Name);
    ImGui::Text("IMsize: %g,%g", window->Size.x, window->Size.y);
    ImGui::Text("IMpos: %g,%g", window->Pos.x, window->Pos.y);
    ImGui::Text("padding: %gx%g", padding.x, padding.y);
    ImGui::Text("static cursor: (%g,%g)", extra::GetStaticCursor().x, extra::GetStaticCursor().y);
    ImGui::Text("global cursor: (%g,%g)", ImGui::GetCursorPosX, ImGui::GetCursorPosY);
    ImGui::Text("mousepos: (%g,%g)", io.MousePos.x, io.MousePos.y);
    ImGui::Text("local cursor: (%g,%g)", extra::GetLocalCursor().x, extra::GetLocalCursor().y);

    ImGui::Text("global cursor: (%g,%g)", ImGui::GetCursorPosX, ImGui::GetCursorPosY);
    ImGui::Text("screen cursor: (%g,%g)", ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y);
    ImGui::Text("ratioS:: %f,%f", extra::GetWindowSRatio().x, GetWindowSRatio().y);
    ImGui::Text("ratioP:: %f,%f", extra::GetWindowPRatio().x, GetWindowPRatio().y);
    ImGui::Text("frame padding: %gx%g", ImGui::GetStyle().FramePadding.x, ImGui::GetStyle().FramePadding.y);
}
