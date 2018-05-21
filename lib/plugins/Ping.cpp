//************************************************************
//************************************************************
#include "Ping.h"

Ping::Ping(painlessMesh &mesh, int btnPin)
  : _mesh(mesh), _btn(Button(btnPin)) {
}

Ping::Ping(painlessMesh &mesh, int btnPin, int debounce, bool puInternal, bool puExternal)
  : _mesh(mesh), _btn(Button(btnPin, debounce, puInternal, puExternal)) {
}

void Ping::init() {
  _btn.begin();

  Serial.println("init");
}

void Ping::loop() {
  _btn.read();

  if (_btn.wasReleased()) {
    onClick();
  }
}

void Ping::onClick() {
  String msg = "PING-";
  msg += _mesh.getNodeId();
  _mesh.sendBroadcast(msg);
  Serial.println("PING");
}
