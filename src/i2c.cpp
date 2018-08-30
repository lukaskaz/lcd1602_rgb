#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "i2c.hpp"

typedef int8_t (I2C::*i2c_transfer_fn_t)(int32_t, uint8_t*, uint8_t) const;


int8_t I2C::i2c_read(int32_t fd, uint8_t* data, uint8_t size) const
{
    int8_t ret = (-1);

    if(data != NULL && size > 1) {
        if(write(fd, data, 1) == 1) {
            data++;
            size--;

            if(read(fd, data, size) == size) {
                ret = 0;
            }
            else {
                log(LOG_ERROR, __func__, "Failed to read all data");
            }
        }
        else {
            log(LOG_ERROR, __func__, "Failed to write all data");
        }

    }

    return ret;
}

int8_t I2C::i2c_write(int32_t fd, uint8_t* data, uint8_t size) const
{
    int8_t ret = (-1);

    if(data != NULL && size > 0) {
        if(write(fd, data, size) == size) {
            ret = 0;
        }
        else {
            log(LOG_ERROR, __func__, "Failed to write all data");
        }
    }

    return ret;
}

int8_t I2C::i2c_transfer(Req_t req, uint8_t* data, uint8_t size) const
{
    int8_t ret = (-1);
    int32_t fd = (-1);
    static const i2c_transfer_fn_t i2c_fn[] = 
    {
       [REQ_UNDEF] = NULL,
       [REQ_READ]  = &I2C::i2c_read,
       [REQ_WRITE] = &I2C::i2c_write
    };
    static const uint8_t i2c_fn_last = sizeof(i2c_fn)/sizeof(*i2c_fn);

    if (req < i2c_fn_last && i2c_fn[req] != NULL) {
        if(i2c_bus.empty() == false && i2c_addr != 0) {
            fd = open(i2c_bus.c_str(), O_RDWR);
            if(fd >= 0) {
                if(ioctl(fd, I2C_SLAVE, i2c_addr) >= 0) {
                    ret = (this->*i2c_fn[req])(fd, data, size);
                }
                else {
                    log(LOG_ERROR, __func__, "Failed to prepare i2c to " \
                        + std::string("communicate to a slave device with addr: ") \
                        + numToString(NUM_HEX, i2c_addr));
                }

                close(fd);
            }
            else {
                log(LOG_ERROR, __func__, "Failed to open bus: " + i2c_bus);
            }
        }
    }
    
    return ret;
}
