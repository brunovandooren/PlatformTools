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
#include <string>

#include "CommandLineArgs.h"

enum class ECommand {
	NONE,
	INSTALL,
	INSTALL_PER_USER,
	UNINSTALL,
	UNINSTALL_PER_USER,
	QUERY
};

class CCommandLine : private CCommandLineArgs
{
private:
	std::wstring m_path;
	std::wstring m_tlbPath;
	std::wstring m_guid;
	ECommand m_command;
	bool m_argsValid;
	WORD m_major;
	WORD m_minor;
	LCID m_locale;
	SYSKIND m_syskind;

	bool ParseCommand(const std::wstring& arg, ECommand& command);

public:
	CCommandLine(int argc, wchar_t* argv[]);

	void PrintUsage(void);
	bool ArgsValid(void);
	std::wstring GetPath(void);
	ECommand GetCommand(void);
	GUID GetGuid(void);
	WORD GetMajor(void);
	WORD GetMinor(void);
	LCID GetLocale(void);
	SYSKIND GetSysKind(void);

};

