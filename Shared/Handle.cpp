//RegTlb.exe a program to manage Type Library registration
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

#include "pch.h"
#include "Handle.h"

namespace w32
{
    //Initialize an empty handle
    CHandle::CHandle() {}

    //Initialize a CHandle and take ownership
    CHandle::CHandle(HANDLE handle) {
        m_handle = handle;
    }

    //Initialize copy a CHandle
    CHandle::CHandle(CHandle const& other) {
        Duplicate(other.m_handle);
    }

    //Destroy the CHandle
    CHandle::~CHandle()
    {
        CloseHandle();
    }

    //Close the handle if it was valid
    void CHandle::CloseHandle() {
        if (IsValid()) {
            ::CloseHandle(m_handle);
            m_handle = INVALID_HANDLE_VALUE;
        }
    }

    //Is the internal handle a valid one?
    bool CHandle::IsValid() {
        return (m_handle != NULL && m_handle != INVALID_HANDLE_VALUE);
    }

    //assign a handle and take ownership
    void CHandle::operator = (HANDLE handle) {
        CloseHandle();
        m_handle = handle;
    }

    //assign a handle and take ownership
    void CHandle::operator = (CHandle const& other) {
        if (this != &other) {
            Duplicate(other.m_handle);
        }
    }

    //cast the class to a handle value for interaction with the win32 api
    CHandle::operator HANDLE () {
        return m_handle;
    }

    //Assign a HANDLE and assume ownership
    void CHandle::Attach(HANDLE handle)
    {
        CloseHandle();
        m_handle = handle;
    }

    //Relinquish a HANDLE without closing
    HANDLE CHandle::Detach()
    {
        HANDLE tempHandle = m_handle;
        m_handle = INVALID_HANDLE_VALUE;
        return m_handle;
    }

    //Duplicate the handle and take ownership of the copy.
    //The original handle remains owned by its current owner.
    void CHandle::Duplicate(HANDLE handle)
    {
        CloseHandle();
        if (!DuplicateHandle(
            GetCurrentProcess(),
            handle,
            GetCurrentProcess(),
            &m_handle,
            0,
            FALSE,
            DUPLICATE_SAME_ACCESS))
            throw ExWin32Error("CHandle::CHandle(CHandle const& other)");
    }

}