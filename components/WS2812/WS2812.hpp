#pragma once

#include "driver/rmt.h"
#include <array>

/**
 * @brief The order of RGB color values the LEDs expect
 *
 */
enum class RGBOrder
{
  RGB,
  RBG,
  GRB,
  GBR,
  BRG,
  BGR
};

/**
 * @brief The Struct holding a RGB value
 *
 */
struct Color
{
  std::uint8_t red = 0;
  std::uint8_t green = 0;
  std::uint8_t blue = 0;
};

template <unsigned int LED_COUNT, RGBOrder RGB_ORDER>
class WS2812
{
public:
  /**
   * @brief Construct a new WS2812 object
   *
   * @param gpioPin The GPIO pin which is connect to a WS2812 LED strip
   * @param rmtChannel The RMT channel to use
   */
  WS2812(gpio_num_t gpioPin, rmt_channel_t rmtChannel = RMT_CHANNEL_0);

  /**
   * @brief Lights the LED strip up corresponding to the provided array of Colors
   *
   * @param ledState The array holding RGB values corresponding to the connected LEDs
   */
  void writeLeds(const std::array<Color, LED_COUNT>& ledState);


private:
  rmt_channel_t rmtChannel_;
  static const uint16_t bitsPerLedCmd_ = 24;
  // These values are determined by measuring pulse timing with logic analyzer and adjusting to
  // match datasheet.
  static const uint16_t highTime0_ = 14;  // 0 bit high time
  static const uint16_t highTime1_ = 52;  // 1 bit high time
  static const uint16_t lowTimeAny_ = 52; // low time for either bit

  /**
   * @brief Sets up the RMT data buffer
   *
   * @param rmtDataBuffer The RMT data buffer to write to
   * @param ledState The array holding RGB colors corresponding to the connected LEDs
   */
  void setupRmtDataBuffer(rmt_item32_t* rmtDataBuffer, std::array<Color, LED_COUNT> ledState);
  std::array<uint32_t, LED_COUNT>
  composeRgbValues(const std::array<Color, LED_COUNT>& ledState) const;
};

template <unsigned int LED_COUNT, RGBOrder RGB_ORDER>
WS2812<LED_COUNT, RGB_ORDER>::WS2812(gpio_num_t gpioPin, rmt_channel_t rmtChannel)
{
  rmtChannel_ = rmtChannel;
  rmt_config_t rmtConfig;
  rmtConfig.rmt_mode = RMT_MODE_TX;
  rmtConfig.channel = rmtChannel;
  rmtConfig.gpio_num = gpioPin;
  rmtConfig.mem_block_num = 3;
  rmtConfig.tx_config.loop_en = false;
  rmtConfig.tx_config.carrier_en = false;
  rmtConfig.tx_config.idle_output_en = true;
  rmtConfig.tx_config.idle_level = static_cast<rmt_idle_level_t>(0);
  rmtConfig.clk_div = 2;
  ESP_ERROR_CHECK(rmt_config(&rmtConfig));
  ESP_ERROR_CHECK(rmt_driver_install(rmtChannel_, 0, 0));
}

template <unsigned int LED_COUNT, RGBOrder RGB_ORDER>
void WS2812<LED_COUNT, RGB_ORDER>::writeLeds(const std::array<Color, LED_COUNT>& ledState)
{
  rmt_item32_t rmtDataBuffer[LED_COUNT * bitsPerLedCmd_] = {};
  setupRmtDataBuffer(rmtDataBuffer, ledState);
  ESP_ERROR_CHECK(rmt_write_items(rmtChannel_, rmtDataBuffer, LED_COUNT * bitsPerLedCmd_, false));
  ESP_ERROR_CHECK(rmt_wait_tx_done(rmtChannel_, portMAX_DELAY));
}

template <unsigned int LED_COUNT, RGBOrder RGB_ORDER>
void WS2812<LED_COUNT, RGB_ORDER>::setupRmtDataBuffer(rmt_item32_t* rmtDataBuffer,
                                                      std::array<Color, LED_COUNT> ledState)
{
  std::array<uint32_t, LED_COUNT> rgbValues = composeRgbValues(ledState);
  for (uint32_t ledIndex = 0; ledIndex < LED_COUNT; ledIndex++)
  {
    uint32_t bitsToSend = rgbValues.at(ledIndex);
    const char* intStr = std::to_string(bitsToSend).c_str();
    uint32_t mask = 1 << (bitsPerLedCmd_ - 1);
    for (uint32_t bitIndex = 0; bitIndex < bitsPerLedCmd_; bitIndex++)
    {
      uint32_t isBitSet = bitsToSend & mask;
      rmtDataBuffer[ledIndex * bitsPerLedCmd_ + bitIndex] =
          isBitSet ? (rmt_item32_t){{{highTime1_, 1, lowTimeAny_, 0}}}
                   : (rmt_item32_t){{{highTime0_, 1, lowTimeAny_, 0}}};
      mask >>= 1;
    }
  }
}

template <unsigned int LED_COUNT, RGBOrder RGB_ORDER>
std::array<uint32_t, LED_COUNT>
WS2812<LED_COUNT, RGB_ORDER>::composeRgbValues(const std::array<Color, LED_COUNT>& ledState) const
{
  std::array<uint32_t, LED_COUNT> rgbValues = {};
  for (unsigned int i = 0; i < ledState.size(); ++i)
  {
    uint32_t redByte = ledState.at(i).red << 16;
    uint32_t greenByte = ledState.at(i).green << 8;
    uint32_t blueByte = ledState.at(i).blue;
    switch (RGB_ORDER)
    {
      case RGBOrder::RGB:
        rgbValues.at(i) = redByte | greenByte | blueByte;
        break;
      case RGBOrder::RBG:
        rgbValues.at(i) = redByte | (greenByte >> 8) | (blueByte << 8);
        break;
      case RGBOrder::GRB:
        rgbValues.at(i) = (redByte >> 8) | (greenByte << 8) | (blueByte);
        break;
      case RGBOrder::GBR:
        rgbValues.at(i) = (redByte >> 16) | (greenByte << 8) | (blueByte << 8);
        break;
      case RGBOrder::BRG:
        rgbValues.at(i) = (redByte >> 8) | (greenByte >> 8) | (blueByte << 16);
        break;
      case RGBOrder::BGR:
        rgbValues.at(i) = (redByte >> 16) | greenByte | (blueByte << 16);
        break;
      default:
        break;
    }
  }
  return rgbValues;
}