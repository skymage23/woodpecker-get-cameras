#include <exceptions.hh>
#include <memory.hh>

#include <cstdio>

char* CustomException::generate_message(
    char* funct_name,
    size_t funct_name_len,
    char* issue,
    size_t issue_len
){
    //Is there always a way to know for sure the type of errors we
    //will see at compile time? Not always. 
    constexpr char* format_string = "Function: %s, Issue: %s\n\n";
    //Allocates a buffer.

    size_t buffer_size = funct_name_len + issue_len + sizeof(format_string);
    char* buffer = nullptr;
    PROTECTED_CALLOC(buffer, char, buffer_size, 1);
    if (std::snprintf(buffer, buffer_size, format_string) < 0){
        throw  std::runtime_error("CustomException::generate_error: text encoding issue.");
    }
    return buffer;
}