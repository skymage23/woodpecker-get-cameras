#include <Windows/err_handling.hh>

#include <string>
#include <map>

static bool initialize_os_specific_error_handling(std::map<unsigned int, std::string> &input) {
    const auto [iterator, success] = input.insert(
        {ERR_HANDLING_FAILED_GET_DEV_IFACE_INFO, "Failed to get the device interface for a reported camera."}
    );

    if(!success){
        return false;
    }
    return true;
}