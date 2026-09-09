#ifndef ERR_HANDLING_HH
#define ERR_HANDLING_HH

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#define ERR_HANDLING_SUCCESS 0
/*typedef struct {
    unsigned int errcode;
    std::string additional_params[];
} Error;*/

class Error {
    public:
    unsigned int errcode;

    private:
    std::unique_ptr<std::vector<std::string>> additional_clarifications;

    //Params is nullable.
    public: 
    Error(unsigned int errcode, std::unique_ptr<std::vector<std::string>> params);
    std::string to_string();
};



static bool initialize_os_specific_error_handling(std::map<unsigned int, std::string> &input);


static void catastrophic_error(const char* message){
    std::cerr << "An unrecoverable error occurred: " << message << "\n\n";
    std::exit(EXIT_FAILURE);
}

static std::map<unsigned int, std::string> get_err_string_map(){
    std::map<unsigned int, std::string> retval;

    //Add common
    const auto [iterator, success] = retval.insert(
        {ERR_HANDLING_SUCCESS, "Success"}
    );

    if (!success || (!initialize_os_specific_error_handling(retval))){
        catastrophic_error("Unable to initialize error string map.");
    }

    return retval;
}

static std::map<unsigned int, std::string> err_string_map = get_err_string_map();

#endif