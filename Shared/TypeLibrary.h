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

#include "TlbInfo.h"

namespace w32
{
	/// <summary>
	/// This class encapsulates a COM ITypeLib and the methods that we 
	/// for the purpose of dealing with registration.
	/// Registration requires an actual tlb file
	/// Unregistration can be done either via the tlb file itself (using that file
	/// as a source for the necessary information), or by specifying everything that
	/// is needed to identify the registration in the registry.
	/// </summary>
	class CTypeLibrary : public CTlbInfo
	{
		CComPtr<ITypeLib> m_typeLib;
		std::wstring m_path;
		std::wstring GetVersionString();
	public:
		CTypeLibrary(std::wstring path);

		void Register(bool perUser);
		void UnRegister(bool perUser);

		static void UnRegister(bool perUser,
			const GUID& guid,
			WORD major,
			WORD minor,
			LCID locale,
			SYSKIND syskind);

		static bool Exists(const GUID& guid, bool perUser);

	};
}
