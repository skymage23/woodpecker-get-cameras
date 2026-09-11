
//Project:
#include <return_handling.hh>
#include <err_handling.hh>
#include <Windows/err_handling.hh>
#include <return_handling.hh>

//Project dependencies:
#include <nlohmann/json.hpp>

//C++ std lib:
#include <concepts>
#include <format>
#include <memory>
#include <string>
#include <type_traits>

//Windows SDK:
#include <windows.h>
#include <initguid.h>
#include <ks.h>
#include <setupapi.h>
#include <cfgmgr32.h>
#include <devpkey.h>
#include <errhandlingapi.h>
#include <winerror.h>


class Device {
    private:
    HDEVINFO set;
    SP_DEVICE_INTERFACE_DATA interface_data;

    const DEVINST* device_instance;

    
    const wchar_t* instance_path;
    const ULONG instance_path_size;

    private:
    void __set_interface_path(){
        DWORD buff_size = 0;
        ULONG winapi_error;
        //Prelim to get the buff size;
        bool success = SetupDiGetDeviceInterfaceDetailW(
            this -> set,
            &(this -> interface_data),
            nullptr,
            0,
            &buff_size,
            nullptr
        );
        winapi_error = GetLastError();
        if(!success && (winapi_error != ERROR_INSUFFICIENT_BUFFER)){
            std::vector<std::string> supp_err_strings = {
                std::format("GetLastError result: {}", winapi_error)
            };

            return {
                Error(
                    ERR_HANDLING_FAILED_GET_DEV_IFACE_INFO,
                    std::make_unique<std::vector<std::string>>(std::move(supp_err_strings))
                ),
                nullptr
            };
        }

        std::vector<BYTE> buff(buff_size);
        ZeroMemory(buff.data(), buff_size);
        auto* detail = reinterpret_cast<SP_DEVICE_INTERFACE_DETAIL_DATA_W*>(buff.data());
        detail -> cbSize =  buff_size;

        success = SetupDiGetDeviceInterfaceDetailW(
            *set,
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
            
            return {
                Error(
                    ERR_HANDLING_FAILED_GET_DEV_IFACE_INFO,
                    std::make_unique<std::vector<std::string>>(std::move(supp_err_strings))
                ),
                nullptr
            };
        }//end i
        
    }

    public:
    Device(HDEVINFO set, SP_DEVICE_INTERFACE_DATA interface_data){
        this -> set = set;
        this -> interface_data = interface_data:

    };

    Device(const Device& righty);

    Device(const Device&& righty);

    ~Device();

    bool set_interface_data(SP_DEVICE_INTERFACE_DATA* data_struct){
        if(this -> interface_data_set){
            return false;
        }

        this -> interface_data = data_struct;
        this -> interface_data_set = true;
        return true;
    }
    
    RetValue<SP_DEVICE_INTERFACE_DATA> get_interface_data(){
        return *(this -> interface_data);
    }

    bool set_interface_path_data(const wchar_t* path, ULONG size){
        if(this -> instance_path_set){
            return false;
        }

        wchar_t* new_buff;
        std::memcpy(new_buff, path, size);

        this -> instance_path = new_buff;
        this -> instance_path_size = size;
        this -> instance_path_set = true;
        return true;
    }

    RetValue<std::unique_ptr<std::wstring>> get_instance_path(){
        if ( this -> instance_path == nullptr){
            return RetValue<std::unique_ptr<std::wstring>> {
                Error(
                    ERR_HANDLING_FETCH_NULL_INSTANCE_PATH,
                    nullptr
                ),
                nullptr
            };
        }
        return RetValue<std::unique_ptr<std::wstring>>{
            Error(ERR_HANDLING_SUCCESS, nullptr),
            std::make_unique<std::wstring>(std::wstring(this -> instance_path))
        };
    }
    

    bool set_device_instance(DEVINST* data_struct){
        if(this -> device_instance_set){
            return false;
        }

        this -> device_instance = data_struct;
        this -> device_instance_set = true;
        return true;
    }

    DEVINST get_device_instance(){
        return *(this -> device_instance);
    }
};

//Error get_device_interface_struct(
    //SP_
//){

//}

RetValue<
    std::shared_ptr<std::vector<SP_DEVICE_INTERFACE_DATA>>
    > get_device_interface_data_structs(HDEVINFO* set){

    std::shared_ptr<std::vector<SP_DEVICE_INTERFACE_DATA>> retval(
            new std::vector<SP_DEVICE_INTERFACE_DATA>()
    );

    SP_DEVICE_INTERFACE_DATA interface_data{};
    interface_data.cbSize = sizeof(interface_data);
    DWORD last_error;
    bool success;
    for (DWORD i = 0; SetupDiEnumDeviceInterfaces(
        *set,
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
            *set,
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

            return {
                Error(
                    ERR_HANDLING_FAILED_GET_DEV_IFACE_INFO,
                    std::make_unique<std::vector<std::string>>(std::move(supp_err_strings))
                ),
                nullptr
            };
        }

        std::vector<BYTE> buff(buff_size);
        ZeroMemory(buff.data(), buff_size);
        auto* detail = reinterpret_cast<SP_DEVICE_INTERFACE_DETAIL_DATA_W*>(buff.data());
        detail -> cbSize =  buff_size;

        success = SetupDiGetDeviceInterfaceDetailW(
            *set,
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
            
            return {
                Error(
                    ERR_HANDLING_FAILED_GET_DEV_IFACE_INFO,
                    std::make_unique<std::vector<std::string>>(std::move(supp_err_strings))
                ),
                nullptr
            };
        }//end if

        Device dev{
            interface_data = interface_data,
            interface_detail_data = detail
        }

    } //end for
} //end get_cameras

static RetValue<std::shared_ptr<std::vector<std::string>>> get_cameras(nlohmann::json){
    //Get device interface set
    //consisting of all plugged in cameras.
    HDEVINFO set = SetupDiGetClassDevsW(
        &KSCATEGORY_VIDEO_CAMERA,
        nullptr,
        nullptr,
        (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE)
    );

    RetValue<std::shared_ptr<std::vector<SP_DEVICE_INTERFACE_DATA>>> retval = get_device_interface_data_structs(&set);
    if (retval.ret_status.errcode != ERR_HANDLING_SUCCESS) {
        return  {
            retval.ret_status,
            nullptr
        };
    }

    std::shared_ptr<std::vector<SP_DEVICE_INTERFACE_DATA>> interface_data = retval.ret_value;

}