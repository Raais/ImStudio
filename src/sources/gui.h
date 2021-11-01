#pragma once

#include "../includes.h"
#include "object.h"
#include "buffer.h"
#include "console.h"

namespace ImStudio
{

    struct GUI
    {
        bool                    state                      = true;                 // Alive
        bool                    wksp_create                = true;                 // Workspace "Create"

        bool                    menubar                    = true;                 // Menubar State
        ImVec2                  mb_P                       = {};                   // Menubar Pos
        ImVec2                  mb_S                       = {};                   // Menubar Size
        void                    ShowMenubar();         

        bool                    sidebar                    = true;                 // Sidebar State
        ImVec2                  sb_P                       = {};                   // Sidebar Pos
        ImVec2                  sb_S                       = {};                   // Sidebar Size
        void                    ShowSidebar();         

        bool                    properties                 = true;                 // Properties State
        ImVec2                  pt_P                       = {};                   // Properties Pos
        ImVec2                  pt_S                       = {};                   // Properties Size
        int                     selectid                   = 0;                    // Selected object (VP)
        int                     previd                     = 0;                    // Previous object
        BaseObject *            selectobj                  = nullptr;              // Pointer to access
        int                     selectproparray            = 0;                    // Selected from prop array
        void                    ShowProperties();      

        bool                    viewport                   = true;                 // Viewport State
        ImVec2                  vp_P                       = {};                   // Viewport Pos
        ImVec2                  vp_S                       = {};                   // Viewport Size
        BufferWindow            bw;            
        void                    ShowViewport               (int gen_rand);         

        bool                    wksp_output                = false;                // Workspace "Output"
        ImVec2                  ot_P                       = {};                   // Output Window Pos
        ImVec2                  ot_S                       = {};                   // Output Window Size
        std::string             output                     = {};
        void                    ShowOutputWorkspace();        

        bool                    child_console              = false;                // Show Console
        static void             ShowConsole                (bool* p_open, GUI* gui_);

        bool                    child_style                = false;                // Show Style Editor
        bool                    child_demo                 = false;                // Show Demo Window
        bool                    child_metrics              = false;                // Show Metrics Window
        bool                    child_color                = false;                // Show Color Export
        bool                    child_stack                = false;                // Show Stack Tool
        bool                    child_resources            = false;                // Show Help Resources
        bool                    child_about                = false;                // Show About Window
    };

}