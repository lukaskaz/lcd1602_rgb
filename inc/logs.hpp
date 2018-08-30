#ifndef __LOGS_H__
#define __LOGS_H__

class Log
{
public:
    Log(): Log(true, false, false, false) {};
    Log(bool simple, bool error, bool info, bool debug): SHOW_SIMPLE_LOGS(simple),
        SHOW_ERROR_LOGS(error), SHOW_INFO_LOGS(info), SHOW_DEBUG_LOGS(debug) {};

    enum log_t{ LOG_NONE = 0, LOG_SIMPLE, LOG_ERROR, LOG_INFO, LOG_DEBUG };

    void log(const std::string& txt) const { log_simple("", txt); };
    void log(log_t type, const std::string& source, const std::string& txt) const;

private:
    const bool SHOW_SIMPLE_LOGS;
    const bool SHOW_ERROR_LOGS;
    const bool SHOW_INFO_LOGS;
    const bool SHOW_DEBUG_LOGS;

    void log_simple(const std::string& source, const std::string& txt) const;
    void log_error(const std::string& source, const std::string& txt) const;
    void log_info(const std::string& source, const std::string& txt) const;
    void log_debug(const std::string& source, const std::string& txt) const;
};

typedef enum { NUM_NONE = 0, NUM_DEC, NUM_HEX } num_t;

const std::string numToString(num_t type, const int32_t num);
const std::string numToString(num_t type, const uint32_t num);


#endif
