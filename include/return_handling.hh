#ifndef RETURN_HANDLING_HH
#define RETURN_HANDLING_HH

template<typename T>
struct RetValue {
    Error ret_status;
    T ret_value;
};

#endif