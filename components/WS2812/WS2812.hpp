#pragma once

#include "driver/rmt.h"
#include <vector>

enum class RGBOrder
{
    RGB,
    RBG,
    GRB,
    GBR,
    BRG,
    BGR
};

class WS2812
{
public:
    /**
     * @brief Construct a new WS2812 object
     * 
     * @param ledCount The number of connect WS2812 LEDs
     * @param gpioPin The GPIO Pin where the LED Strip is connected to
     * @param rmtChannel The RMT Channel to use
     * @param rgbOrder The RGB color order to use (some vendors use a different wiring)
     */
    WS2812(uint32_t ledCount, gpio_num_t gpioPin, RGBOrder rgbOrder = RGBOrder::RGB, rmt_channel_t rmtChannel = RMT_CHANNEL_0);

    /**
     * @brief Lights the LED strip up corresponding to the provided vector of colors
     * 
     * @param ledState The vector holding RGB colors corresponding to the connected LEDs
     */
    void writeLeds(std::vector<uint32_t> ledState);
    uint32_t getLedCount() const;

private:
    rmt_config_t rmtConfig_;
    uint32_t ledCount_;
    RGBOrder rgbOrder_;

    const uint16_t bitsPerLedCmd_ = 24;
    // These values are determined by measuring pulse timing with logic analyzer and adjusting to match datasheet.
    const uint16_t highTime0_ = 14;  // 0 bit high time
    const uint16_t highTime1_ = 52;  // 1 bit high time
    const uint16_t lowTimeAny_ = 52; // low time for either bit

    /**
     * @brief Sets up the RMT data buffer
     * 
     * @param rmtDataBuffer the RMT data buffer
     * @param ledState The vector holding RGB colors corresponding to the connected LEDs
     */
    void setupRmtDataBuffer(rmt_item32_t *rmtDataBuffer, std::vector<uint32_t> ledState);

    /**
     * @brief Change order of red, green, and blue value (lower 3 bytes) in uint32_t color value
     * 
     * @param ledState The vector holding RGB colors corresponding to the connected LEDs
     * @return std::vector<uint32_t> The ledState vector with changed RGB order
     */
    std::vector<uint32_t> changeRGBOrder(std::vector<uint32_t> ledState) const;
};