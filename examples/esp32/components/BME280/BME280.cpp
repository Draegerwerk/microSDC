#include "BME280.hpp"

BME280::BME280(i2c_port_t i2cPort, std::uint8_t deviceAddress, gpio_num_t sdaPin, gpio_num_t sclPin)
  : i2cPort_(i2cPort)
{
  i2c_config_t i2cConfig{};
  i2cConfig.mode = I2C_MODE_MASTER;
  i2cConfig.sda_io_num = sdaPin;
  i2cConfig.sda_pullup_en = GPIO_PULLUP_ENABLE;
  i2cConfig.scl_io_num = sclPin;
  i2cConfig.scl_pullup_en = GPIO_PULLUP_ENABLE;
  i2cConfig.master.clk_speed = i2cFreq__;
  ESP_ERROR_CHECK(i2c_param_config(i2cPort_, &i2cConfig));
  ESP_ERROR_CHECK(i2c_driver_install(i2cPort_, I2C_MODE_MASTER, 0, 0, 0));
  initSensor(deviceAddress);
}

BME280::~BME280()
{
  i2c_driver_delete(i2cPort_);
}

bme280_data BME280::getSensorData()
{
  bme280_data tempSensorData{};
  bme280_get_sensor_data(BME280_ALL, &tempSensorData, &device_);
  return tempSensorData;
}

void BME280::delay(uint32_t period)
{
  vTaskDelay(period / portTICK_PERIOD_MS);
}

void BME280::initSensor(std::uint8_t deviceAddress)
{
  device_.dev_id = BME280_I2C_ADDR_PRIM;
  device_.intf = BME280_I2C_INTF;
  switch (i2cPort_)
  {
    case I2C_NUM_0: {
      device_.read = readI2C<I2C_NUM_0>;
      device_.write = writeI2C<I2C_NUM_0>;
      break;
    }
    case I2C_NUM_1: {
      device_.read = readI2C<I2C_NUM_1>;
      device_.write = writeI2C<I2C_NUM_1>;
      break;
    }
    default: {
      ESP_LOGE("BME280.hpp", "Unknown I2C port type. Must be one of I2C_NUM_0, I2C_NUM_1");
    }
  }
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