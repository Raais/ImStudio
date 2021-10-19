#pragma once

#include "../includes.h"

class Object
{
  public:
    int         id          = 0;
    std::string identifier  = {};
    std::string type        = {};
    bool        init        = false;
    bool        state       = true;
    bool        value_b     = false;
    bool        moving      = false;
    bool        propinit    = false;
    std::string value_s     = {};
    ImVec2      pos         = ImVec2(100, 100);
    ImVec2      size        = {};
    ImRect      child       = ImRect(ImVec2(500, 200), ImVec2(700, 400));
    ImVec2      child_grab1 = ImVec2(100, 100);
    ImVec2      child_grab2 = ImVec2(200, 200);
    int         child_id1   = 0;
    int         child_id2   = 0;

    Object(int idvar_, std::string type_);
    void draw(int *select, int gen_rand);
    void del();

  private:
    void highlight(int *select);
};