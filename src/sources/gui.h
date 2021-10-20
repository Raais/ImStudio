#pragma once

#include "../includes.h"
#include "object.h"
#include "buffer.h"

namespace ImStudio
{

void Menubar
(
ImVec2 &mb_P,
ImVec2 &mb_S, 
bool &child_debug, 
bool &child_sty, 
bool &child_demo, 
bool &child_metrics,
bool &child_stack, 
bool &child_colexp, 
bool &main_state, 
bool &ly_save, 
bool &wksp_logic,
bool &wksp_interface
);

void Sidebar(ImVec2 &sb_P, ImVec2 &sb_S, ImVec2 &sb_Sr, bool &ly_save, bool &resizing, int &w_w, int &w_h,
             BufferWindow &bf, bool &main_state);

void Properties(ImVec2 &pt_P, ImVec2 &pt_Pr, ImVec2 &pt_S, ImVec2 &pt_Sr, ImVec2 &mb_S, int &w_w, int &w_h,
                bool &ly_save, bool &resizing, BufferWindow &bf, int &select, int &item_current, Object *selectobj,
                Object *selectobjprev);

} // namespace ImStudio