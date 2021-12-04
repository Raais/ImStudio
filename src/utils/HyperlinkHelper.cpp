/*
https://github.com/pthom/imgui_manual/blob/master/src/imgui_utilities/HyperlinkHelper.cpp

The MIT License (MIT)

Copyright (c) 2019-2020 Pascal Thomet

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
//include <fplus/fplus.hpp>

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#elif defined(_WIN32)
#include <windows.h>
#include <Shellapi.h>
#elif defined(__APPLE__)
#include <TargetConditionals.h>
#include <cstdlib>
#endif

#include "HyperlinkHelper.h"


namespace HyperlinkHelper
{
    void OpenUrl(const std::string &url)
    {
        /*
        bool isAbsoluteUrl = fplus::is_prefix_of(std::string("http"), url);
        if (!isAbsoluteUrl)
            return;
        */
#if defined(__EMSCRIPTEN__)
        std::string js_command = "window.open(\"" + url + "\");";
            emscripten_run_script(js_command.c_str());
#elif defined(_WIN32)
        ShellExecuteA( NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL );
#elif defined(TARGET_OS_MAC)
        std::string cmd = std::string("open ") + url.c_str();
        system(cmd.c_str());
#endif

    }

}