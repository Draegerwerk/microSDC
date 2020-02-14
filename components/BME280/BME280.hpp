#pragma once

#include "BME280_driver/bme280.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "freertos/task.h"
#include <cstdint>

/**
 * @brief Struct holding measurement of BME280 sensor
 *
 */
struct SensorData
{
  double pressure;
  double temperature;
  double humidity;
};

template <i2c_port_t I2C_PORT>
class BME280
{
public:
  /**
   * @brief Construct a new BME280 object
   *
   * @param deviceAddress The I2C address of the BME280 sensor (either 0x76 or 0x77)
   * @param sdaPin The GPIO pin of the I2C data line
   * @param sclPin The GPIO pin of the I2C clock line
   */
  BME280(std::uint8_t deviceAddress, gpio_num_t sdaPin, gpio_num_t sclPin);
  ~BME280();
  /**
   * @brief Get the Sensor Data object
   *
   * @return SensorData The measuring point containing air pressure, temperature and humidity
   */
  SensorData getSensorData();

private:
  bme280_dev device_;
  /// I2C bus frequency (ESP32 maximum is 1 MHz)
  static const unsigned int i2cFreq__ = 100000;
  /// Number of ticks to wait
  static const unsigned int i2cTicksToWait__ = 100 / portTICK_PERIOD_MS;

  /**
   * @brief Delay a task for a given number of milliseconds as required by BME280_driver API
   *
   * @param period The time to wait in milliseconds
   */
  static void delay(uint32_t period);
  /**
   * @brief Read operation on the I2C as required by BME280_driver API
   *
   * @param devAddress The sensor's address
   * @param regAddress The sensor's register to write to
   * @param regData The data to write into the register
   * @param dataLen The length of the data to write
   * @return int8_t 0 means success, 1 means failure
   */
  static int8_t readI2C(uint8_t devAddress, uint8_t regAddress, uint8_t* regData, uint16_t dataLen);
  /**
   * @brief Write operation on the I2C bus as required by BME280_driver API
   *
   * @param devAddress The sensor's address
   * @param regAddress The sensor's register to read from
   * @param regData The data which is read
   * @param datalen The length of the data to read
   * @return int8_t 0 means success, 1 means failure
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

template <i2c_port_t I2C_PORT>
BME280<I2C_PORT>::BME280(std::uint8_t deviceAddress, gpio_num_t sdaPin, gpio_num_t sclPin)
{
  i2c_config_t i2cConfig;
  i2cConfig.mode = I2C_MODE_MASTER;
  i2cConfig.sda_io_num = sdaPin;
  i2cConfig.sda_pullup_en = GPIO_PULLUP_ENABLE;
  i2cConfig.scl_io_num = sclPin;
  i2cConfig.scl_pullup_en = GPIO_PULLUP_ENABLE;
  i2cConfig.master.clk_speed = i2cFreq__;
  ESP_ERROR_CHECK(i2c_param_config(I2C_PORT, &i2cConfig));
  ESP_ERROR_CHECK(i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, 0, 0, 0));
  initSensor(deviceAddress);
}

template <i2c_port_t I2C_PORT>
BME280<I2C_PORT>::~BME280()
{
  i2c_driver_delete(I2C_PORT);
}

template <i2c_port_t I2C_PORT>
SensorData BME280<I2C_PORT>::getSensorData()
{
  bme280_data tempSensorData;
  bme280_get_sensor_data(BME280_ALL, &tempSensorData, &device_);
  SensorData sensorData;
  sensorData.pressure = tempSensorData.pressure;
  sensorData.temperature = tempSensorData.temperature;
  sensorData.humidity = tempSensorData.humidity;
  return sensorData;
}

template <i2c_port_t I2C_PORT>
void BME280<I2C_PORT>::delay(uint32_t period)
{
  vTaskDelay(period / portTICK_PERIOD_MS);
}

template <i2c_port_t I2C_PORT>
int8_t BME280<I2C_PORT>::readI2C(uint8_t devAddress, uint8_t regAddress, uint8_t* regData,
                                 uint16_t dataLen)
{
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  // master indicates a write (set LSB of device address to 0)
  i2c_master_write_byte(cmd, (devAddress << 1u) | I2C_MASTER_WRITE, true);
  i2c_master_write_byte(cmd, regAddress, true);
  i2c_master_start(cmd);
  // master indicates a read (set LSB of device address to 1)
  i2c_master_write_byte(cmd, (devAddress << 1u) | I2C_MASTER_READ, true);
  // NACK and stop condition indicate the end of the read operation
  if (dataLen > 1)
  {
    i2c_master_read(cmd, regData, dataLen - 1, I2C_MASTER_ACK);
  }
  i2c_master_read_byte(cmd, regData + dataLen - 1, I2C_MASTER_NACK);
  i2c_master_stop(cmd);
  // execute command link
  esp_err_t espError = i2c_master_cmd_begin(I2C_PORT, cmd, i2cTicksToWait__);
  i2c_cmd_link_delete(cmd);
  if (espError == ESP_OK)
  {
    return 0;
  }
  else
  {
    ESP_LOGE("BME280.hpp", "Error reading from I2C bus");
    return 1;
  }
}

template <i2c_port_t I2C_PORT>
int8_t BME280<I2C_PORT>::writeI2C(uint8_t devAddress, uint8_t regAddress, uint8_t* regData,
                                  uint16_t dataLen)
{
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  // master indicates a write (set LSB of device address to 0)
  i2c_master_write_byte(cmd, (devAddress << 1u) | I2C_MASTER_WRITE, true);
  i2c_master_write_byte(cmd, regAddress, true);
  i2c_master_write(cmd, regData, dataLen, true);
  i2c_master_stop(cmd);
  // execute command link
  esp_err_t espError = i2c_master_cmd_begin(I2C_PORT, cmd, i2cTicksToWait__);
  i2c_cmd_link_delete(cmd);
  if (espError == ESP_OK)
  {
    return 0;
  }
  else
  {
    ESP_LOGE("BME280.hpp", "Error writing to I2C bus");
    return 1;
  }
}

template <i2c_port_t I2C_PORT>
void BME280<I2C_PORT>::initSensor(std::uint8_t deviceAddress)
{
  device_.dev_id = BME280_I2C_ADDR_PRIM;
  device_.intf = BME280_I2C_INTF;
  device_.read = readI2C;
  device_.write = writeI2C;
  device_.delay_ms = delay;
  device_.settings.osr_h = BME280_OVERSAMPLING_1X;
  device_.settings.osr_p = BME280_OVERSAMPLING_16X;
  device_.settings.osr_t = BME280_OVERSAMPLING_2X;
  device_.settings.filter = BME280_FILTER_COEFF_16;
  device_.settings.standby_time = BME280_STANDBY_TIME_62_5_MS;
  std::uint8_t ret = bme280_init(&device_);
  if (ret != 0)
  {
    ESP_LOGE("BME280.hpp", "BM280 sensor init failed");
  }
  std::uint8_t settingsSelector = BME280_OSR_PRESS_SEL;
  settingsSelector |= BME280_OSR_TEMP_SEL;
  settingsSelector |= BME280_OSR_HUM_SEL;
  settingsSelector |= BME280_STANDBY_SEL;
  settingsSelector |= BME280_FILTER_SEL;
  bme280_set_sensor_settings(settingsSelector, &device_);
  bme280_set_sensor_mode(BME280_NORMAL_MODE, &device_);
}