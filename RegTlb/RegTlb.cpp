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
#include <iostream>
#include "TypeLibrary.h"
#include "CommandLine.h"
#include "CommandLineArgs.h"
#include "ConsoleHelper.h"
#include "HKey.h"

using namespace std;
using namespace w32;

int wmain(int argc, wchar_t* argv[])
{
    try
    {
        CCommandLine cmdLine(argc, argv);

        if (!cmdLine.ArgsValid()) {
            cmdLine.PrintUsage();
            return 0;
        }

        switch (cmdLine.GetCommand())
        {
        case ECommand::QUERY:
            if (cmdLine.GetPath().empty()) {
                GUID guid = cmdLine.GetGuid();
                wcout << L"Querying for type library " <<
                    WStringFromGUID(guid) <<
                    L" in the registry" << endl;

                std::wstring guidStr = WStringFromGUID(guid);
                std::wstring regPath = L"Software\\Classes\\TypeLib\\";
                regPath += guidStr;

                if (CTypeLibrary::Exists(guid, true)) {
                    std::wcout << L"GUID " << guidStr << L" exists in the user hive." << std::endl;
                    CHKey key = CHKey::Open(HKEY_CURRENT_USER, regPath);
                    PrintRegKeyContents(key);
                }
                else {
                    std::wcout << L"GUID " << guidStr << L" does not exist in the user hive." << std::endl;
                }
                if (CTypeLibrary::Exists(guid, false)) {
                    std::wcout << L"GUID" << guidStr << L" exists in the machine hive." << std::endl;
                    CHKey key = CHKey::Open(HKEY_LOCAL_MACHINE, regPath);
                    PrintRegKeyContents(key);
                }
                else {
                    std::wcout << L"GUID " << guidStr << L" does not exist in the machine hive." << std::endl;
                }

            }
            else {
                wstring path = cmdLine.GetPath();
                wcout << L"Querying type library " << path <<
                    L" for embedded information." << endl;
                CTypeLibrary tlb(path);
                tlb.PrintTlbInfo();
            }
            break;
        case ECommand::INSTALL:
        case ECommand::INSTALL_PER_USER: {
            CTypeLibrary typelib(cmdLine.GetPath());
            typelib.Register(cmdLine.GetCommand() == ECommand::INSTALL_PER_USER);
            break;
        }
        case ECommand::UNINSTALL:
        case ECommand::UNINSTALL_PER_USER: {
            bool perUser = cmdLine.GetCommand() == ECommand::UNINSTALL_PER_USER;
            if (cmdLine.GetPath().empty()) {
                CTypeLibrary::UnRegister(perUser,
                    cmdLine.GetGuid(),
                    cmdLine.GetMajor(),
                    cmdLine.GetMinor(),
                    cmdLine.GetLocale(),
                    cmdLine.GetSysKind());
            }
            else {
                CTypeLibrary typelib(cmdLine.GetPath());
                typelib.UnRegister(perUser);
            }
            break;
        }
        }
    }
    catch (const AppException& ex) {

        cout << ex.what() << endl;
    }
}