#pragma once

#include "includes.h"
#include "sources/object.h"
#include "sources/buffer.h"
#include "sources/gui.h"

struct State
{
    ImStudio::GUI gui;
    std::mt19937 rng;
    int w_w = 1280;
    int w_h = 720;
};

void MainWindowStyle();
void MainWindowGUI(State & state);