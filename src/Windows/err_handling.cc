#include <Windows/err_handling.hh>

#include <string>
#include <map>

static bool initialize_os_specific_error_handling(std::map<unsigned int, std::string> &input) {
    const auto [iterator, success] = input.insert(
        {ERR_HANDLING_FAILED_GET_DEV_IFACE_INFO, "Failed to get the device interface for a reported camera."},
        {ERR_HANDLING_FETCH_NULL_INSTANCE_PATH, "Attempted to fetch the instance path of a device object before it has been set."}
    );

    if(!success){
        return false;
    }
    return true;
}