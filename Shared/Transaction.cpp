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
#include "Transaction.h"

//required for transaction support
//If this source file is compiled in the project, the linker directive
//will automatically make it pull in the required dependency
#pragma comment(lib, "KtmW32")

namespace w32
{
    CTransaction::CTransaction() {}
    CTransaction::CTransaction(HANDLE handle) : CHandle(handle) {}
    CTransaction::~CTransaction() {}

    void CTransaction::Commit() {
        if (!CommitTransaction(m_handle))
            throw ExWin32Error();
    }

    void CTransaction::RollBack() {
        if (!RollbackTransaction(m_handle))
            throw ExWin32Error();
    }

    //
    // This function create a transaction with default settings, which is what is appropriate in
    // most cases.
    //
    void CTransaction::Create(LPWSTR Description) {
        if (IsValid())
            CloseHandle();

        m_handle = CreateTransaction(
            NULL,                   //Using default security.
            NULL,                   //Reserved
            0,                      //Create options, only relevant for inheriting handles
            0,                      //Reserved
            0,                      //Reserved
            0,                      //Timeout
            Description);           //User readable description

        if (m_handle == INVALID_HANDLE_VALUE)
            throw ExWin32Error();
    }

}