#include "OperatingModeUtilities.h"
#include "OperatingModes.h"
#include "SolderingCommon.h"
#include "SpleenFont.h"
#include "TipThermoModel.h"
#ifdef OLED_128x32

static uint8_t spleen_temp_digits() {
  TemperatureType_t Temp = getTipTemp();
  uint16_t          v     = Temp < 0 ? 0 : (uint16_t)Temp;
  uint8_t           digits = 1;
  for (uint16_t n = v; n >= 10; n /= 10) {
    digits++;
  }
  return digits;
}

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
// Left-aligned with no leading padding so it sits flush at the cursor; callers
// that want right-alignment offset the cursor by ui_tip_temperature_spleen_width().
void ui_draw_tip_temperature_spleen(bool symbol, bool large) {
  OLED::printSpleenNumber(getTipTemp(), spleen_temp_digits(), large, false);
  if (symbol) {
    OLED::printSpleenDeg(large);
  }
}

uint8_t ui_tip_temperature_spleen_width(bool symbol, bool large) {
  const uint8_t cells = spleen_temp_digits() + (symbol ? 2 : 0); // +2 for the degree sign and unit letter
  return cells * (large ? SPLEEN_12X24_WIDTH : SPLEEN_8X16_WIDTH);
}
#endif