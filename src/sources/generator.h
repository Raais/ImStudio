#pragma once

#include "../includes.h"
#include "object.h"
#include "buffer.h"

namespace ImStudio
{

    void Recreate(BaseObject obj, std::string* output, bool staticlayout);
    void GenerateCode(std::string* output, BufferWindow* bw);

}