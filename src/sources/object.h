//TODO: NEEDS REWORK; DESIGN CHANGE;
#pragma once

#include "../includes.h"

class Object;

class BaseObject
{
  public:
    int                     id                      = 0;                    //Unique ID
    std::string             type                    = {};                   //Widget type
    std::string             identifier              = {};                   //type+id
    bool                    state                   = true;                 //Alive

    ImVec2                  pos                     = ImVec2(100, 100);     //--
    ImVec2                  size                    = {};                   //  | Widget vectors
    float                   width                   = 200;                  //--

    bool                    init                    = false;                //--
    bool                    propinit                = false;                //  | Initialised
    bool                    selectinit              = true;                 //--

    bool                    locked                  = false;                //--
    bool                    center_h                = false;                //  | Properties
    bool                    autoresize              = true;                 //  |
    bool                    animate                 = true;                 //--

    std::string             label                   = "Label";              //--
    std::string             value_s                 = {};                   //  | Widget values/contents
    bool                    value_b                 = false;                //--


    Object*                 parent                  = nullptr;              //--
    //int                     parentid                = 0;                  //  | For child objects and
    bool                    ischild                 = false;                //  | child widgets
    bool                    ischildwidget           = false;                //--

    int                     item_current            = 0;                    //

    void draw               (int *select,           int gen_rand,           bool staticlayout);
    void del                ();

    BaseObject              ()                      = default;
    BaseObject              (int idvar_,            std::string type_,      int parent_id_);

  private:
    void highlight          (int *select);
};

class Child
{
  public:
    int                     id                      = 0;
    ImRect                  freerect                = {};
    ImRect                  windowrect              = {};
    ImVec2                  size                    = {};
    ImVec2                  pos                     = {};
    bool                    open                    = true;
    bool                    locked                  = false;
    bool                    init                    = false;
    std::vector<BaseObject> objects                 = {};
    void drawall            (int *select,           int gen_rand,           bool staticlayout);
  
  private:
    ImVec2                  grab1                   = ImVec2(90, 90);
    ImVec2                  grab2                   = ImVec2(200, 200);
    int                     grab1_id                = 0;
    int                     grab2_id                = 0;
    bool                    grabinit                = false;
};

//Object can now store either a single BaseObject or a vector of BaseObjects (child.objects)
class Object : public BaseObject
{
  public:
    //BaseObject{}
    Child                   child;
    Object                  (int idvar_, std::string type_);

};