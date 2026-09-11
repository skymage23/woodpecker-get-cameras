#include <Windows/exceptions.hh>
#include <memory.hh>

#include <cstdio>
#include <cstring>
#include <limits>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <errhandlingapi.h>

char* Win32CustomException::win32_errcode_to_string(DWORD errcode) {
    constexpr char format_msg_failure[] = "\"FormatMessageW\" failure: ";
    constexpr char sys_msg_too_big[] = "Message from Windows system is too big.";
    constexpr char unknown_error[] = "Unknown error: ";
    constexpr char utf8_convert_failure[] = "UTF-8 conversion failed.";

    constexpr size_t format_msg_fail_len = sizeof(format_msg_failure) - 1;
    constexpr size_t sys_msg_too_big_len = sizeof(sys_msg_too_big) - 1;
    constexpr size_t unknown_error_len = sizeof(unknown_error) - 1;
    constexpr size_t utf8_convert_failure_len = sizeof(utf8_convert_failure) - 1;
    constexpr size_t max_dword_ascii = std::numeric_limits<DWORD>::digits10 + 2;

    wchar_t* wide = nullptr;
    const DWORD n = FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        errcode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPWSTR>(&wide),
        0,
        nullptr
    );

    if (n != 0 && wide != nullptr) {
        DWORD wide_len = n;
        while (wide_len > 0 && (wide[wide_len - 1] == L'\r' || wide[wide_len - 1] == L'\n')) {
            --wide_len;
        }

        const int needed = WideCharToMultiByte(
            CP_UTF8,
            0,
            wide,
            static_cast<int>(wide_len),
            nullptr,
            0,
            nullptr,
            nullptr
        );

        char* utf8 = nullptr;
        if (needed > 0) {
            PROTECTED_CALLOC(utf8, char, needed + 1, 1);
            WideCharToMultiByte(
                CP_UTF8,
                0,
                wide,
                static_cast<int>(wide_len),
                utf8,
                needed,
                nullptr,
                nullptr
            );
        }

        LocalFree(wide);

        if (utf8 != nullptr) {
            return utf8;
        }

        char* buffer = nullptr;
        PROTECTED_CALLOC(buffer, char, format_msg_fail_len + utf8_convert_failure_len + 1, 1);
        memcpy(buffer, format_msg_failure, format_msg_fail_len);
        memcpy(buffer + format_msg_fail_len, utf8_convert_failure, utf8_convert_failure_len);
        return buffer;
    }

    const DWORD fmt_err = GetLastError();
    LocalFree(wide);

    if (fmt_err == ERROR_MORE_DATA) {
        char* buffer = nullptr;
        PROTECTED_CALLOC(buffer, char, format_msg_fail_len + sys_msg_too_big_len + 1, 1);
        memcpy(buffer, format_msg_failure, format_msg_fail_len);
        memcpy(buffer + format_msg_fail_len, sys_msg_too_big, sys_msg_too_big_len);
        return buffer;
    }

    char* buffer = nullptr;
    PROTECTED_CALLOC(buffer, char, format_msg_fail_len + unknown_error_len + max_dword_ascii + 1, 1);
    memcpy(buffer, format_msg_failure, format_msg_fail_len);
    memcpy(buffer + format_msg_fail_len, unknown_error, unknown_error_len);
    snprintf(
        buffer + format_msg_fail_len + unknown_error_len,
        max_dword_ascii + 1,
        "%lu",
        static_cast<unsigned long>(fmt_err)
    );
    return buffer;
}

Win32CustomException::Win32CustomException(
    char* funct_name,
    size_t funct_name_len,
    char* issue
)
try
    : CustomException(
          funct_name,
          funct_name_len,
          issue,
          issue != nullptr ? strlen(issue) : 0
      )
{
    free(issue);
}
catch (...) {
    free(issue);
    throw;
}

Win32CustomException::Win32CustomException(
    char* funct_name,
    size_t funct_name_len,
    DWORD errcode
)
    : Win32CustomException(funct_name, funct_name_len, win32_errcode_to_string(errcode))
{}
