#pragma once

#include <string>

#include "imgui.h"
#include "fmt/format.h"

#include "ims_object.h"
#include "ims_buffer.h"

namespace ImStudio
{

    void Recreate(BaseObject obj, std::string* output, bool staticlayout);
    void GenerateCode(std::string* output, BufferWindow* bw);

}