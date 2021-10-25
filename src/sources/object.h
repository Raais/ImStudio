#pragma once

#include "../includes.h"

class BaseObject
{
  public:
    int               id              = 0;
    std::string       identifier      = {};
    std::string       type            = {};
    bool              init            = false;
    bool              state           = true;
    bool              value_b         = false;
    bool              propinit        = false;
    bool              locked          = false;
    bool              cond_1          = false;
    std::string       label           = "Label";
    std::string       value_s         = {};
    ImVec2            pos             = ImVec2(100, 100);
    ImVec2            size            = {};
    float             width           = 200;
    void*           parent          = nullptr;
    bool              open            = false;
    bool ischild = false;
    bool ischildwidget = false;
    ImVec2            child_grab1     = ImVec2(100, 100);
    ImVec2            child_grab2     = ImVec2(200, 200);
    int               child_id1       = 0;
    int               child_id2       = 0;
    int               item_current    = 0;
    int               ii0             = 123;
    float             fi0             = 0.001f;
    double            di0             = 999999.00000001;
    float             fi1             = 1.e10f;
    float             vec4a[4]        = { 0.10f, 0.20f, 0.30f, 0.44f };
    int               id1             = 50;
    int               id2             = 42;
    float             fd1             = 1.00f;
    float             fd2             = 0.0067f;
    int               is1             = 0;
    float             fs1             = 0.123f;
    float             fs2             = 0.0f;
    float             angle           = 0.0f;
    float             col1[3]         = { 1.0f, 0.0f, 0.2f };
    float             col2[3]         = { 1.0f, 0.0f, 0.2f };
    float             col3[4]         = { 0.4f, 0.7f, 0.0f, 0.5f };
    BaseObject();
    BaseObject(int idvar_, std::string type_);// for child objects only
    void              draw(int *select, int gen_rand, bool staticlayout);
    void              del();

  private:
    void              highlight(int *select);
};

class Child
{
  public:
    int id = 0;
    ImRect rect = {};
    ImVec2 size = {};
    ImVec2 pos = {};
    bool locked = false;
    std::vector<BaseObject> objects = {};
    void        drawall(int *select, int gen_rand, bool staticlayout);
  private:
    bool init = false;
    ImVec2 grab1 = ImVec2(90, 90);
    ImVec2 grab2 = ImVec2(200, 200);
    int grab1_id = 0;
    int grab2_id = 0;
    //void              highlight(int *select);
};

class Object : public BaseObject
{
  public:
    Child child;
    Object(int idvar_, std::string type_);
};