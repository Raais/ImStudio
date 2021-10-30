//TODO: NEEDS REWORK; DESIGN CHANGE;
#pragma once

#include "../includes.h"

class Object;

struct InputStatics
{
  //replace with statics?
  int                       ii0                     = 123;
  float                     fi0                     = 0.001f;
  double                    di0                     = 999999.00000001;
  float                     fi1                     = 1.e10f;
  float                     vec4a[4]                = { 0.10f, 0.20f, 0.30f, 0.44f };
  int                       id1                     = 50;
  int                       id2                     = 42;
  float                     fd1                     = 1.00f;
  float                     fd2                     = 0.0067f;
  int                       is1                     = 0;
  float                     fs1                     = 0.123f;
  float                     fs2                     = 0.0f;
  float                     angle                   = 0.0f;
  float                     col1[3]                 = { 1.0f, 0.0f, 0.2f };
  float                     col2[3]                 = { 1.0f, 0.0f, 0.2f };
  float                     col3[4]                 = { 0.4f, 0.7f, 0.0f, 0.5f };
  bool                      animate                 = true;
  float                     progress                = 0.0f;
  float                     progress_dir            = 1.0f;
};

class BaseObject : private InputStatics
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
    bool                    autoresize              = true;                 //--

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