#ifndef WINDOWS_EXCEPTIONS_HH
#define WINDOWS_EXCEPTIONS_HH

#include "../exceptions.hh"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <errhandlingapi.h>

class Win32CustomException : public CustomException {
    private:
    static char* win32_errcode_to_string(DWORD errcode);

    Win32CustomException(char* funct_name, size_t funct_name_len, char* issue);

    public:
    Win32CustomException(char* funct_name, size_t funct_name_len, DWORD errcode);
};

#endif
