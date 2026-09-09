#include <err_handling.hh>
#include <memory>
#include <vector>
#include <sstream>

Error::Error(
    unsigned int errcode,
    std::unique_ptr<std::vector<std::string>> additional_clarifications
){
    this -> errcode = errcode;
    this -> additional_clarifications = std::move(additional_clarifications);
}


std::string Error::to_string(){
    std::ostringstream str_builder;

    std::string str1;
    try{
        str1 = err_string_map[this -> errcode];
    } catch (std::out_of_range) {
        str1 = std::string("UNKNOWN ERRCODE");
    }
    
    str_builder << str1;
    if( this -> additional_clarifications != nullptr){
        for(int i = 0; i < this -> additional_clarifications -> size(); i++){
            str_builder << (*(this -> additional_clarifications))[i];
        }
    }

    return str_builder.str();
}