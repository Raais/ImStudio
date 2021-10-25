#pragma once

#include "../includes.h"
#include "object.h"
#include "buffer.h"
#include "console.h"

struct GUI
{
    bool                state                       = true;
    bool                wksp_create                 = true;

    bool                menubar                     = true;
    ImVec2              mb_P                        = {};
    ImVec2              mb_S                        = {};
    void                ShowMenubar();          

    bool                sidebar                     = true;
    ImVec2              sb_P                        = {};
    ImVec2              sb_S                        = {};
    void                ShowSidebar();          

    bool                properties                  = true;
    ImVec2              pt_P                        = {};
    ImVec2              pt_S                        = {};
    int                 selectid                    = 0;
    BaseObject *        selectobj                   = nullptr;
    BaseObject *        selectobjprev               = nullptr;
    int                 selectproparray             = 0;
    bool                globaldelete                = false;
    void                ShowProperties();       

    bool                viewport                    = true;
    ImVec2              vp_P                        = {};
    ImVec2              vp_S                        = {};
    BufferWindow        bw;             
    void                ShowViewport                (int gen_rand);

    bool                wksp_output                 = false;
    ImVec2              ot_P                        = {};
    ImVec2              ot_S                        = {};
    void                ShowOutputWorkspace();      

    bool                child_console               = false;
    static void         ShowConsole                 (bool* p_open, GUI* gui_);

    bool                child_debug                 = false;
    bool                child_sty                   = false;
    bool                child_demo                  = false;
    bool                child_metrics               = false;
    bool                child_colexp                = false;
    bool                child_stack                 = false;
};