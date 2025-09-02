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
#include <WinBase.h>
#include "HKey.h"

/// <summary>
/// Various helper routines for console applications
/// </summary>

namespace w32
{

    void PrintRegKeyValues(CHKey& key, std::wstring offset = L"");
    void PrintRegKeyContents(CHKey& key, std::wstring offset = L"");

}