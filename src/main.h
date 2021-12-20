#pragma once

#include "includes.h"
#include "sources/object.h"
#include "sources/buffer.h"
#include "sources/gui.h"

struct State
{
    ImStudio::GUI gui;
    std::mt19937 rng;
};

void MainWindowStyle();
void MainWindowGUI(State & state);