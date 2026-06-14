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

static uint8_t tip_temp_digits() {
  TemperatureType_t Temp   = getTipTemp();
  uint16_t          v      = Temp < 0 ? 0 : (uint16_t)Temp;
  uint8_t           digits = 1;
  for (uint16_t n = v; n >= 10; n /= 10) {
    digits++;
  }
  return digits;
}

static uint8_t font_cell_width(const FontStyle font) {
  switch (font) {
  case FontStyle::SMALL:
    return 6;
  case FontStyle::MEDIUM:
    return 8;
  default:
    return 12; // LARGE / EXTRA_LARGE / EXTRAS
  }
}

// Tip temperature for the detailed readouts: drawn left-aligned with exactly its
// digit count (no leading blanks) so it can be flushed to an edge.
void ui_draw_tip_temperature_readout(bool symbol, const FontStyle font) {
  OLED::printNumber(getTipTemp(), tip_temp_digits(), font, false);
  if (symbol) {
    OLED::printSymbolDeg(font);
  }
}

// Pixel width the above occupies, for right-aligning it on the opposite rotation.
uint8_t ui_tip_temperature_readout_width(bool symbol, const FontStyle font) {
  return (tip_temp_digits() + (symbol ? 2 : 0)) * font_cell_width(font);
}
#endif