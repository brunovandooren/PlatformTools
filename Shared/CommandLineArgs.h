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

/// <summary>
/// Base class for parsing an argument list for a command line executable
/// </summary>
class CCommandLineArgs
{
private:
	int m_argc;
	wchar_t** m_argv;

	int m_arg;
	std::wstring m_current;

public:
	CCommandLineArgs(int argc, wchar_t* argv[]);
	void ResetIterator();
	bool GetNext(std::wstring& arg);
	bool GetNext();
	std::wstring GetCurrent();
	bool TryParseFlag(const std::wstring& id, bool& value);
	bool TryParseArg(const std::wstring& id, std::wstring& value);
	bool TryParseArg(const std::wstring& id, int& value);
	bool TryParseArg(const std::wstring& id, WORD& value);
	bool TryParseArg(const std::wstring& id, bool& value);
	bool TryParseArg(const std::wstring& id, double& value);
	bool TryParseArg(const std::wstring& id, float& value);
};

