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

int CCDynamicArray::Dispose(const char *address, bool force)
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

    delete[] (address - MemHeaderSz);
    return 1;
}

int CCDynamicArray::Serialize(const char *address, char *buffer, int bufsize)
{
    const Header &hdr = GetHeader(address);
    int sizeToWrite = hdr.TotalSize + FileHeaderSz;
    if (sizeToWrite > bufsize)
    {
        // buffer not big enough, ask for a bigger one
        return -sizeToWrite;
    }
    MemoryStream mems(reinterpret_cast<uint8_t*>(buffer), bufsize, kStream_Write);
    mems.WriteInt32(hdr.ElemCount);
    mems.WriteInt32(hdr.TotalSize);
    mems.Write(address, hdr.TotalSize); // elements
    return static_cast<int32_t>(mems.GetPosition());
}

void CCDynamicArray::Unserialize(int index, const char *serializedData, int dataSize)
{
    char *new_arr = new char[(dataSize - FileHeaderSz) + MemHeaderSz];
    MemoryStream mems(reinterpret_cast<const uint8_t*>(serializedData), dataSize);
    Header &hdr = reinterpret_cast<Header&>(*new_arr);
    hdr.ElemCount = mems.ReadInt32();
    hdr.TotalSize = mems.ReadInt32();
    memcpy(new_arr + MemHeaderSz, serializedData + FileHeaderSz, dataSize - FileHeaderSz);
    ccRegisterUnserializedObject(index, &new_arr[MemHeaderSz], this);
}

DynObjectRef CCDynamicArray::Create(int numElements, int elementSize, bool isManagedType)
{
    char *new_arr = new char[numElements * elementSize + MemHeaderSz];
    memset(new_arr, 0, numElements * elementSize + MemHeaderSz);
    Header &hdr = reinterpret_cast<Header&>(*new_arr);
    hdr.ElemCount = numElements | (ARRAY_MANAGED_TYPE_FLAG * isManagedType);
    hdr.TotalSize = elementSize * numElements;
    void *obj_ptr = &new_arr[MemHeaderSz];
    // TODO: investigate if it's possible to register real object ptr directly
    int32_t handle = ccRegisterManagedObject(obj_ptr, this);
    if (handle == 0)
    {
        delete[] new_arr;
        return DynObjectRef(0, nullptr);
    }
    return DynObjectRef(handle, obj_ptr);
}


const char* CCDynamicArray::GetFieldPtr(const char *address, intptr_t offset)
{
    return address + offset;
}

void CCDynamicArray::Read(const char *address, intptr_t offset, void *dest, int size)
{
    memcpy(dest, address + offset, size);
}

uint8_t CCDynamicArray::ReadInt8(const char *address, intptr_t offset)
{
    return *(uint8_t*)(address + offset);
}

int16_t CCDynamicArray::ReadInt16(const char *address, intptr_t offset)
{
    return *(int16_t*)(address + offset);
}

int32_t CCDynamicArray::ReadInt32(const char *address, intptr_t offset)
{
    return *(int32_t*)(address + offset);
}

float CCDynamicArray::ReadFloat(const char *address, intptr_t offset)
{
    return *(float*)(address + offset);
}

void CCDynamicArray::Write(const char *address, intptr_t offset, void *src, int size)
{
    memcpy((void*)(address + offset), src, size);
}

void CCDynamicArray::WriteInt8(const char *address, intptr_t offset, uint8_t val)
{
    *(uint8_t*)(address + offset) = val;
}

void CCDynamicArray::WriteInt16(const char *address, intptr_t offset, int16_t val)
{
    *(int16_t*)(address + offset) = val;
}

void CCDynamicArray::WriteInt32(const char *address, intptr_t offset, int32_t val)
{
    *(int32_t*)(address + offset) = val;
}

void CCDynamicArray::WriteFloat(const char *address, intptr_t offset, float val)
{
    *(float*)(address + offset) = val;
}

CCDynamicArray globalDynamicArray;


DynObjectRef DynamicArrayHelpers::CreateStringArray(const std::vector<const char*> items)
{
    // NOTE: we need element size of "handle" for array of managed pointers
    DynObjectRef arr = globalDynamicArray.Create(items.size(), sizeof(int32_t), true);
    if (!arr.second)
        return arr;
    // Create script strings and put handles into array
    int32_t *slots = static_cast<int32_t*>(arr.second);
    for (auto s : items)
    {
        DynObjectRef str = stringClassImpl->CreateString(s);
        // We must add reference count, because the string is going to be saved
        // within another object (array), not returned to script directly
        ccAddObjectReference(str.first);
        *(slots++) = str.first;
    }
    return arr;
}
