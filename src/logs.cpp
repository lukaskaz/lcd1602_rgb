#include <iostream>
#include <sstream>

#include "logs.hpp"

typedef void (Log::*log_fn_t)(const std::string&, const std::string&) const;

void Log::log(log_t type, const std::string& source, const std::string& txt) const
{
    static log_fn_t log_fn[] = 
    {
        [LOG_NONE]   = NULL,
        [LOG_SIMPLE] = &Log::log_simple,
        [LOG_ERROR]  = &Log::log_error,
        [LOG_INFO]   = &Log::log_info,
        [LOG_DEBUG]  = &Log::log_debug,
    };
    static const uint8_t log_fn_last = sizeof(log_fn)/sizeof(*log_fn);

    if(type < log_fn_last && log_fn[type] != NULL) {
        (this->*log_fn[type])(source, txt);
    }
}

void Log::log_simple(const std::string& source, const std::string& txt) const
{
    if(SHOW_SIMPLE_LOGS == true) {
        std::cout<<"[LOG] "<<txt<<std::endl;
    }
}

void Log::log_error(const std::string& source, const std::string& txt) const
{
    if(SHOW_ERROR_LOGS == true) {
        std::cerr<<"[ERROR]["<<source<<"] "<<txt<<std::endl;
    }
}

void Log::log_info(const std::string& source, const std::string& txt) const
{
    if(SHOW_INFO_LOGS == true) {
        std::cerr<<"[INFO]["<<source<<"] "<<txt<<std::endl;
    }
}

void Log::log_debug(const std::string& source, const std::string& txt) const
{
    if(SHOW_DEBUG_LOGS == true) {
        std::cerr<<"[DEBUG]["<<source<<"] "<<txt<<std::endl;
    }
}

const std::string numToString(num_t type, const int32_t num)
{
    std::ostringstream stream;
    
    switch(type) {
        case NUM_DEC:
            stream<<std::dec<<num;
            break;
        case NUM_HEX:
            stream<<std::showbase<<std::hex<<num<<std::dec;
            break;
        default:
            // unsupported case, do nothing
            break;
    }
            
    return stream.str();
}

const std::string numToString(num_t type, const uint32_t num)
{
    std::ostringstream stream;
    
    switch(type) {
        case NUM_DEC:
            stream<<std::dec<<num;
            break;
        case NUM_HEX:
            stream<<std::showbase<<std::hex<<num<<std::dec;
            break;
        default:
            // unsupported case, do nothing
            break;
    }
            
    return stream.str();
}
