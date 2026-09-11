#ifndef EXCEPTIONS_HH
#define EXCEPTIONS_HH

#include <stdexcept>
#include <string>

class CustomException : public std::runtime_error {

    private:
    static char* generate_message(
        char* funct_name,
        size_t funct_name_len,
        char* issue,
        size_t issue_len
    );

    public:
    CustomException(
        char* funct_name,
        size_t funct_name_len,
        char* issue,
        size_t issue_len
    ):
    std::runtime_error(generate_message(funct_name, funct_name_len, issue, issue_len)){}
};
#endif