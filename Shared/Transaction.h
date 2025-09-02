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

#pragma once

#include <winbase.h>
#include "Handle.h"

namespace w32
{
    /// <summary>
    /// The class wraps a win32 transaction as a CHandle.
    /// </summary>
    class CTransaction : public CHandle
    {
    public:
        CTransaction();
        CTransaction(HANDLE handle);
        ~CTransaction();

        void Create(LPWSTR Description = NULL);
        void Commit();
        void RollBack();
    };

}


