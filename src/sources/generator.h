#pragma once

#include "../includes.h"
#include "object.h"
#include "buffer.h"

namespace ImStudio
{

    void Recreate(BaseObject obj, std::string* str);
    void GenerateCode(BufferWindow* bw);

}