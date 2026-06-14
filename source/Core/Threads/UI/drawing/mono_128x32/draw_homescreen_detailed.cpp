#include "SpleenFont.h"
#include "ui_drawing.hpp"
#ifdef OLED_128x32

extern uint8_t buttonAF[sizeof(buttonA)];
extern uint8_t buttonBF[sizeof(buttonB)];
extern uint8_t disconnectedTipF[sizeof(disconnectedTip)];

void ui_draw_homescreen_detailed(TemperatureType_t tipTemp) {
  if (isTipDisconnected()) {
    if (OLED::getRotation()) {
      // in right handed mode we want to draw over the first part
      OLED::drawArea(54, 0, 56, 32, disconnectedTipF);
    } else {
      OLED::drawArea(0, 0, 56, 32, disconnectedTip);
    }
    if (OLED::getRotation()) {
      OLED::setCursor(-1, 0);
    } else {
      OLED::setCursor(56, 0);
    }
    uint32_t Vlt = getInputVoltageX10(getSettingValue(SettingsOptions::VoltageDiv), 0);
    OLED::printNumber(Vlt / 10, 2, FontStyle::LARGE);
    OLED::print(LargeSymbolDot, FontStyle::LARGE);
    OLED::printNumber(Vlt % 10, 1, FontStyle::LARGE);
    if (OLED::getRotation()) {
      OLED::setCursor(48, 8);
    } else {
      OLED::setCursor(91, 8);
    }
    OLED::print(SmallSymbolVolts, FontStyle::SMALL);
  } else {
    // Tip temperature in the big 12x24 font flush to one edge, with the set
    // temperature and input voltage stacked in the 8x16 font flush to the
    // other edge (filling the full panel height). Sides flip with rotation.
    const bool    rot     = OLED::getRotation();
    const uint8_t statusW = 5 * SPLEEN_8X16_WIDTH;            // "NNN°C" / "NN.NV" are both 5 cells
    const uint8_t tempW   = ui_tip_temperature_spleen_width(true, true);
    const int16_t tempX   = rot ? (OLED_WIDTH - tempW) : 0;   // temp flush to its edge
    const int16_t statusX = rot ? 0 : (OLED_WIDTH - statusW); // status flush to the opposite edge

    if (!(getSettingValue(SettingsOptions::CoolingTempBlink) && (tipTemp > 55) && (xTaskGetTickCount() % 1000 < 300))) {
      // Blink temp if setting enable and temp < 55° (OFF 300ms / ON 700ms)
      OLED::setCursor(tempX, 4); // vertically centred (4px above/below the 24px glyph)
      ui_draw_tip_temperature_spleen(true, true);
    }

    // Set temperature (top row)
    OLED::setCursor(statusX, 0);
    OLED::printSpleenNumber(getSettingValue(SettingsOptions::SolderingTemp), 3, false);
    OLED::printSpleenDeg(false);

    // Input voltage (bottom row)
    OLED::setCursor(statusX, 16);
    const uint32_t Vlt = getInputVoltageX10(getSettingValue(SettingsOptions::VoltageDiv), 0);
    OLED::printSpleenNumber(Vlt / 10, 2, false);
    OLED::printSpleen(".", false);
    OLED::printSpleenNumber(Vlt % 10, 1, false);
    OLED::printSpleen("V", false);
  }
}
#endif