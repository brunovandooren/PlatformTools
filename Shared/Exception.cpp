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
#include "Exception.h"
#include <sstream>
#include "StringHelper.h"

using namespace std;

namespace w32
{
	/////////////////////////////////////////////////////////////
	//AppException
	/////////////////////////////////////////////////////////////
	AppException::AppException() : exception() {
	}

	AppException::AppException(wstring const& message) {
		m_What = WStringToString(message);
	}

	AppException::AppException(string const& message) : m_What(message) {
	}

	const char* AppException::what() const noexcept {
		return m_What.c_str();
	}

	/////////////////////////////////////////////////////////////
	//Win32Exception
	/////////////////////////////////////////////////////////////
	Win32Exception::Win32Exception() {
	}

	Win32Exception::Win32Exception(DWORD value) :_value(value) {
	}

	DWORD Win32Exception::Value() {
		return _value;
	}

	string Win32Exception::GetErrorMessage(int code)
	{
		LPSTR errorText = NULL;
		string error;
		FormatMessageA(
			FORMAT_MESSAGE_FROM_SYSTEM
			| FORMAT_MESSAGE_ALLOCATE_BUFFER
			| FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			code,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&errorText,
			0,
			NULL);

		if (NULL != errorText)
		{
			error = std::string(errorText);
			LocalFree(errorText);
		}

		return error;
	}

	wstring Win32Exception::GetErrorMessageW(int code)
	{
		LPWSTR errorText = NULL;
		wstring error;
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


	/////////////////////////////////////////////////////////////
	//ExNtStatus
	/////////////////////////////////////////////////////////////
	ExNtStatus::ExNtStatus(DWORD value) : Win32Exception(value) {
		stringstream stream;
		stream << "Function returned NTSTATUS " << value << ": " << GetErrorMessage(value) << ".";
		m_What = stream.str();
	}

	/////////////////////////////////////////////////////////////
	//ExWin32Error
	/////////////////////////////////////////////////////////////
	ExWin32Error::ExWin32Error() {
		_value = GetLastError();
		stringstream stream;
		stream << "Function returned error code " << _value << ": " << GetErrorMessage(_value) << ".";
		m_What = stream.str();
	}

	ExWin32Error::ExWin32Error(DWORD value) : Win32Exception(value) {
		stringstream stream;
		stream << "Function returned error code " << value << ": " << GetErrorMessage(value) << ".";
		m_What = stream.str();
	}

	ExWin32Error::ExWin32Error(wstring const& message) {
		_value = GetLastError();
		stringstream stream;
		stream << "Function returned error code " << _value << ": " << GetErrorMessage(_value) << ". " << WStringToString(message) << ".";
		m_What = stream.str();
	}

	ExWin32Error::ExWin32Error(string const& message) {
		_value = GetLastError();
		stringstream stream;
		stream << "Function returned error code " << _value << ": " << GetErrorMessage(_value) << ". " << message << ".";
		m_What = stream.str();
	}

	ExWin32Error::ExWin32Error(DWORD value, wstring const& message) : Win32Exception(value) {
		stringstream stream;
		stream << "Function returned error code " << value << ": " << GetErrorMessage(value) << ". " << WStringToString(message) << ".";
		m_What = stream.str();
	}

	ExWin32Error::ExWin32Error(DWORD value, string const& message) : Win32Exception(value) {
		stringstream stream;
		stream << "Function returned error code " << value << ": " << GetErrorMessage(value) << value << ". " << message << ".";
		m_What = stream.str();
	}

	/////////////////////////////////////////////////////////////
	//ExHResult
	/////////////////////////////////////////////////////////////

	ExHResult::ExHResult(DWORD value) : Win32Exception(value) {
		stringstream stream;
		stream << "Function returned hresult 0x" << hex << value << ": " << GetErrorMessage(value) << ".";
		m_What = stream.str();
	}

	ExHResult::ExHResult(DWORD value, wstring const& message) : Win32Exception(value) {
		stringstream stream;
		stream << "Function returned hresult 0x" << hex << value << ": " << GetErrorMessage(value) << ". ";
		stream << WStringToString(message) << ".";
		m_What = stream.str();
	}
	ExHResult::ExHResult(DWORD value, string const& message) : Win32Exception(value) {
		stringstream stream;
		stream << "Function returned hresult 0x" << hex << value << ": " << GetErrorMessage(value) << ". " << message << ".";
		m_What = stream.str();
	}
}