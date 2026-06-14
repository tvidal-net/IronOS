#include "OperatingModeUtilities.h"
#include "OperatingModes.h"
#include "SolderingCommon.h"
#include "TipThermoModel.h"
#ifdef OLED_128x32

void ui_draw_tip_temperature(bool symbol, const FontStyle font) {
  // Draw tip temp handling unit conversion & tolerance near setpoint
  TemperatureType_t Temp = getTipTemp();

  OLED::printNumber(Temp, 3, font); // Draw the tip temp out
  if (symbol) {
    // For big font, can draw nice symbols, otherwise fall back to chars
    OLED::printSymbolDeg(font == FontStyle::LARGE ? FontStyle::EXTRAS : font);
  }
}

// As above, but rendered in the Spleen readout font (large = 12x24, else 8x16).
void ui_draw_tip_temperature_spleen(bool symbol, bool large) {
  OLED::printSpleenNumber(getTipTemp(), 3, large);
  if (symbol) {
    OLED::printSpleenDeg(large);
  }
}
#endif