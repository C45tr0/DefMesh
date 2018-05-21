
#ifndef ping_h
#define ping_h
#include "painlessMesh.h"
#include "JC_Button.h"

class Ping {

public:
  Ping(painlessMesh &mesh, int btnPin);
  Ping(painlessMesh &mesh, int btnPin, int debounce, bool puInternal, bool puExternal);

  virtual void init();
  virtual void loop();

private:
  painlessMesh &_mesh;
  Button _btn;

  virtual void onClick();
};

#endif