#pragma once

#include "BME280_driver/bme280.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include <cstdint>

class BME280
{
public:
  BME280(std::uint8_t deviceAddress, gpio_num_t sdaPin, gpio_num_t sclPin,
         i2c_port_t i2cPort = I2C_NUM_0);
  ~BME280();

  struct SensorData
  {
    double pressure;
    double temperature;
    double humidity;
  };
  SensorData getSensorData();

private:
  bme280_dev device_;
  static const i2c_port_t i2cPort_ = I2C_NUM_0;
  static const unsigned int i2cFreq__ = 10000;
  static const unsigned int i2cTicksToWait__ = 10 / portTICK_PERIOD_MS;

  /**
   * @brief Delay a task for a given number of milliseconds as required by BME280_driver API
   *
   * @param period The time to wait in milliseconds
   */
  static void delay(uint32_t period);
  /**
   * @brief Read operation on the I2C as required by BME280_driver API
   *
   * @param devAddress
   * @param regAddress
   * @param regData
   * @param dataLen
   * @return int8_t
   */
  static int8_t readI2C(uint8_t devAddress, uint8_t regAddress, uint8_t* regData, uint16_t dataLen);
  /**
   * @brief Write operation on the I2C bus as required by BME280_driver API
   *
   * @param devAddress
   * @param regAddress
   * @param regData
   * @param datalen
   * @return int8_t
   */
  static int8_t writeI2C(uint8_t devAddress, uint8_t regAddress, uint8_t* regData,
                         uint16_t datalen);
  /**
   * @brief Initialize BME280 sensor in normal mode
   *
   * @param devAddress The sensor's address
   */
  void initSensor(std::uint8_t devAddress);
};