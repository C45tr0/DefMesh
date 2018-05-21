//************************************************************
//************************************************************
#include "Mute.h"

Mute::Mute(int ledPin, int btnPin)
  : _btn(Button(btnPin)) {
  _ledPin = btnPin;
}

Mute::Mute(int ledPin, int btnPin, int debounce, bool puInternal, bool puExternal)
  : _btn(Button(btnPin, debounce, puInternal, puExternal)) {
  _ledPin = ledPin;
}

void Mute::init() {
  _btn.begin();

  pinMode(_ledPin, OUTPUT);
  Serial.println("init");
}

void Mute::loop() {
  _btn.read();

  if (_btn.wasReleased()) {
    onClick();
  }

  digitalWrite(_ledPin, _isMuted ? HIGH : LOW);
}

bool Mute::isMuted() {
  return _isMuted;
}

void Mute::onClick() {
  _isMuted = !_isMuted;
  Serial.println("onClick");
}
