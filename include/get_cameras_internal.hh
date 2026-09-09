#ifndef GET_CAMERAS_INTERNAL_HH
#define GET_CAMERAS_INTERNAL_HH
//Nothing here needs to be extern "C".
#include<string>
#include<nlohmann/json.hpp>
#include<err_handling.hh>

//error handling:
static std::string get_err_message(unsigned int input);

//Returns error codes. 
static unsigned int get_cameras(nlohmann::json);



#endif