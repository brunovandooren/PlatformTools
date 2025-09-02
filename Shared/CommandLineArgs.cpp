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
#include "CommandLineArgs.h"
#include "StringHelper.h"

using namespace w32;

CCommandLineArgs::CCommandLineArgs(int argc, wchar_t* argv[])
{
	m_argc = argc;
	m_argv = argv;
	m_arg = 0;
}

//Restart the command line parsing
void CCommandLineArgs::ResetIterator()
{
	m_arg = 0;
	m_current = L"";
}

//Get the next argument for inspection
//This pops the argument, store it for inspection and immediately return it
//as well. This is convenient for situations where a specific pattern is recognized
//and the next argument is data that belongs to the command or flag.
bool CCommandLineArgs::GetNext(std::wstring& arg)
{
	if (m_arg < m_argc)
	{
		arg = m_argv[m_arg];
		ToWLowerInPlace(arg);
		m_current = arg;
		m_arg++;
		return true;
	}

	return false;
}

//Get the next argument for inspection
//This pops the argument, store it for inspection
bool CCommandLineArgs::GetNext()
{
	if (m_arg < m_argc)
	{
		m_current = m_argv[m_arg];
		ToWLowerInPlace(m_current);
		m_arg++;
		return true;
	}
	return false;
}

//Get the argument that is currently being inspected.
std::wstring CCommandLineArgs::GetCurrent()
{
	return m_current;
}

//See if the current argument is a match for a specific command or flag
bool CCommandLineArgs::TryParseFlag(const std::wstring& flagid, bool& value)
{
	if (m_current == flagid) {
		value = true;
		return true;
	}

	return false;
}

//see if the current arugment is a command and get the accompanying data
bool CCommandLineArgs::TryParseArg(const std::wstring& id, std::wstring& value)
{
	if (m_current == id && GetNext(value))
		return true;

	return false;
}

//see if the current arugment is a command and get the accompanying data
bool CCommandLineArgs::TryParseArg(const std::wstring& id, bool& value)
{
	if (m_current == id && GetNext()) {
		if (m_current == L"true" || m_current == L"1")
			value = true;
		else if (m_current == L"false" || m_current == L"0")
			value = false;
		else
			return false;
		return true;
	}

	return false;
}

//see if the current arugment is a command and get the accompanying data
bool CCommandLineArgs::TryParseArg(const std::wstring& id, int& value)
{
	if (m_current == id && GetNext()) {
		try {
			value = std::stoi(m_current);
			return true;
		}
		catch (std::exception) {
			return false;
		}
	}

	return false;
}
//see if the current arugment is a command and get the accompanying data
bool CCommandLineArgs::TryParseArg(const std::wstring& id, double& value)
{
	if (m_current == id && GetNext()) {
		try {
			value = std::stod(m_current);
			return true;
		}
		catch (std::exception& ) {
			return false;
		}
	}

	return false;
}

//see if the current arugment is a command and get the accompanying data
bool CCommandLineArgs::TryParseArg(const std::wstring& id, float& value)
{
	if (m_current == id && GetNext()) {
		try {
			value = std::stof(m_current);
			return true;
		}
		catch (std::exception& ) {
			return false;
		}
	}

	return false;
}

//see if the current arugment is a command and get the accompanying data
bool CCommandLineArgs::TryParseArg(const std::wstring& id, WORD& value)
{
	if (m_current == id && GetNext()) {
		try {
			value = std::stoi(m_current);
			return true;
		}
		catch (std::exception& ) {
			return false;
		}
	}

	return false;
}
