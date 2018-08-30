#ifndef __I2C_H__
#define __I2C_H__

#include <string>

#include "types.hpp"
#include "logs.hpp"

class I2C:
    public Log
{
public:
    enum Req_t { REQ_UNDEF = 0, REQ_READ, REQ_WRITE };

    I2C(const std::string& bus, uint8_t addr): Log(false, true, true, true),
                                    i2c_bus(bus), i2c_addr(addr) { };
    int8_t i2c_transfer(Req_t req, uint8_t* data, uint8_t size) const;

private:
    const std::string i2c_bus;
    const uint8_t i2c_addr;

    I2C(): I2C("", 0) { };
   
    int8_t i2c_read(int32_t fd, uint8_t* data, uint8_t size) const ;
    int8_t i2c_write(int32_t fd, uint8_t* data, uint8_t size) const;
};

#endif
