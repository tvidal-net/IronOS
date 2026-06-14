#include "SolderingCommon.h"
#include "power.hpp"
#include "ui_drawing.hpp"
#ifdef OLED_128x32

void ui_draw_soldering_basic_status(bool boostModeOn) {
  // 128x32: draw the tip temperature in the Spleen 12x24 font. At 12px/char it
  // is much narrower than the old 24px-wide 2x font, so the heat indicator and
  // power-source icon still fit alongside it.
  const TemperatureType_t temp = getTipTemp();
  OLED::setCursor(0, 0);
  // We switch the layout direction depending on the orientation of the oled
  if (OLED::getRotation()) {
    // battery / power source (may leave the cursor on the lower strip)
    ui_draw_power_source_icon();
    OLED::print(LargeSymbolSpace, FontStyle::LARGE);
    // Draw current tip temp (big), resetting to the top strip first
    OLED::setCursor(OLED::getCursorX(), 0);
    OLED::printSpleenNumber(temp, 3, true);
    OLED::printSpleenDeg(true);
    // We draw boost arrow if boosting
    if (boostModeOn) {
      OLED::drawSymbol(2);
    }
    // Draw heating/cooling symbols
    OLED::drawHeatSymbol(X10WattsToPWM(x10WattHistory.average()));
  } else {
    // Draw heating/cooling symbols
    OLED::drawHeatSymbol(X10WattsToPWM(x10WattHistory.average()));
    // We draw boost arrow if boosting
    if (boostModeOn) {
      OLED::drawSymbol(2);
    }
    // Draw current tip temp (big)
    OLED::setCursor(OLED::getCursorX(), 0);
    OLED::printSpleenNumber(temp, 3, true);
    OLED::printSpleenDeg(true);
    OLED::print(LargeSymbolSpace, FontStyle::LARGE);
    OLED::setCursor(OLED::getCursorX(), 0);
    ui_draw_power_source_icon();
  }
}

#endif
