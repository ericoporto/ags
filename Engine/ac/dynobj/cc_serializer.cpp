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
#include <string.h>
#include "ac/dynobj/cc_serializer.h"
#include "ac/dynobj/all_dynamicclasses.h"
#include "ac/dynobj/all_scriptclasses.h"
#include "ac/dynobj/dynobj_manager.h"
#include "ac/dynobj/cc_dynamicarray.h"
#include "ac/dynobj/scriptuserobject.h"
#include "ac/dynobj/scriptcamera.h"
#include "ac/dynobj/scriptcontainers.h"
#include "ac/dynobj/scriptfile.h"
#include "ac/dynobj/scriptviewport.h"
#include "ac/game.h"
#include "debug/debug_log.h"
#include "plugin/agsplugin.h"
#include "plugin/pluginobjectreader.h"
#include "util/memorystream.h"

using namespace AGS::Common;

extern CCGUIObject ccDynamicGUIObject;
extern CCCharacter ccDynamicCharacter;
extern CCHotspot   ccDynamicHotspot;
extern CCRegion    ccDynamicRegion;
extern CCInventory ccDynamicInv;
extern CCGUI       ccDynamicGUI;
extern CCObject    ccDynamicObject;
extern CCDialog    ccDynamicDialog;
extern ScriptDrawingSurface* dialogOptionsRenderingSurface;
extern ScriptDialogOptionsRendering ccDialogOptionsRendering;
extern PluginObjectReader pluginReaders[MAX_PLUGIN_OBJECT_READERS];
extern int numPluginReaders;

// *** De-serialization of script objects

void AGSDeSerializer::Unserialize(int index, const char *objectType, const char *serializedData, int dataSize) {

    if (dataSize < 0)
    {
        quitprintf("Unserialise: invalid data size (%d) for object type '%s'", dataSize, objectType);
        return; // TODO: don't quit, return error
    }
    // Note that while our builtin classes may accept Stream object,
    // classes registered by plugin cannot, because streams are not (yet)
    // part of the plugin API.
    size_t data_sz = static_cast<size_t>(dataSize);
    MemoryStream mems(reinterpret_cast<const uint8_t*>(serializedData), dataSize);

    // TODO: consider this: there are object types that are part of the
    // script's foundation, because they are created by the bytecode ops:
    // such as DynamicArray and UserObject. *Maybe* these should be moved
    // to certain "base serializer" class which guarantees their restoration.
    //
    // TODO: should we support older save versions here (DynArray, UserObj)?
    // might have to use older class names to distinguish save formats
    if (strcmp(objectType, CCDynamicArray::TypeName) == 0) {
        CCDynamicArray *arr = new CCDynamicArray();
        arr->Unserialize(index, &mems, data_sz);
    }
    else if (strcmp(objectType, ScriptUserObject::TypeName) == 0) {
        ScriptUserObject *suo = new ScriptUserObject();
        suo->Unserialize(index, &mems, data_sz);
    }
    else if (strcmp(objectType, "GUIObject") == 0) {
        ccDynamicGUIObject.Unserialize(index, &mems, data_sz);
    }
    else if (strcmp(objectType, "Character") == 0) {
        ccDynamicCharacter.Unserialize(index, &mems, data_sz);
    }
    else if (strcmp(objectType, "Hotspot") == 0) {
        ccDynamicHotspot.Unserialize(index, &mems, data_sz);
    }
    else if (strcmp(objectType, "Region") == 0) {
        ccDynamicRegion.Unserialize(index, &mems, data_sz);
    }
    else if (strcmp(objectType, "Inventory") == 0) {
        ccDynamicInv.Unserialize(index, &mems, data_sz);
    }
    else if (strcmp(objectType, "Dialog") == 0) {
        ccDynamicDialog.Unserialize(index, &mems, data_sz);
    }
    else if (strcmp(objectType, "GUI") == 0) {
        ccDynamicGUI.Unserialize(index, &mems, data_sz);
    }
    else if (strcmp(objectType, "Object") == 0) {
        ccDynamicObject.Unserialize(index, &mems, data_sz);
    }
    else if (strcmp(objectType, "String") == 0) {
        ScriptString *scf = new ScriptString();
        scf->Unserialize(index, &mems, data_sz);
    }
    else if (strcmp(objectType, "File") == 0) {
        // files cannot be restored properly -- so just recreate
        // the object; attempting any operations on it will fail
        sc_File *scf = new sc_File();
        ccRegisterUnserializedObject(index, scf, scf);
    }
    else if (strcmp(objectType, "Overlay") == 0) {
        ScriptOverlay *scf = new ScriptOverlay();
        scf->Unserialize(index, &mems, data_sz);
    }
    else if (strcmp(objectType, "DateTime") == 0) {
        ScriptDateTime *scf = new ScriptDateTime();
        scf->Unserialize(index, &mems, data_sz);
    }
    else if (strcmp(objectType, "ViewFrame") == 0) {
        ScriptViewFrame *scf = new ScriptViewFrame();
        scf->Unserialize(index, &mems, data_sz);
    }
    else if (strcmp(objectType, "DynamicSprite") == 0) {
        ScriptDynamicSprite *scf = new ScriptDynamicSprite();
        scf->Unserialize(index, &mems, data_sz);
    }
    else if (strcmp(objectType, "DrawingSurface") == 0) {
        ScriptDrawingSurface *sds = new ScriptDrawingSurface();
        sds->Unserialize(index, &mems, data_sz);

        if (sds->isLinkedBitmapOnly)
        {
            dialogOptionsRenderingSurface = sds;
        }
    }
    else if (strcmp(objectType, "DialogOptionsRendering") == 0)
    {
        ccDialogOptionsRendering.Unserialize(index, &mems, data_sz);
    }
    else if (strcmp(objectType, "StringDictionary") == 0)
    {
        Dict_Unserialize(index, &mems, data_sz);
    }
    else if (strcmp(objectType, "StringSet") == 0)
    {
        Set_Unserialize(index, &mems, data_sz);
    }
    else if (strcmp(objectType, "Viewport2") == 0)
    {
        Viewport_Unserialize(index, &mems, data_sz);
    }
    else if (strcmp(objectType, "Camera2") == 0)
    {
        Camera_Unserialize(index, &mems, data_sz);
    }
    else if (!unserialize_audio_script_object(index, objectType, &mems, data_sz))
    {
        // check if the type is read by a plugin
        for (int ii = 0; ii < numPluginReaders; ii++) {
            if (strcmp(objectType, pluginReaders[ii].type) == 0) {
                pluginReaders[ii].reader->Unserialize(index, serializedData, dataSize);
                return;
            }
        }
        quitprintf("Unserialise: unknown object type: '%s'", objectType);
    }
}

AGSDeSerializer ccUnserializer;
