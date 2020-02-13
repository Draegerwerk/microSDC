#include "BME280.hpp"
#include "esp_log.h"
#include "freertos/task.h"

static constexpr const char* TAG = "BME280.cpp";


BME280::BME280(std::uint8_t deviceAddress, gpio_num_t sdaPin, gpio_num_t sclPin, i2c_port_t i2cPort)
{
  // i2cPort_ = i2cPort;
  i2c_config_t i2cConfig;
  i2cConfig.mode = I2C_MODE_MASTER;
  i2cConfig.sda_io_num = sdaPin;
  i2cConfig.sda_pullup_en = GPIO_PULLUP_ENABLE;
  i2cConfig.scl_io_num = sclPin;
  i2cConfig.scl_pullup_en = GPIO_PULLUP_ENABLE;
  i2cConfig.master.clk_speed = i2cFreq__;
  ESP_ERROR_CHECK(i2c_param_config(i2cPort_, &i2cConfig));
  ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0));
  initSensor(deviceAddress);
}

BME280::~BME280()
{
  i2c_driver_delete(i2cPort_);
}

void BME280::delay(uint32_t period)
{
  vTaskDelay(period / portTICK_PERIOD_MS);
}

int8_t BME280::readI2C(uint8_t devAddress, uint8_t regAddress, uint8_t* regData, uint16_t dataLen)
{
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  // master indicates a write (set LSB of device address to 0)
  i2c_master_write_byte(cmd, (devAddress << 1u) | I2C_MASTER_WRITE, true);
  i2c_master_write_byte(cmd, regAddress, true);
  // i2c_master_stop(cmd);
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
  esp_err_t espError = i2c_master_cmd_begin(i2cPort_, cmd, i2cTicksToWait__);
  i2c_cmd_link_delete(cmd);
  if (espError == ESP_OK)
  {
    return 0;
  }
  else
  {
    ESP_LOGE(TAG, "Error reading from I2C bus");
    ESP_ERROR_CHECK_WITHOUT_ABORT(espError);
    return 1;
  }
}

int8_t BME280::writeI2C(uint8_t devAddress, uint8_t regAddress, uint8_t* regData, uint16_t dataLen)
{
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  // master indicates a write (set LSB of device address to 0)
  i2c_master_write_byte(cmd, (devAddress << 1u) | I2C_MASTER_WRITE, true);
  i2c_master_write(cmd, regData, dataLen, true);
  i2c_master_stop(cmd);
  // execute command link
  esp_err_t espError = i2c_master_cmd_begin(i2cPort_, cmd, i2cTicksToWait__);
  i2c_cmd_link_delete(cmd);
  if (espError == ESP_OK)
  {
    return 0;
  }
  else
  {
    ESP_LOGE(TAG, "Error writing to I2C bus");
    return 1;
  }
}

void BME280::initSensor(std::uint8_t deviceAddress)
{
  // device_.chip_id =
  device_.dev_id = BME280_I2C_ADDR_PRIM;
  device_.intf = BME280_I2C_INTF;
  device_.read = readI2C;
  device_.write = writeI2C;
  device_.delay_ms = delay;
  // device_.calib_data =
  device_.settings.osr_h = BME280_OVERSAMPLING_1X;
  device_.settings.osr_p = BME280_OVERSAMPLING_16X;
  device_.settings.osr_t = BME280_OVERSAMPLING_2X;
  device_.settings.filter = BME280_FILTER_COEFF_16;
  device_.settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

  std::uint8_t settingsSelector = BME280_OSR_PRESS_SEL;
  settingsSelector |= BME280_OSR_TEMP_SEL;
  settingsSelector |= BME280_OSR_HUM_SEL;
  settingsSelector |= BME280_STANDBY_SEL;
  settingsSelector |= BME280_FILTER_SEL;
  std::uint8_t result = bme280_init(&device_);
  ESP_LOGI(TAG, "bm280_init returns: %d", result);
  bme280_set_sensor_settings(settingsSelector, &device_);
  bme280_set_sensor_mode(BME280_NORMAL_MODE, &device_);
}

BME280::SensorData BME280::getSensorData()
{
  bme280_data tempSensorData;
  bme280_get_sensor_data(BME280_ALL, &tempSensorData, &device_);
  SensorData sensorData;
  sensorData.pressure = tempSensorData.pressure;
  sensorData.temperature = tempSensorData.temperature;
  sensorData.humidity = tempSensorData.humidity;
  return sensorData;
}