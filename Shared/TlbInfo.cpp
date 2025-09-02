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
#include "TlbInfo.h"
#include <iostream>
#include "StringHelper.h"

using namespace std;

namespace w32
{

    void CTlbInfo::PrintTlbInfo()
    {
        for (GUID coc : CoClasses)
        {
            //CCOMClass comClass(hive, coc, transaction);
            //classes are not linked against a specific version of a typelib
            //comClass.SetTlbInfo(Guid);
            wcout << L"CoClass: " << WStringFromGUID(coc) << endl;
        }

        for (GUID itf : Interfaces)
        {
            //CCOMInterface comItf(hive, itf, transaction);
            //comItf.SetTlbInfo(Guid, GetVersionString());
            wcout << L"Interface: " << WStringFromGUID(itf) << endl;
        }

        for (GUID dispItf : DispInterfaces)
        {
            //for this purpose, there is no difference between regular interfaces
            //and dispatch interfaces in terms of registration
            //CCOMInterface comItf(hive, dispItf, transaction);
            //comItf.SetTlbInfo(Guid, GetVersionString());
            wcout << L"DispInterfaces: " << WStringFromGUID(dispItf) << endl;
        }
    }
}