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
#include "ConsoleHelper.h"
#include <iostream>

using namespace std;

namespace w32
{

	//Print the values under a specific key
    void PrintRegKeyValues(CHKey& key, std::wstring offset)
    {
		for (wstring valueName : key.GetValues())
		{

			wcout << offset << L"  " << ((valueName == L"") ? L"(Default)" : valueName) << L":  ";
			switch (key.GetValueType(valueName))
			{
			case REG_DWORD:
				wcout << key.GetDWValue(valueName);
				break;
			case REG_SZ:
				wcout << key.GetWSValue(valueName);
				break;
			}
			wcout << L"" << valueName << endl;
		}
    }

	//Print the entire contents (values and subkeys) under a specified key.
	//an offset is built during recursion to make the printout hierarchical
	void PrintRegKeyContents(CHKey& key, std::wstring offset)
	{
		if(offset == L"")
			wcout << offset << key.Path() << endl;
		else
			wcout << offset << L"> " << key.RelPath() << endl;

		PrintRegKeyValues(key, offset);

		wstring subOffset = offset + L"  ";
		for (wstring keyName : key.GetSubKeys())
		{
			CHKey subkey = key.OpenSubKey(keyName);
			PrintRegKeyContents(subkey, subOffset);
		}
	}
}