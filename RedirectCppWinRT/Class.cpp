#include "pch.h"
#include "Class.h"
#include "Class.g.cpp"

namespace winrt::RedirectCppWinRT::implementation
{
    int32_t Class::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void Class::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }
}

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <fileapifromapp.h>
#include <mfapi.h>

// Same signature are CreateFile2, forward it on to ...FromApp
HANDLE WINAPI CreateFile2Forwarder(LPCWSTR lpFileName, DWORD dwDesiredAccess,
    DWORD dwShareMode, DWORD dwCreationDisposition, LPCREATEFILE2_EXTENDED_PARAMETERS pCreateExParams)
{
    return CreateFile2FromAppW(lpFileName, dwDesiredAccess, dwShareMode, dwCreationDisposition, pCreateExParams);
}

// Same signature are DeleteFileW, forward it on to ...FromApp
BOOL WINAPI DeleteFileWForwarder(LPCWSTR lpFileName)
{
    return DeleteFileFromAppW(lpFileName);
}

// Same signature are GetFileAttributesExW, forward it on to ...FromApp
BOOL WINAPI GetFileAttributesExWForwarder(LPCWSTR lpFileName, GET_FILEEX_INFO_LEVELS fInfoLevelId,
    LPVOID lpFileInformation)
{
    return GetFileAttributesExFromAppW(lpFileName, fInfoLevelId, lpFileInformation);
}

STDAPI MFStartupForwarder(ULONG Version, DWORD dwFlags)
{
    OutputDebugStringA("Someone called MFStartup!\n");

    return MFStartup(Version, dwFlags);
}

// List of {exporting DLL}, {exported function name}, {replacement function pointer}
const REDIRECTION_FUNCTION_DESCRIPTOR RedirectedFunctions[] =
{
    { "api-ms-win-core-file-l1-2-1.dll", "CreateFile2", &CreateFile2Forwarder },
    { "api-ms-win-core-file-l1-2-1.dll", "DeleteFileW", &DeleteFileWForwarder },
    { "api-ms-win-core-file-l1-2-1.dll", "GetFileAttributesExW", &GetFileAttributesExWForwarder },
    { "mfplat.dll", "MFStartup", &MFStartupForwarder },
};

// The exported table, with version and size information.
extern "C" __declspec(dllexport) const REDIRECTION_DESCRIPTOR __RedirectionInformation__ =
{
    CURRENT_IMPORT_REDIRECTION_VERSION, // version number of the structure
    ARRAYSIZE(RedirectedFunctions),
    RedirectedFunctions
};
