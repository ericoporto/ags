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
#include "cc_dynamicarray.h"
#include <string.h>
#include "ac/dynobj/dynobj_manager.h"
#include "util/memorystream.h"

using namespace AGS::Common;

const char *CCDynamicArray::TypeName = "CCDynamicArray";

// return the type name of the object
const char *CCDynamicArray::GetType()
{
    return TypeName;
}

CCDynamicArray::~CCDynamicArray()
{
    delete[] _data;
}

int CCDynamicArray::Dispose(void *address, bool force)
{
    // If it's an array of managed objects, release their ref counts;
    // except if this array is forcefully removed from the managed pool,
    // in which case just ignore these.
    if (!force)
    {
        const Header &hdr = GetHeader(address);
        bool is_managed = (hdr.ElemCount & ARRAY_MANAGED_TYPE_FLAG) != 0;
        const uint32_t el_count = hdr.ElemCount & (~ARRAY_MANAGED_TYPE_FLAG);

        if (is_managed)
        { // Dynamic array of managed pointers: subref them directly
            const uint32_t *handles = reinterpret_cast<const uint32_t*>(address);
            for (uint32_t i = 0; i < el_count; ++i)
            {
                if (handles[i] > 0)
                    ccReleaseObjectReference(handles[i]);
            }
        }
    }

    delete this;
    return 1;
}

size_t CCDynamicArray::CalcSerializeSize(void *address)
{
    const Header &hdr = GetHeader(address);
    return hdr.TotalSize + FileHeaderSz;
}

void CCDynamicArray::Serialize(void *address, AGS::Common::Stream *out)
{
    const Header &hdr = GetHeader(address);
    out->WriteInt32(hdr.ElemCount);
    out->WriteInt32(hdr.TotalSize);
    out->Write(address, hdr.TotalSize); // elements
}

void CCDynamicArray::Unserialize(int index, Stream *in, size_t data_sz)
{
    char *new_arr = new char[(data_sz - FileHeaderSz)];
    Header &hdr = reinterpret_cast<Header&>(*new_arr);
    hdr.ElemCount = in->ReadInt32();
    hdr.TotalSize = in->ReadInt32();
    in->Read(new_arr, data_sz - FileHeaderSz);
    ccRegisterUnserializedObject(index, &new_arr, this);
}

DynObjectRef CCDynamicArray::Create(int numElements, int elementSize, bool isManagedType)
{
    CCDynamicArray *arr_obj = new CCDynamicArray();
    uint8_t *new_arr = new uint8_t[numElements * elementSize];
    memset(new_arr, 0, numElements * elementSize);
    arr_obj->_data = new_arr;
    Header &hdr = arr_obj->_hdr;
    hdr.ElemCount = numElements | (ARRAY_MANAGED_TYPE_FLAG * isManagedType);
    hdr.TotalSize = elementSize * numElements;
    void *obj_ptr = new_arr;
    // TODO: investigate if it's possible to register real object ptr directly
    int32_t handle = ccRegisterManagedObject(obj_ptr, arr_obj);
    if (handle == 0)
    {
        delete[] new_arr;
        return DynObjectRef();
    }
    return DynObjectRef(handle, obj_ptr, arr_obj);
}


DynObjectRef DynamicArrayHelpers::CreateStringArray(const std::vector<const char*> items)
{
    // NOTE: we need element size of "handle" for array of managed pointers
    DynObjectRef arr = CCDynamicArray::Create(items.size(), sizeof(int32_t), true);
    if (!arr.Obj)
        return DynObjectRef();
    // Create script strings and put handles into array
    int32_t *slots = static_cast<int32_t*>(arr.Obj);
    for (auto s : items)
    {
        DynObjectRef str = stringClassImpl->CreateString(s);
        // We must add reference count, because the string is going to be saved
        // within another object (array), not returned to script directly
        ccAddObjectReference(str.Handle);
        *(slots++) = str.Handle;
    }
    return arr;
}
