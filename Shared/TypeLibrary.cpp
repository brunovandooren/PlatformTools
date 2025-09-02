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
#include "TypeLibrary.h"
#include "Transaction.h"
#include "HKey.h"
#include <iostream>
#include <filesystem>

using namespace std;

namespace w32
{
    std::wstring CTypeLibrary::GetVersionString()
    {
        return MajorVersion + L"" + MinorVersion;
    }

    CTypeLibrary::CTypeLibrary(std::wstring path) :
        CTlbInfo()
    {
        //ensure that the path is absolute.
        //LoadTypeLibEx accepts relative paths but RegisterTypeLib and others require
        //absolute paths.
        m_path = filesystem::absolute(path).wstring();

        HRESULT hRes = LoadTypeLibEx(path.c_str(), REGKIND_NONE, &m_typeLib);
        if (FAILED(hRes)) {
            throw ExHResult(hRes, L"Cannot open library ");
        }

        //Get TLB guid and version.
        TLIBATTR* tlbAttr = NULL;
        hRes = m_typeLib->GetLibAttr(&tlbAttr);
        if (FAILED(hRes)) {
            throw ExHResult(hRes, L"Cannot get typelib attributes ");
        }

        Guid = tlbAttr->guid;
        MajorVersion = tlbAttr->wMajorVerNum;
        MinorVersion = tlbAttr->wMinorVerNum;
        LocaleID = tlbAttr->lcid;
        SysKind = tlbAttr->syskind;
        m_typeLib->ReleaseTLibAttr(tlbAttr);

        //load the type definitions from the tlb file.
        UINT numTypeInfos = m_typeLib->GetTypeInfoCount();

        for (UINT i = 0; i < numTypeInfos; ++i) {
            CComPtr<ITypeInfo> itypeInfo;
            hRes = m_typeLib->GetTypeInfo(i, &itypeInfo);
            if (FAILED(hRes)) {
                throw ExHResult(hRes, L"Cannot get GetTypeInfo");
            }

            TYPEATTR* typeAttr = NULL;
            HRESULT hr = itypeInfo->GetTypeAttr(&typeAttr);
            if (FAILED(hRes)) {
                throw ExHResult(hRes, L"Cannot get GetTypeInfo Attributes");
            }

            //We only need to create entries for the COM interfaces
            //Any other types in the TLB are for internal use or for
            //method parameters or similar, and are not exposed in the registry
            //so we can ignore them.
            if (typeAttr->typekind == TKIND_INTERFACE) {
                Interfaces.push_back(typeAttr->guid);
            }
            else if (typeAttr->typekind == TKIND_DISPATCH) {
                DispInterfaces.push_back(typeAttr->guid);
            }
            else if (typeAttr->typekind == TKIND_COCLASS) {
                CoClasses.push_back(typeAttr->guid);
            }
            itypeInfo->ReleaseTypeAttr(typeAttr);
        }
    }

    //Register the current type library in the registry
    void CTypeLibrary::Register(bool perUser)
    {
        HRESULT hRes = S_OK;
        if (perUser) {
            hRes = RegisterTypeLibForUser(m_typeLib, const_cast<OLECHAR*>(m_path.c_str()), NULL);
        }
        else {
            hRes = RegisterTypeLib(m_typeLib, const_cast<OLECHAR*>(m_path.c_str()), NULL);
        }
        if (FAILED(hRes)) {
            throw ExHResult(hRes, L"Cannot register type library");
        }

    }

    //unregister the type library from the registry, based on the identifying information
    //inside the loaded tlb
    void CTypeLibrary::UnRegister(bool perUser)
    {
        UnRegister(perUser, Guid, MajorVersion, MinorVersion, LocaleID, SysKind);
    }

    //unregister the type library from the registry
    void CTypeLibrary::UnRegister(bool perUser, const GUID& guid, WORD major, WORD minor, LCID locale, SYSKIND syskind)
    {
        HRESULT hRes = S_OK;
        if (perUser) {
            hRes = UnRegisterTypeLibForUser(guid, major, minor, locale, syskind);
        }
        else {
            hRes = UnRegisterTypeLib(guid, major, minor, locale, syskind);
        }
        if (FAILED(hRes)) {
            throw ExHResult(hRes, L"Cannot unregister type library");
        }
    }

    //is a library with this guid registered
    bool CTypeLibrary::Exists(const GUID& guid, bool perUser)
    {
        HKEY hive = perUser ? HKEY_CURRENT_USER : HKEY_LOCAL_MACHINE;
        std::wstring regPath = L"Software\\Classes\\TypeLib\\";
        regPath += WStringFromGUID(guid);

        return CHKey::Exists(hive, regPath);
    }
}