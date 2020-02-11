#include "WS2812.hpp"
#include "esp_log.h"

static constexpr const char* TAG = "WS1812";

WS2812::WS2812(uint32_t ledCount, gpio_num_t gpioPin, RGBOrder rgbOrder, rmt_channel_t rmtChannel)
{
  ledCount_ = ledCount;
  rgbOrder_ = rgbOrder;

  rmtConfig_.rmt_mode = RMT_MODE_TX;
  rmtConfig_.channel = rmtChannel;
  rmtConfig_.gpio_num = gpioPin;
  rmtConfig_.mem_block_num = 3;
  rmtConfig_.tx_config.loop_en = false;
  rmtConfig_.tx_config.carrier_en = false;
  rmtConfig_.tx_config.idle_output_en = true;
  rmtConfig_.tx_config.idle_level = static_cast<rmt_idle_level_t>(0);
  rmtConfig_.clk_div = 2;

  ESP_ERROR_CHECK(rmt_config(&rmtConfig_));
  ESP_ERROR_CHECK(rmt_driver_install(rmtConfig_.channel, 0, 0));
}

void WS2812::writeLeds(std::vector<uint32_t> ledState)
{
  rmt_item32_t rmtDataBuffer[ledCount_ * bitsPerLedCmd_] = {};
  setupRmtDataBuffer(rmtDataBuffer, ledState);
  ESP_ERROR_CHECK(
      rmt_write_items(rmtConfig_.channel, rmtDataBuffer, ledCount_ * bitsPerLedCmd_, false));
  ESP_ERROR_CHECK(rmt_wait_tx_done(rmtConfig_.channel, portMAX_DELAY));
}

void WS2812::setupRmtDataBuffer(rmt_item32_t* rmtDataBuffer, std::vector<uint32_t> ledState)
{
  if (ledState.size() != ledCount_)
  {
    ESP_LOGW(TAG, "Size of ledState doesn't match number of LEDs");
  }
  if (rgbOrder_ != RGBOrder::RGB)
  {
    ledState = changeRGBOrder(ledState);
  }
  for (uint32_t ledIndex = 0; ledIndex < ledCount_; ledIndex++)
  {
    uint32_t bitsToSend = ledState.at(ledIndex);
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

std::vector<uint32_t> WS2812::changeRGBOrder(std::vector<uint32_t> ledState) const
{
  for (auto& rgbValue : ledState)
  {
    uint32_t redByte = rgbValue & 0xFF0000;
    uint32_t greenByte = rgbValue & 0x00FF00;
    uint32_t blueByte = rgbValue & 0x0000FF;

    switch (rgbOrder_)
    {
      case RGBOrder::RGB:
        [[fallthrough]];
      case RGBOrder::RBG:
        rgbValue = redByte | (greenByte >> 8) | (blueByte << 8);
        break;
      case RGBOrder::GRB:
        rgbValue = (redByte >> 8) | (greenByte << 8) | (blueByte);
        break;
      case RGBOrder::GBR:
        rgbValue = (redByte >> 16) | (greenByte << 8) | (blueByte << 8);
        break;
      case RGBOrder::BRG:
        rgbValue = (redByte >> 8) | (greenByte >> 8) | (blueByte << 16);

        break;
      case RGBOrder::BGR:
        rgbValue = (redByte >> 16) | greenByte | (blueByte << 16);
        break;
    }
  }
  return ledState;
}

uint32_t WS2812::getLedCount() const
{
  return ledCount_;
}