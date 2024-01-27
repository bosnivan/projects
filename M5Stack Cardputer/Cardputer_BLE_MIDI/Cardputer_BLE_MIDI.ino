#include <Arduino.h>
#include <BLEMidi.h>
#include <M5Cardputer.h">

int pitch;

void setup() {
  Serial.begin(115200);
  auto cfg = M5.config();
  M5Cardputer.begin(cfg, true);
  M5Cardputer.Display.setRotation(1);
  M5Cardputer.Display.setTextColor(GREEN);
  M5Cardputer.Display.setTextDatum(middle_center);
  M5Cardputer.Display.setTextFont(&fonts::Orbitron_Light_32);
  M5Cardputer.Display.setTextSize(1);
  M5Cardputer.Display.drawString("MIDI",
                                 M5Cardputer.Display.width() / 2,
                                 M5Cardputer.Display.height() / 2);
  BLEMidiServer.begin("Cardputer BLE MIDI");
}

void loop() {
  M5Cardputer.update();
  if (M5Cardputer.Keyboard.isChange()) {
    Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
    if (M5Cardputer.Keyboard.isPressed()) {
      pitch = status.word[0] - 40;
      if (BLEMidiServer.isConnected()) {
        BLEMidiServer.noteOn(0, pitch, 127);
      } else {
        noteOn(pitch, 127);
      }
      M5Cardputer.Display.clear();
      M5Cardputer.Display.drawString(String(pitch),
                                     M5Cardputer.Display.width() / 2,
                                     M5Cardputer.Display.height() / 2);
    } else {
      if (BLEMidiServer.isConnected()) {
        BLEMidiServer.noteOff(0, pitch, 127);
      } else {
        noteOff(pitch);
      }
      M5Cardputer.Display.clear();
      M5Cardputer.Display.drawString("MIDI",
                                     M5Cardputer.Display.width() / 2,
                                     M5Cardputer.Display.height() / 2);
    }
  }
  delay(100);
}

void noteOn(int pitch, int velocity) {
  Serial.write(0x90);
  Serial.write(pitch);
  Serial.write(velocity);
}

void noteOff(int pitch) {
  Serial.write(0x80);
  Serial.write(pitch);
  Serial.write(0);
}
