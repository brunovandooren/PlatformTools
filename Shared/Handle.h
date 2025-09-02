
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

namespace w32 {
    /// <summary>
    /// This handle wraps Windows HANDLE values for lifecycle management
    /// </summary>
    class CHandle
    {
    protected:
        HANDLE m_handle = NULL;
    public:
        //Create an invalid handle
        CHandle();

        //Initialize a handle object and assume ownership of the handle
        CHandle(HANDLE handle);

        //duplicate the handle
        CHandle(CHandle const& handle);
        virtual ~CHandle();

        //Close the internal handle
        virtual void CloseHandle();

        //Is the internal HANDLE a valid one?
        bool IsValid();

        //Assign a HANDLE and assume ownership
        void operator = (HANDLE handle);

        //Assign a CHandle and duplicate it
        void operator = (CHandle const& handle);

        //Assign a HANDLE and assume ownership
        void Attach(HANDLE handle);

        //Relinquish a HANDLE without closing
        HANDLE Detach();

        //Duplicate the handle and take ownership of the copy.
        //The original handle remains owned by its current owner.
        void Duplicate(HANDLE handle);

        //cast to a HANDLE for use with Windows APIs
        operator HANDLE ();

    };

}