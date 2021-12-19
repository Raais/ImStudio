#pragma once

#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <memory>
#include <stdexcept>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_internal.h"
#include "fmt/format.h"
#include "utils/utils.h"
#ifdef __EMSCRIPTEN__
#include "utils/JsClipboardTricks.h"
#include "utils/HyperlinkHelper.h"
#endif
#ifndef PROJECT_VERSION_STRING
#define PROJECT_VERSION_STRING "0.0.0"
#endif
#ifndef GIT_SHA1
#define GIT_SHA1 "0000000"
#endif