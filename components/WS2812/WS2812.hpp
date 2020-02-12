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

template <std::size_t LED_COUNT, RGBOrder RGB_ORDER>
class WS2812
{
public:
  /**
   * @brief Construct a new WS2812 object
   *
   * @param gpioPin The GPIO pin which is connect to a WS2812 LED strip
   * @param rmtChannel The RMT channel to use
   */
  explicit WS2812(gpio_num_t gpioPin, rmt_channel_t rmtChannel = RMT_CHANNEL_0);

  /**
   * @brief Lights the LED strip up corresponding to the provided array of Colors
   *
   * @param ledState The array holding RGB values corresponding to the connected LEDs
   */
  void writeLeds(const std::array<Color, LED_COUNT>& ledState);


private:
  rmt_channel_t rmtChannel_;
  static const std::uint8_t bitsPerLedCmd__ = 24;
  static const unsigned int rmtBufferSize__ = LED_COUNT * bitsPerLedCmd__;
  /// These values are determined by measuring pulse timing with logic analyzer and adjusting to
  /// match datasheet.
  /// 0 bit high time
  static const std::uint8_t highTime0__ = 14;
  /// 1 bit high time
  static const std::uint8_t highTime1__ = 52;
  // low time for either bit
  static const std::uint8_t lowTimeAny__ = 52;

  /**
   * @brief Sets up the RMT data buffer
   *
   * @param rmtDataBuffer The RMT data buffer to write to
   * @param ledState The array holding RGB colors corresponding to the connected LEDs
   */
  void setupRmtDataBuffer(std::array<rmt_item32_t, rmtBufferSize__>& rmtDataBuffer,
                          std::array<Color, LED_COUNT> ledState);
  std::array<uint32_t, LED_COUNT>
  composeRgbValues(const std::array<Color, LED_COUNT>& ledState) const;
};

template <std::size_t LED_COUNT, RGBOrder RGB_ORDER>
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

template <std::size_t LED_COUNT, RGBOrder RGB_ORDER>
void WS2812<LED_COUNT, RGB_ORDER>::writeLeds(const std::array<Color, LED_COUNT>& ledState)
{
  std::array<rmt_item32_t, rmtBufferSize__> rmtDataBuffer;
  rmtDataBuffer.fill((rmt_item32_t){{{0, 0, 0, 0}}});
  setupRmtDataBuffer(rmtDataBuffer, ledState);
  ESP_ERROR_CHECK(rmt_write_items(rmtChannel_, rmtDataBuffer.data(), rmtBufferSize__, false));
  ESP_ERROR_CHECK(rmt_wait_tx_done(rmtChannel_, portMAX_DELAY));
}

template <std::size_t LED_COUNT, RGBOrder RGB_ORDER>
void WS2812<LED_COUNT, RGB_ORDER>::setupRmtDataBuffer(
    std::array<rmt_item32_t, rmtBufferSize__>& rmtDataBuffer, std::array<Color, LED_COUNT> ledState)
{
  std::array<uint32_t, LED_COUNT> rgbValues = composeRgbValues(ledState);
  for (std::size_t ledIndex = 0; ledIndex < LED_COUNT; ++ledIndex)
  {
    uint32_t bitsToSend = rgbValues[ledIndex];
    uint32_t mask = 1u << (bitsPerLedCmd__ - 1u);
    for (std::size_t bitIndex = 0; bitIndex < bitsPerLedCmd__; ++bitIndex)
    {
      uint32_t isBitSet = bitsToSend & mask;
      rmtDataBuffer[ledIndex * bitsPerLedCmd__ + bitIndex] =
          isBitSet != 0u ? (rmt_item32_t){{{highTime1__, 1, lowTimeAny__, 0}}}
                   : (rmt_item32_t){{{highTime0__, 1, lowTimeAny__, 0}}};
      mask >>= 1u;
    }
  }
}

template <std::size_t LED_COUNT, RGBOrder RGB_ORDER>
std::array<uint32_t, LED_COUNT>
WS2812<LED_COUNT, RGB_ORDER>::composeRgbValues(const std::array<Color, LED_COUNT>& ledState) const
{
  std::array<uint32_t, LED_COUNT> rgbValues;
  rgbValues.fill(0);
  for (std::size_t i = 0; i < ledState.size(); ++i)
  {
    uint32_t redByte = ledState[i].red;
    uint32_t greenByte = ledState[i].green;
    uint32_t blueByte = ledState[i].blue;
    if constexpr (RGB_ORDER == RGBOrder::RGB)
    {
      rgbValues[i] = redByte << 16u | greenByte << 8u | blueByte;
    }
    else if constexpr (RGB_ORDER == RGBOrder::RBG)
    {
      rgbValues[i] = redByte << 16u | greenByte | (blueByte << 8u);
    }
    else if constexpr (RGB_ORDER == RGBOrder::GRB)
    {
      rgbValues[i] = (redByte << 8u) | (greenByte << 16u) | blueByte;
    }
    else if constexpr (RGB_ORDER == RGBOrder::GBR)
    {
      rgbValues[i] = redByte | (greenByte << 16u) | (blueByte << 8u);
    }
    else if constexpr (RGB_ORDER == RGBOrder::BRG)
    {
      rgbValues[i] = (redByte << 8u) | greenByte | (blueByte << 16u);
    }
    else if constexpr (RGB_ORDER == RGBOrder::BGR)
    {
      rgbValues[i] = redByte | (greenByte << 8u) | (blueByte << 16u);
    }
  }
  return rgbValues;
}