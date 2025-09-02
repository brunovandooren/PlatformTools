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
#include <ole2.h>
#include <vector>
#include <string>


namespace w32
{
	/// <summary>
	/// A set of information that represents some of the contents of a TLB file
	/// </summary>
	struct CTlbInfo
	{
		GUID Guid;
		std::wstring Version;
		std::vector<GUID> CoClasses;
		std::vector<GUID> Interfaces;
		std::vector<GUID> DispInterfaces;
		WORD MajorVersion;
		WORD MinorVersion;
		LCID LocaleID;
		SYSKIND SysKind;

		void PrintTlbInfo();
	};
}

