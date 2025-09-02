//Copyright(C) 2024 Bruno van Dooren
//
//This program is free software : you can redistribute it and /or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.If not, see < https://www.gnu.org/licenses/>

#pragma once

#include "pch.h"
#include "Exception.h"

namespace w32
{
    /// <summary>
    /// small boilerplate class for dynamically managing an array
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template<class T>
    class CArray {
        T* m_ptr = NULL;
        size_t m_elements = 0;
    public:
        CArray(size_t elements) : m_elements(elements) {
            m_ptr = new T[elements];
            Zero();
        }

        ~CArray() {
            delete[] m_ptr;
        }

        operator T* () {
            return m_ptr;
        }

        operator void* () {
            return static_cast<void*> (m_ptr);
        }

        void Zero() {
            memset(m_ptr, 0, m_elements * sizeof(T));
        }
    };

}