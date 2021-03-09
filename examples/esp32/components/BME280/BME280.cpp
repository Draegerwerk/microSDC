#include "BME280.hpp"

BME280::BME280(i2c_port_t i2c_port, std::uint8_t device_address, gpio_num_t sda_pin, gpio_num_t scl_pin)
  : i2c_port_(i2c_port)
{
  i2c_config_t i2c_config{};
  i2c_config.mode = I2C_MODE_MASTER;
  i2c_config.sda_io_num = sda_pin;
  i2c_config.sda_pullup_en = true;
  i2c_config.scl_io_num = scl_pin;
  i2c_config.scl_pullup_en = true;
  i2c_config.master.clk_speed = i2c_freq__;
  ESP_ERROR_CHECK(i2c_param_config(i2c_port_, &i2c_config));
  ESP_ERROR_CHECK(i2c_driver_install(i2c_port_, I2C_MODE_MASTER, 0, 0, 0));
  init_sensor(device_address);
}

BME280::~BME280()
{
  i2c_driver_delete(i2c_port_);
}

bme280_data BME280::get_sensor_data()
{
  bme280_data temp_sensor_data{};
  bme280_get_sensor_data(BME280_ALL, &temp_sensor_data, &device_);
  return temp_sensor_data;
}

void BME280::delay(uint32_t period)
{
  vTaskDelay(period / portTICK_PERIOD_MS);
}

void BME280::init_sensor(std::uint8_t  /*device_address*/)
{
  device_.dev_id = BME280_I2C_ADDR_PRIM;
  device_.intf = BME280_I2C_INTF;
  switch (i2c_port_)
  {
    case I2C_NUM_0: {
      device_.read = read_i2c<I2C_NUM_0>;
      device_.write = write_i2c<I2C_NUM_0>;
      break;
    }
    case I2C_NUM_1: {
      device_.read = read_i2c<I2C_NUM_1>;
      device_.write = write_i2c<I2C_NUM_1>;
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
  std::uint8_t settings_selector = BME280_OSR_PRESS_SEL;
  settings_selector |= BME280_OSR_TEMP_SEL;
  settings_selector |= BME280_OSR_HUM_SEL;
  settings_selector |= BME280_STANDBY_SEL;
  settings_selector |= BME280_FILTER_SEL;
  bme280_set_sensor_settings(settings_selector, &device_);
  bme280_set_sensor_mode(BME280_NORMAL_MODE, &device_);
}