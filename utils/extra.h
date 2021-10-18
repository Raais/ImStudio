#include <memory>
#include <stdexcept>

#pragma once

namespace extra {

void glfwSetWindowCenter(GLFWwindow *window) {
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

  if (monitors == NULL) {
    // Got no monitors back
    return;
  }

  // Figure out which monitor the window is in
  GLFWmonitor *owner = NULL;
  int          owner_x, owner_y, owner_width, owner_height;

  for (int i = 0; i < monitors_length; i++) {
    // Get the monitor position
    int monitor_x, monitor_y;
    glfwGetMonitorPos(monitors[i], &monitor_x, &monitor_y);

    // Get the monitor size from its video mode
    int          monitor_width, monitor_height;
    GLFWvidmode *monitor_vidmode = (GLFWvidmode *)glfwGetVideoMode(monitors[i]);

    if (monitor_vidmode == NULL) {
      // Video mode is required for width and height, so skip this monitor
      continue;

    } else {
      monitor_width  = monitor_vidmode->width;
      monitor_height = monitor_vidmode->height;
    }

    // Set the owner to this monitor if the center of the window is within its
    // bounding box
    if ((window_x > monitor_x && window_x < (monitor_x + monitor_width)) &&
        (window_y > monitor_y && window_y < (monitor_y + monitor_height))) {
      owner = monitors[i];

      owner_x = monitor_x;
      owner_y = monitor_y;

      owner_width  = monitor_width;
      owner_height = monitor_height;
    }
  }

  if (owner != NULL) {
    // Set the window position to the center of the owner monitor
    glfwSetWindowPos(window, owner_x + (owner_width * 0.5) - window_width,
                     owner_y + (owner_height * 0.5) - window_height);
  }
}

ImVec2 GetLocalCursor() {
  ImGuiIO &     io = ImGui::GetIO();
  ImGuiContext &g  = *ImGui::GetCurrentContext();
  ImGuiWindow * w  = g.CurrentWindow;
  ImVec2 cursor    = ImVec2(io.MousePos.x - w->Pos.x, io.MousePos.y - w->Pos.y);
  return cursor;
}

ImVec2 GetStaticCursor() {
  ImGuiContext &g      = *ImGui::GetCurrentContext();
  ImGuiWindow * w      = g.CurrentWindow;
  ImVec2        cursor = ImVec2(ImGui::GetCursorPosX() - w->Pos.x,
                         ImGui::GetCursorPosY() - w->Pos.y);
  return cursor;
}

ImVec2 GetWindowSRatio() {  // AKA scale factor
  ImGuiWindow *  w     = ImGui::GetCurrentWindow();
  ImGuiViewport *v     = ImGui::GetMainViewport();
  ImVec2         ratio = ImVec2(v->Size.x / w->Size.x, v->Size.y / w->Size.y);
  return ratio;
}

ImVec2 GetWindowPRatio() {  // AKA scale factor
  ImGuiWindow *  w     = ImGui::GetCurrentWindow();
  ImGuiViewport *v     = ImGui::GetMainViewport();
  ImVec2         ratio = ImVec2(v->Size.x / w->Pos.x, v->Size.y / w->Pos.y);
  return ratio;
}

ImVec2 GetLastItemPos() {//Dangerous. FramePadding has to be 4x3
  ImGuiContext &g            = *ImGui::GetCurrentContext();
  ImGuiWindow * w            = ImGui::GetCurrentWindow();
  ImRect        r            = g.LastItemData.Rect;
  ImVec2        FramePadding = ImGui::GetStyle().FramePadding;
  ImVec2        pos = ImVec2(r.Max.x - w->Pos.x - 71, r.Max.y - w->Pos.y - 19);
  return pos;
}

void GrabButton(ImVec2 pos,int random_int) {
  ImGui::SetCursorPos(pos);
  ImGui::PushID(random_int);
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,ImVec2(0.00f, 0.00f));
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,0);
  ImGui::Button("  ");
  ImGui::PopStyleVar(2);
  ImGui::PopID();
}

static void HelpMarker(const char* desc) {
  ImGui::TextDisabled("(?)");
  if (ImGui::IsItemHovered()) {
    ImGui::BeginTooltip();
    ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
    ImGui::TextUnformatted(desc);
    ImGui::PopTextWrapPos();
    ImGui::EndTooltip();
  }
}

void metrics() {  // ugly debug stuff

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
  ImGui::Text("static cursor: (%g,%g)", extra::GetStaticCursor().x,
              extra::GetStaticCursor().y);
  ImGui::Text("global cursor: (%g,%g)", ImGui::GetCursorPosX,
              ImGui::GetCursorPosY);
  ImGui::Text("mousepos: (%g,%g)", io.MousePos.x, io.MousePos.y);
  ImGui::Text("local cursor: (%g,%g)", extra::GetLocalCursor().x,
              extra::GetLocalCursor().y);

  ImGui::Text("global cursor: (%g,%g)", ImGui::GetCursorPosX,
              ImGui::GetCursorPosY);
  ImGui::Text("screen cursor: (%g,%g)", ImGui::GetCursorScreenPos().x,
              ImGui::GetCursorScreenPos().y);
  ImGui::Text("ratioS:: %f,%f", extra::GetWindowSRatio().x,
              GetWindowSRatio().y);
  ImGui::Text("ratioP:: %f,%f", extra::GetWindowPRatio().x,
              GetWindowPRatio().y);
  ImGui::Text("frame padding: %gx%g", ImGui::GetStyle().FramePadding.x,
              ImGui::GetStyle().FramePadding.y);
}

template <typename... Args>
std::string string_format(const std::string &format, Args... args) {
  int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) +
               1;  // Extra space for '\0'
  if (size_s <= 0) {
    throw std::runtime_error("Error during formatting.");
  }
  auto size = static_cast<size_t>(size_s);
  auto buf  = std::make_unique<char[]>(size);
  std::snprintf(buf.get(), size, format.c_str(), args...);
  return std::string(buf.get(),
                     buf.get() + size - 1);  // We don't want the '\0' inside
}

}  // namespace extra
