#pragma once

#include "bme280.hpp"
#include "driver/gpio.h"
#include "driver/i2c.h"

class BME280 {
public:
  BME280(gpio_num_t sdaPin, gpio_num_t sclPin);

private:
};

BME280::BME280(gpio_num_t sdaPin, gpio_num_t sclPin) {
  i2c_config_t i2cConfig = {.mode = I2C_MODE_MASTER,
                            .sda_io_num = sdaPin,
                            .sda_pullup_en = GPIO_PULLUP_ENABLE,
                            .scl_io_num = sclPin,
                            .scl_pullup_en = GPIO_PULLUP_ENABLE,
                            .master.clk_speed = 1000000};
  i2c_param_config(I2C_NUM_0, &i2cConfig);
  i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
}