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
#include "ac/global_overlay.h"
#include "ac/common.h" // quit
#include "ac/draw.h"
#include "ac/overlay.h"
#include "ac/runtime_defines.h"


void RemoveOverlay(int ovrid) {
    if (find_overlay_of_type(ovrid) < 0) quit("!RemoveOverlay: invalid overlay id passed");
    remove_screen_overlay(ovrid);
}

int CreateGraphicOverlay(int x, int y, int slott, int trans) {
    auto *over = Overlay_CreateGraphicCore(false, x, y, slott, trans != 0, true); // always clone
    return over ? over->type : 0;
}

int CreateTextOverlay(int xx, int yy, int wii, int fontid, int text_color, const char* text, int disp_type) {
    TextShrink shrink_type = kTextShrinkNone;

    if (xx != OVR_AUTOPLACE) {
        data_to_game_coords(&xx,&yy);
        wii = data_to_game_coord(wii);
    }
    else  // allow DisplaySpeechBackground to be shrunk
        shrink_type = kTextShrinkLeft;

    auto *over = Overlay_CreateTextCore(false, xx, yy, wii, fontid, text_color, text, disp_type, shrink_type);
    return over ? over->type : 0;
}

void SetTextOverlay(int ovrid, int xx, int yy, int wii, int fontid, int text_color, const char *text) {
    RemoveOverlay(ovrid);
    const int disp_type = ovrid;
    if (CreateTextOverlay(xx, yy, wii, fontid, text_color, text, disp_type) !=ovrid)
        quit("SetTextOverlay internal error: inconsistent type ids");
}

void MoveOverlay(int ovrid, int newx,int newy) {
    data_to_game_coords(&newx, &newy);

    int ovri=find_overlay_of_type(ovrid);
    if (ovri<0) quit("!MoveOverlay: invalid overlay ID specified");
    screenover[ovri].x=newx;
    screenover[ovri].y=newy;
}

int IsOverlayValid(int ovrid) {
    if (find_overlay_of_type(ovrid) < 0)
        return 0;

    return 1;
}
