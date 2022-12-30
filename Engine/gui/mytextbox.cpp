//=============================================================================
//
// Adventure Game Studio (AGS)
//
// Copyright (C) 1999-2011 Chris Jones and 2011-20xx others
// The full list of copyright holders can be found in the Copyright.txt
// file, which is part of this source code distribution.
//
// The AGS source code is provided under the Artistic License 2.0.
// A copy of this license can be found in the file License.txt and at
// http://www.opensource.org/licenses/artistic-license-2.0.php
//
//=============================================================================
#include <stdio.h>
#include <string.h>
#include "ac/keycode.h"
#include "font/fonts.h"
#include "gui/mytextbox.h"
#include "gui/guidialoginternaldefs.h"
#include "gfx/bitmap.h"
#include "util/string.h"

using AGS::Common::Bitmap;
using AGS::Common::String;

extern int windowbackgroundcolor;
extern int cbuttfont;

MyTextBox::MyTextBox(int xx, int yy, int wii, String tee)
{
    x = xx;
    y = yy;
    wid = wii;
    text = tee;
    hit = TEXT_HT + 1;
}

void MyTextBox::draw(Bitmap *ds)
{
    color_t draw_color = ds->GetCompatibleColor(windowbackgroundcolor);
    ds->FillRect(Rect(x, y, x + wid, y + hit), draw_color);
    draw_color = ds->GetCompatibleColor(0);
    ds->DrawRect(Rect(x, y, x + wid, y + hit), draw_color);
    color_t text_color = ds->GetCompatibleColor(0);
    wouttextxy(ds, x + 2, y + 1, cbuttfont, text_color, text.GetCStr());

    char tbu[2] = "_";
    wouttextxy(ds, x + 2 + get_text_width(text.GetCStr(), cbuttfont), y + 1, cbuttfont, text_color, tbu);
}

int MyTextBox::pressedon(int /*mx*/, int /*my*/)
{
    return 0;
}

int MyTextBox::processmessage(int mcode, int wParam, String& lParam)
{
    if (mcode == CTB_SETTEXT) {
        text = lParam;
        needredraw = 1;
    }
    else if (mcode == CTB_GETTEXT) {
        lParam = text; // FIXME! dangerous
    } 
    else if (mcode == CTB_KEYPRESS) {
        // NOTE: this deprecated control does not support UTF-8
        int key = wParam;
        int uchar = lParam.GetAt(0);
        size_t len = text.GetLength();
        if (key == eAGSKeyCodeBackspace) {
            if (len > 0)
                text.ClipRight(1);
            drawandmouse();
            return 0;
        }

        if (len >= TEXTBOX_MAXLEN - 1)
            return 0; // buffer full;
        if (uchar == 0)
            return 0; // not a textual event
        if ((uchar >= 128) && (!font_supports_extended_characters(cbuttfont)))
            return 0; // unsupported letter
        if (get_text_width(text.GetCStr(), cbuttfont) >= wid - 5)
            return 0; // not enough control space
        text.AppendChar(uchar);
        drawandmouse();
    } else
        return -1;

    return 0;
}
