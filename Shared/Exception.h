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
#include "pch.h"
#include <WinBase.h>
#include <string>

namespace w32
{
	//Base class for regular application exceptions
	//Because applications can use both string and wstring
	//we support both types.
	class AppException : public std::exception
	{
	protected:
		std::string m_What;
	public:
		AppException();
		AppException(std::string const& message);
		AppException(std::wstring const& message);

		const char* what() const noexcept override;
	};

	//Base class for exceptions raised in interaction with the Windows
	//subsystem.
	class Win32Exception : public AppException {
	protected:
		DWORD _value = 0;
		std::string GetErrorMessage(int code);
		std::wstring GetErrorMessageW(int code);

	public:
		Win32Exception();
		Win32Exception(DWORD value);
		virtual DWORD Value();
	};

	//Class for errors that return as NTSTATUS codes which are
	//different from HRESULT or error codes
	class ExNtStatus : public Win32Exception {
	public:
		ExNtStatus(DWORD value);
	};

	//Class for errors that are returned as error codes
	//There are a bunch of different constructors because
	//either we supply the value or it is fetched from GetLastError
	//and then we can supply a message or no, as string or wstring.
	class ExWin32Error : public Win32Exception {
	public:
		ExWin32Error();
		ExWin32Error(std::wstring const& message);
		ExWin32Error(std::string const& message);

		ExWin32Error(DWORD value);
		ExWin32Error(DWORD value, std::wstring const& message);
		ExWin32Error(DWORD value, std::string const& message);
	};

	//Class for errors that are returned as HRESULT
	//There is no default constructor because we always need to supply the HRESULT
	class ExHResult : public Win32Exception {
	public:
		ExHResult(DWORD value);
		ExHResult(DWORD value, std::wstring const& message);
		ExHResult(DWORD value, std::string const& message);
	};
}
