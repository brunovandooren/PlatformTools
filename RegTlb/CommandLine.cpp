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
#include "CommandLine.h"
#include "StringHelper.h"
#include <iostream>
#include <Shlwapi.h>

using namespace std;
using namespace w32;

bool CCommandLine::ParseCommand(const std::wstring& arg, ECommand& command)
{
	if (arg == L"/i") {
		command = ECommand::INSTALL;
	}
	else if (arg == L"/u") {
		command = ECommand::UNINSTALL;
	}
	else if (arg == L"/i_user") {
		command = ECommand::INSTALL_PER_USER;
	}
	else if (arg == L"/u_user") {
		command = ECommand::UNINSTALL_PER_USER;
	}
	else if (arg == L"/q") {
		command = ECommand::QUERY;
	}
	else {
		return false;
	}

	return true;
}


CCommandLine::CCommandLine(int argc, wchar_t* argv[]) :
	CCommandLineArgs(argc, argv)
{
	m_argsValid = true;
	m_command = ECommand::NONE;
	m_guid = L"";
	m_major = 0;
	m_minor = 0;
	m_syskind = SYS_WIN32;
	m_locale = 0;

	std::wstring temp;
	int tempint;
	GetNext(m_path);
	while (m_argsValid && GetNext())
	{
		if (ParseCommand(GetCurrent(), m_command)) {
			continue;
		}
		else if (
			TryParseArg(L"/guid", m_guid) ||
			TryParseArg(L"/major", m_major) ||
			TryParseArg(L"/minor", m_minor)) {
			continue;
		}
		else if (TryParseArg(L"/locale", tempint)) {
			m_locale = tempint;
			continue;
		}
		else if (TryParseArg(L"/tlb", m_tlbPath)) {
			if (PathFileExistsW(m_tlbPath.c_str())) {
				continue;
			}
			else
				m_argsValid = false;
		}
		else if (TryParseArg(L"/syskind", temp)) {
			if (temp == L"win64") {
				m_syskind = SYS_WIN64;
			}
			else if (temp == L"win32") {
				m_syskind = SYS_WIN32;
			}
			else {
				m_argsValid = false;
				return;
			}
			continue;
		}
		else {
			wcout << L"Invalid argument " << endl;
			m_argsValid = false;
			return;
		}
	}

	//bail if args are not valid
	if (!m_argsValid)
		return;

	//bail if the application is not running for install or uninstall
	if (m_command == ECommand::NONE) {
		m_argsValid = false;
		return;
	}

	//if the purpose is install, path cannot be empty
	if ((m_command == ECommand::INSTALL ||
		m_command == ECommand::INSTALL_PER_USER) && m_tlbPath.empty()) {
		m_argsValid = false;
		return;
	}

	//if the purpose is uninstall, EITHER the path needs to be supplied, or all the ID info.
	if ((m_command == ECommand::UNINSTALL ||
		m_command == ECommand::UNINSTALL_PER_USER)) {

		bool tlbsupplied = !m_tlbPath.empty();
		bool idinfosupplied = !m_guid.empty() &&
			(m_syskind == SYS_WIN32 || m_syskind == SYS_WIN64) &&
			m_major >= 0 &&
			m_minor >= 0;

		if (!(tlbsupplied ^ idinfosupplied)) {
			m_argsValid = false;
			return;
		}

	}

	//if the purpose is to query, we need a GUID
}

void CCommandLine::PrintUsage(void)
{
	wcout << L"This program can register or unregister type libraries." << endl;
	//wcout << L"It also has the ability to add or remove the type library references for applicable COM classes and interfaces." << endl;
	wcout << L"USAGE:" << endl;
	wcout << L"RegTlb [/i | /i_user] /tlb <library path>" << endl;
	wcout << L"/i\t\t\tRegister the type library" << endl;
	wcout << L"/i_user\t\t\tRegister the type library for the current user" << endl;
	wcout << L"/tlb <path>\t\tthe full path of the tlb file. Use double quotes if the path has spaces." << endl;
	wcout << L"Relative paths are allowed for the tlb file. The relative path will be converted." << endl;
	wcout << L"to absolute paths when registering the tlb file in the registry." << endl << endl << endl;

	wcout << L"RegTlb [/u | /u_user] /tlb <library path>" << endl;
	wcout << L"Unregister the type library, based on the information found in the tlb file." << endl;
	wcout << L"/u\t\t\tUnregister the type library." << endl;
	wcout << L"/u_user\t\t\tUnregister the type library for the current user." << endl;
	wcout << L"/tlb <path>\t\tthe full path of the tlb file. Use double quotes if the path has spaces." << endl << endl << endl;

	wcout << L"RegTlb /q [/tlb <library path> | /guid <guid>]" << endl;
	wcout << L"/q\t\t\tQuery type library information" << endl;
	wcout << L"/tlb <library path>\t\tQuery type library information that is contained in the library (identifiers)" << endl;
	wcout << L"/guid <guid>\t\tQuery type library information that is contained in the registry for the specified GUID" << endl << endl << endl;

	wcout << L"RegTlb /u /guid <guid> /major <version> /minor <version> [/locale <lcid>] /syskind <kind> " << endl;
	wcout << L"/u\t\t\tUnregister the type library" << endl;
	wcout << L"/guid <guid>\t\tthe guid of the library." << endl;
	wcout << L"/major <version>\tInteger value. The major version number of the tlb." << endl;
	wcout << L"/minor <version>\tInteger value. The minor version number of the tlb." << endl;
	wcout << L"/locale <lcid>\t\tInteger value. The locale id. if not specified, 0 (LOCALE_NEUTRAL) is used." << endl;
	wcout << L"/syskind <kind>\t\tString value. The system kind. WIN32 or WIN64 are supported." << endl;
	wcout << L"In case of doubt, use RegTlb /q /guid <guid> first to find the appropriate values." << endl << endl << endl;

	

	//wcout << L"Optional arguments" << endl;
	//wcout << L"/interfaces\t\twhen registering or unregistering the type library, also create or delete the entries under the interface keys." << endl;
	//wcout << L"Example:" << endl;
	//wcout << L"    To register a type library for the current user: RegTlb /i /peruser <library path>" << endl;
}

bool CCommandLine::ArgsValid(void)
{
	return m_argsValid;
}

std::wstring CCommandLine::GetPath(void)
{
	return m_tlbPath;
}

ECommand CCommandLine::GetCommand(void)
{
	return m_command;
}

GUID CCommandLine::GetGuid(void)
{
	GUID guid;
	GUIDFromWString(m_guid, guid);
	return guid;
}

WORD CCommandLine::GetMajor(void)
{
	return m_major;
}

WORD CCommandLine::GetMinor(void)
{
	return m_minor;
}

LCID CCommandLine::GetLocale(void)
{
	return m_locale;
}

SYSKIND CCommandLine::GetSysKind(void)
{
	return m_syskind;
}