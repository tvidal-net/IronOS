#include "SpleenFont.h"
#include "power.hpp"
#include "ui_drawing.hpp"
#include <OperatingModes.h>
#ifdef OLED_128x32

void ui_draw_soldering_power_status(bool boost_mode_on) {
  // Mirrors the detailed idle layout: big 12x24 temperature flush to one edge,
  // and two 8x16 status rows (wattage over voltage) flush to the other edge.
  // The sleep countdown and power-source label are dropped to fit two larger
  // rows; sides flip with rotation.
  (void)boost_mode_on;
  const bool    rot     = OLED::getRotation();
  const uint8_t statusW = 5 * SPLEEN_8X16_WIDTH;            // "NN.NW" / "NN.NV" are 5 cells
  const uint8_t tempW   = ui_tip_temperature_spleen_width(true, true);
  const int16_t tempX   = rot ? (OLED_WIDTH - tempW) : 0;   // temp flush to its edge
  const int16_t statusX = rot ? 0 : (OLED_WIDTH - statusW); // status flush to the opposite edge

  // Big temperature, vertically centred (4px above/below the 24px glyph)
  OLED::setCursor(tempX, 4);
  ui_draw_tip_temperature_spleen(true, true);

  // Wattage (top row)
  OLED::setCursor(statusX, 0);
  {
    uint32_t x10Watt = x10WattHistory.average();
    if (x10Watt > 999) {
      // Above 99.9W drop the decimal place to keep it to 5 cells
      OLED::printSpleenNumber(x10Watt / 10, 3, false);
    } else {
      OLED::printSpleenNumber(x10Watt / 10, 2, false);
      OLED::printSpleen(".", false);
      OLED::printSpleenNumber(x10Watt % 10, 1, false);
    }
    OLED::printSpleen("W", false);
  }

  // Input voltage (bottom row)
  OLED::setCursor(statusX, 16);
  {
    uint32_t Vlt = getInputVoltageX10(getSettingValue(SettingsOptions::VoltageDiv), 0);
    OLED::printSpleenNumber(Vlt / 10, 2, false);
    OLED::printSpleen(".", false);
    OLED::printSpleenNumber(Vlt % 10, 1, false);
    OLED::printSpleen("V", false);
  }
}
#endif
