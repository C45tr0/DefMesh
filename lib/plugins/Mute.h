
#ifndef mute_h
#define mute_h
#include "JC_Button.h"

class Mute {

public:
  Mute(int ledPin, int btnPin);
  Mute(int ledPin, int btnPin, int debounce, bool puInternal, bool puExternal);

  virtual void init();
  virtual void loop();
  virtual bool isMuted();

private:
  Button _btn;
  int _ledPin;
  bool _isMuted = LOW;

  virtual void onClick();
};

#endif