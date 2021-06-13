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

#include "font/ttffontrenderer.h"
#include "ac/game_version.h"
#include "core/platform.h"
#include <SDL_ttf.h>
#include <debug/out.h>
#include <gfx/blender.h>
#include "allegro/gfx.h"
#include "core/platform.h"
#include "gfx/bitmap.h"

#define AGS_OUTLINE_FONT_FIX (!AGS_PLATFORM_OS_WINDOWS)

#include "core/assetmanager.h"
#include "font/fonts.h"
#include "util/stream.h"

using namespace AGS::Common;

#define algetr32(c) getr32(c)
#define algetg32(c) getg32(c)
#define algetb32(c) getb32(c)
#define algeta32(c) geta32(c)

// project-specific implementation
extern bool ShouldAntiAliasText();

// ***** TTF RENDERER *****
void TTFFontRenderer::AdjustYCoordinateForFont(int *ycoord, int /*fontNumber*/)
{
  // TTF fonts already have space at the top, so try to remove the gap
  // TODO: adding -1 was here before (check the comment above),
  // but how universal is this "space at the top"?
  // Also, why is this function used only in one case of text rendering?
  // Review this after we upgrade the font library.
  ycoord[0]--;
}

void TTFFontRenderer::EnsureTextValidForFont(char * /*text*/, int /*fontNumber*/)
{
  // do nothing, TTF can handle all characters
}

int TTFFontRenderer::GetTextWidth(const char *text, int fontNumber)
{
    int w;
    TTF_SizeText(_fontData[fontNumber].Font , text, &w, nullptr);
    return w;
}

int TTFFontRenderer::GetTextHeight(const char * text, int fontNumber)
{
    int h;
    TTF_SizeText(_fontData[fontNumber].Font , text, nullptr, &h);
    return h;
}

void TTFFontRenderer::RenderText(const char *text, int fontNumber, BITMAP *destination, int x, int y, int colour)
{
    if (y > destination->cb)  // optimisation
        return;

    const int dest_depth = bitmap_color_depth(destination);
    SDL_Surface* glyph;
    SDL_Color sdlColor = {(Uint8) algetr32(colour),
                          (Uint8) algetg32(colour),
                          (Uint8) algetb32(colour),
                          (Uint8) algeta32(colour)};

    TTF_Font* font = _fontData[fontNumber].Font;
    if ((ShouldAntiAliasText()) && (dest_depth > 8))
        glyph = TTF_RenderText_Blended(font, text, sdlColor);
    else
        glyph = TTF_RenderText_Solid(font, text, sdlColor);

    if(!glyph) {
        const char *errormsg = TTF_GetError();
        printf("Error : %s", errormsg);
        return;
    }

    SDL_PixelFormatEnum pixelFormat = dest_depth == 32 ? SDL_PIXELFORMAT_ARGB8888 : SDL_PIXELFORMAT_INDEX8;
    SDL_Surface * surface = SDL_CreateRGBSurfaceWithFormat(0, glyph->w,glyph->h, dest_depth, pixelFormat);
    SDL_BlitSurface(glyph, nullptr, surface, nullptr);

    BITMAP *sourcebmp = wrap_bitmap_sdl_surface(surface, dest_depth);

    set_argb2argb_blender(255); // don't know what to do with 8bit yet

    // Y - 1 because it seems to get drawn down a bit, need to check if still the case
    draw_trans_sprite(destination, sourcebmp, x, y-1);

    SDL_FreeSurface(glyph);
    SDL_FreeSurface(surface);
}

bool TTFFontRenderer::LoadFromDisk(int fontNumber, int fontSize)
{
  return LoadFromDiskEx(fontNumber, fontSize, nullptr, nullptr);
}

bool TTFFontRenderer::IsBitmapFont()
{
    return false;
}

static int GetAlfontFlags(int load_mode)
{
  int flags = ALFONT_FLG_FORCE_RESIZE | ALFONT_FLG_SELECT_NOMINAL_SZ;
  // Compatibility: font ascender is always adjusted to the formal font's height;
  // EXCEPTION: not if it's a game made before AGS 3.4.1 with TTF anti-aliasing
  // (the reason is uncertain, but this is to emulate old engine's behavior).
  if (((load_mode & FFLG_ASCENDERFIXUP) != 0) &&
      !(ShouldAntiAliasText() && (loaded_game_file_version < kGameVersion_341)))
      flags |= ALFONT_FLG_ASCENDER_EQ_HEIGHT;
  return flags;
}

