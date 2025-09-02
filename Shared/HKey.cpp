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
#include "HKey.h"
#include "Array.h"
#include "Transaction.h"
#include "Exception.h"
#include <iostream>

using namespace std;

namespace w32
{
	//create a new key. Only called in the static methods
	CHKey::CHKey() : m_transaction(), m_handle(NULL){}

	//move constructor for use in the static methods
	CHKey::CHKey(CHKey&& key) noexcept {
		m_handle = key.m_handle;
		m_path = key.m_path;
		m_transaction = key.m_transaction;
		key.m_handle = NULL;
		key.m_transaction = NULL;
	}

	CHKey::~CHKey() {
		if (m_handle) {
			RegCloseKey(m_handle);
			m_handle = NULL;
		}
	}

	//Cast to raw HKEY
	CHKey::operator HKEY () {
		return m_handle;
	}

	bool CHKey::IsWellKnownKey()
	{
		return IsWellKnownKey(m_handle);
	}

	//Get the full path of the key, inasmuch as we've been able to build it from the start
	std::wstring CHKey::Path()
	{
		return m_path;
	}

	//Get the path of this key, relative to the key from which it was opened.
	std::wstring CHKey::RelPath()
	{
		return m_relPath;
	}

	//Get the name of the key
	std::wstring CHKey::Name()
	{
		size_t index = m_path.find_last_of(L'\\');
		if (index >= 0)
			return m_path.substr(index + 1);
		return m_path; //no separator
	}

	//Does a specific subkey exist
	bool CHKey::SubKeyExists(std::wstring subKey)
	{
		return Exists(m_handle, subKey, m_transaction);
	}

	//Open a new subkey relative to this one
	//The key must exist
	CHKey CHKey::OpenSubKey(
		std::wstring regkey,      //keyname
		REGSAM samDesired) {
		CHKey key = CHKey::Open(m_handle, regkey, samDesired, m_transaction);
		key.m_path = m_path + L"\\" + regkey;
		return key;
	}

	//Create or open a subkey
	CHKey CHKey::CreateSubKey(
		std::wstring regkey,      //keyname
		REGSAM samDesired) {
		CHKey key = CHKey::Create(m_handle, regkey, samDesired, m_transaction);
		key.m_path = m_path + L"\\" + regkey;
		return key;
	}

	//Delete a subkey
	void CHKey::DeleteSubKey(
		std::wstring regkey,
		HANDLE transaction) {
		LSTATUS retVal = RegDeleteTreeW(m_handle, regkey.c_str());
		if (retVal)
			throw ExWin32Error(retVal);
	}

	//Set a value under this key
	void CHKey::SetValue(
		std::wstring valueName,   //value name (NULL is default value)
		std::wstring value) {
		LSTATUS retVal;
		if (!(retVal = RegSetValueExW(
			m_handle, valueName.c_str(), 0, REG_SZ,
			(const BYTE*)value.c_str(), (DWORD)((value.length() + 1) * sizeof(wchar_t)))))
			throw ExWin32Error(retVal);

	}

	//Get a string value 
	std::wstring CHKey::GetWSValue(
		std::wstring valueName)  //value name (NULL is default value)
	{
		LSTATUS retVal;
		DWORD type = 0; //REG_SZ
		DWORD dwSize = 0;
		retVal = RegGetValueW(
			m_handle, NULL, valueName.c_str(), RRF_RT_REG_SZ, &type,
			NULL, &dwSize);
		if (retVal != ERROR_SUCCESS)
			throw ExWin32Error(retVal);

		//REG_SZ may or may not be stored with a 0 termination.
		//safest is to assume there isn't one, and oversize by 1 character.
		dwSize += sizeof(wchar_t);
		//wchar_t* buffer = new wchar_t[dwSize / sizeof(wchar_t)];
		//memset(buffer, 0, dwSize);

		CArray<wchar_t> buffer(dwSize);

		retVal = RegGetValue(
			m_handle, NULL, valueName.c_str(), RRF_RT_REG_SZ, &type,
			buffer, &dwSize);
		if (retVal != ERROR_SUCCESS) {
			throw ExWin32Error(retVal);
		}

		return std::wstring(buffer);
	}

