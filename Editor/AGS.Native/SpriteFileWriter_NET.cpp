//=============================================================================
//
// Adventure Game Studio (AGS)
//
// Copyright (C) 1999-2011 Chris Jones and 2011-2025 various contributors
// The full list of copyright holders can be found in the Copyright.txt
// file, which is part of this source code distribution.
//
// The AGS source code is provided under the Artistic License 2.0.
// A copy of this license can be found in the file License.txt and at
// https://opensource.org/license/artistic-2-0/
//
//=============================================================================
#include "SpriteFileWriter_NET.h"
#include <allegro.h>
#include "NativeUtils.h"
#include "gfx/bitmap.h"
#include "util/file.h"

using AGSBitmap = AGS::Common::Bitmap;
using AGSString = AGS::Common::String;
using AGSStream = AGS::Common::Stream;

extern AGSBitmap *CreateBlockFromBitmap(System::Drawing::Bitmap ^bmp, RGB *imgpal, int *srcPalLen,
    bool fixColourDepth, bool importAlpha, bool keepTransparency, int *originalColDepth);
extern AGSBitmap *CreateNativeBitmap(System::Drawing::Bitmap ^bmp, int spriteImportMethod,
    int transColour, bool remapColours,
    bool useRoomBackgroundColours, bool alphaChannel, int *flags);

namespace AGS
{
namespace Native
{

SpriteFileWriter::SpriteFileWriter(System::String ^filename)
{
    AGSString fn = TextHelper::ConvertUTF8(filename);
    std::unique_ptr<AGSStream> out(AGS::Common::File::CreateFile(fn));
    _nativeWriter = new AGS::Common::SpriteFileWriter(std::move(out));
}

SpriteFileWriter::!SpriteFileWriter()
{
    delete _nativeWriter;
}

void SpriteFileWriter::Begin(int store_flags, AGS::Types::SpriteCompression compress)
{
    _nativeWriter->Begin(store_flags, (AGS::Common::SpriteCompression)compress);
}

void SpriteFileWriter::WriteBitmap(System::Drawing::Bitmap ^image)
{
    RGB imgPalBuf[256];
    int importedColourDepth;
    std::unique_ptr<AGSBitmap> native_bmp(CreateBlockFromBitmap(image, imgPalBuf, nullptr, true, true /* FIXME */, true, &importedColourDepth));
    _nativeWriter->WriteBitmap(native_bmp.get());
}

void SpriteFileWriter::WriteBitmap(System::Drawing::Bitmap ^image, AGS::Types::SpriteImportTransparency transparency,
    int transColour, bool remapColours, bool useRoomBackgroundColours, bool alphaChannel)
{
    std::unique_ptr<AGSBitmap> native_bmp(CreateNativeBitmap(image, (int)transparency, transColour,
        remapColours, useRoomBackgroundColours, alphaChannel, nullptr));
    _nativeWriter->WriteBitmap(native_bmp.get());
}

void SpriteFileWriter::WriteNativeBitmap(NativeBitmap ^bitmap)
{
    _nativeWriter->WriteBitmap(bitmap->GetNativePtr());
}

void SpriteFileWriter::WriteRawData(RawSpriteData^ data)
{
    _nativeWriter->WriteRawData(*data->GetHeader(), &data->GetData()->front(), data->GetData()->size());
}

void SpriteFileWriter::WriteEmptySlot()
{
    _nativeWriter->WriteEmptySlot();
}

void SpriteFileWriter::End()
{
    _nativeWriter->Finalize();
}

} // namespace Native
} // namespace AGS
