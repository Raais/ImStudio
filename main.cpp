#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

#include "extra.h"
#include "font/opensans.cpp"

//////////////////////////////////////////////////////////////////////////////////////////////////
int w_w = 900;
int w_h = 600;
bool nodeco = false;//No window decorations
bool drag = false;//Handle dragging manually
//////////////////////////////////////////////////////////////////////////////////////////////////
///OBJECTS

//////////////////////////////////////////////////////////////////////////////////////////////////
///GLFW FUNCS

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

void cursor_position_callback(GLFWwindow* glwindow, double x, double y);

void mouse_button_callback(GLFWwindow *glwindow, int button, int action, int mods);

static void glfw_error_callback(int error, const char* description){
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int cp_x,cp_y,offset_cpx,offset_cpy,w_posx,w_posy,buttonEvent;

//////////////////////////////////////////////////////////////////////////////////////////////////
///MAIN FUNC()
///GLFW BOILERPLATE

int main(int argc, char* argv[])
{

    ///////////////////////////////////////////////
    ///ARGS
    std::vector<std::string> args(argv, argv+argc);

    for (size_t i = 1; i < args.size(); ++i)
    {
      if (args[i] == "-nd") {nodeco = true;}
      if (args[i] == "-dg") {drag = true;} 
    }
    ///////////////////////////////////////////////

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

#if defined(IMGUI_IMPL_OPENGL_ES2)
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////
///CREATE glwindow

    if(nodeco){glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);} else {glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);}
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);//glwindow to transparent; handle color through (internal) ImGui Window;
    
    GLFWwindow* glwindow = glfwCreateWindow(w_w, w_h, "OpenGL", NULL, NULL);

    if (glwindow == NULL)
        return 1;

    if(drag){
        glfwSetCursorPosCallback(glwindow, cursor_position_callback);
        glfwSetMouseButtonCallback(glwindow, mouse_button_callback);
    }

    glfwMakeContextCurrent(glwindow);
    glfwSwapInterval(1); // Enable vsync
    extra::glfwSetWindowCenter(glwindow);

//////////////////////////////////////////////////////////////////////////////////////////////////
///CONTEXT

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(glwindow, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

//////////////////////////////////////////////////////////////////////////////////////////////////
///IMGUI WINDOWS

    bool imwindow = true;

//////////////////////////////////////////////////////////////////////////////////////////////////
///STYLES

    //io.Fonts->Build();
    io.IniFilename = NULL;
    ImVec4 bg = ImVec4(0.123f,0.123f,0.123,1.00f); //Main bg color

//////////////////////////////////////////////////////////////////////////////////////////////////
///VARS

//////////////////////////////////////////////////////////////////////////////////////////////////
///MAIN LOOP >>>>

    while(!glfwWindowShouldClose(glwindow)){   

        ///////////////////////////////////////////////////////////////////////////
        //Handle window drags
        if(drag){

            if(buttonEvent == 1){

                glfwGetWindowPos(glwindow, &w_posx, &w_posy);
                glfwSetWindowPos(glwindow, w_posx + offset_cpx, w_posy + offset_cpy);
                offset_cpx = 0;
                offset_cpy = 0;
                cp_x += offset_cpx;
                cp_y += offset_cpy;

            }
        
            glfwPostEmptyEvent();

        } else {glfwPollEvents(); glfwGetWindowSize(glwindow, &w_w, &w_h);}
        ///////////////////////////////////////////////////////////////////////////

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(0,0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::SetNextWindowBgAlpha(0.00f);

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
///GUI >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

        if(imwindow){
            
            ImGui::Begin("imwindow", &imwindow, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

            if(drag){
                if(ImGui::Button("X")){
                    break;
                }
            }
            ImGui::Text("%dx%d",w_w,w_h);
    
            ImGui::Text("Hello World");

            ImGui::ColorEdit3("Color", (float*)&bg, ImGuiColorEditFlags_Float);
            if(ImGui::Button("Export")){
                std::string exp = "ImVec4 col = ImVec4(" + std::to_string(bg.x) + "f," + std::to_string(bg.y) + "f," + std::to_string(bg.z) + "f,1.00f);";
                ImGui::LogToClipboard();
                ImGui::LogText(exp.c_str());
                ImGui::LogFinish();
            }

            

            if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape))){
                break;
            }

            ImGui::End();
        }

///GUI END <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
///RENDER

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(glwindow, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(bg.x * bg.w, bg.y * bg.w, bg.z * bg.w, bg.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(glwindow);

        if(drag){
            glfwWaitEvents();
            glfwPollEvents();            
        }

    }

///MAIN LOOP END <<<<
//////////////////////////////////////////////////////////////////////////////////////////////////

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(glwindow);
    glfwTerminate();

    return 0;

///MAIN FUNC END()
}

void cursor_position_callback(GLFWwindow* glwindow, double x, double y){

    if(buttonEvent == 1){
        offset_cpx = x - cp_x;
        offset_cpy = y - cp_y;
    }

}

void mouse_button_callback(GLFWwindow* glwindow, int button, int action, int mods){

    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        buttonEvent = 1;
        double x, y;
        glfwGetCursorPos(glwindow, &x, &y);
        cp_x = floor(x);
        cp_y = floor(y);
    }
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
        buttonEvent = 0;
        cp_x = 0;
        cp_y = 0;
    }
    
}