	//Get a DWORD value
	DWORD CHKey::GetDWValue(
		std::wstring valueName)
	{
		LSTATUS retVal;
		DWORD type = 0; //RRF_RT_REG_DWORD

		DWORD buffer = 0;
		DWORD dwSize = sizeof(DWORD);

		retVal = RegGetValue(
			m_handle, NULL, valueName.c_str(), RRF_RT_REG_DWORD, &type,
			&buffer, &dwSize);
		if (retVal != ERROR_SUCCESS) {
			throw ExWin32Error(retVal);
		}

		return buffer;

	}

	//Get the subkeys under this key
	std::vector<std::wstring> CHKey::GetSubKeys()
	{
		DWORD numsubkeys;
		DWORD maxSubkeyLength;

		//Get the number and the length of the longest name
		LSTATUS retVal = RegQueryInfoKeyW(m_handle, NULL, NULL, NULL, &numsubkeys, &maxSubkeyLength,
			NULL, NULL, NULL, NULL, NULL, NULL);
		if (retVal != ERROR_SUCCESS) {
			throw ExWin32Error(retVal);
		}

		std::vector<std::wstring> subKeys;
		CArray<wchar_t> buffer(maxSubkeyLength + 1);

		//Get the name of each key, using the longest length as buffer size
		for (DWORD i = 0; i < numsubkeys; i++)
		{
			DWORD subkeyLength = maxSubkeyLength + 1;
			buffer.Zero();
			retVal = RegEnumKeyExW(m_handle, i, buffer, &subkeyLength, NULL, NULL, NULL, NULL);
			if (retVal != ERROR_SUCCESS) {
				throw ExWin32Error(retVal);
			}
			subKeys.push_back(std::wstring(buffer));
		}

		return subKeys;
	}

	//Get the names of the values under this key
	std::vector<std::wstring> CHKey::GetValues()
	{
		DWORD numValues;
		DWORD maxValueNameLength;
		DWORD maxValueLength;

		//Get the number and the length of the longest name
		LSTATUS retVal = RegQueryInfoKeyW(m_handle, NULL, NULL, NULL, NULL, NULL,
			NULL, &numValues, &maxValueNameLength, &maxValueLength, NULL, NULL);
		if (retVal != ERROR_SUCCESS) {
			throw ExWin32Error(retVal);
		}

		std::vector<std::wstring> values;
		CArray<wchar_t> buffer(maxValueNameLength + 1);

		//Get the name of each key, using the longest length as buffer size
		for (DWORD i = 0; i < numValues; i++)
		{
			DWORD valueLength = maxValueNameLength + 1;
			buffer.Zero();
			retVal = RegEnumValueW(m_handle, i, buffer, &valueLength, NULL, NULL, NULL, NULL);
			if (retVal != ERROR_SUCCESS) {
				throw ExWin32Error(retVal);
			}
			values.push_back(std::wstring(buffer));
		}

		return values;
	}

	//Get the type of a specific value
	DWORD CHKey::GetValueType(const std::wstring& valueName)
	{
		LSTATUS retVal;
		DWORD type = 0;
		DWORD dwSize = 0;
		retVal = RegGetValueW(
			m_handle, NULL, valueName.c_str(), RRF_RT_ANY, &type,
			NULL, &dwSize);
		if (retVal != ERROR_SUCCESS)
			throw ExWin32Error(retVal);
		return type;
	}


	////////////////////////////////////////////////////////////////////////////////////
	//static methods
	////////////////////////////////////////////////////////////////////////////////////