// Loads a TTF font of a certain size
static ALFONT_FONT *LoadTTF(const String &filename, int fontSize, int alfont_flags)
{
    std::unique_ptr<Stream> reader(AssetMgr->OpenAsset(filename));
    if (!reader)
        return nullptr;

    const size_t lenof = reader->GetLength();
    std::vector<char> buf; buf.resize(lenof);
    reader->Read(&buf.front(), lenof);
    reader.reset();

    ALFONT_FONT *alfptr = alfont_load_font_from_mem(&buf.front(), lenof);
    if (!alfptr)
        return nullptr;
    alfont_set_font_size_ex(alfptr, fontSize, alfont_flags);
    return alfptr;
}

// Fill the FontMetrics struct from the given ALFONT
static void FillMetrics(ALFONT_FONT *alfptr, FontMetrics *metrics)
{
    metrics->Height = alfont_get_font_height(alfptr);
    metrics->RealHeight = alfont_get_font_real_height(alfptr);
    metrics->CompatHeight = metrics->Height; // just set to default here
}

bool TTFFontRenderer::LoadFromDiskEx(int fontNumber, int fontSize,
    const FontRenderParams *params, FontMetrics *metrics)
{
    String filename = String::FromFormat("agsfnt%d.ttf", fontNumber);
    if (fontSize <= 0)
        fontSize = 8; // compatibility fix
    assert(params);
    FontRenderParams f_params = params ? *params : FontRenderParams();
    if (f_params.SizeMultiplier > 1)
        fontSize *= f_params.SizeMultiplier;


    // Load the font data into a memory buffer
    SDL_RWops* pFontMem = SDL_RWFromConstMem(membuffer, lenof);
    if(!pFontMem)
    {
        printf("Error when reading font from memory");
        // Some error occurred while trying to read the data, act accordingly to that
    }

    // Load the font from the memory buffer
    TTF_Font* pFont = TTF_OpenFontRW(pFontMem, 1, fontSize);
    if(!pFont) {
        printf("Error when loading font TTF_OpenFontRW");
    }

    ALFONT_FONT *alfptr = LoadTTF(filename, fontSize,
        GetAlfontFlags(f_params.LoadMode));
    if (!alfptr)
        return false;

    _fontData[fontNumber].AlFont = alfptr;
    _fontData[fontNumber].Params = f_params;
    if (metrics)
        FillMetrics(alfptr, metrics);
    return true;
}

const char *TTFFontRenderer::GetFontName(int fontNumber)
{
  return TTF_FontFaceFamilyName(_fontData[fontNumber].Font);
}

int TTFFontRenderer::GetFontHeight(int fontNumber)
{
  return TTF_FontHeight(_fontData[fontNumber].Font);
}

void TTFFontRenderer::GetFontMetrics(int fontNumber, FontMetrics *metrics)
{
    //FillMetrics(_fontData[fontNumber].AlFont, metrics);
}

void TTFFontRenderer::AdjustFontForAntiAlias(int fontNumber, bool /*aa_mode*/)
{
  if (loaded_game_file_version < kGameVersion_341)
  {
    TTF_Font *ttfptr = _fontData[fontNumber].Font;
    const FontRenderParams &params = _fontData[fontNumber].Params;
    int old_height = alfont_get_font_height(alfptr);
    alfont_set_font_size_ex(alfptr, old_height, GetAlfontFlags(params.LoadMode));
  }
}

void TTFFontRenderer::FreeMemory(int fontNumber)
{
   TTF_CloseFont(_fontData[fontNumber].Font);
  _fontData.erase(fontNumber);
}

bool TTFFontRenderer::MeasureFontOfPointSize(const String &filename, int size_pt, FontMetrics *metrics)
{
    ALFONT_FONT *alfptr = LoadTTF(filename, size_pt, ALFONT_FLG_FORCE_RESIZE | ALFONT_FLG_SELECT_NOMINAL_SZ);
    if (!alfptr)
        return false;
    FillMetrics(alfptr, metrics);
    alfont_destroy_font(alfptr);
    return true;
}

bool TTFFontRenderer::MeasureFontOfPixelHeight(const String &filename, int pixel_height, FontMetrics *metrics)
{
    ALFONT_FONT *alfptr = LoadTTF(filename, pixel_height, ALFONT_FLG_FORCE_RESIZE);
    if (!alfptr)
        return false;
    FillMetrics(alfptr, metrics);
    alfont_destroy_font(alfptr);
    return true;
}
