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
#include "StringHelper.h"
#include <algorithm>
#include <codecvt>
#include "Exception.h"

using namespace std;

namespace w32
{

    //convert a wstring to a string
    string WStringToString(wstring const& ws) {
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), -1, NULL, 0, NULL, NULL);
        std::string narrow(size_needed, 0);
        WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), -1, &narrow[0], size_needed, NULL, NULL);
        return narrow;
    }

    //convert a string to wstring
    wstring StringToWString(string const& s) {
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);
        std::wstring wide(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, &wide[0], size_needed);
        return wide;
    }

    /// <summary>
    /// Convert a GUID to a string for use in the registry
    /// with added curly braces.
    /// </summary>
    std::wstring WStringFromGUID(const GUID& guid, bool braces)
    {
        const int numChars = 128;
        wchar_t buffer[numChars];
        memset(buffer, 0, sizeof(buffer));
        StringFromGUID2(guid, buffer, numChars);
        return buffer;
    }

    /// <summary>
    // Convert a string to GUID. The string can have curly braces or not.
    /// </summary>
    void GUIDFromWString(const std::wstring& strGuid, GUID& guid)
    {
        HRESULT hRes = S_OK;

        //IIDFromString takes the string with curly braces
        if (strGuid.find(L'{') == 0) {
            hRes = IIDFromString(strGuid.c_str(), &guid);

        }
        //Add the curly braces. Alternatively we could call Uuid­From­StringW but we'd
        //need to include rpc.h and link to Rpcrt4.lib
        else
        {
            std::wstring modifiedGuidString = L"{" + strGuid + L"}";
            hRes = IIDFromString(strGuid.c_str(), &guid);
        }
        if (FAILED(hRes))
            throw ExHResult(hRes);
    }

    //Get the human readable message for a windows error code
    std::wstring GetMessageForError(int code)
    {
        LPWSTR errorText = NULL;
        std::wstring error;
        FormatMessageW(
            FORMAT_MESSAGE_FROM_SYSTEM
            | FORMAT_MESSAGE_ALLOCATE_BUFFER
            | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            code,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPWSTR)&errorText,
            0,
            NULL);

        if (NULL != errorText)
        {
            error = std::wstring(errorText);
            LocalFree(errorText);
        }

        return error;
    }

    //convert to lowercase
    void ToWLowerInPlace(wstring& ws) {
        transform(ws.begin(), ws.end(), ws.begin(), towlower);
    }

    //convert to uppercase
    void ToWUpperInPlace(wstring& ws) {
        transform(ws.begin(), ws.end(), ws.begin(), towupper);
    }

}