	CHKey CHKey::Open(
		HKEY parentKey,             //location where we want to open a new key
		std::wstring regkey,      //keyname
		REGSAM samDesired,          //requested rights
		HANDLE transaction) {  //transaction under which the key is opened.

		CHKey key;
		LSTATUS retVal = NO_ERROR;
		if (transaction != INVALID_HANDLE_VALUE && transaction != NULL) {
			retVal = RegOpenKeyTransacted(
				parentKey, regkey.c_str(), 0, samDesired,
				&key.m_handle,
				transaction, NULL);
		}
		else {
			retVal = RegOpenKeyEx(
				parentKey, regkey.c_str(), 0, samDesired,
				&key.m_handle);
		}

		if (retVal)
			throw ExWin32Error(retVal);

		//even though the HKEY itself has no need for the transaction anymore
		//we need to pair it with the CHKey because otherwise it cannot open or
		//create transacted subkeys
		key.m_transaction = transaction;

		if (IsWellKnownKey(parentKey)) {
			key.m_path = GetWellKnownKeyName(parentKey) + L"\\" + regkey;
		}
		else {
			key.m_path = std::wstring(L"<Unknown>\\") + regkey;
		}
		key.m_relPath = regkey;
		return key;
	}

	CHKey CHKey::Create(
		HKEY parentKey,             //location where we want to open a new key
		std::wstring regkey,      //keyname
		REGSAM samDesired,          //requested rights
		HANDLE transaction) {  //transaction under which the key is opened.

		CHKey key;
		LSTATUS retVal = NO_ERROR;
		if (transaction != INVALID_HANDLE_VALUE && transaction != NULL) {
			retVal = RegCreateKeyTransacted(
				parentKey, regkey.c_str(), 0,
				NULL,                   //user class. can be ignored
				REG_OPTION_NON_VOLATILE,//the change is to be permanent
				samDesired,
				NULL,                   //security attributes. NULL -> default security inherited
				&key.m_handle,
				NULL,                   //disposition feedback -> was it created or opened? don't care.
				transaction,
				NULL);					//reserved
		}
		else {
			retVal = RegCreateKeyEx(
				parentKey,
				regkey.c_str(),
				0,                      //reserved
				NULL,                   //user class. can be ignored
				REG_OPTION_NON_VOLATILE,//the change is to be permanent
				samDesired,
				NULL,                   //security attributes. NULL -> default security inherited
				&key.m_handle,
				NULL);                 //disposition feedback -> was it created or opened? don't care.
		}
		if (retVal)
			throw ExWin32Error(retVal);

		//even though the HKEY itself has no need for the transaction anymore
		//we need to pair it with the CHKey because otherwise it cannot open or
		//create transacted subkeys
		key.m_transaction = transaction;

		if (IsWellKnownKey(parentKey)) {
			key.m_path = GetWellKnownKeyName(parentKey) + L"\\" + regkey;
		}
		else {
			key.m_path = std::wstring(L"<Unknown>\\") + regkey;
		}
		key.m_relPath = regkey;

		return key;
	}

