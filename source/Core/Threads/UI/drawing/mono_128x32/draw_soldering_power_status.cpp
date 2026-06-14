#include "power.hpp"
#include "ui_drawing.hpp"
#include <OperatingModes.h>
#ifdef OLED_128x32

void ui_draw_soldering_power_status(bool boost_mode_on) {
  // Tip temperature in the big 12x24 font flush to one edge; the status detail
  // (boost/sleep, wattage, power source, voltage) stacked as four small-font
  // rows flush to the other edge so the full panel height is used. Sides flip
  // with rotation.
  const bool    rot     = OLED::getRotation();
  const uint8_t tempW   = ui_tip_temperature_spleen_width(true, true);
  const int16_t tempX   = rot ? (OLED_WIDTH - tempW) : 0;       // temp flush to its edge
  const int16_t statusX = rot ? 0 : (OLED_WIDTH - (7 * 6));     // status column (~7 small chars) on the other edge

  // Big temperature, vertically centred (rows 8..32)
  OLED::setCursor(tempX, 8);
  ui_draw_tip_temperature_spleen(true, true);

  // Row 0: boost arrow, or the sleep countdown
  OLED::setCursor(statusX, 0);
  if (boost_mode_on) {
    OLED::print(SmallSymbolPlus, FontStyle::SMALL);
  } else {
#ifndef NO_SLEEP_MODE
    if (getSettingValue(SettingsOptions::Sensitivity) && getSettingValue(SettingsOptions::SleepTime)) {
      printCountdownUntilSleep(getSleepTimeout());
    }
#endif
  }

  // Row 1: wattage
  OLED::setCursor(statusX, 8);
  {
    uint32_t x10Watt = x10WattHistory.average();
    if (x10Watt > 999) {
      // If we exceed 99.9W we drop the decimal place to keep it all fitting
      OLED::print(SmallSymbolSpace, FontStyle::SMALL);
      OLED::printNumber(x10Watt / 10, 3, FontStyle::SMALL);
    } else {
      OLED::printNumber(x10Watt / 10, 2, FontStyle::SMALL);
      OLED::print(SmallSymbolDot, FontStyle::SMALL);
      OLED::printNumber(x10Watt % 10, 1, FontStyle::SMALL);
    }
    OLED::print(SmallSymbolWatts, FontStyle::SMALL);
  }

  // Row 2: power source
  OLED::setCursor(statusX, 16);
  OLED::print(PowerSourceNames[getPowerSourceNumber()], FontStyle::SMALL, 2);

  // Row 3: input voltage
  OLED::setCursor(statusX, 24);
  printVoltage();
  OLED::print(SmallSymbolVolts, FontStyle::SMALL);
}
#endif
