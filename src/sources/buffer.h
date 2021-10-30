#pragma once

#include "../includes.h"
#include "object.h"

class BufferWindow
{
  public:
    int                     id                      = 0;                    //
    bool                    state                   = false;                //
    ImVec2                  size                    = ImVec2(1070, 680);    // FIXME
    ImVec2                  pos                     = ImVec2(280, 120);     // FIXME
    int                     idvar                   = 0;                    //
    Object*                 current_child           = nullptr;              //
  
    bool                    staticlayout            = false;                //
  
    std::vector<Object>     objects                 = {};                   //

    void                    drawall                 (int *select, int gen_rand);
    Object *                getobj                  (int id);
    BaseObject *            getbaseobj              (int id);
    void                    create                  (std::string type_);
};