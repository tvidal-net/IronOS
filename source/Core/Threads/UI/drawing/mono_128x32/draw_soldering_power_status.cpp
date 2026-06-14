#include "power.hpp"
#include "ui_drawing.hpp"
#include <OperatingModes.h>
#ifdef OLED_128x32

void ui_draw_soldering_power_status(bool boost_mode_on) {
  // Big 12x24 tip temperature flush to one edge; wattage (top) and input
  // voltage (bottom) in the 8x16 font flush to the other edge. The sleep
  // countdown and power-source label are dropped to keep two larger rows.
  // Sides flip with rotation.
  (void)boost_mode_on;
  const bool    rot     = OLED::getRotation();
  const uint8_t statusW = 5 * 8; // "NN.NW" / "NN.NV" are 5 cells in the 8x16 font
  const uint8_t tempW   = ui_tip_temperature_readout_width(true, FontStyle::EXTRA_LARGE);
  const int16_t tempX   = rot ? (OLED_WIDTH - tempW) : 0;
  const int16_t statusX = rot ? 0 : (OLED_WIDTH - statusW);

  // Big temperature, vertically centred (4px above/below the 24px glyph)
  OLED::setCursor(tempX, 4);
  ui_draw_tip_temperature_readout(true, FontStyle::EXTRA_LARGE);

  // Wattage (top row)
  OLED::setCursor(statusX, 0);
  {
    uint32_t x10Watt = x10WattHistory.average();
    if (x10Watt > 999) {
      // Above 99.9W drop the decimal place to keep it to 5 cells
      OLED::printNumber(x10Watt / 10, 3, FontStyle::MEDIUM);
    } else {
      OLED::printNumber(x10Watt / 10, 2, FontStyle::MEDIUM);
      OLED::print(LargeSymbolDot, FontStyle::MEDIUM);
      OLED::printNumber(x10Watt % 10, 1, FontStyle::MEDIUM);
    }
    OLED::print(LargeSymbolWatts, FontStyle::MEDIUM);
  }

  // Input voltage (bottom row)
  OLED::setCursor(statusX, 16);
  {
    uint32_t Vlt = getInputVoltageX10(getSettingValue(SettingsOptions::VoltageDiv), 0);
    OLED::printNumber(Vlt / 10, 2, FontStyle::MEDIUM);
    OLED::print(LargeSymbolDot, FontStyle::MEDIUM);
    OLED::printNumber(Vlt % 10, 1, FontStyle::MEDIUM);
    OLED::print(LargeSymbolVolts, FontStyle::MEDIUM);
  }
}
#endif
