//[https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp | Example App: Debug Console / ShowExampleAppConsole()]
#pragma once

#include "../includes.h"
#include "object.h"
#include "buffer.h"
#include "gui.h"

namespace ImStudio
{

    struct Console
    {
        char                  InputBuf[256];
        ImVector<char*>       Items;
        ImVector<const char*> Commands;
        ImVector<char*>       History;
        int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
        ImGuiTextFilter       Filter;
        bool                  AutoScroll;
        bool                  ScrollToBottom;
        bool                  StdoutMode;
        ImStudio::GUI*        gui_;
    
    
        Console               (ImStudio::GUI* _gui_);
        ~Console              ();
    
        // Portable helpers
        static int            Stricmp(const char* s1, const char* s2);
        static int            Strnicmp(const char* s1, const char* s2, int n);
        static char*          Strdup(const char* s);
        static void           Strtrim(char* s);
    
        void                  ClearLog();
            
        void                  AddLog(const char* fmt, ...) IM_FMTARGS(2);
            
        void                  Draw(const char* title, bool* p_open);
            
        void                  ExecCommand(const char* command_line);
    
        static int            TextEditCallbackStub(ImGuiInputTextCallbackData* data);
    
        int                   TextEditCallback(ImGuiInputTextCallbackData* data);
    };

}