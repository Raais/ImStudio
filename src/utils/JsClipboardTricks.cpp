/*
https://github.com/pthom/imgui_manual/blob/master/src/JsClipboardTricks.cpp

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
#include "imgui.h"
#include "JsClipboardTricks.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>

// The clipboard handling features take inspiration from sokol
// https://github.com/floooh/sokol

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef IMGUIMANUAL_CLIPBOARD_IMPORT_FROM_BROWSER

    EM_JS(void, sapp_add_js_hook_clipboard, (void), {
        // See also https://whatwebcando.today/clipboard.html
        // for the new async api with user permissions dialog
        Module.sokol_paste = function(event) {
            // console.log("Got paste event ");
            var pasted_str = event.clipboardData.getData('text');
            ccall('_sapp_emsc_onpaste', 'void', ['string'], [pasted_str]);
        };
        // console.log("sapp_add_js_hook_clipboard 4");
        window.addEventListener('paste', Module.sokol_paste);
    });

    EM_JS(void, sapp_remove_js_hook_clipboard, (void), {
        window.removeEventListener('paste', Module.sokol_paste);
    });

    void EMSCRIPTEN_KEEPALIVE _sapp_emsc_onpaste(const char *str)
    {
        // std::cout << "_sapp_emsc_onpaste " << str << std::endl;
        ImGui::SetClipboardText(str);
    }

#endif // #ifdef IMGUIMANUAL_CLIPBOARD_IMPORT_FROM_BROWSER


#ifdef IMGUIMANUAL_CLIPBOARD_EXPORT_TO_BROWSER

    EM_JS(void, sapp_js_write_clipboard, (const char* c_str), {
        var str = UTF8ToString(c_str);
        var ta = document.createElement('textarea');
        ta.setAttribute('autocomplete', 'off');
        ta.setAttribute('autocorrect', 'off');
        ta.setAttribute('autocapitalize', 'off');
        ta.setAttribute('spellcheck', 'false');
        ta.style.left = -100 + 'px';
        ta.style.top = -100 + 'px';
        ta.style.height = 1;
        ta.style.width = 1;
        ta.value = str;
        document.body.appendChild(ta);
        ta.select();
        document.execCommand('copy');
        document.body.removeChild(ta);
        //console.log("Set clipboard to " + str);
    });

#endif // #ifdef IMGUIMANUAL_CLIPBOARD_EXPORT_TO_BROWSER

#ifdef __cplusplus
}
#endif

#ifdef IMGUIMANUAL_CLIPBOARD_EXPORT_TO_BROWSER
    void JsClipboard_SetClipboardText(const char* str)
    {
        sapp_js_write_clipboard(str);
    }
#endif

#ifdef IMGUIMANUAL_CLIPBOARD_IMPORT_FROM_BROWSER
    void JsClipboard_AddJsHook()
    {
        sapp_add_js_hook_clipboard();
    }
#endif

#endif // __EMSCRIPTEN__