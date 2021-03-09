#pragma once

#include "BME280_driver/bme280.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "freertos/task.h"
#include <cstdint>

class BME280
{
public:
  /**
   * @brief Construct a new BME280 object
   * @param deviceAddress The I2C address of the BME280 sensor (either 0x76 or 0x77)
   * @param sdaPin The GPIO pin of the I2C data line
   * @param sclPin The GPIO pin of the I2C clock line
   */
  BME280(i2c_port_t i2c_port, std::uint8_t device_address, gpio_num_t sda_pin, gpio_num_t scl_pin);
  BME280(const BME280& other) = delete;
  BME280(BME280&& other) noexcept = default;
  BME280& operator=(const BME280& other) = delete;
  BME280& operator=(BME280&& other) noexcept = delete;
  /**
   * @brief desctructs the BME280 object. Stops the i2c driver.
   */
  ~BME280();
  /**
   * @brief Get the Sensor Data object
   * @return SensorData The measuring point containing air pressure, temperature and humidity
   */
  bme280_data get_sensor_data();

private:
  /// port of the i2c driver
  const i2c_port_t i2c_port_;
  /// bme280 driver device representation
  bme280_dev device_{};
  /// I2C bus frequency (ESP32 maximum is 1 MHz)
  static const unsigned int i2c_freq__ = 100000;
  /// Number of ticks to wait
  static const unsigned int i2c_ticks_to_wait__ = 100 / portTICK_PERIOD_MS;

  /**
   * @brief Delay a task for a given number of milliseconds as required by BME280_driver API
   * @param period The time to wait in milliseconds
   */
  static void delay(uint32_t period);
  /**
   * @brief Read operation on the I2C as required by BME280_driver API
   * @param devAddress The sensor's address
   * @param regAddress The sensor's register to write to
   * @param regData The data to write into the register
   * @param dataLen The length of the data to write
   * @return int8_t 0 means success, 1 means failure
   */
  template <i2c_port_t I2C_PORT>
  static int8_t read_i2c(uint8_t dev_address, uint8_t reg_address, uint8_t* reg_data,
                          uint16_t data_len)
  {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    // master indicates a write (set LSB of device address to 0)
    i2c_master_write_byte(cmd, (dev_address << 1U) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg_address, true);
    i2c_master_start(cmd);
    // master indicates a read (set LSB of device address to 1)
    i2c_master_write_byte(cmd, (dev_address << 1U) | I2C_MASTER_READ, true);
    // NACK and stop condition indicate the end of the read operation
    if (data_len > 1)
    {
      i2c_master_read(cmd, reg_data, data_len - 1, I2C_MASTER_ACK);
    }
    i2c_master_read_byte(cmd, reg_data + data_len - 1, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    // execute command link
    esp_err_t esp_error = i2c_master_cmd_begin(I2C_PORT, cmd, i2c_ticks_to_wait__);
    i2c_cmd_link_delete(cmd);
    if (esp_error == ESP_OK)
    {
      return 0;
    }
    ESP_LOGE("BME280.hpp", "Error reading from I2C bus");
    return 1;
  }
  /**
   * @brief Write operation on the I2C bus as required by BME280_driver API
   * @param devAddress The sensor's address
   * @param regAddress The sensor's register to read from
   * @param regData The data which is read
   * @param datalen The length of the data to read
   * @return int8_t 0 means success, 1 means failure
   */
  template <i2c_port_t I2C_PORT>
  static int8_t write_i2c(uint8_t dev_address, uint8_t reg_address, uint8_t* reg_data,
                           uint16_t data_len)
  {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    // master indicates a write (set LSB of device address to 0)
    i2c_master_write_byte(cmd, (dev_address << 1U) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg_address, true);
    i2c_master_write(cmd, reg_data, data_len, true);
    i2c_master_stop(cmd);
    // execute command link
    esp_err_t esp_error = i2c_master_cmd_begin(I2C_PORT, cmd, i2c_ticks_to_wait__);
    i2c_cmd_link_delete(cmd);
    if (esp_error == ESP_OK)
    {
      return 0;
    }
    ESP_LOGE("BME280.hpp", "Error writing to I2C bus");
    return 1;
  }
  /**
   * @brief Initialize BME280 sensor in normal mode
   * @param devAddress The sensor's address
   */
  void init_sensor(std::uint8_t device_address);
};