	//Delete all values and keys underneath a given key and - optionally- the subkey
	//itself. This is performed in a transacted way. Either an overall transaction is
	//supplied, or a local one is created and transacted locally.
	//This has to be a static method because it allows the root itself to be deleted.
	//This is not possible as a member method because a key cannot delete itself.
	void CHKey::DeleteTree(
		HKEY hKeyRoot,              //root of the tree to be delete
		LPCTSTR subKey,             //subkey that is to be deleted
		bool deleteSubKey,          //do we delete the subkey itself or only what's underneath?
		HANDLE transaction)         //transaction
	{
		//The root cannot be NULL
		if (hKeyRoot == NULL)
			throw ExWin32Error(ERROR_INVALID_PARAMETER);

		//Do not accidentally delete a root
		if ((hKeyRoot == HKEY_CLASSES_ROOT ||
			hKeyRoot == HKEY_CURRENT_CONFIG ||
			hKeyRoot == HKEY_CURRENT_USER ||
			hKeyRoot == HKEY_LOCAL_MACHINE ||
			hKeyRoot == HKEY_USERS) && subKey == NULL)
			throw ExWin32Error(ERROR_INVALID_PARAMETER);

		//If the subkey itself needs to be deleted, it cannot be null
		if (deleteSubKey && subKey == NULL)
			throw ExWin32Error(ERROR_INVALID_PARAMETER);

		//If an overall transaction was supplied, we use that.
		//If not, we use a local one.
		CTransaction localTransaction;
		if (transaction == INVALID_HANDLE_VALUE) {
			localTransaction.Create();
		}

		try
		{
			//Open a transacted handle and delete everything underneath the specified key / subkey
			DWORD retVal = ERROR_SUCCESS;

			CHKey key = CHKey::Open(hKeyRoot, subKey, KEY_WRITE | KEY_READ, localTransaction);

			if (!(retVal = RegDeleteTree(key, NULL)))
				throw ExWin32Error(retVal);

			if (deleteSubKey) {
				if (!(retVal = RegDeleteKeyTransacted(
					hKeyRoot, subKey, KEY_WRITE | KEY_READ, 0, localTransaction, NULL)))
					throw ExWin32Error(retVal);
			}

			//there was a local transaction
			if (localTransaction.IsValid()) {
				localTransaction.Commit();
			}
		}
		catch (...) {
			if (localTransaction.IsValid()) {
				localTransaction.RollBack();
			}
			throw;
		}
	}

	//Simple check to see if the handle is one of the well known pseudo handles
	bool CHKey::IsWellKnownKey(HKEY key)
	{
		return (key == HKEY_LOCAL_MACHINE ||
			key == HKEY_CURRENT_USER ||
			key == HKEY_USERS ||
			key == HKEY_CLASSES_ROOT ||
			key == HKEY_PERFORMANCE_DATA ||
			key == HKEY_PERFORMANCE_TEXT ||
			key == HKEY_PERFORMANCE_NLSTEXT ||
			key == HKEY_CURRENT_CONFIG ||
			key == HKEY_DYN_DATA ||
			key == HKEY_CURRENT_USER_LOCAL_SETTINGS);
	}

	//Check if a key exists
	bool CHKey::Exists(HKEY root, std::wstring subKeyName, HANDLE transaction)
	{
		HKEY subKey = NULL;
		LSTATUS result = NO_ERROR;
		if (transaction != INVALID_HANDLE_VALUE) {
			result = RegOpenKeyTransactedW(root, subKeyName.c_str(), 0, KEY_READ, &subKey, transaction, NULL);
		}
		else {
			result = RegOpenKeyExW(root, subKeyName.c_str(), 0, KEY_READ, &subKey);
		}
		if (result == ERROR_SUCCESS) {
			::CloseHandle(subKey);
			return true;
		}
		else if (result != ERROR_FILE_NOT_FOUND) {
			throw ExWin32Error(result);
		}

		return false;
	}

	//Get a human readable name for a pseudo key
	std::wstring CHKey::GetWellKnownKeyName(HKEY key)
	{
		std::wstring keyName;

		if (key == HKEY_LOCAL_MACHINE)
			keyName = L"HKLM";
		else if (key == HKEY_USERS)
			keyName = L"HKU";
		else if (key == HKEY_CURRENT_USER)
			keyName = L"HKCU";
		else if (key == HKEY_CLASSES_ROOT)
			keyName = L"HKCR";
		else if (key == HKEY_PERFORMANCE_DATA)
			keyName = L"HKPD";
		else if (key == HKEY_PERFORMANCE_TEXT)
			keyName = L"HKPT";
		else if (key == HKEY_PERFORMANCE_NLSTEXT)
			keyName = L"HKPN";
		else if (key == HKEY_CURRENT_CONFIG)
			keyName = L"HKCC";
		else if (key == HKEY_DYN_DATA)
			keyName = L"HKDD";
		else if (key == HKEY_CURRENT_USER_LOCAL_SETTINGS)
			keyName = L"HKCULS";
		else
			throw AppException(L"Not a well-known key");

		return keyName;
	}
}