#include <nlohmann/json.hpp>
#include <err_handling.hh>
#include <Windows/err_handling.hh>

#include <concepts>
#include <format>
#include <type_traits>

#include <windows.h>
#include <initguid.h>
#include <ks.h>
#include <setupapi.h>
#include <cfgmgr32.h>
#include <devpkey.h>
#include <errhandlingapi.h>
#include <winerror.h>

template<typename T>
requires std::is_pointer_v<T>
struct RetValue {
    Error ret_status;
    T ret_value;
};

Error get_device_interface_struct(
    SP_
){

}

RetValue<
    std::shared_ptr<std::vector<SP_DEVICE_INTERFACE_DATA>>
    > get_cameras(){

    //Get device interface set
    //consisting of all plugged in cameras.
    HDEVINFO set = SetupDiGetClassDevsW(
        &KSCATEGORY_VIDEO_CAMERA,
        nullptr,
        nullptr,
        (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE)
    );

    SP_DEVICE_INTERFACE_DATA interface_data{};
    interface_data.cbSize = sizeof(interface_data);
    DWORD last_error;
    bool success;
    for (DWORD i = 0; SetupDiEnumDeviceInterfaces(
        set,
        nullptr,
        &KSCATEGORY_VIDEO_CAMERA,
        i,
        &interface_data
    ); i++){
        //Get the device's interface struct.
        //Windows uses a ton of buffers allocated to size.
        //Get used to this pattern.
        DWORD buff_size = 0;

        //Prelim to get the buff size;
        success = SetupDiGetDeviceInterfaceDetailW(
            set,
            &interface_data,
            nullptr,
            0,
            &buff_size,
            nullptr
        );
        last_error = GetLastError();
        if(!success && (last_error != ERROR_INSUFFICIENT_BUFFER)){
            std::vector<std::string> supp_err_strings = {
                std::format("GetLastError result: {}", last_error)
            };

            //What am I doing wrong here?
            return RetValue{
                Error(
                    ERR_HANDLING_FAILED_GET_DEV_IFACE_INFO,
                    std::unique_ptr<std::vector<std::string>>(supp_err_strings)
                ),
            nullptr};
        }

        std::vector<BYTE> buff(buff_size);
        ZeroMemory(buff.data(), buff_size);
        auto* detail = reinterpret_cast<SP_DEVICE_INTERFACE_DETAIL_DATA_W*>(buff.data());
        detail -> cbSize =  buff_size;

        success = SetupDiGetDeviceInterfaceDetailW(
            set,
            &interface_data,
            detail,
            0,
            &buff_size,
            nullptr
        );
        last_error = GetLastError();
        if(!success){
            std::vector<std::string> supp_err_strings = {
                std::format("GetLastError result: {}", last_error)
            };
        }

    }
}

static unsigned int get_cameras(nlohmann::json){

}