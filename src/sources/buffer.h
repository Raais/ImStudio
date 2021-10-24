#pragma once

#include "../includes.h"
#include "object.h"

class PropertyBuffer
{
  public:
    std::string prop_text1 = "change me";
    std::string prop_text2 = "change me";
    bool        prop_bool1 = false;
    void        resetpropbuffer();
};

class BufferWindow : public PropertyBuffer
{
  public:
    int         id    = 0;
    bool        state = false;
    std::string name  = "window_0";
    ImVec2      size  = ImVec2(1070, 680); // FIXME
    ImVec2      pos   = ImVec2(280, 120);  // FIXME
    int         idvar = 0;
    bool        addingtochild = false;
    Object*     cur_child = nullptr;

    bool staticlayout = false;

    std::vector<Object> objects = {};

    void        drawall(int *select, int gen_rand);
    BaseObject *    getobj(int id);
    bool        AnySelected(Object *selected_);
    std::string gettype(int id);
    void        create(std::string type_);
};