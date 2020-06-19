/**
 * @file
 *
 * @author OmniBlade
 * @author CCHyper
 *
 * @brief Classes for resizable arrays ("vectors").
 *
 * @copyright Remnant is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#pragma once

#ifndef VECTOR_H
#define VECTOR_H

#include "always.h"
#include "bitarray.h"
#include <captainslog.h>
#include <new>
#include <stdlib.h>
#include <string.h>

class NoInitClass;

template<typename T>
class VectorClass
{
public:
    VectorClass(int size = 0, const T *array = nullptr);
    VectorClass(const NoInitClass &noinit) {}
    VectorClass(const VectorClass<T> &that);
    virtual ~VectorClass();

    T &operator[](int index);
    const T &operator[](int index) const;

    virtual VectorClass<T> &operator=(VectorClass<T> const &that);
    virtual BOOL operator==(const VectorClass<T> &that) const;

    virtual BOOL Resize(int newsize, const T *array = nullptr);
    virtual void Clear();
    virtual int ID(const T *ptr);
    virtual int ID(const T &ptr);

    int Length() const { return m_VectorMax; }

protected:
    T *m_Vector;
    int m_VectorMax;
#ifndef CHRONOSHIFT_NO_BITFIELDS
    BOOL m_IsAllocated : 1;
#else
    bool m_IsAllocated;
#endif
};

template<typename T>
T &VectorClass<T>::operator[](int index)
{
    captainslog_assert(unsigned(index) < unsigned(m_VectorMax));
    return m_Vector[index];
}

template<typename T>
const T &VectorClass<T>::operator[](int index) const
{
    captainslog_assert(unsigned(index) < unsigned(m_VectorMax));
    return m_Vector[index];
}

template<typename T>
VectorClass<T>::VectorClass(int size, const T *array) : m_Vector(nullptr), m_VectorMax(size), m_IsAllocated(false)
{
    // Allocate the vector. The default constructor will be called for every object in this vector.
    if (size > 0) {
        if (array != nullptr) {
            m_Vector = new ((void *)array) T[size];
        } else {
            m_Vector = new T[size];
            m_IsAllocated = true;
        }
    }
}

template<typename T>
VectorClass<T>::~VectorClass()
{
    VectorClass<T>::Clear();
}

template<typename T>
VectorClass<T>::VectorClass(VectorClass<T> const &vector) : m_Vector(nullptr), m_VectorMax(0), m_IsAllocated(false)
{
    *this = vector;
}

template<typename T>
VectorClass<T> &VectorClass<T>::operator=(VectorClass<T> const &vector)
{
    if (this != &vector) {
        Clear();
        m_VectorMax = vector.Length();

        if (m_VectorMax > 0) {
            m_Vector = new T[m_VectorMax];

            if (m_Vector != nullptr) {
                m_IsAllocated = true;

                for (int index = 0; index < m_VectorMax; ++index) {
                    m_Vector[index] = vector[index];
                }
            }
        } else {
            m_Vector = nullptr;
            m_IsAllocated = false;
        }
    }

    return *this;
}

template<typename T>
BOOL VectorClass<T>::operator==(VectorClass<T> const &vector) const
{
    if (m_VectorMax == vector.Length()) {
        for (int index = 0; index < m_VectorMax; ++index) {
            if (m_Vector[index] != vector[index]) {
                return false;
            }
        }

        return true;
    }

    return false;
}

template<typename T>
inline int VectorClass<T>::ID(const T *ptr)
{
    return ((uintptr_t)ptr - (uintptr_t)m_Vector) / sizeof(T);
}

template<typename T>
int VectorClass<T>::ID(const T &object)
{
    for (int index = 0; index < m_VectorMax; ++index) {
        if ((*this)[index] == object) {
            return index;
        }
    }

    return -1;
}

template<typename T>
void VectorClass<T>::Clear()
{
    if (m_Vector != nullptr && m_IsAllocated) {
        delete[] m_Vector;
        m_Vector = nullptr;
    }

    m_IsAllocated = false;
    m_VectorMax = 0;
}

template<typename T>
BOOL VectorClass<T>::Resize(int newsize, const T *array)
{
    if (newsize != 0) {
        T *newptr;

        if (array == nullptr) {
            newptr = new T[newsize];
        } else {
            newptr = new ((void *)array) T[newsize];
        }

        if (newptr == nullptr) {
            return false;
        }

        if (m_Vector != nullptr) {
            int copy_count = (newsize < m_VectorMax) ? newsize : m_VectorMax;

            for (int i = 0; i < copy_count; i++) {
                newptr[i] = m_Vector[i];
            }

            if (m_IsAllocated) {
                delete[] m_Vector;
                m_Vector = nullptr;
            }
        }

        m_Vector = newptr;
        m_VectorMax = newsize;
        m_IsAllocated = (m_Vector && !array);

    } else {
        Clear();
    }

    return true;
}

template<typename T>
class DynamicVectorClass : public VectorClass<T>
{
protected:
#ifndef COMPILER_WATCOM
    // Looks like watcom doesn't like these declarations, newer compilers
    // need them for standards compliance related to template lookup.
    using VectorClass<T>::m_Vector;
    using VectorClass<T>::m_VectorMax;
    using VectorClass<T>::m_IsAllocated;
#endif

public:
    DynamicVectorClass(unsigned size = 0, const T *array = nullptr);

    BOOL operator==(const DynamicVectorClass &src) { return false; }
    BOOL operator!=(const DynamicVectorClass &src) { return true; }
    DynamicVectorClass<T> &operator=(DynamicVectorClass<T> const &rvalue);

    virtual BOOL Resize(int newsize, const T *array = nullptr) override;
    virtual void Clear() override;
    virtual int ID(const T *ptr) override { return VectorClass<T>::ID(ptr); };
    virtual int ID(const T &ptr) override;

    void Reset_Active() { m_ActiveCount = 0; }
    void Set_Active(int count) { m_ActiveCount = count; }
    int Count() const { return m_ActiveCount; }
    BOOL Empty() const { return !(m_ActiveCount > 0); }
    BOOL Add(const T &object);
    BOOL Add_Head(const T &object);
    const T &Fetch_Head() const { return (*this)[0]; }
    BOOL Insert(int index, const T &object);
    BOOL Delete(const T &object);
    BOOL Delete(int index);
    void Delete_All();
    int Set_Growth_Step(int step) { return m_GrowthStep = step; }
    int Growth_Step() { return m_GrowthStep; }
    T *Uninitialized_Add();

protected:
    int m_ActiveCount;
    int m_GrowthStep;
};

template<typename T>
void DynamicVectorClass<T>::Clear()
{
    m_ActiveCount = 0;
    VectorClass<T>::Clear();
};

template<typename T>
DynamicVectorClass<T> &DynamicVectorClass<T>::operator=(DynamicVectorClass<T> const &rvalue)
{
    VectorClass<T>::operator=(rvalue);
    m_ActiveCount = rvalue.m_ActiveCount;
    m_GrowthStep = rvalue.m_GrowthStep;

    return *this;
}

template<typename T>
DynamicVectorClass<T>::DynamicVectorClass(unsigned size, const T *array) : VectorClass<T>(size, array)
{
    m_GrowthStep = 10;
    m_ActiveCount = 0;
}

template<typename T>
BOOL DynamicVectorClass<T>::Resize(int newsize, const T *array)
{
    if (VectorClass<T>::Resize(newsize, array)) {
        if (m_VectorMax < m_ActiveCount) {
            m_ActiveCount = m_VectorMax;
        }

        return true;
    }

    return false;
}

template<typename T>
int DynamicVectorClass<T>::ID(const T &object)
{
    for (int index = 0; index < Count(); ++index) {
        if ((*this)[index] == object)
            return index;
    }

    return -1;
}

template<typename T>
BOOL DynamicVectorClass<T>::Add(const T &object)
{
    if (m_ActiveCount >= m_VectorMax) {
        if ((m_IsAllocated || !m_VectorMax) && m_GrowthStep > 0) {
            if (!Resize(m_VectorMax + m_GrowthStep)) {
                return false;
            }
        } else {
            return false;
        }
    }

    (*this)[m_ActiveCount++] = object;

    return true;
}

template<typename T>
BOOL DynamicVectorClass<T>::Add_Head(const T &object)
{
    if (m_ActiveCount >= m_VectorMax) {
        if ((m_IsAllocated || !m_VectorMax) && m_GrowthStep > 0) {
            if (!Resize(m_VectorMax + m_GrowthStep)) {
                return false;
            }
        } else {
            return false;
        }
    }

    if (m_ActiveCount) {
        memmove(&(*this)[1], &(*this)[0], m_ActiveCount * sizeof(T));
    }

    (*this)[0] = object;
    ++m_ActiveCount;

    return true;
}

template<typename T>
BOOL DynamicVectorClass<T>::Insert(int index, const T &object)
{
    if (index < 0 || index > m_ActiveCount) {
        return false;
    }

    if (m_ActiveCount >= m_VectorMax) {
        if ((m_IsAllocated || !m_VectorMax) && m_GrowthStep > 0) {
            if (!Resize(m_VectorMax + m_GrowthStep)) {
                return false;
            }
        } else {
            return false;
        }
    }

    if (index < m_ActiveCount) {
        memmove(&(*this)[index + 1], &(*this)[index], (m_ActiveCount - index) * sizeof(T));
    }

    (*this)[index] = object;
    ++m_ActiveCount;

    return true;
}

template<typename T>
BOOL DynamicVectorClass<T>::Delete(const T &object)
{
    int id = ID(object);

    if (id != -1) {
        return Delete(id);
    }

    return false;
}

template<typename T>
BOOL DynamicVectorClass<T>::Delete(int index)
{
    if (index < m_ActiveCount) {
        --m_ActiveCount;

        for (int i = index; i < m_ActiveCount; ++i) {
            (*this)[i] = (*this)[i + 1];
        }

        return true;
    }

    return false;
}

template<typename T>
void DynamicVectorClass<T>::Delete_All()
{
    int len = m_VectorMax;
    Clear();
    Resize(len);
}

template<typename T>
T *DynamicVectorClass<T>::Uninitialized_Add()
{
    if (m_ActiveCount >= m_VectorMax) {
        if (m_GrowthStep > 0) {
            if (!Resize(m_VectorMax + m_GrowthStep)) {
                return nullptr;
            }
        } else {
            return nullptr;
        }
    }

    return &((*this)[m_ActiveCount++]);
}

class BooleanVectorClass
{
public:
    BooleanVectorClass(unsigned size = 0, uint8_t *array = nullptr);
    BooleanVectorClass(BooleanVectorClass const &vector);

    BooleanVectorClass &operator=(BooleanVectorClass const &vector);
    BOOL operator==(BooleanVectorClass const &vector) const;

    void Init(unsigned size, uint8_t *array);
    void Init(unsigned size);
    int Length() { return m_BitCount; }
    void Reset();
    void Set();
    void Clear();
    int Resize(unsigned size);

    BOOL const &operator[](int index) const
    {
        if (m_LastIndex != index) {
            Fixup(index);
        }

        return m_Copy;
    };

    BOOL &operator[](int index)
    {
        if (m_LastIndex != index) {
            Fixup(index);
        }

        return m_Copy;
    };

    BOOL Is_True(int index) const
    {
        if (index == m_LastIndex) {
            return m_Copy;
        }

        return Get_Bit(&m_BitArray[0], index);
    };

    int First_False() const
    {
        if (m_LastIndex != -1) {
            Fixup(-1);
        }

        int retval = First_False_Bit(&m_BitArray[0], m_BitArray.Length());

        if (retval < m_BitCount) {
            return retval;
        }

        return -1;
    }

    int First_True() const
    {
        if (m_LastIndex != -1)
            Fixup(-1);

        int retval = First_True_Bit(&m_BitArray[0], m_BitArray.Length());

        if (retval < m_BitCount) {
            return retval;
        }

        return -1;
    }

    const VectorClass<uint8_t> &Get_Bit_Array() { return m_BitArray; }

protected:
    void Fixup(int index = -1) const;
    int m_BitCount;
    mutable BOOL m_Copy;
    mutable int m_LastIndex;

    VectorClass<uint8_t> m_BitArray;
};

template<typename T>
int Pointer_Vector_Add(T *ptr, VectorClass<T *> &vec)
{
    int id = 0;
    BOOL foundspot = false;

    for (int index = 0; index < vec.Length(); index++) {
        if (vec[index] == nullptr) {
            id = index;
            foundspot = true;
            break;
        }
    }

    if (!foundspot) {
        id = vec.Length();
        vec.Resize((vec.Length() + 1) * 2);

        for (int index = id; index < vec.Length(); index++) {
            vec[index] = nullptr;
        }
    }

    vec[id] = ptr;

    return id;
}

template<typename T>
BOOL Pointer_Vector_Remove(const T *ptr, VectorClass<T *> &vec)
{
    int id = vec.ID((T *)ptr);

    if (id != -1) {
        vec[id] = nullptr;

        return true;
    }

    return false;
}

#endif