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
#include <WinBase.h>
#include "Handle.h"
#include <vector>
#include <string>
#include "Transaction.h"

namespace w32
{
	/// <summary>
	/// Class for working with registry keys.
	/// Instances can only be created through the static methods, usually starting with
	/// well known keys, because that is the only way to ensure that the path is build
	/// correctly for display and troubleshooting purposes. There is no other way to get
	/// the registry path for open keys.
	/// For this reason we also don't allow assignment or construction from raw HKEY values.
	/// It would be possible to do this in a similar manner as e.g. COM smart pointers take
	/// ownership, but it would prevent building the path information and correctly dealing
	/// with both situations and making it make sense is more bother than it is worth.
	/// Especially since there is no obvious compelling use case at present.
	/// 
	/// The transaction is passed down through the root key because registry contents may be
	/// subject to a current transaction and not yet exist in the committed state.
	/// These keys support use with registry transaction based on whether you supply one.
	/// </summary>
	class CHKey
	{
		std::wstring m_path = L"<unknown>";
		std::wstring m_relPath = L"<unknown>";
		HKEY m_handle;
		HANDLE m_transaction;

		//creation only allowed in static methods
		//because that is the only way to guarantee proper
		//path tracking since it's not possible to get the path from
		//an open handle
		CHKey();

	public:
		

		CHKey(CHKey&& key) noexcept;
		~CHKey();

		//Cast the CHKey object to a HKEY for interaction with
		//plain win32 API calls
		operator HKEY ();

		bool IsWellKnownKey();

		std::wstring Path();

		std::wstring RelPath();

		std::wstring Name();

		bool SubKeyExists(std::wstring subKey);

		//Open a key below this one
		CHKey OpenSubKey(
			std::wstring  regkey,
			REGSAM samDesired = GENERIC_READ);

		//Create a new subkey below this one
		CHKey CreateSubKey(
			std::wstring  regkey,
			REGSAM samDesired = GENERIC_READ);

		//Delete a subkey below this one
		void DeleteSubKey(
			std::wstring regkey,
			HANDLE transaction = INVALID_HANDLE_VALUE);

		//Set a string value in the current key
		void SetValue(
			std::wstring valueName,   //value name (NULL is default value)
			std::wstring value);      //value

		std::wstring GetWSValue(
			std::wstring valueName);  //value name (NULL is default value)

		DWORD GetDWValue(
			std::wstring valueName);  //value name (NULL is default value)

		//Get the names of the subkeys under this key
		std::vector<std::wstring> GetSubKeys();

		//Get the names of the values under this key
		std::vector<std::wstring> GetValues();

		//Get the data type of a specific value
		DWORD GetValueType(const std::wstring& valueName);

		//Open a registry key. We cannot do that directly because a registry key
		//is always opened or created below a parent key. A programmer can open
		//a registry key by using this static function or by using the constructor
		//with a wellknown key, and then use that to open or create a subkey
		static CHKey Open(
			HKEY parentKey,             //location where we want to open a new key
			std::wstring regkey,      //keyname
			REGSAM samDesired = GENERIC_READ,          //requested rights
			HANDLE transaction = INVALID_HANDLE_VALUE);  //transaction under which the key is opened.

		//Open a registry key. We cannot do that directly because a registry key
		//is always opened or created below a parent key. A programmer can open
		//a registry key by using this static function or by using the constructor
		//with a wellknown key, and then use that to open or create a subkey
		static CHKey Create(
			HKEY parentKey,             //location where we want to open a new key
			std::wstring regkey,      //keyname
			REGSAM samDesired = GENERIC_READ | GENERIC_WRITE,          //requested rights
			HANDLE transaction = INVALID_HANDLE_VALUE);  //transaction under which the key is opened.

		//Delete all values and keys underneath a given key and - optionally- the subkey
		//itself. This is performed in a transacted way. Either an overall transaction is
		//supplied, or a local one is created and transacted locally.
		static void DeleteTree(
			HKEY hKeyRoot,              //root of the tree to be delete
			LPCTSTR subKey,             //subkey that is to be deleted
			bool deleteSubKey,          //do we delete the subkey itself or only what's underneath?
			HANDLE transaction = INVALID_HANDLE_VALUE); //transaction

		//Is the specified key one of the well known ones
		static bool IsWellKnownKey(HKEY key);

		//does a specific key exist?
		static bool Exists(HKEY root, std::wstring subKey, HANDLE transaction = INVALID_HANDLE_VALUE);

		//get a readable name for a well known key 
		static std::wstring GetWellKnownKeyName(HKEY key);


	};
}