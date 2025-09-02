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

#include <WinBase.h>
#include <string>

namespace w32
{

    /// <summary>
    /// Get the minimum required size for the string + termination
    /// </summary>
    /// <param name="charStr">input string</param>
    /// <returns>required buffer size</returns>
    inline size_t GetReqBufSize(PCHAR charStr) {
        return strlen(charStr) + 1;
    }

    /// <summary>
    /// Get the minimum required size for the string + termination
    /// </summary>
    /// <param name="charStr">input string</param>
    /// <returns>required buffer size</returns>
    inline size_t GetReqBufSize(PWCHAR charStr) {
        return (lstrlen(charStr) + 1) * sizeof(WCHAR);
    }

    /// <summary>
    /// Get the minimum required size for the string + termination
    /// </summary>
    /// <param name="charStr">input string</param>
    /// <returns>required buffer size</returns>
    inline size_t GetReqBufSize(std::string const& str) {
        return str.length() + 1;
    }

    /// <summary>
    /// Get the minimum required size for the string + termination
    /// </summary>
    /// <param name="charStr">input string</param>
    /// <returns>required buffer size</returns>
    inline size_t GetReqBufSize(std::wstring const& str) {
        return (str.length() + 1) * sizeof(WCHAR);
    }

    //Convert a wstring to a string
    std::string WStringToString(std::wstring const& ws);

    //Convert a string to a wstring
    std::wstring StringToWString(std::string const& s);

    //Convert a GUID to string
    std::wstring WStringFromGUID(const GUID& guid, bool braces = true);

    //Parse a GUID from a string
    void GUIDFromWString(const std::wstring& strGuid, GUID& guid);

    //Get the canonical message from a Windows error code
    std::wstring GetMessageForError(int code);

    //convert a wstring to all lowercase
    void ToWLowerInPlace(std::wstring& ws);

    //convert a wstring to all uppercase
    void ToWUpperInPlace(std::wstring& ws);

}