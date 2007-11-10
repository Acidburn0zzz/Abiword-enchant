/* Copyright (c) 2007 Eric Scott Albright
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef __ENCHANTTESTFIXTURE
#define __ENCHANTTESTFIXTURE

#if defined(_MSC_VER)
#pragma once
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlwapi.h>
#endif

#include <glib.h>
#include <string>

struct EnchantTestFixture
{
    std::string savedRegistryHomeDir;
    std::string savedUserRegistryModuleDir;
    std::string savedMachineRegistryModuleDir;
    std::string savedUserRegistryConfigDir;
    std::string savedMachineRegistryConfigDir;

    //Setup
    EnchantTestFixture()
    {
#ifdef _WIN32
        savedRegistryHomeDir = GetRegistryHomeDir();
        ClearRegistryHomeDir();

        savedUserRegistryModuleDir = GetUserRegistryModuleDir();
        ClearUserRegistryModuleDir();

        savedMachineRegistryModuleDir = GetMachineRegistryModuleDir();
        ClearMachineRegistryModuleDir();

        savedUserRegistryConfigDir = GetUserRegistryConfigDir();
        ClearUserRegistryConfigDir();

        savedMachineRegistryConfigDir = GetMachineRegistryConfigDir();
        ClearMachineRegistryConfigDir();
#endif
    }

    //Teardown
    ~EnchantTestFixture(){
#ifdef _WIN32
        if(savedRegistryHomeDir.empty()) {
            ClearRegistryHomeDir();
        }
        else {
            SetRegistryHomeDir(savedRegistryHomeDir);
        }
                
        if(savedUserRegistryModuleDir.empty()) {
            ClearUserRegistryModuleDir();
        }
        else{
            SetUserRegistryModuleDir(savedUserRegistryModuleDir);
        }

        if(savedMachineRegistryModuleDir.empty())
        {
            ClearMachineRegistryModuleDir();
        }
        else{
            SetMachineRegistryModuleDir(savedMachineRegistryModuleDir);
        }

        if(savedUserRegistryConfigDir.empty())
        {
            ClearUserRegistryConfigDir();
        }
        else{
            SetUserRegistryConfigDir(savedUserRegistryConfigDir);
        }

        if(savedMachineRegistryConfigDir.empty()) {
            ClearMachineRegistryConfigDir();
        }
        else {
            SetMachineRegistryConfigDir(savedMachineRegistryConfigDir);
        }
#endif
    }

    std::string Convert(const std::wstring & ws)
    {
        gchar* str = g_utf16_to_utf8((gunichar2*)ws.c_str(), (glong)ws.length(), NULL, NULL, NULL);
        std::string s(str);
        g_free(str);
        return s;
    }

    std::wstring Convert(const std::string & s)
    {
        gunichar2* str = g_utf8_to_utf16(s.c_str(), (glong)s.length(), NULL, NULL, NULL);
        std::wstring ws((wchar_t*)str);
        g_free(str);
        return ws;
    }

    std::string GetDirectoryOfThisModule()
    {
        std::string result;
#if defined(_WIN32)
        // should be in the same directory as our test fixture
        WCHAR szFilename[MAX_PATH];
        GetModuleFileName(NULL, (LPWSTR) &szFilename, sizeof(szFilename));
        PathRemoveFileSpec((LPWSTR)&szFilename);

        result = Convert(szFilename);
#elif defined(ENABLE_BINRELOC) 
        gchar* prefix = gbr_find_prefix(NULL);
        result = std::string(prefix);
        g_free(prefix);
#endif
        return result;
      }

    std::string GetRegistryHomeDir()
    {
        return Convert(GetRegistryValue(HKEY_CURRENT_USER, L"Software\\Enchant\\Config", L"Home_Dir"));
    }

    void SetRegistryHomeDir(const std::string & dir)
    {
        SetRegistryValue(HKEY_CURRENT_USER, L"Software\\Enchant\\Config", L"Home_Dir", dir);
    }

    void ClearRegistryHomeDir()
    {
        ClearRegistryValue(HKEY_CURRENT_USER, L"Software\\Enchant\\Config", L"Home_Dir");
    }

    std::string GetUserRegistryModuleDir()
    {
        return Convert(GetRegistryValue(HKEY_CURRENT_USER, L"Software\\Enchant\\Config", L"Module_Dir"));

    }

    void SetUserRegistryModuleDir(const std::string & dir)
    {
        SetRegistryValue(HKEY_CURRENT_USER, L"Software\\Enchant\\Config", L"Module_Dir", dir);
    }

    void ClearUserRegistryModuleDir()
    {
        ClearRegistryValue(HKEY_CURRENT_USER, L"Software\\Enchant\\Config", L"Module_Dir");
    }

    std::string GetMachineRegistryModuleDir()
    {
        return Convert(GetRegistryValue(HKEY_LOCAL_MACHINE, L"Software\\Enchant\\Config", L"Module_Dir"));

    }

    void SetMachineRegistryModuleDir(const std::string & dir)
    {
        SetRegistryValue(HKEY_LOCAL_MACHINE, L"Software\\Enchant\\Config", L"Module_Dir", dir);
    }

    void ClearMachineRegistryModuleDir()
    {
        ClearRegistryValue(HKEY_LOCAL_MACHINE, L"Software\\Enchant\\Config", L"Module_Dir");
    }

    std::string GetUserRegistryConfigDir()
    {
        return Convert(GetRegistryValue(HKEY_CURRENT_USER, L"Software\\Enchant\\Config", L"Data_Dir"));
    }

    void SetUserRegistryConfigDir(const std::string & dir)
    {
        SetRegistryValue(HKEY_CURRENT_USER, L"Software\\Enchant\\Config", L"Data_Dir", dir);
    }

    void ClearUserRegistryConfigDir()
    {
        ClearRegistryValue(HKEY_CURRENT_USER, L"Software\\Enchant\\Config", L"Data_Dir");
    }

    std::string GetMachineRegistryConfigDir()
    {
        return Convert(GetRegistryValue(HKEY_LOCAL_MACHINE, L"Software\\Enchant\\Config", L"Data_Dir"));
    }

    void SetMachineRegistryConfigDir(const std::string & dir)
    {
        SetRegistryValue(HKEY_LOCAL_MACHINE, L"Software\\Enchant\\Config", L"Data_Dir", dir);
    }

    void ClearMachineRegistryConfigDir()
    {
        ClearRegistryValue(HKEY_LOCAL_MACHINE, L"Software\\Enchant\\Config", L"Data_Dir");
    }

    std::wstring GetRegistryValue(HKEY baseKey, const std::wstring& key, const std::wstring& valueName)
  {
      std::wstring result;

      WCHAR data[2048];
      DWORD dwSize = sizeof(data) * sizeof(WCHAR);
      HKEY hkey;

      if(RegOpenKeyEx(baseKey, 
                      key.c_str(), 
                      0, KEY_WRITE, &hkey) == ERROR_SUCCESS)
      {
         if(RegQueryValueEx(hkey, 
                            valueName.c_str(),
                            0, 
                            NULL,
                           (LPBYTE)&data, &dwSize) 
                                    == ERROR_SUCCESS){
            result = std::wstring(data,dwSize);
          }
      }
      return result;
  }

  void SetRegistryValue(HKEY baseKey, 
                       const std::wstring& key, 
                       const std::wstring& valueName, 
                       const std::string& value)
  {
      SetRegistryValue(baseKey, key, valueName, Convert(value));
  }
  void SetRegistryValue(HKEY baseKey, 
                       const std::wstring& key, 
                       const std::wstring& valueName, 
                       const std::wstring& value)
  {
    HKEY hkey;
    if(RegCreateKeyEx(baseKey, 
                      key.c_str(), 
                      0, NULL,
                      REG_OPTION_NON_VOLATILE,
                      KEY_WRITE,
                      NULL, &hkey,
                      NULL) == ERROR_SUCCESS)
    {
      RegSetValueEx(hkey, 
                    valueName.c_str(), 
                    0, 
                    REG_SZ, 
                    (LPBYTE)value.c_str(), 
                    (DWORD)((value.length()+1)*sizeof(wchar_t)) );
    
      RegCloseKey(hkey);
    }
  }

  void ClearRegistryValue(HKEY baseKey,
      const std::wstring& key,
      const std::wstring& valueName)
  {
    HKEY hkey;
    if(RegOpenKeyEx(baseKey, 
                    key.c_str(), 
                    0, KEY_WRITE, &hkey) == ERROR_SUCCESS)
    {
        RegDeleteValue(hkey, valueName.c_str());
        RegCloseKey(hkey);
    }
  }

};

#endif